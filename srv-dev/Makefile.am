#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: srv-dev/Makefile.am					#
# Author: Copyright (C) 2016-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswocserver-dev project from	#
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
# 18/12/2016	MG	1.0.2	Removed swocserver.conf(5) to the	#
#				libswocserver package.			#
# 21/10/2017	MG	1.0.3	Remove validateconfig build.		#
# 12/01/2018	MG	1.0.4	Add SPDX license tags to source files.	#
# 15/01/2018	MG	1.0.5	Remove unnecessary config.h generation.	#
# 05/02/2018	MG	1.0.6	Remove unnecessary -I m4extra.		#
# 25/04/2018	MG	1.0.7	Add srctarball target to build a source	#
#				tarball in the build directory. Uses	#
#				git archive to archive HEAD.		#
# 22/05/2018	MG	1.0.8	Simplify src directory structure.	#
# 08/06/2018	MG	1.0.9	Add info messages to srctarball target.	#
# 25/10/2018	MG	1.0.10	Change srctarball target to build with	#
#				src between package name and version in	#
#				filename.				#
#									#
#########################################################################

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src/man/3
SUBDIRS += src/prg/c/inc
SUBDIRS += src/prg/c/src


srctarball:
	@cwd="$${PWD}" && \
	targ="$${cwd}/@PACKAGE_TARNAME@-src-@PACKAGE_VERSION@.tar.gz" && \
	cd @srcdir@ && \
	echo "making srctarball from @srcdir@" && \
	echo "GA	$${targ}" && \
	git archive --format=tar.gz \
		--prefix=@PACKAGE_TARNAME@-@PACKAGE_VERSION@/ \
		--output=$${targ} \
		HEAD && \
	cd -

