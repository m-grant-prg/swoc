/**
 * @file src/prg/c/src/com-lib/libswoccommon/validateconfig.c
 *
 * To parse and validate the config file.
 *
 * Used for swocserver and swocclient not swocserverd.
 *
 * @author Copyright (C) 2017-2019, 2021  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.6 ==== 10/10/2021_
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
 * 07/03/2018	MG	1.0.3	Remove redundant global variable portno	*
 * 18/05/2019	MG	1.0.4	Merge sub-projects into one.		*
 * 01/06/2019	MG	1.0.5	Trivial type safety improvements.	*
 * 10/10/2021	MG	1.0.6	Use newly internalised common header.	*
 *									*
 ************************************************************************
 */

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <syslog.h>

#include <configmake.h>

#include <configfile.h>
#include <libswoccommon.h>
#include <mge-errno.h>

static int validateconfigfileparams(const struct confsection *);
static int validatepollint(const struct confsection *);
static int validatessh(const struct confsection *ps);
static int validateserver(const struct confsection *);
static int validatesrvportno(const struct confsection *);
static int validatesshportno(const struct confsection *);
static int validatesshuser(const struct confsection *ps);

int pollint;			     /**< Polling interval. */
int ssh;			     /**< Use SSH false == 0, true == 1 */
char server[_POSIX_HOST_NAME_MAX];   /**< Server name. */
int srvportno;			     /**< Server port number. */
int sshportno;			     /**< Local port to use if using SSH. */
char sshuser[_POSIX_LOGIN_NAME_MAX]; /**< Server username for SSH. */

/**
 * Parse and validate the config file.
 * On error mge_errno is set.
 * @return 0 on success, non-zero on failure.
 */
int swcom_validate_config(void)
{
	int swscom_err = 0;
	/* Expand config file full path. */
	char *configfile = SYSCONFDIR "/swoc.conf";
	struct confsection *psections;

	/* Set up config file parameters. */
	int nsections = 3;
	psections = malloc((sizeof(struct confsection)) * (size_t)nsections);
	if (psections == NULL) {
		sav_errno = errno;
		mge_errno = MGE_ERRNO;
		return mge_errno;
	}

	psections[0] = (struct confsection){ "General",
					     1,
					     0,
					     { { "pollint", 1, 0, "" },
					       { "ssh", 1, 0, "" } } };

	psections[1] = (struct confsection){ "Server",
					     1,
					     0,
					     { { "server", 1, 0, "" },
					       { "srvportno", 1, 0, "" } } };

	psections[2] = (struct confsection){ "SSH",
					     1,
					     0,
					     { { "sshportno", 1, 0, "" },
					       { "sshuser", 1, 0, "" } } };

	/* Parse config file. */
	swscom_err = parsefile(psections, nsections, configfile);
	if (swscom_err)
		goto exit;

	/* Validate config file params. */
	swscom_err = validateconfigfileparams(psections);

exit:
	free(psections);
	return swscom_err;
}

/*
 * Validate config file params.
 */
static int validateconfigfileparams(const struct confsection *ps)
{
	int e;
	if ((e = validatepollint(ps)))
		return e;
	if ((e = validatessh(ps)))
		return e;
	if ((e = validateserver(ps)))
		return e;
	if ((e = validatesrvportno(ps)))
		return e;
	if ((e = validatesshportno(ps)))
		return e;
	e = validatesshuser(ps);
	return e;
}

/*
 * Ensure config file param pollint contains a reasonable value, ie under
 * 6 hours.
 */
static int validatepollint(const struct confsection *ps)
{
	size_t x = 0;

	if ((strlen(ps->keys[0].value) < (size_t)1)
	    || (strlen(ps->keys[0].value) > (size_t)5))
		goto poll_error;
	while ((isdigit(ps->keys[0].value[x]))
	       && (x < strlen(ps->keys[0].value)))
		x++;
	if (x != strlen(ps->keys[0].value))
		goto poll_error;
	pollint = atoi(ps->keys[0].value);
	if ((pollint <= 0) || (pollint > 21600))
		goto poll_error;
	return 0;

poll_error:
	mge_errno = MGE_CONFIG_PARAM;
	syslog((int)(LOG_USER | LOG_NOTICE),
	       "Config param pollint does not "
	       "contain a valid polling interval - %s",
	       ps->keys[0].value);
	return mge_errno;
}

/*
 * SSH parameter can be yes or no, case insensitive.
 */
static int validatessh(const struct confsection *ps)
{
	size_t x = 0;
	char tmpanswer[MAX_KEYVAL_LENGTH] = { '\0' };

	while ((tmpanswer[x] = (char)tolower(ps->keys[1].value[x]))
	       && (x < strlen(ps->keys[1].value)))
		x++;
	if (strcmp(tmpanswer, "yes") && strcmp(tmpanswer, "no"))
		goto ssh_error;
	if (!strcmp(tmpanswer, "yes"))
		ssh = 1;
	else
		ssh = 0;
	return 0;

ssh_error:
	mge_errno = MGE_CONFIG_PARAM;
	syslog((int)(LOG_USER | LOG_NOTICE),
	       "Config param ssh does not "
	       "contain yes or no - %s",
	       ps->keys[1].value);
	return mge_errno;
}

/*
 * Ensure the server name is a valid length.
 */
static int validateserver(const struct confsection *ps)
{
	if ((strlen((ps + 1)->keys[0].value) < (size_t)1)
	    || (strlen((ps + 1)->keys[0].value) > sizeof(server))) {
		mge_errno = MGE_CONFIG_PARAM;
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Config param server "
		       "does not contain a valid server name.");
		return mge_errno;
	}
	strcpy(server, (ps + 1)->keys[0].value);
	return 0;
}

/*
 * Ensure config file param srvportno contains a valid port number.
 */
static int validatesrvportno(const struct confsection *ps)
{
	size_t x = 0;

	if (strlen((ps + 1)->keys[1].value) != (size_t)5)
		goto port_error;
	while ((isdigit((ps + 1)->keys[1].value[x]))
	       && (x < strlen((ps + 1)->keys[1].value)))
		x++;
	if (x != strlen((ps + 1)->keys[1].value))
		goto port_error;
	srvportno = atoi((ps + 1)->keys[1].value);
	if ((srvportno < 49152) || (srvportno > 65535))
		goto port_error;
	return 0;

port_error:
	mge_errno = MGE_CONFIG_PARAM;
	syslog((int)(LOG_USER | LOG_NOTICE),
	       "Config param srvportno does not "
	       "contain a valid port number - %s",
	       (ps + 1)->keys[1].value);
	return mge_errno;
}

/*
 * Ensure config file param sshportno contains a valid port number.
 */
static int validatesshportno(const struct confsection *ps)
{
	size_t x = 0;

	if (strlen((ps + 2)->keys[0].value) != (size_t)5)
		goto port_error;
	while ((isdigit((ps + 2)->keys[0].value[x]))
	       && (x < strlen((ps + 2)->keys[0].value)))
		x++;
	if (x != strlen((ps + 2)->keys[0].value))
		goto port_error;
	sshportno = atoi((ps + 2)->keys[0].value);
	if ((sshportno < 49152) || (sshportno > 65535)
	    || (sshportno == srvportno))
		goto port_error;
	return 0;

port_error:
	mge_errno = MGE_CONFIG_PARAM;
	syslog((int)(LOG_USER | LOG_NOTICE),
	       "Config param sshportno does not "
	       "contain a valid port number - %s",
	       (ps + 2)->keys[0].value);
	return mge_errno;
}

/*
 * Ensure the user name is a valid length.
 */
static int validatesshuser(const struct confsection *ps)
{
	if ((strlen((ps + 2)->keys[1].value) < (size_t)1)
	    || (strlen((ps + 2)->keys[1].value) > sizeof(sshuser))) {
		mge_errno = MGE_CONFIG_PARAM;
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Config param sshuser "
		       "does not contain a valid user name.");
		return mge_errno;
	}
	strcpy(sshuser, (ps + 2)->keys[1].value);
	return 0;
}

