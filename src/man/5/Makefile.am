#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: srv-prg/src/man/5/Makefile.am				#
# Author: Copyright (C) 2017-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate SWOCServerd config file manual	#
# pages from source.							#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 19/05/2017	MG	1.0.0	First release.				#
# 12/01/2018	MG	1.0.1	Add SPDX license tags to source files.	#
#									#
#########################################################################


swocserverd.conf.5: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	$(edit) $(srcdir)/$(@F).man.in > $@.tmp
	txt2manwrap $(@F) 5 "SWOCServer Library Manual" $@.tmp $@
	rm -f $@.tmp

swocserverd.conf.5: $(srcdir)/swocserverd.conf.5.man.in

man_MANS = swocserverd.conf.5

EXTRA_DIST = $(srcdir)/swocserverd.conf.5.man.in

CLEANFILES = *.tmp swocserverd.conf.5
