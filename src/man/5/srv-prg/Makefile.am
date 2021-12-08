#########################################################################
#									#
# Script ID: src/man/5/srv-prg/Makefile.am				#
# Author: Copyright (C) 2017-2019, 2021  Mark Grant			#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
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
# 16/01/2018	MG	1.0.2	Determine the manual section from the	#
#				target suffix. If the suffix is blank	#
#				(foo.) or if there is no suffix (foo)	#
#				then error out.				#
# 19/05/2019	MG	1.0.3	Merge sub-projects into one.		#
#				Remove shebang.				#
# 07/12/2021	MG	1.0.4	Tighten SPDX tag.			#
#									#
#########################################################################


swocserverd.conf.5: Makefile
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

swocserverd.conf.5: $(srcdir)/swocserverd.conf.5.man.in

man_MANS = swocserverd.conf.5

EXTRA_DIST = $(srcdir)/swocserverd.conf.5.man.in

CLEANFILES = *.tmp swocserverd.conf.5

