#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: com-lib/Makefile.am					#
# Author: Copyright (C) 2016-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswoccommon project from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 14/12/2016	MG	1.0.1	First release.				#
# 24/04/2017	MG	1.0.2	Add temporary header directory.		#
# 09/06/2017	MG	1.0.3	Add support for temporary libraries.	#
# 17/10/2017	MG	1.0.4	Changes to support SSH introduction.	#
#				Add man 5 build.			#
# 12/01/2018	MG	1.0.5	Add SPDX license tags to source files.	#
# 14/01/2018	MG	1.0.6	Correct the generation of config.h	#
# 05/02/2018	MG	1.0.7	Remove unnecessary -I m4extra.		#
#									#
#########################################################################

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src/conf/etc
SUBDIRS += src/man/5
SUBDIRS += src/prg/bash
SUBDIRS += src/prg/c/gen/lib
SUBDIRS += src/prg/c/inc-tmp src/prg/c/lib-tmp
SUBDIRS += src/prg/c/src/libswoccommon
SUBDIRS += src/prg/c/src/test-messages src/prg/c/src/test-ssh
SUBDIRS += src/prg/c/src/test-tcp src/prg/c/src/test-validate

EXTRA_DIST  = m4/gnulib-cache.m4
