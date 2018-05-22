#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: cli-dev/src/man/3/Makefile.am				#
# Author: Copyright (C) 2016-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswocclient manual pages	#
# from source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 08/06/2016	MG	1.0.1	Initial release.		.	#
# 10/06/2016	MG	1.0.2	Change to new naming convention.	#
# 06/02/2017	MG	1.0.3	Add page for new swc_client_wait	#
#				function.				#
# 24/04/2017	MG	1.0.4	The output of the $(edit) was being	#
#				ignored.				#
#				Change source file extension to .man.in	#
# 12/01/2018	MG	1.0.5	Add SPDX license tags to source files.	#
# 13/01/2018	MG	1.0.6	Determine the manual section from the	#
#				target suffix. If the suffix is blank	#
#				(foo.) or if there is no suffix (foo)	#
#				then error out.				#
# 02/05/2018	MG	1.0.7	Add support for client block list.	#
# 18/05/2018	MG	1.0.8	Add swc_show_srv_block_status.3		#
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
