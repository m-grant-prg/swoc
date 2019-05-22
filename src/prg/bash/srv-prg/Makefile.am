#########################################################################
#									#
# Script ID: src/prg/bash/srv-prg/Makefile.am				#
# Author: Copyright (C) 2017-2019  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate swocserverd scripts from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 19/05/2017	MG	1.0.0	First release.				#
# 12/01/2018	MG	1.0.2	Add SPDX license tags to source files.	#
# 23/06/2018	MG	1.0.3	Build setup.sh in AC_CONFIG_FILES.	#
# 27/06/2018	MG	1.0.4	Reverse 1.0.3 as AC_CONFIG_FILES cannot	#
#				be used for the substitution of		#
#				Installation Directory Variables.	#
# 19/05/2019	MG	1.0.5	Merge sub-projects into one.		#
#				Remove shebang.				#
# 22/05/2019	MG	1.0.6	Change script name as following the	#
#				collapse of all sub-projects into one,	#
#				the setup script names must be unique.	#
#				(This is more a requirement for		#
#				downstream packaging but sensible	#
#				none-the-less).				#
#									#
#########################################################################


swocserverd-setup.sh: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(edit) $${srcdir}$(@F).in >$@.tmp
	mv $@.tmp $@
	chmod a+x $@
swocserverd-setup.sh: $(srcdir)/swocserverd-setup.sh.in

pkglibexec_SCRIPTS = swocserverd-setup.sh

EXTRA_DIST = $(srcdir)/swocserverd-setup.sh.in

CLEANFILES = *.tmp swocserverd-setup.sh

