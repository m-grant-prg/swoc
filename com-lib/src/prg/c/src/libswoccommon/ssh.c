/**
 * @file com-lib/src/prg/c/src/libswoccommon/ssh.c
 *
 * SSH connection processing functions.
 *
 * Covers tunnel creation and destruction including all authentication. Creates
 * a seperate thread for data relay through the tunnel.
 *
 * @author Copyright (C) 2017-2018  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.3 ==== 15/08/2018_
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
 *									*
 ************************************************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <syslog.h>
#include <pthread.h>
#include <libssh/libssh.h>

#include <mge-errno.h>
#include <mgememory.h>
#include <libswoccommon.h>


static int verify_knownhost(void);
static int try_auth_methods_seq(void);
static int authenticate_kbdint(void);
static int authenticate_password(void);
static int direct_forwarding(void);
static void *relay_data(void *arg);


static ssh_session ssh_sess;
static ssh_channel fwd_chan;
static int ssh_sock;
static pthread_t relay_thread;


/**
 * Establish SSH connection.
 * Create session, connect to server, create a tunnel and spawn a thread to
 * relay data through the tunnel.
 * On error mge_errno will be set.
 * @return 0 on success, -1 on error.
 */
int open_ssh_tunnel(void)
{
	int res;
	void *arg = NULL;

	ssh_sess = ssh_new();
	if (ssh_sess == NULL) {
		mge_errno = MGE_SSH;
		syslog((int) (LOG_USER | LOG_NOTICE), "Error creating SSH "
			"session.");
		return -1;
	}

	ssh_options_set(ssh_sess, SSH_OPTIONS_USER, sshuser);
	ssh_options_set(ssh_sess, SSH_OPTIONS_HOST, server);

	res = ssh_connect(ssh_sess);
	if (res != SSH_OK) {
		mge_errno = MGE_SSH;
		syslog((int) (LOG_USER | LOG_NOTICE), "Error connecting to %s: "
			"%s", server, ssh_get_error(ssh_sess));
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
		syslog((int) (LOG_USER | LOG_NOTICE), "Error creating thread. "
			"%s", mge_strerror(mge_errno));
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
	return -1;
}

/**
 * Disconnect and close an SSH session.
 * Join data relay thread, free channel and disconnect session.
 * On error mge_errno will be set.
 * @return 0 on success, -1 on failure.
 */
int close_ssh_tunnel(void)
{
	int res;
	void *retval = NULL;

	sav_errno = pthread_join(relay_thread, &retval);
	if (sav_errno) {
		mge_errno = MGE_ERRNO;
		syslog((int) (LOG_USER | LOG_NOTICE), "Error joining thread. "
			"%s.", mge_strerror(mge_errno));
		res = -1;
		goto err_exit_1;
	}
	if (retval == NULL) {
		mge_errno = MGE_SSH;
		syslog((int) (LOG_USER | LOG_NOTICE), "Error allocating memory "
			"in relay_data - %s.", mge_strerror(mge_errno));
		res = -1;
		goto err_exit_1;
	}
	mge_errno = *(int *)retval;
	if (mge_errno) {
		syslog((int) (LOG_USER | LOG_NOTICE), "Error relaying data - "
			"%s.", mge_strerror(mge_errno));
		res = -1;
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
	free(retval);
	ssh_disconnect(ssh_sess);
	ssh_free(ssh_sess);
	return res;
}

/*
 * Check if the server is in the known hosts file, if not try to add it.
 */
static int verify_knownhost(void)
{
	int res;
	int state;
	size_t hlen;
	ssh_key serv_key;
	unsigned char *hash = NULL;
	char *hexa = NULL;
	char buf[10];

	state = ssh_is_server_known(ssh_sess);

	res = ssh_get_publickey(ssh_sess, &serv_key);
	if (res == SSH_ERROR) {
		mge_errno = MGE_SSH;
		syslog((int) (LOG_USER | LOG_NOTICE), "Error retrieving server "
			"public key - %s", ssh_get_error(ssh_sess));
		return -1;
	}

	/*
	 * The following code snippet would be a replacement for the above
	 * snippet and appears to be valid for version 0.7.6. At least it is in
	 * the master branch after the 0.7.5 tag and before any 0.7.6 tag.
	ssh_errno = ssh_get_server_publickey(ssh_sess, &serv_key);
	if (ssh_errno == SSH_ERROR) {
		fprintf(stderr, "Error retrieving server public key.\n");
		mge_errno = MGE_SSH;
		return mge_errno;
	}
	 */


	res = ssh_get_publickey_hash(serv_key, SSH_PUBLICKEY_HASH_SHA1,
			&hash, &hlen);
	if (res) {
		res = -1;
		mge_errno = MGE_SSH;
		syslog((int) (LOG_USER | LOG_NOTICE), "Error retrieving server "
			"public key hash - %s", ssh_get_error(ssh_sess));
		goto exit_0;
	}


	switch (state) {
	case SSH_SERVER_KNOWN_OK:
		break; /* ok */
	case SSH_SERVER_KNOWN_CHANGED:
		res = -1;
		mge_errno = MGE_SSH;
		syslog((int) (LOG_USER | LOG_NOTICE), "Host key for server "
			"changed. Stopping for security reasons.");
		goto exit_1;
	case SSH_SERVER_FOUND_OTHER:
		res = -1;
		mge_errno = MGE_SSH;
		syslog((int) (LOG_USER | LOG_NOTICE), "The host key for this "
			"server was not found but another type of key exists. "
			"Stopping for security reasons.");
		goto exit_1;
	case SSH_SERVER_FILE_NOT_FOUND:
		fprintf(stderr, "Could not find known host file.\n");
		fprintf(stderr, "If you accept the host key here, the file "
			"will be automatically created.\n");
		/* fallthru deliberate. */
	case SSH_SERVER_NOT_KNOWN:
		hexa = ssh_get_hexa(hash, hlen);
		if (hexa == NULL) {
			res = -1;
			mge_errno = MGE_SSH;
			syslog((int) (LOG_USER | LOG_NOTICE), "Error retrieving"
				" hex key.");
			goto exit_1;
		}
		fprintf(stderr,"The server is unknown. Do you trust the host "
			"key?\n");
		fprintf(stderr, "Public key hash: %s\n", hexa);

		if (fgets(buf, sizeof(buf), stdin) == NULL) {
			res = -1;
			mge_errno = MGE_SSH;
			fprintf(stderr, "Invalid input.\n");
			syslog((int) (LOG_USER | LOG_NOTICE), "Invalid input.");
			goto exit_2;
		}
		if (strncasecmp(buf, "yes", 3) != 0) {
			res = -1;
			mge_errno = MGE_SSH;
			syslog((int) (LOG_USER | LOG_NOTICE), "User did not "
				"answer yes to trust this key question");
			goto exit_2;
		}
		res = ssh_write_knownhost(ssh_sess);
		if (res == SSH_ERROR) {
			res = -1;
			mge_errno = MGE_SSH;
			syslog((int) (LOG_USER | LOG_NOTICE), "Error writing "
			"known hosts - %s", ssh_get_error(ssh_sess));
			goto exit_2;
		}
		break;
	case SSH_SERVER_ERROR:
		res = -1;
		mge_errno = MGE_SSH;
		syslog((int) (LOG_USER | LOG_NOTICE), "Error - %s",
			ssh_get_error(ssh_sess));
		goto exit_2;
	default:
		res = -1;
		mge_errno = MGE_SSH;
		syslog((int) (LOG_USER | LOG_NOTICE), "Unknown SSH error.");
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

	if (method & SSH_AUTH_METHOD_PUBLICKEY) {
		res = ssh_userauth_publickey_auto(ssh_sess, NULL, NULL);
		if (res == SSH_AUTH_SUCCESS)
			return 0;
	}
	if (method & SSH_AUTH_METHOD_INTERACTIVE) {
		res = authenticate_kbdint();
		if (res == SSH_AUTH_SUCCESS)
			return 0;
	}
	if (method & SSH_AUTH_METHOD_PASSWORD) {
		res = authenticate_password();
		if (res == SSH_AUTH_SUCCESS)
			return 0;
	}
	if (method & SSH_AUTH_METHOD_NONE){
		res = ssh_userauth_none(ssh_sess, NULL);
		if (res == SSH_AUTH_SUCCESS)
			return 0;
	}
err_exit:
	mge_errno = MGE_SSH;
	syslog((int) (LOG_USER | LOG_NOTICE), "SSH authentication error.");
	return -1;
}

/*
 * Authenticate interactively.
 */
static int authenticate_kbdint(void)
{
	int iprompt, nprompts, res;
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
		for (iprompt = 0; iprompt < nprompts; iprompt++) {
			prompt = ssh_userauth_kbdint_getprompt(ssh_sess,
					iprompt, &echo);
			if (echo) {
				printf("%s", prompt);
				if (fgets(ans, sizeof(ans), stdin) == NULL)
					return -1;
				ans[sizeof(ans) - 1] = '\0';
				if ((ptr = strchr(ans, '\n')) != NULL)
					*ptr = '\0';
				if (ssh_userauth_kbdint_setanswer(ssh_sess,
					iprompt, ans) < 0)
					return -1;
			} else {
				ptr = getpass(prompt);
				if (ssh_userauth_kbdint_setanswer(ssh_sess,
					iprompt, ptr) < 0)
					return -1;
			}
		}
		res = ssh_userauth_kbdint(ssh_sess, NULL, NULL);
	}
	return res ? -1 : 0;
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
	return res ? -1 : 0;
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
	syslog((int) (LOG_USER | LOG_NOTICE), "SSH tunnel authentication "
		"error.");
	return -1;
}

/*
 * Relay data from local port through the tunnel and back.
 * @param arg Required argument using pthread_create. Here it is always NULL.
 */
static void *relay_data(void *arg)
{
	int res;
	int *retval;
	ssize_t n, r;
	char sock_buf[SOCK_BUF_SIZE];
	char *ret_buf;

	int accsockfd;
	struct sockaddr_in cli_addr;
	socklen_t clilen;
	clilen = sizeof(cli_addr);

	retval = malloc(sizeof(int));
	if (retval == NULL)
		return retval;

	accsockfd = accept(ssh_sock, (struct sockaddr *) &cli_addr, &clilen);
	if (accsockfd < 0)
		goto err_exit_0;

	bzero(sock_buf, sizeof(sock_buf));

	while ((n = recv(accsockfd, sock_buf, sizeof(sock_buf), 0)) != 0) {
		if (n < 0)
			goto err_exit_1;

		res = ssh_channel_write (fwd_chan, sock_buf, n);
		if ( res == SSH_ERROR )
			goto err_exit_1;

		n = ssh_channel_poll_timeout(fwd_chan, SSH_CHAN_POLL_TIMEOUT,
				0);
		if(n == SSH_ERROR)
			goto err_exit_1;

		ret_buf = mg_realloc(NULL, (size_t) n);
		if (ret_buf == NULL)
			goto err_exit_1;

		r = ssh_channel_read(fwd_chan, ret_buf, n, 0);
		if(r == SSH_ERROR)
			goto err_exit_2;

		n = send(accsockfd, ret_buf, r, 0);
		if (n < 0)
			goto err_exit_2;

		free(ret_buf);
	}
	*retval = close_sock(&accsockfd);
	return retval;

err_exit_2:
	free(ret_buf);

err_exit_1:
	close_sock(&accsockfd);

err_exit_0:
	*retval = -1;
	syslog((int) (LOG_USER | LOG_NOTICE), "SSH tunnel data relay error.");
	return retval;
}
