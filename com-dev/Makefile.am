#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: com-dev/Makefile.am					#
# Author: Copyright (C) 2016-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswoccommon-dev project from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 18/12/2016	MG	1.0.1	First release.				#
# 18/10/2017	MG	1.0.2	Add open_ssh_tunnel, close_ssh_tunnel	#
#				and validateconfig man 3 pages.		#
# 12/01/2018	MG	1.0.3	Add SPDX license tags to source files.	#
# 14/01/2018	MG	1.0.4	Remove unnecessary config.h generation.	#
# 05/02/2018	MG	1.0.5	Remove unnecessary -I m4extra.		#
#									#
#########################################################################

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src/man/3/messages src/man/3/ssh src/man/3/tcp
SUBDIRS += src/man/3/validateconfig
SUBDIRS += src/prg/c/inc
SUBDIRS += src/prg/c/src/libswoccommon
