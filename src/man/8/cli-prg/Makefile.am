#########################################################################
#									#
# Script ID: src/man/8/cli-prg/Makefile.am				#
# Author: Copyright (C) 2015-2019, 2021  Mark Grant			#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate SWOCClient manual pages from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 08/07/2015	MG	1.0.1	First release.				#
# 27/09/2015	MG	1.0.2	Change to conform to AutoTools Template	#
#				version 1.0.4.				#
# 25/04/2017	MG	1.0.3	Output from $(edit) was being ignored.	#
#				Change source file extension to .man.in	#
#				Move to section 8.			#
# 12/01/2018	MG	1.0.4	Add SPDX license tags to source files.	#
# 15/01/2018	MG	1.0.5	Determine the manual section from the	#
#				target suffix. If the suffix is blank	#
#				(foo.) or if there is no suffix (foo)	#
#				then error out.				#
# 19/05/2019	MG	1.0.6	Merge sub-projects into one.		#
#				Remove shebang.				#
# 07/12/2021	MG	1.0.7	Tighten SPDX tag.			#
#									#
#########################################################################


swocclient.8: Makefile
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

swocclient.8: $(srcdir)/swocclient.8.man.in

man_MANS = swocclient.8

EXTRA_DIST = $(srcdir)/swocclient.8.man.in

CLEANFILES = *.tmp swocclient.8

