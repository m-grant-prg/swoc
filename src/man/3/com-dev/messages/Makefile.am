#########################################################################
#									#
# Author: Copyright (C) 2016-2019, 2021, 2023  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswoccommon manual pages	#
# from source.								#
#									#
#########################################################################


exch_msg.3 parse_msg.3 send_outgoing_msg.3: Makefile
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

exch_msg.3: $(srcdir)/exch_msg.3.man.in
parse_msg.3: $(srcdir)/parse_msg.3.man.in
send_outgoing_msg.3: $(srcdir)/send_outgoing_msg.3.man.in

man_MANS = exch_msg.3 parse_msg.3 send_outgoing_msg.3

EXTRA_DIST = $(srcdir)/exch_msg.3.man.in $(srcdir)/parse_msg.3.man.in \
	$(srcdir)/send_outgoing_msg.3.man.in

CLEANFILES = *.tmp exch_msg.3 parse_msg.3 send_outgoing_msg.3

