#########################################################################
#									#
# Script ID: src/prg/c/inc-tmp/Makefile.am				#
# Author: Copyright (C) 2017-2019  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to produce temporary headers.		#
#									#
# For example. Temporary headers can be used in development by a 	#
# library package to house, modify and use copies of an include file	#
# which is normally part of a -dev package. At the end of development	#
# this, of course, has to be moved back to the -dev package and		#
# released from there.							#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 19/05/2017	MG	1.0.0	First release.				#
# 02/10/2017	MG	1.0.1	Add mge-errno.h possibility.		#
# 12/01/2018	MG	1.0.2	Add SPDX license tags to source files.	#
# 16/01/2018	MG	1.0.3	Change to include headers from relevant	#
#				-dev projects.				#
# 01/05/2018	MG	1.0.4	Add libswoccommon.h.			#
# 03/06/2018	MG	1.0.5	Add bstree.h				#
# 17/06/2018	MG	1.0.6	Add mgebuffer & mgemessage candidates.	#
# 19/05/2019	MG	1.0.7	Merge sub-projects into one.		#
#				Remove shebang.				#
#									#
#########################################################################


noinst_HEADERS = $(tmpheaders)

EXTRA_HEADERS = bstree.h libswoccommon.h mgebuffer.h mge-errno.h mgemessage.h

