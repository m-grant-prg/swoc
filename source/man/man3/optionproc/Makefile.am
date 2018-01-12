#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: srv-dev/source/man/man3/optionproc/Makefile.am		#
# Author: Copyright (C) 2016-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswocserver manual pages	#
# from source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 27/05/2016	MG	1.0.1	Initial release.		.	#
# 12/06/2016	MG	1.0.2	Update with more unique function names.	#
# 17/12/2016	MG	1.0.3	Add sws_end_daemon man page.		#
# 13/02/2017	MG	1.0.4	Add sws_reload_config man page.		#
# 26/04/2017	MG	1.0.5	Output from $(edit) was being ignored.	#
#				Rename source file extensions .man.in	#
# 15/09/2017	MG	1.0.6	Change force_unlock to unlock.		#
# 12/01/2018	MG	1.0.7	Add SPDX license tags to source files.	#
#									#
#########################################################################


sws_end_daemon.3 sws_reload_config.3 sws_server_wait.3 sws_show_status.3 \
	sws_unlock.3: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	$(edit) $(srcdir)/$(@F).man.in > $@.tmp
	txt2manwrap $(@F) 3 "SWOCServer Library Manual" $@.tmp $@
	rm -f $@.tmp

sws_end_daemon.3: $(srcdir)/sws_end_daemon.3.man.in
sws_reload_config.3: $(srcdir)/sws_reload_config.3.man.in
sws_server_wait.3: $(srcdir)/sws_server_wait.3.man.in
sws_show_status.3: $(srcdir)/sws_show_status.3.man.in
sws_unlock.3: $(srcdir)/sws_unlock.3.man.in

man_MANS = sws_end_daemon.3 sws_reload_config.3 sws_server_wait.3 \
	sws_show_status.3 sws_unlock.3

EXTRA_DIST = $(srcdir)/sws_end_daemon.3.man.in \
	$(srcdir)/sws_reload_config.3.man.in \
	$(srcdir)/sws_server_wait.3.man.in $(srcdir)/sws_show_status.3.man.in \
	$(srcdir)/sws_unlock.3.man.in

CLEANFILES = *.tmp sws_end_daemon.3 sws_reload_config.3 sws_server_wait.3 \
	sws_show_status.3 sws_unlock.3
