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
# 24/04/2018	MG	1.0.6	Add srctarball target to build a source	#
#				tarball in the build directory. Uses	#
#				git archive to archive HEAD.		#
# 22/05/2018	MG	1.0.7	Simplify src directory structure.	#
# 08/06/2018	MG	1.0.8	Add info messages to srctarball target.	#
# 24/10/2018	MG	1.0.9	Change srctarball target to build with	#
#				src between package name and version in	#
#				filename.				#
#									#
#########################################################################

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src/man/3/messages src/man/3/ssh src/man/3/tcp
SUBDIRS += src/man/3/validateconfig
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

