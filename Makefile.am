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
# 13/01/2018	MG	1.0.5	Remove unnecessary config.h generation.	#
# 05/02/2018	MG	1.0.6	Remove unnecessary -I m4extra.		#
# 24/04/2018	MG	1.0.7	Add srctarball target to build a source	#
#				tarball in the build directory. Uses	#
#				git archive to archive HEAD.		#
#									#
#########################################################################

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src/man/3/optionproc
SUBDIRS += src/prg/c/inc
SUBDIRS += src/prg/c/src/libswocclient


srctarball:
	@cwd="$${PWD}" && \
	cd @srcdir@ && \
	git archive --format=tar.gz \
		--prefix=@PACKAGE_TARNAME@-@PACKAGE_VERSION@/ \
		--output=$${cwd}/@PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz \
		HEAD && \
	cd -
