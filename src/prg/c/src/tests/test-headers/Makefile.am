#########################################################################
#									#
# Script ID: src/prg/c/src/tests/test-headers/Makefile.am		#
# Author: Copyright (C) 2022  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate the headers test program.	#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 13/09/2022	MG	1.0.1	First release.				#
#									#
#########################################################################


compile-headers.sh: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(edit) $${srcdir}$(@F).in > $@.tmp
	mv $@.tmp $@
	chmod a+x $@
compile-headers.sh: $(srcdir)/compile-headers.sh.in

check_SCRIPTS = compile-headers.sh

EXTRA_DIST = $(srcdir)/compile-headers.sh.in

CLEANFILES = *.tmp *.sh *.h.* *.gch

