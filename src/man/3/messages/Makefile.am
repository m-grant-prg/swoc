#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: com-dev/src/man/3/messages/Makefile.am			#
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
# 20/12/2016	MG	1.0.1	Initial release.			#
# 23/04/2017	MG	1.0.2	Correct $(edit) output was being	#
#				ignored.				#
#				Rename source file extensions .man.in	#
# 12/01/2018	MG	1.0.3	Add SPDX license tags to source files.	#
#									#
#########################################################################


exch_msg.3 parse_msg.3 send_outgoing_msg.3: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	$(edit) $(srcdir)/$(@F).man.in > $@.tmp
	txt2manwrap $(@F) 3 "SWOCCommon Library Manual" $@.tmp $@
	rm -f $@.tmp

exch_msg.3: $(srcdir)/exch_msg.3.man.in
parse_msg.3: $(srcdir)/parse_msg.3.man.in
send_outgoing_msg.3: $(srcdir)/send_outgoing_msg.3.man.in

man_MANS = exch_msg.3 parse_msg.3 send_outgoing_msg.3

EXTRA_DIST = $(srcdir)/exch_msg.3.man.in $(srcdir)/parse_msg.3.man.in \
	$(srcdir)/send_outgoing_msg.3.man.in

CLEANFILES = *.tmp exch_msg.3 parse_msg.3 send_outgoing_msg.3
