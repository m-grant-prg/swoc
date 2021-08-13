#########################################################################
#									#
# Script ID: src/prg/c/src/srv-prg/swocserver/Makefile.am		#
# Author: Copyright (C) 2015-2020  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate swocserver program from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 22/06/2015	MG	1.0.1	First release.				#
# 06/07/2015	MG	1.0.2	Make libconffile pkg-config check	#
#				display name more meaningful.		#
# 12/07/2015	MG	1.0.3	Change to use libmgesysutils.		#
# 24/10/2015	MG	1.0.4	Restructure and change to conform to	#
#				AutoTools General Template v1.0.6.	#
# 10/05/2016	MG	1.0.4	Move header files to include directory.	#
# 28/05/2016	MG	1.0.5	Add flags for libswocserver following	#
#				it's introduction.			#
# 04/10/2016	MG	1.0.6	Add support for configure debug option.	#
# 05/10/2016	MG	1.0.7	Change compiler optimisation flag to	#
#				-Og for debugging.			#
# 13/01/2017	MG	1.1.0	Add uninstall-local target to get	#
#				systemd to shutdown the swocserver	#
#				daemon service before we uninstall	#
#				swocserver. This is because the service	#
#				ExecStop uses swocserver -e. This only	#
#				gets executed if configure is run with	#
#				--enable-systemd=yes.			#
#				Change to eliminate -g compiler option	#
#				if not debugging.			#
# 27/05/2017	MG	1.1.1	Add support for temporary header files.	#
#				Add support for temporary libraries.	#
# 20/10/2017	MG	1.1.2	Change to use _LDADD for temporary	#
#				libraries as passing -L and the temp	#
#				directory does not necessarily leave	#
#				LD_LIBRARY_PATH in the correct order.	#
# 12/01/2018	MG	1.1.3	Add SPDX license tags to source files.	#
# 16/01/2018	MG	1.1.4	Correct usage of config.h		#
#				Change to include headers from relevant	#
#				-dev projects.				#
# 24/03/2018	MG	1.1.5	Add support for sparse.			#
# 22/05/2018	MG	1.1.6	Change from swocserver.h to internal.h	#
# 23/06/2018	MG	1.1.7	Disable sparse variable length array	#
#				warnings.				#
# 08/07/2018	MG	1.1.8	Improve debug flags used. Use gdb	#
#				orientated level 3 (includes		#
#				pre-processor macros).			#
# 13/08/2018	MG	1.1.9	Support configure time otion to add -H	#
#				compiler flag to show included files	#
#				and their depth in the include stack.	#
#				Use compiler version dependent CFLAGS	#
#				assembled in configure.			#
# 19/05/2019	MG	1.1.10	Merge sub-projects into one.		#
#				Remove shebang.				#
# 15/03/2020	MG	1.1.11	Add libswoccommon.la to LDADD for	#
#				testing and debugging with libtool	#
#				mode=execute.				#
#									#
#########################################################################


AM_CPPFLAGS = -I$(srcdir)/../../../inc-tmp
AM_CPPFLAGS += -I$(srcdir)/../../../inc/com-dev
AM_CPPFLAGS += -I$(srcdir)/../../../inc/srv-dev
AM_CPPFLAGS += -I$(srcdir)/../../../inc/srv-prg
AM_CPPFLAGS += -I../../../gen/inc -I../../../gen/lib


AM_CFLAGS = @mg_cflags@
AM_CFLAGS += $(LIBSWOCSERVER_CFLAGS) $(LIBMGEC_CFLAGS)

if DEBUG
AM_CFLAGS += -ggdb3 -Og
else
AM_CFLAGS += -O2
endif

if HEADERCHECK
AM_CFLAGS += -H
endif

if SPARSE
AM_CFLAGS += -Wsparse-all -Wno-vla
endif


noinst_HEADERS = internal.h


sbin_PROGRAMS = swocserver
swocserver_SOURCES = main.c version.c cmdlineargs.c signalhandle.c


AM_LDFLAGS = $(LIBMGEC_LIBS)


LDADD = $(tmplibraries)
LDADD += ../../com-lib/libswoccommon/libswoccommon.la
LDADD += ../../srv-lib/libswocserver/libswocserver.la


# systemd requires some ordering of it's actions. Please refer to the comment in
# the top-level Makefile.am
if SYSD
uninstall-local:
	systemctl stop swocserverd.service
	systemctl disable swocserverd.service
endif

