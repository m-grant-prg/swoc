#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: cli-dev/Makefile.am					#
# Author: Copyright (C) 2016-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswocclient-dev project from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 27/05/2016	MG	1.0.1	First release.				#
# 06/01/2017	MG	1.0.2	Update on conversion to use swocserverd	#
#				daemon and TCP stream.			#
#				Also swocclient.conf(5) moved to	#
#				libswocserver package.			#
# 20/10/2017	MG	1.0.3	Remove man 3 page for validateconfig.	#
# 12/01/2018	MG	1.0.4	Add SPDX license tags to source files.	#
#									#
#########################################################################

ACLOCAL_AMFLAGS = -I m4 -I m4extra

SUBDIRS = src/man/3/optionproc
SUBDIRS += src/prg/c/inc
SUBDIRS += src/prg/c/gen/inc
SUBDIRS += src/prg/c/src/libswocclient
