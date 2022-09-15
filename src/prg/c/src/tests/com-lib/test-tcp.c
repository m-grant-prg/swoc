/* **********************************************************************
 *									*
 * Source: test-tcp.c							*
 * Author: Mark Grant							*
 *									*
 * Purpose:								*
 *	Test program for tcp in libswoccommon shared library.		*
 * 									*
 * Released under the GPLv3 only.					*
 * SPDX-License-Identifier: GPL-3.0-only				*
 *									*
 ************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libswoccommon.h>

/*
 * tcp test program.
 */
int main(void)
{
	int prog_error = 0;
	int recv_socket, send_socket;
	char *srv = "localhost";
	int portno = 62525;

	printf("Recv socket.\n");
	prog_error = prep_recv_sock(&recv_socket, &portno);

	if (prog_error)
		printf("prep_recv_sock error - %i\n", prog_error);

	if (prog_error)
		exit(prog_error);

	printf("Send socket.\n");
	portno = 62522;
	prog_error = init_conn(&send_socket, &portno, srv);

	if (prog_error)
		printf("init_conn error - %i\n", prog_error);

	printf("Close Send socket.\n");
	if (!prog_error)
		prog_error = close_sock(&send_socket);

	if (prog_error)
		printf("close_sock send error - %i\n", prog_error);

	printf("Close Recv socket.\n");
	if (!prog_error)
		prog_error = close_sock(&recv_socket);

	if (prog_error)
		printf("close_sock recv error - %i\n", prog_error);

	portno = 62525;

	printf("Re-open Recv socket.\n");
	prog_error = prep_recv_sock(&recv_socket, &portno);

	if (prog_error)
		printf("prep_recv_sock error - %i\n", prog_error);

	printf("Close Recv socket.\n");
	if (!prog_error)
		prog_error = close_sock(&recv_socket);

	if (prog_error)
		printf("close_sock recv error - %i\n", prog_error);

	exit(prog_error);
}
