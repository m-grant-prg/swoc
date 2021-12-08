#########################################################################
#									#
# Script ID: src/man/3/srv-dev/Makefile.am				#
# Author: Copyright (C) 2016-2019, 2021  Mark Grant			#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswocserver manual pages	#
# from source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 27/05/2016	MG	1.0.1	Initial release.		.	#
# 12/06/2016	MG	1.0.2	Update with more unique function names.	#
# 17/12/2016	MG	1.0.3	Add sws_end_daemon man page.		#
# 13/02/2017	MG	1.0.4	Add sws_reload_config man page.		#
# 26/04/2017	MG	1.0.5	Output from $(edit) was being ignored.	#
#				Rename source file extensions .man.in	#
# 15/09/2017	MG	1.0.6	Change force_unlock to unlock.		#
# 12/01/2018	MG	1.0.7	Add SPDX license tags to source files.	#
# 15/01/2018	MG	1.0.8	Determine the manual section from the	#
#				target suffix. If the suffix is blank	#
#				(foo.) or if there is no suffix (foo)	#
#				then error out.				#
# 11/05/2018	MG	1.0.9	Improve function name consistency,	#
#				unlock -> release.			#
#				Add show_cli_blocklist man page.	#
#				Add client block and unblock man pages.	#
#				Add server block and unblock man pages.	#
# 19/05/2019	MG	1.0.10	Merge sub-projects into one.		#
#				Remove shebang.				#
# 07/12/2021	MG	1.0.11	Tighten SPDX tag.			#
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

