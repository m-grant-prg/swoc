#########################################################################
#									#
# Script ID: src/man/3/com-dev/messages/Makefile.am			#
# Author: Copyright (C) 2016-2019  Mark Grant				#
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
# 20/12/2016	MG	1.0.1	Initial release.			#
# 23/04/2017	MG	1.0.2	Correct $(edit) output was being	#
#				ignored.				#
#				Rename source file extensions .man.in	#
# 12/01/2018	MG	1.0.3	Add SPDX license tags to source files.	#
# 14/01/2018	MG	1.0.4	Determine the manual section from the	#
#				target suffix. If the suffix is blank	#
#				(foo.) or if there is no suffix (foo)	#
#				then error out.				#
# 19/05/2019	MG	1.0.5	Merge sub-projects into one.		#
#				Remove shebang.				#
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

