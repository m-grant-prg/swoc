#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: com-dev/source/man/man3/tcp/Makefile.am			#
# Author: Copyright (C) 2016-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswoccommon manual pages	#
# from source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 18/12/2016	MG	1.0.1	Initial release.			#
# 23/04/2017	MG	1.0.2	Correct $(edit) output was being	#
#				ignored.				#
#				Rename source file extensions .man.in	#
# 12/01/2018	MG	1.0.3	Add SPDX license tags to source files.	#
#									#
#########################################################################


close_sock.3 est_connect.3 init_conn.3 listen_sock.3 prep_recv_sock.3: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	$(edit) $(srcdir)/$(@F).man.in > $@.tmp
	txt2manwrap $(@F) 3 "SWOCCommon Library Manual" $@.tmp $@
	rm -f $@.tmp

close_sock.3: $(srcdir)/close_sock.3.man.in
est_connect.3: $(srcdir)/est_connect.3.man.in
init_conn.3: $(srcdir)/init_conn.3.man.in
listen_sock.3: $(srcdir)/listen_sock.3.man.in
prep_recv_sock.3: $(srcdir)/prep_recv_sock.3.man.in

man_MANS = close_sock.3 est_connect.3 init_conn.3 listen_sock.3 prep_recv_sock.3

EXTRA_DIST = $(srcdir)/close_sock.3.man.in $(srcdir)/est_connect.3.man.in \
		$(srcdir)/init_conn.3.man.in $(srcdir)/listen_sock.3.man.in \
		$(srcdir)/prep_recv_sock.3.man.in

CLEANFILES = *.tmp close_sock.3 est_connect.3 init_conn.3 listen_sock.3 \
		prep_recv_sock.3
