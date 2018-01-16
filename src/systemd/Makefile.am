#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: srv-prg/src/systemd/Makefile.am				#
# Author: Copyright (C) 2017-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to install systemd service file.		#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 10/01/2017	MG	1.0.1	First release. This only gets executed	#
#				if configure is run with		#
#				--enable-systemd=yes.			#
# 12/01/2018	MG	1.0.2	Add SPDX license tags to source files.	#
#									#
#########################################################################

systemddir = $(libdir)/systemd/system

swocserverd.service: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(edit) $${srcdir}$(@F).in > $@.tmp
	mv $@.tmp $@
swocserverd.service: $(srcdir)/swocserverd.service.in

systemd_DATA = swocserverd.service

install-data-hook:
	systemctl enable swocserverd.service
	systemctl start swocserverd.service

EXTRA_DIST = $(srcdir)/swocserverd.service.in

CLEANFILES = *.tmp swocserverd.service
