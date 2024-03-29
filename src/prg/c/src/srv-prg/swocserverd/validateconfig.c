/**
 * @file src/prg/c/src/srv-prg/swocserverd/validateconfig.c
 *
 * To parse and validate the config file.
 *
 * @author Copyright (C) 2017-2019, 2021-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.0 ==== 26/11/2023_
 */

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include <configmake.h>

#include "internal.h"
#include <libmgec/mge-errno.h>
#include <libmgesysutils/mge-configfile.h>

static int validateportnos(const struct confsection *ps);

/**
 * Parse and validate the config file.
 * On error mge_errno is set.
 * @return 0 on success, non-zero on failure.
 */
int swsd_validate_config(void)
{
	/* Expand config file full path. */
	const char *configfile = SYSCONFDIR "/swocserverd.conf";
	struct confsection *psections;
	int nsections;

	swsd_err = 0;

	/* Set up config file parameters. */
	nsections = 1;
	psections = malloc((sizeof(struct confsection)) * (size_t)nsections);
	if (psections == NULL) {
		sav_errno = errno;
		mge_errno = MGE_ERRNO;
		return mge_errno;
	}

	psections[0] = (struct confsection){ "Ports",
					     1,
					     0,
					     { { "portno-0", 1, 0, "" },
					       { "portno-1", 0, 0, "" },
					       { "portno-2", 0, 0, "" },
					       { "portno-3", 0, 0, "" },
					       { "portno-4", 0, 0, "" } } };

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
	int k, p = 0;
	size_t x;

	if (!(ps->present))
		return 0;
	for (k = 0; k < 5; k++) {
		if (!(ps->keys[k].present))
			continue;
		x = 0;
		if (strlen(ps->keys[k].value) != (size_t)5)
			goto port_error;
		while ((isdigit(ps->keys[k].value[x]))
		       && (x < strlen(ps->keys[k].value)))
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
	syslog((int)(LOG_USER | LOG_NOTICE),
	       "Config param portno does not "
	       "contain a valid port number - %s",
	       ps->keys[k].value);
	return mge_errno;
}
