#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: com-lib/source/progs/bash/Makefile.am			#
# Author: Copyright (C) 2017-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate swoc.conf setup scripts from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 03/10/2017	MG	1.0.0	First release.				#
# 12/01/2018	MG	1.0.1	Add SPDX license tags to source files.	#
#									#
#########################################################################


setup.sh: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(edit) $${srcdir}$(@F).in >$@.tmp
	mv $@.tmp $@
	chmod a+x $@
setup.sh: $(srcdir)/setup.sh.in

pkglibexec_SCRIPTS = setup.sh

EXTRA_DIST = $(srcdir)/setup.sh.in

CLEANFILES = *.tmp setup.sh
