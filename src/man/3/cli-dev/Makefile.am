#########################################################################
#									#
# Author: Copyright (C) 2016-2019, 2021, 2023  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswocclient manual pages	#
# from source.								#
#									#
#########################################################################


swc_block.3 swc_client_wait.3 swc_rel_lock.3 swc_reset.3 swc_set_lock.3 \
	swc_show_srv_block_status.3 swc_show_status.3 swc_unblock.3: Makefile
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

swc_block.3: $(srcdir)/swc_block.3.man.in
swc_client_wait.3: $(srcdir)/swc_client_wait.3.man.in
swc_rel_lock.3: $(srcdir)/swc_rel_lock.3.man.in
swc_reset.3: $(srcdir)/swc_reset.3.man.in
swc_set_lock.3: $(srcdir)/swc_set_lock.3.man.in
swc_show_srv_block_status.3: $(srcdir)/swc_show_srv_block_status.3.man.in
swc_show_status.3: $(srcdir)/swc_show_status.3.man.in
swc_unblock.3: $(srcdir)/swc_unblock.3.man.in

man_MANS = swc_block.3 swc_client_wait.3 swc_rel_lock.3 swc_reset.3 \
	swc_set_lock.3 swc_show_srv_block_status.3 swc_show_status.3 \
	swc_unblock.3

EXTRA_DIST = $(srcdir)/swc_block.3.man.in $(srcdir)/swc_client_wait.3.man.in \
	$(srcdir)/swc_rel_lock.3.man.in $(srcdir)/swc_reset.3.man.in \
	$(srcdir)/swc_set_lock.3.man.in \
	$(srcdir)/swc_show_srv_block_status.3.man.in \
	$(srcdir)/swc_show_status.3.man.in $(srcdir)/swc_unblock.3.man.in

CLEANFILES = *.tmp swc_block.3 swc_client_wait.3 swc_rel_lock.3 swc_reset.3 \
	swc_set_lock.3 swc_show_srv_block_status.3 swc_show_status.3 \
	swc_unblock.3

