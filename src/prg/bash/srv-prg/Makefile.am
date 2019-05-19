#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: src/prg/bash/srv-prg/Makefile.am				#
# Author: Copyright (C) 2017-2019  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate swocserverd scripts from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 19/05/2017	MG	1.0.0	First release.				#
# 12/01/2018	MG	1.0.2	Add SPDX license tags to source files.	#
# 23/06/2018	MG	1.0.3	Build setup.sh in AC_CONFIG_FILES.	#
# 27/06/2018	MG	1.0.4	Reverse 1.0.3 as AC_CONFIG_FILES cannot	#
#				be used for the substitution of		#
#				Installation Directory Variables.	#
# 18/05/2019	MG	1.0.5	Merge sub-projects into one.		#
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

