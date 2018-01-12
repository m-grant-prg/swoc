#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: com-dev/source/man/man3/validateconfig/Makefile.am		#
# Author: Copyright (C) 2017-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswoccommon manual pages	#
# from source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 17/10/2017	MG	1.0.1	Initial release.			#
# 12/01/2018	MG	1.0.2	Add SPDX license tags to source files.	#
#									#
#########################################################################


swcom_validate_config.3: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	$(edit) $(srcdir)/$(@F).man.in > $@.tmp
	txt2manwrap $(@F) 3 "SWOCCommon Library Manual" $@.tmp $@
	rm -f $@.tmp

swcom_validate_config.3: $(srcdir)/swcom_validate_config.3.man.in

man_MANS = swcom_validate_config.3

EXTRA_DIST = $(srcdir)/swcom_validate_config.3.man.in

CLEANFILES = *.tmp swcom_validate_config.3
