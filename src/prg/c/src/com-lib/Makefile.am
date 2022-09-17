#########################################################################
#									#
# Script ID: src/prg/c/src/com-lib/Makefile.am				#
# Author: Copyright (C) 2016-2019, 2021, 2022  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswoccommon library from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 11/12/2016	MG	1.0.1	First release.				#
# 13/01/2017	MG	1.0.2	Set debug or normal compile flags here.	#
# 09/06/2017	MG	1.0.3	Add support for temporary header files.	#
#				Add support for temporary libraries.	#
# 03/10/2017	MG	1.0.4	Add ssh support.			#
# 19/10/2017	MG	1.0.5	Change to use _LIBADD for temporary	#
#				libraries as passing -L and the temp	#
#				directory does not necessarily leave	#
#				LD_LIBRARY_PATH in the correct order.	#
#				Re-order into logical steps.		#
# 12/01/2018	MG	1.0.6	Add SPDX license tags to source files.	#
# 14/01/2018	MG	1.0.7	Correct usage of config.h		#
# 16/01/2018	MG	1.0.8	Change to include headers from relevant	#
#				-dev projects.				#
# 26/01/2018	MG	1.0.9	Add correct pthread handling.		#
# 26/03/2018	MG	1.0.10	Add support for sparse.			#
# 23/06/2018	MG	1.0.11	Disable sparse variable length array	#
#				warnings.				#
#				Use lib_version_info AC_SUBST variable	#
#				passed via AC_CONFIG_FILES to enable	#
#				all top level versioning to be done in	#
#				configure.ac				#
# 07/07/2018	MG	1.0.12	Improve debug flags used. Use gdb	#
#				orientated level 3 (includes		#
#				pre-processor macros).			#
# 12/08/2018	MG	1.0.13	Support configure time otion to add -H	#
#				compiler flag to show included files	#
#				and their depth in the include stack.	#
#				Use compiler version dependent CFLAGS	#
#				assembled in configure.			#
# 19/05/2019	MG	1.0.14	Merge sub-projects into one.		#
#				Remove shebang.				#
# 09/10/2021	MG	1.0.15	Capitalise MG_CFLAGS variable.		#
#				Use CPPFLAGS prepared by configure.	#
#				Use LDFLAGS prepared by configure.	#
# 11/10/2021	MG	1.0.16	Use api and internal header directories.#
# 15/10/2021	MG	1.0.17	Change debug optimisation to -O0	#
# 05/11/2021	MG	1.0.18	Use (non-)debug CPPFLAGS from configure	#
#				Use (non-)debug CFLAGS from configure	#
# 08/12/2021	MG	1.0.19	Tighten SPDX tag.			#
# 06/06/2022	MG	1.0.20	Add compiler analyzer option.		#
# 17/09/2022	MG	1.0.21	Rename tmplibraries.			#
#				Flatten directory structure.		#
#				Flatten inc directory to pkgincludedir.	#
#									#
#########################################################################


CC = $(PTHREAD_CC)


if DEBUG
AM_CPPFLAGS = $(MG_DEBUG_CPPFLAGS)
else
AM_CPPFLAGS = $(MG_CPPFLAGS)
endif
AM_CPPFLAGS += $(PTHREAD_CFLAGS)
AM_CPPFLAGS += -I$(srcdir)/../../inc-tmp
AM_CPPFLAGS += -I$(srcdir)/../../inc
AM_CPPFLAGS += -I../../gen/inc -I../../gen/lib


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
AM_CFLAGS += $(PTHREAD_CFLAGS)
AM_CFLAGS += $(LIBMGEC_CFLAGS) $(LIBMGESYSUTILS_CFLAGS)
AM_CFLAGS += $(LIBSSH_CFLAGS)


lib_LTLIBRARIES = libswoccommon.la


libswoccommon_la_SOURCES = messages.c ssh.c tcp.c validateconfig.c
libswoccommon_la_SOURCES += version.c


AM_LDFLAGS = $(MG_LDFLAGS)
AM_LDFLAGS += $(PTHREAD_CFLAGS) $(LDFLAGS) $(PTHREAD_LIBS) $(LIBS)
AM_LDFLAGS += $(LIBMGEC_LIBS) $(LIBMGESYSUTILS_LIBS)
AM_LDFLAGS += $(LIBSSH_LIBS)
AM_LDFLAGS += -version-info @common_lib_version_info@


libswoccommon_la_LIBADD = $(tmplibraries_depth_2)

