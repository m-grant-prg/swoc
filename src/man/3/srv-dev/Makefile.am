#########################################################################
#									#
# Author: Copyright (C) 2016-2019, 2021, 2023  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswocserver manual pages	#
# from source.								#
#									#
#########################################################################


sws_cli_block.3 sws_cli_unblock.3 sws_end_daemon.3 sws_release.3 \
	sws_reload_config.3 sws_server_wait.3 sws_show_block_status.3 \
	sws_show_cli_blocklist.3 sws_show_status.3 sws_srv_block.3 \
	sws_srv_unblock.3: Makefile
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
sws_cli_block.3:$(srcdir)/sws_cli_block.3.man.in
sws_cli_unblock.3:$(srcdir)/sws_cli_unblock.3.man.in
sws_end_daemon.3: $(srcdir)/sws_end_daemon.3.man.in
sws_release.3: $(srcdir)/sws_release.3.man.in
sws_reload_config.3: $(srcdir)/sws_reload_config.3.man.in
sws_server_wait.3: $(srcdir)/sws_server_wait.3.man.in
sws_show_block_status.3: $(srcdir)/sws_show_block_status.3.man.in
sws_show_cli_blocklist.3: $(srcdir)/sws_show_cli_blocklist.3.man.in
sws_show_status.3: $(srcdir)/sws_show_status.3.man.in
sws_srv_block.3: $(srcdir)/sws_srv_block.3.man.in
sws_srv_unblock.3: $(srcdir)/sws_srv_unblock.3.man.in

man_MANS = sws_cli_block.3 sws_cli_unblock.3 sws_end_daemon.3 sws_release.3 \
	sws_reload_config.3 sws_server_wait.3 sws_show_block_status.3 \
	sws_show_cli_blocklist.3 sws_show_status.3 sws_srv_block.3 \
	sws_srv_unblock.3

EXTRA_DIST = $(srcdir)/sws_cli_block.3.man.in \
	$(srcdir)/sws_cli_unblock.3.man.in $(srcdir)/sws_end_daemon.3.man.in \
	$(srcdir)/sws_release.3.man.in $(srcdir)/sws_reload_config.3.man.in \
	$(srcdir)/sws_server_wait.3.man.in \
	$(srcdir)/sws_show_block_status.3.man.in \
	$(srcdir)/sws_show_cli_blocklist.3.man.in \
	$(srcdir)/sws_show_status.3.man.in $(srcdir)/sws_srv_block.3.man.in \
	$(srcdir)/sws_srv_unblock.3.man.in

CLEANFILES = *.tmp sws_cli_block.3 sws_cli_unblock.3 sws_end_daemon.3 \
	sws_release.3 sws_reload_config.3 sws_server_wait.3 \
	sws_show_block_status.3 sws_show_cli_blocklist.3 sws_show_status.3 \
	sws_srv_block.3 sws_srv_unblock.3

