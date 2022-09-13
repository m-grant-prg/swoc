#########################################################################
#									#
# Script ID: src/prg/c/pkg-config/srv-dev/Makefile.am			#
# Author: Copyright (C) 2016-2019, 2021, 2022  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswocserver pkgconfig from	#
# source.								#
#									#
#########################################################################

#########################################################################
# 18/05/2019	MG	1.0.3	Merge sub-projects into one.		#
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 27/05/2016	MG	1.0.1	First release.				#
# 12/01/2018	MG	1.0.2	Add SPDX license tags to source files.	#
# 19/05/2019	MG	1.0.3	Merge sub-projects into one.		#
#				Remove shebang.				#
# 08/12/2021	MG	1.0.4	Tighten SPDX tag.			#
# 13/09/2022	MG	1.0.5	Perform variable substitution here	#
#				rather than with AC_CONFIG_FILES.	#
#									#
#########################################################################


libswocserver.pc: Makefile
	mkdir -p $(@D)
	rm -f $@
	$(edit) $(srcdir)/$(@F).in > $@

libswocserver.pc: $(srcdir)/libswocserver.pc.in

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libswocserver.pc

EXTRA_DIST = $(srcdir)/libswocserver.pc.in

CLEANFILES = libswocserver.pc

