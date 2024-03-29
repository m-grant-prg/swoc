/* **********************************************************************
 *									*
 * Source: test-messages.c						*
 * Author: Mark Grant							*
 *									*
 * Purpose:								*
 *	Test program for messages in libswoccommon shared library.	*
 * 									*
 * Released under the GPLv3 only.					*
 * SPDX-License-Identifier: GPL-3.0-only				*
 *									*
 ************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libmgec/mge-errno.h>
#include <swoc/libswoccommon.h>

/*
 * messages test program.
 */
int main(void)
{
	int prog_error = 0;
	const char *outgoing_msg = "swocserver,status;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;

	pollint = 10;
	ssh = 0;
	strcpy(server, "localhost");
	srvportno = 62522;
	sshportno = 62521;
	strcpy(sshuser, "swoc");

	printf("exch_msg.\n");
	prog_error = exch_msg(outgoing_msg, om_length, msg);

	printf("prog_error is %i\n", prog_error);
	if (prog_error)
		fprintf(stderr, "%s\n", mge_strerror(mge_errno));
	else
		printf("msg->message is %s\n", msg->message);
	clear_msg(msg, ';', ',');

	exit(prog_error);
}
