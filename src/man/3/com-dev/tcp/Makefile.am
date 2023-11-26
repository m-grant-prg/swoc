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


close_sock.3 est_connect.3 init_conn.3 listen_sock.3 prep_recv_sock.3: Makefile
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

close_sock.3: $(srcdir)/close_sock.3.man.in
est_connect.3: $(srcdir)/est_connect.3.man.in
init_conn.3: $(srcdir)/init_conn.3.man.in
listen_sock.3: $(srcdir)/listen_sock.3.man.in
prep_recv_sock.3: $(srcdir)/prep_recv_sock.3.man.in

man_MANS = close_sock.3 est_connect.3 init_conn.3 listen_sock.3 prep_recv_sock.3

EXTRA_DIST = $(srcdir)/close_sock.3.man.in $(srcdir)/est_connect.3.man.in \
		$(srcdir)/init_conn.3.man.in $(srcdir)/listen_sock.3.man.in \
		$(srcdir)/prep_recv_sock.3.man.in

CLEANFILES = *.tmp close_sock.3 est_connect.3 init_conn.3 listen_sock.3 \
		prep_recv_sock.3

