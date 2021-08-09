#########################################################################
#									#
# Script ID: src/systemd/srv-prg/Makefile.am				#
# Author: Copyright (C) 2017-2019, 2021  Mark Grant			#
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
# 23/06/2018	MG	1.0.3	Build swocserverd.service in		#
#				AC_CONFIG_FILES.			#
# 27/06/2018	MG	1.0.4	Reverse 1.0.3 as AC_CONFIG_FILES cannot	#
#				be used for the substitution of		#
#				Installation Directory Variables.	#
# 19/05/2019	MG	1.0.5	Merge sub-projects into one.		#
#				Remove shebang.				#
# 09/08/2021	MG	1.0.6	Add systemd commands for uninstall.	#
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

uninstall-local:
	systemctl stop swocserverd.service
	systemctl disable swocserverd.service

uninstall-hook:
	systemctl daemon-reload

EXTRA_DIST = $(srcdir)/swocserverd.service.in

CLEANFILES = *.tmp swocserverd.service

