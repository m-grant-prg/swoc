#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: srv-prg/source/conf/etc/Makefile.am			#
# Author: Copyright (C) 2017-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
# AutoMake script file to install swocserverd configuration files.	#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 19/05/2017	MG	1.0.0	Initial release.			#
# 12/01/2018	MG	1.0.1	Add SPDX license tags to source files.	#
#									#
#########################################################################


confdir = $(sysconfdir)

swocserverd.conf: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(edit) $${srcdir}$(@F).in > $@.tmp
	mv $@.tmp $@
swocserverd.conf: $(srcdir)/swocserverd.conf.in

conf_DATA = swocserverd.conf

EXTRA_DIST = $(srcdir)/swocserverd.conf.in

CLEANFILES = *.tmp swocserverd.conf
