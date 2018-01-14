#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: com-lib/src/man/5/Makefile.am				#
# Author: Copyright (C) 2017-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate SWOC config file manual	#
# pages from source.							#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 17/10/2017	MG	1.0.1	First release.				#
# 12/01/2018	MG	1.0.2	Add SPDX license tags to source files.	#
#									#
#########################################################################


swoc.conf.5: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	$(edit) $(srcdir)/$(@F).man.in > $@.tmp
	txt2manwrap $(@F) 5 "Server Wait on Client Manual" $@.tmp $@
	rm -f $@.tmp

swoc.conf.5: $(srcdir)/swoc.conf.5.man.in

man_MANS = swoc.conf.5

EXTRA_DIST = $(srcdir)/swoc.conf.5.man.in

CLEANFILES = *.tmp swoc.conf.5
