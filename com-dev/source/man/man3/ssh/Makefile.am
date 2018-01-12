#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: com-dev/source/man/man3/ssh/Makefile.am			#
# Author: Copyright (C) 2017-2018  Mark Grant				#
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
# 18/10/2017	MG	1.0.1	Initial release.			#
# 12/01/2018	MG	1.0.2	Add SPDX license tags to source files.	#
#									#
#########################################################################


close_ssh_tunnel.3 open_ssh_tunnel.3: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	$(edit) $(srcdir)/$(@F).man.in > $@.tmp
	txt2manwrap $(@F) 3 "SWOCCommon Library Manual" $@.tmp $@
	rm -f $@.tmp

close_ssh_tunnel.3: $(srcdir)/close_ssh_tunnel.3.man.in
open_ssh_tunnel.3: $(srcdir)/open_ssh_tunnel.3.man.in

man_MANS = close_ssh_tunnel.3 open_ssh_tunnel.3

EXTRA_DIST = $(srcdir)/close_ssh_tunnel.3.man.in \
		$(srcdir)/open_ssh_tunnel.3.man.in

CLEANFILES = *.tmp close_ssh_tunnel.3 open_ssh_tunnel.3
