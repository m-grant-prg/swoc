#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: srv-lib/Makefile.am					#
# Author: Copyright (C) 2016-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswocserver project from	#
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
# 19/11/2016	MG	1.0.2	Remove bash directory as it is removed	#
#				to swocserver-c package.		#
# 17/12/2016	MG	1.0.3	Add man5 directory.			#
#				Moved bash config file setup script	#
#				back to libswocserver as this library	#
#				can exist without swocserver-c package.	#
# 27/04/2017	MG	1.0.4	Add support for temporary headers	#
#				directory.				#
# 16/05/2017	MG	1.0.5	Add support for config file.		#
# 09/06/2017	MG	1.0.6	Add support for temporary libraries.	#
# 17/10/2017	MG	1.0.7	Remove config file man page after move	#
#				to libswoccommon.			#
# 12/01/2018	MG	1.0.8	Add SPDX license tags to source files.	#
# 15/01/2018	MG	1.0.9	Correct generation of config.h		#
# 05/02/2018	MG	1.0.10	Remove unnecessary -I m4extra.		#
# 25/04/2018	MG	1.0.11	Add srctarball target to build a source	#
#				tarball in the build directory. Uses	#
#				git archive to archive HEAD.		#
#									#
#########################################################################

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src/prg/c/gen/lib
SUBDIRS += src/prg/c/inc-tmp
SUBDIRS += src/prg/c/lib-tmp
SUBDIRS += src/prg/c/src/libswocserver
SUBDIRS += src/prg/c/src/test-optionproc

EXTRA_DIST  = m4/gnulib-cache.m4


srctarball:
	@cwd="$${PWD}" && \
	cd @srcdir@ && \
	git archive --format=tar.gz \
		--prefix=@PACKAGE_TARNAME@-@PACKAGE_VERSION@/ \
		--output=$${cwd}/@PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz \
		HEAD && \
	cd -
