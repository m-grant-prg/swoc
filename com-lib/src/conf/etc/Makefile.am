#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: com-lib/src/conf/etc/Makefile.am				#
# Author: Copyright (C) 2017-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
# AutoMake script file to install swoc server & client common		#
# configuration file.							#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 02/10/2017	MG	1.0.0	Initial release.			#
# 12/01/2018	MG	1.0.1	Add SPDX license tags to source files.	#
#									#
#########################################################################


confdir = $(sysconfdir)

swoc.conf: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(edit) $${srcdir}$(@F).in > $@.tmp
	mv $@.tmp $@
swoc.conf: $(srcdir)/swoc.conf.in

conf_DATA = swoc.conf

EXTRA_DIST = $(srcdir)/swoc.conf.in

CLEANFILES = *.tmp swoc.conf
