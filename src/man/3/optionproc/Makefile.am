#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: cli-dev/src/man/3/optionproc/Makefile.am			#
# Author: Copyright (C) 2016-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswocclient manual pages	#
# from source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 08/06/2016	MG	1.0.1	Initial release.		.	#
# 10/06/2016	MG	1.0.2	Change to new naming convention.	#
# 06/02/2017	MG	1.0.3	Add page for new swc_client_wait	#
#				function.				#
# 24/04/2017	MG	1.0.4	The output of the $(edit) was being	#
#				ignored.				#
#				Change source file extension to .man.in	#
# 12/01/2018	MG	1.0.5	Add SPDX license tags to source files.	#
#									#
#########################################################################


swc_client_wait.3 swc_rel_lock.3 swc_set_lock.3 swc_show_status.3: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	$(edit) $(srcdir)/$(@F).man.in > $@.tmp
	txt2manwrap $(@F) 3 "Server Wait on Client Manual" $@.tmp $@
	rm -f $@.tmp

swc_client_wait.3: $(srcdir)/swc_client_wait.3.man.in
swc_rel_lock.3: $(srcdir)/swc_rel_lock.3.man.in
swc_set_lock.3: $(srcdir)/swc_set_lock.3.man.in
swc_show_status.3: $(srcdir)/swc_show_status.3.man.in

man_MANS = swc_client_wait.3 swc_rel_lock.3 swc_set_lock.3 swc_show_status.3

EXTRA_DIST = $(srcdir)/swc_client_wait.3.man.in \
	$(srcdir)/swc_rel_lock.3.man.in $(srcdir)/swc_set_lock.3.man.in \
	$(srcdir)/swc_show_status.3.man.in

CLEANFILES = *.tmp swc_client_wait.3 swc_rel_lock.3 swc_set_lock.3 \
	swc_show_status.3
