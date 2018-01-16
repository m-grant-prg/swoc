#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: srv-lib/src/prg/c/inc-tmp/Makefile.am			#
# Author: Copyright (C) 2017-2018  Mark Grant				#
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
# 27/04/2017	MG	1.0.0	First release.				#
# 12/01/2018	MG	1.0.1	Add SPDX license tags to source files.	#
# 16/01/2018	MG	1.0.2	Change to include headers from relevant	#
#				-dev projects.				#
#									#
#########################################################################


noinst_HEADERS = $(tmpheaders)

EXTRA_HEADERS = mge-errno.h
