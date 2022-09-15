#########################################################################
#									#
# Script ID: src/prg/c/src/srv-lib/libswocserver/Makefile.am		#
# Author: Copyright (C) 2016-2019, 2021, 2022  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswocserver library from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 25/05/2016	MG	1.0.1	First release.				#
# 13/06/2016	MG	1.0.2	Increment library interface version as	#
#				changes to make function names more	#
#				unique breaks backwards compatibility.	#
# 20/11/2016	MG	1.0.3	Change to new source directory		#
#				structure. Add swoccommon library info.	#
# 14/01/2017	MG	1.0.4	Set debug or normal compile flags here.	#
# 15/05/2017	MG	1.0.5	Change validate config to swocserver-	#
#				only, aot common with swocserverd.	#
# 09/06/2017	MG	1.0.6	Add support for temporary libraries.	#
# 20/10/2017	MG	1.0.7	Change to use _LIBADD for temporary	#
#				libraries as passing -L and the temp	#
#				directory does not necessarily leave	#
#				LD_LIBRARY_PATH in the correct order.	#
# 12/01/2018	MG	1.0.8	Add SPDX license tags to source files.	#
# 15/01/2018	MG	1.0.9	Correct consumption of config.h		#
# 16/01/2018	MG	1.0.10	Change to include headers from relevant	#
#				-dev projects.				#
# 26/03/2018	MG	1.0.11	Add support for sparse.			#
# 23/06/2018	MG	1.0.12	Disable sparse variable length array	#
#				warnings.				#
#				Use lib_version_info AC_SUBST variable	#
#				passed via AC_CONFIG_FILES to enable	#
#				all top level versioning to be done in	#
#				configure.ac				#
# 08/07/2018	MG	1.0.13	Improve debug flags used. Use gdb	#
#				orientated level 3 (includes		#
#				pre-processor macros).			#
# 13/08/2018	MG	1.0.14	Support configure time otion to add -H	#
#				compiler flag to show included files	#
#				and their depth in the include stack.	#
#				Use compiler version dependent CFLAGS	#
#				assembled in configure.			#
# 19/05/2019	MG	1.0.15	Merge sub-projects into one.		#
#				Remove shebang.				#
# 09/10/2021	MG	1.0.16	Capitalise MG_CFLAGS variable.		#
#				Use CPPFLAGS prepared by configure.	#
#				Use LDFLAGS prepared by configure.	#
# 11/10/2021	MG	1.0.17	Use api and internal header directories.#
# 15/10/2021	MG	1.0.18	Change debug optimisation to -O0	#
# 05/11/2021	MG	1.0.19	Use (non-)debug CPPFLAGS from configure	#
#				Use (non-)debug CFLAGS from configure	#
# 08/12/2021	MG	1.0.20	Tighten SPDX tag.			#
# 06/06/2022	MG	1.0.21	Add compiler analyzer option.		#
# 15/09/2022	MG	1.0.22	Rename tmplibraries.			#
#				New com-lib directory structure.	#
#									#
#########################################################################


if DEBUG
AM_CPPFLAGS = $(MG_DEBUG_CPPFLAGS)
else
AM_CPPFLAGS = $(MG_CPPFLAGS)
endif
AM_CPPFLAGS += -I$(srcdir)/../../../inc-tmp
AM_CPPFLAGS += -I$(srcdir)/../../../inc/api
AM_CPPFLAGS += -I$(srcdir)/../../../inc/internal
AM_CPPFLAGS += -I../../../gen/inc -I../../../gen/lib


if DEBUG
AM_CFLAGS = $(MG_DEBUG_CFLAGS)
else
AM_CFLAGS = $(MG_CFLAGS)
endif
if HEADERCHECK
AM_CFLAGS += -H
endif
if SPARSE
AM_CFLAGS += -Wsparse-all -Wno-vla
endif
if ANALYZER
AM_CFLAGS += $(MG_ANALYZER_CFLAGS)
endif
AM_CFLAGS += $(LIBMGEC_CFLAGS) $(LIBSWOCCOMMON_CFLAGS)


lib_LTLIBRARIES = libswocserver.la

libswocserver_la_SOURCES = optionproc.c
libswocserver_la_SOURCES += version.c


AM_LDFLAGS = $(MG_LDFLAGS)
AM_LDFLAGS += -version-info @server_lib_version_info@
AM_LDFLAGS += $(LIBMGEC_LIBS)


libswocserver_la_LIBADD = $(tmplibraries_depth_3)
libswocserver_la_LIBADD += ../../com-lib/libswoccommon.la

