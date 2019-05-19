#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: src/man/8/srv-prg/Makefile.am				#
# Author: Copyright (C) 2015-2019  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate SWOCServer manual pages from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 22/06/2015	MG	1.0.1	First release.				#
# 27/09/2015	MG	1.0.2	Change to conform to AutoTools Template	#
#				version 1.0.4.				#
# 09/01/2017	MG	1.0.3	Move to section 8 and add daemon page.	#
# 22/04/2017	MG	1.0.4	The resulting .tmp file from the	#
#				$(edit) was being ignored.		#
#				Change man page source file extensions	#
#				to .man.in				#
# 12/01/2018	MG	1.0.5	Add SPDX license tags to source files.	#
# 16/01/2018	MG	1.0.6	Determine the manual section from the	#
#				target suffix. If the suffix is blank	#
#				(foo.) or if there is no suffix (foo)	#
#				then error out.				#
# 18/05/2019	MG	1.0.7	Merge sub-projects into one.		#
#									#
#########################################################################


swocserver.8 swocserverd.8: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	$(edit) $(srcdir)/$(@F).man.in > $@.tmp
	targfile=$(@F);	\
	if [[ $$targfile == *"."* ]]; then \
		if [[ $${targfile##*.} == "" ]]; then \
			echo "Source file suffix used as section but empty."; \
			exit 1; \
		else \
			section=$${targfile##*.}; \
		fi; \
	else \
		echo "Source file suffix used as section but no dot & empty."; \
		exit 1; \
	fi; \
	txt2manwrap $(@F) $$section "Server Wait on Client Manual" $@.tmp $@
	rm -f $@.tmp

swocserver.8: $(srcdir)/swocserver.8.man.in
swocserverd.8: $(srcdir)/swocserverd.8.man.in

man_MANS = swocserver.8 swocserverd.8

EXTRA_DIST = $(srcdir)/swocserver.8.man.in $(srcdir)/swocserverd.8.man.in

CLEANFILES = *.tmp swocserver.8 swocserverd.8

