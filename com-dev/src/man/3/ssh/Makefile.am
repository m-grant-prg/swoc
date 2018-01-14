#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: com-dev/src/man/3/ssh/Makefile.am				#
# Author: Copyright (C) 2017-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswoccommon manual pages	#
# from source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 18/10/2017	MG	1.0.1	Initial release.			#
# 12/01/2018	MG	1.0.2	Add SPDX license tags to source files.	#
# 14/01/2018	MG	1.0.3	Determine the manual section from the	#
#				target suffix. If the suffix is blank	#
#				(foo.) or if there is no suffix (foo)	#
#				then error out.				#
#									#
#########################################################################


close_ssh_tunnel.3 open_ssh_tunnel.3: Makefile
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

close_ssh_tunnel.3: $(srcdir)/close_ssh_tunnel.3.man.in
open_ssh_tunnel.3: $(srcdir)/open_ssh_tunnel.3.man.in

man_MANS = close_ssh_tunnel.3 open_ssh_tunnel.3

EXTRA_DIST = $(srcdir)/close_ssh_tunnel.3.man.in \
		$(srcdir)/open_ssh_tunnel.3.man.in

CLEANFILES = *.tmp close_ssh_tunnel.3 open_ssh_tunnel.3
