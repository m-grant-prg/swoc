/* **********************************************************************
 *									*
 * Source: main.c							*
 * Author: Mark Grant							*
 *									*
 * Purpose:								*
 *	Test program for ssh in libswoccommon shared library.		*
 * 									*
 * Released under the GPLv3 only.					*
 * SPDX-License-Identifier: GPL-3.0					*
 *									*
 ************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mge-errno.h>
#include <libswoccommon.h>


/*
 * ssh test program.
 */
int main(int argc, char** argv)
{
	int prog_error = 0;
	char *outgoing_msg = "swocserver,status;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;

	pollint = 10;
	ssh = 1;
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
