/**
 * @file src/prg/c/src/com-lib/ssh.c
 *
 * SSH connection processing functions.
 *
 * Covers tunnel creation and destruction including all authentication. Creates
 * a seperate thread for data relay through the tunnel.
 *
 * @author Copyright (C) 2017-2019, 2021, 2022  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.0.12 ==== 15/09/2022_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 19/10/2017	MG	1.0.1	First release.				*
 * 10/11/2017	MG	1.0.2	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 15/08/2018	MG	1.0.3	Move ret_buf from stack to heap.	*
 *				Apply unused attribute to relay_data	*
 *				parameter which is only required	*
 *				because of pthread_create.		*
 * 23/09/2018	MG	1.0.4	Replace use of deprecated bzero() with	*
 *				memset().				*
 * 18/05/2019	MG	1.0.5	Merge sub-projects into one.		*
 * 01/06/2019	MG	1.0.6	Trivial type safety improvements.	*
 * 09/11/2019	MG	1.0.7	Use ssh_get_server_publickey() AOT	*
 *				deprecated ssh_get_publickey().		*
 * 10/10/2021	MG	1.0.8	Use newly internalised common header.	*
 * 14/10/2021	MG	1.0.9	Replace deprecated functions.		*
 *				Eliminate -Wsign-conversion warnings.	*
 * 08/12/2021	MG	1.0.10	Tighten SPDX tag.			*
 * 02/04/2022	MG	1.0.11	Improve error handling consistency.	*
 * 15/09/2022	MG	1.0.12	Rename mgememory.h			*
 *				Use pkginclude location.		*
 *				Correct included headers.		*
 *				Flatten directory structure.		*
 *									*
 ************************************************************************
 */

#include <libssh/libssh.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <syslog.h>
#include <unistd.h>

#include <libmgec/mge-errno.h>
#include <libmgec/mge-memory.h>
#include <libswoccommon.h>

static int verify_knownhost(void);
static int try_auth_methods_seq(void);
static int authenticate_kbdint(void);
static int authenticate_password(void);
static int direct_forwarding(void);
static void *relay_data(__attribute__((unused)) void *arg);

static ssh_session ssh_sess;
static ssh_channel fwd_chan;
static int ssh_sock;
static pthread_t relay_thread;
static const int relay_data_success = 0;
static const int relay_data_failure = -MGE_SSH;

/**
 * Establish SSH connection.
 * Create session, connect to server, create a tunnel and spawn a thread to
 * relay data through the tunnel.
 * On error mge_errno will be set.
 * @return 0 on success, < zero on error.
 */
int open_ssh_tunnel(void)
{
	int res;
	void *arg = NULL;

	ssh_sess = ssh_new();
	if (ssh_sess == NULL) {
		mge_errno = MGE_SSH;
		syslog((int)(LOG_USER | LOG_NOTICE), "Error creating SSH "
						     "session.");
		return -mge_errno;
	}

	ssh_options_set(ssh_sess, SSH_OPTIONS_USER, sshuser);
	ssh_options_set(ssh_sess, SSH_OPTIONS_HOST, server);

	res = ssh_connect(ssh_sess);
	if (res == SSH_OK) {
		res = 0;
	} else {
		mge_errno = MGE_SSH;
		res = -mge_errno;
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Error connecting to %s: "
		       "%s",
		       server, ssh_get_error(ssh_sess));
		goto err_exit_0;
	}
	res = verify_knownhost();
	if (res)
		goto err_exit_1;

	res = try_auth_methods_seq();
	if (res)
		goto err_exit_1;

	res = prep_recv_sock(&ssh_sock, &sshportno);
	if (res)
		goto err_exit_1;

	res = direct_forwarding();
	if (res)
		goto err_exit_2;

	sav_errno = pthread_create(&relay_thread, NULL, relay_data, arg);
	if (sav_errno) {
		mge_errno = MGE_ERRNO;
		res = -mge_errno;
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Error creating thread. "
		       "%s",
		       mge_strerror(mge_errno));
		goto err_exit_3;
	}

	return 0;

err_exit_3:
	ssh_channel_free(fwd_chan);
err_exit_2:
	close_sock(&ssh_sock);
err_exit_1:
	ssh_disconnect(ssh_sess);
err_exit_0:
	ssh_free(ssh_sess);
	return res;
}

/**
 * Disconnect and close an SSH session.
 * Join data relay thread, free channel and disconnect session.
 * On error mge_errno will be set.
 * @return 0 on success, < zero on failure.
 */
int close_ssh_tunnel(void)
{
	int res;
	void *retval = NULL;

	sav_errno = pthread_join(relay_thread, &retval);
	if (sav_errno) {
		mge_errno = MGE_ERRNO;
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Error joining thread. "
		       "%s.",
		       mge_strerror(mge_errno));
		res = -mge_errno;
		goto err_exit_1;
	}
	if (retval == NULL) {
		mge_errno = MGE_SSH;
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Error allocating memory "
		       "in relay_data - %s.",
		       mge_strerror(mge_errno));
		res = -mge_errno;
		goto err_exit_1;
	}
	res = *(int *)retval;
	if (res) {
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Error relaying data - "
		       "%s.",
		       mge_strerror(mge_errno));
		goto err_exit_1;
	}
	ssh_channel_free(fwd_chan);
	res = close_sock(&ssh_sock);
	if (res)
		goto err_exit_0;
	goto exit_0;

err_exit_1:
	ssh_channel_free(fwd_chan);
	close_sock(&ssh_sock);

err_exit_0:
exit_0:
	ssh_disconnect(ssh_sess);
	ssh_free(ssh_sess);
	return res;
}

/*
 * Check if the server is in the known hosts file, if not try to add it.
 */
static int verify_knownhost(void)
{
	enum ssh_known_hosts_e state;
	unsigned char *hash = NULL;
	char *hexa = NULL;
	ssh_key serv_key;
	char buf[10];
	size_t hlen;
	int res;

	state = ssh_session_is_known_server(ssh_sess);

	res = ssh_get_server_publickey(ssh_sess, &serv_key);
	if (res == SSH_ERROR) {
		mge_errno = MGE_SSH;
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Error retrieving server public key - %s",
		       ssh_get_error(ssh_sess));
		return -mge_errno;
	}

	res = ssh_get_publickey_hash(serv_key, SSH_PUBLICKEY_HASH_SHA1, &hash,
				     &hlen);
	if (res) {
		mge_errno = MGE_SSH;
		res = -mge_errno;
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Error retrieving server "
		       "public key hash - %s",
		       ssh_get_error(ssh_sess));
		goto exit_0;
	}

	switch (state) {
	case SSH_KNOWN_HOSTS_OK:
		break; /* ok */
	case SSH_KNOWN_HOSTS_CHANGED:
		mge_errno = MGE_SSH;
		res = -mge_errno;
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Host key for server "
		       "changed. Stopping for security reasons.");
		goto exit_1;
	case SSH_KNOWN_HOSTS_OTHER:
		mge_errno = MGE_SSH;
		res = -mge_errno;
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "The host key for this "
		       "server was not found but another type of key exists. "
		       "Stopping for security reasons.");
		goto exit_1;
	case SSH_KNOWN_HOSTS_NOT_FOUND:
		fprintf(stderr, "Could not find known host file.\n");
		fprintf(stderr, "If you accept the host key here, the file "
				"will be automatically created.\n");
		/* fallthrough */
	case SSH_KNOWN_HOSTS_UNKNOWN:
		hexa = ssh_get_hexa(hash, hlen);
		if (hexa == NULL) {
			mge_errno = MGE_SSH;
			res = -mge_errno;
			syslog((int)(LOG_USER | LOG_NOTICE), "Error retrieving"
							     " hex key.");
			goto exit_1;
		}
		fprintf(stderr, "The server is unknown. Do you trust the host "
				"key?\n");
		fprintf(stderr, "Public key hash: %s\n", hexa);

		if (fgets(buf, sizeof(buf), stdin) == NULL) {
			mge_errno = MGE_SSH;
			res = -mge_errno;
			fprintf(stderr, "Invalid input.\n");
			syslog((int)(LOG_USER | LOG_NOTICE), "Invalid input.");
			goto exit_2;
		}
		if (strncasecmp(buf, "yes", 3) != 0) {
			mge_errno = MGE_SSH;
			res = -mge_errno;
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "User did not "
			       "answer yes to trust this key question");
			goto exit_2;
		}
		res = ssh_session_update_known_hosts(ssh_sess);
		if (res == SSH_ERROR) {
			mge_errno = MGE_SSH;
			res = -mge_errno;
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "Error writing "
			       "known hosts - %s",
			       ssh_get_error(ssh_sess));
			goto exit_2;
		}
		break;
	case SSH_KNOWN_HOSTS_ERROR:
		mge_errno = MGE_SSH;
		res = -mge_errno;
		syslog((int)(LOG_USER | LOG_NOTICE), "Error - %s",
		       ssh_get_error(ssh_sess));
		goto exit_2;
	default:
		mge_errno = MGE_SSH;
		res = -mge_errno;
		syslog((int)(LOG_USER | LOG_NOTICE), "Unknown SSH error.");
		goto exit_2;
	}
	res = 0;

exit_2:
	ssh_string_free_char(hexa);
exit_1:
	ssh_clean_pubkey_hash(&hash);
exit_0:
	ssh_key_free(serv_key);

	return res;
}

/*
 * Try authentication methods sequentially.
 */
static int try_auth_methods_seq(void)
{
	int method, res;

	res = ssh_userauth_none(ssh_sess, NULL);
	if (res == SSH_AUTH_SUCCESS)
		return 0;
	if (res == SSH_AUTH_ERROR)
		goto err_exit;

	method = ssh_userauth_list(ssh_sess, NULL);

	if ((unsigned int)method & SSH_AUTH_METHOD_PUBLICKEY) {
		res = ssh_userauth_publickey_auto(ssh_sess, NULL, NULL);
		if (res == SSH_AUTH_SUCCESS)
			return 0;
	}
	if ((unsigned int)method & SSH_AUTH_METHOD_INTERACTIVE) {
		res = authenticate_kbdint();
		if (res == SSH_AUTH_SUCCESS)
			return 0;
	}
	if ((unsigned int)method & SSH_AUTH_METHOD_PASSWORD) {
		res = authenticate_password();
		if (res == SSH_AUTH_SUCCESS)
			return 0;
	}
	if ((unsigned int)method & SSH_AUTH_METHOD_NONE) {
		res = ssh_userauth_none(ssh_sess, NULL);
		if (res == SSH_AUTH_SUCCESS)
			return 0;
	}
err_exit:
	mge_errno = MGE_SSH;
	syslog((int)(LOG_USER | LOG_NOTICE), "SSH authentication error.");
	return -mge_errno;
}

/*
 * Authenticate interactively.
 */
static int authenticate_kbdint(void)
{
	unsigned int iprompt;
	int nprompts, res;
	const char *inst, *name, *prompt;
	char ans[128], echo, *ptr;

	res = ssh_userauth_kbdint(ssh_sess, NULL, NULL);
	while (res == SSH_AUTH_INFO) {
		name = ssh_userauth_kbdint_getname(ssh_sess);
		inst = ssh_userauth_kbdint_getinstruction(ssh_sess);
		nprompts = ssh_userauth_kbdint_getnprompts(ssh_sess);
		if (strlen(name) > 0)
			printf("%s\n", name);
		if (strlen(inst) > 0)
			printf("%s\n", inst);
		for (iprompt = 0; iprompt < (unsigned int)nprompts; iprompt++) {
			prompt = ssh_userauth_kbdint_getprompt(ssh_sess,
							       iprompt, &echo);
			if (echo) {
				printf("%s", prompt);
				if (fgets(ans, sizeof(ans), stdin) == NULL) {
					mge_errno = MGE_SSH;
					return -mge_errno;
				}
				ans[sizeof(ans) - 1] = '\0';
				if ((ptr = strchr(ans, '\n')) != NULL)
					*ptr = '\0';
				if (ssh_userauth_kbdint_setanswer(ssh_sess,
								  iprompt, ans)
				    < 0) {
					mge_errno = MGE_SSH;
					return -mge_errno;
				}
			} else {
				ptr = getpass(prompt);
				if (ssh_userauth_kbdint_setanswer(ssh_sess,
								  iprompt, ptr)
				    < 0) {
					mge_errno = MGE_SSH;
					return -mge_errno;
				}
			}
		}
		res = ssh_userauth_kbdint(ssh_sess, NULL, NULL);
	}
	if (res != SSH_AUTH_SUCCESS) {
		mge_errno = MGE_SSH;
		return -mge_errno;
	}
	return 0;
}

/*
 * Authenticate via password.
 */
static int authenticate_password(void)
{
	int res;
	char *password;

	password = getpass("Enter your password: ");
	res = ssh_userauth_password(ssh_sess, NULL, password);
	if (res != SSH_AUTH_SUCCESS) {
		mge_errno = MGE_SSH;
		return -mge_errno;
	}
	return 0;
}

/*
 * Setup tunnel to swocserver daemon.
 */
static int direct_forwarding(void)
{
	int res;

	fwd_chan = ssh_channel_new(ssh_sess);
	if (fwd_chan == NULL)
		goto err_exit;

	res = ssh_channel_open_forward(fwd_chan, server, srvportno, "localhost",
				       sshportno);
	if (res == SSH_OK)
		return 0;

	ssh_channel_free(fwd_chan);

err_exit:
	mge_errno = MGE_SSH;
	syslog((int)(LOG_USER | LOG_NOTICE), "SSH tunnel authentication "
					     "error.");
	return -mge_errno;
}

/*
 * Relay data from local port through the tunnel and back.
 * @param arg Required argument using pthread_create. Here it is always NULL.
 */
static void *relay_data(__attribute__((unused)) void *arg)
{
	int r, res;
	ssize_t n;
	char sock_buf[SOCK_BUF_SIZE];
	char *ret_buf;
	int accsockfd;
	struct sockaddr_in cli_addr;
	socklen_t clilen;
	clilen = sizeof(cli_addr);

	accsockfd = accept(ssh_sock, (struct sockaddr *)&cli_addr, &clilen);
	if (accsockfd < 0)
		goto err_exit_0;

	memset(sock_buf, '\0', sizeof(sock_buf));

	while ((n = recv(accsockfd, sock_buf, sizeof(sock_buf), 0)) != 0) {
		if (n < 0)
			goto err_exit_1;

		res = ssh_channel_write(fwd_chan, sock_buf, (unsigned int)n);
		if (res == SSH_ERROR)
			goto err_exit_1;

		n = ssh_channel_poll_timeout(fwd_chan, SSH_CHAN_POLL_TIMEOUT,
					     0);
		if (n == SSH_ERROR)
			goto err_exit_1;

		ret_buf = mg_realloc(NULL, (size_t)n);
		if (ret_buf == NULL)
			goto err_exit_1;

		r = ssh_channel_read(fwd_chan, ret_buf, (unsigned int)n, 0);
		if (r == SSH_ERROR)
			goto err_exit_2;

		n = send(accsockfd, ret_buf, (size_t)r, 0);
		if (n < 0)
			goto err_exit_2;

		free(ret_buf);
	}
	res = close_sock(&accsockfd);
	if (res)
		goto err_exit_0;
	return ((void *)&relay_data_success);

err_exit_2:
	free(ret_buf);

err_exit_1:
	close_sock(&accsockfd);

err_exit_0:
	mge_errno = MGE_SSH;
	syslog((int)(LOG_USER | LOG_NOTICE), "SSH tunnel data relay error.");
	return ((void *)&relay_data_failure);
}
