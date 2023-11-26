#########################################################################
#									#
# Author: Copyright (C) 2017-2019, 2021, 2023  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate SWOCServerd config file manual	#
# pages from source.							#
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

