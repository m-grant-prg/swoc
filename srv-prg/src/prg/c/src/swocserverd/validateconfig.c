/**
 * @file srv-prg/src/prg/c/src/swocserverd/validateconfig.c
 *
 * To parse and validate the config file.
 *
 * @author Copyright (C) 2017  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.6 ==== 18/11/2017_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 27/05/2017	MG	1.0.1	First release.				*
 * 03/06/2017	MG	1.0.2	Only validate and load new port number	*
 *				parameters. Not yet used in further	*
 *				processing. Still just uses one non-SSL	*
 *				port.					*
 * 04/06/2017	MG	1.0.3	Tidy up unnecessary include statements.	*
 * 09/09/2017	MG	1.0.4	Change what is done so far from SSL to	*
 *				TLS.					*
 * 29/10/2017	MG	1.0.5	Remove references to TLS. Security now	*
 *				implemented from client side via SSH	*
 *				tunnelling.				*
 * 18/11/2017	MG	1.0.6	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 *									*
 ************************************************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <ctype.h>
#include <errno.h>

#include <configmake.h>

#include <mge-errno.h>
#include <configfile.h>
#include <swocserverd.h>


static int validateportnos(const struct confsection *ps);


/**
 * Parse and validate the config file.
 * On error mge_errno is set.
 * @return 0 on success, non-zero on failure.
 */
int swsd_validate_config(void)
{
	swsd_err = 0;
	/* Expand config file full path. */
	char *configfile = SYSCONFDIR"/swocserverd.conf";
	struct confsection *psections;

	/* Set up config file parameters. */
	int nsections = 1;
	psections = malloc((sizeof(struct confsection)) * nsections);
	if (psections == NULL) {
		sav_errno = errno;
		mge_errno = MGE_ERRNO;
		return mge_errno;
	}

	psections[0] = (struct confsection) {"Ports", 1, 0, {
			{"portno-0", 1, 0, ""},
			{"portno-1", 0, 0, ""},
			{"portno-2", 0, 0, ""},
			{"portno-3", 0, 0, ""},
			{"portno-4", 0, 0, ""}
			}};

	/* Parse config file. */
	swsd_err = parsefile(psections, nsections, configfile);
	if (swsd_err)
		goto exit;

	/* Validate config file params. */
	swsd_err = validateportnos(psections);

exit:
	free(psections);
	return swsd_err;
}

/*
 * Ensure config file params contain vailid port numbers.
 */
static int validateportnos(const struct confsection *ps)
{
	int k, p = 0, x;

	if (!(ps->present))
		return 0;
	for (k = 0; k < 5; k++) {
		if (!(ps->keys[k].present))
			continue;
		x = 0;
		if (strlen(ps->keys[k].value) != (size_t) 5)
			goto port_error;
		while ((isdigit(ps->keys[k].value[x])) &&
			(x < strlen(ps->keys[k].value)))
			x++;
		if (x != strlen(ps->keys[k].value))
			goto port_error;
		(port_spec + p)->portno = atoi(ps->keys[k].value);
		if (((port_spec + p)->portno < 49152)
			|| ((port_spec + p)->portno > 65535))
			goto port_error;
		p++;
	}
	return 0;

port_error:
	mge_errno = MGE_CONFIG_PARAM;
	syslog((int) (LOG_USER | LOG_NOTICE), "Config param portno does not "
		"contain a valid port number - %s", ps->keys[k].value);
	return mge_errno;
}
