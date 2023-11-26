#########################################################################
#									#
# Author: Copyright (C) 2015-2019, 2021, 2023  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate SWOCServer manual pages from	#
# source.								#
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

