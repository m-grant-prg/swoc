#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: cli-prg/Makefile.am					#
# Author: Copyright (C) 2015-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate SWOCClient-C project from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 08/07/2015	MG	1.0.1	First release.				#
# 24/10/2015	MG	1.0.2	Restructure and change to conform to	#
#				AutoTools General Template v1.0.6.	#
# 09/05/2016	MG	1.0.3	Move header files to include directory.	#
# 11/06/2016	MG	1.0.4	Modified following libswocclient	#
#				library introduction.			#
# 25/04/2017	MG	1.0.5	Change man page to section 8.		#
# 31/05/2017	MG	1.0.6	Add support for temporary header files.	#
# 08/06/2017	MG	1.0.7	Add support for temporary libraries.	#
# 12/01/2018	MG	1.0.8	Add SPDX license tags to source files.	#
# 15/01/2018	MG	1.0.9	Correct generation of config.h		#
# 05/02/2018	MG	1.0.10	Remove unnecessary -I m4extra.		#
# 26/04/2018	MG	1.0.11	Add srctarball target to build a source	#
#				tarball in the build directory. Uses	#
#				git archive to archive HEAD.		#
# 22/05/2018	MG	1.0.12	Simplify src directory structure and	#
#				header file location.			#
# 08/06/2018	MG	1.0.13	Add info messages to srctarball target.	#
#									#
#########################################################################

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src/man/8
SUBDIRS += src/prg/c/gen/lib
SUBDIRS += src/prg/c/inc-tmp
SUBDIRS += src/prg/c/lib-tmp
SUBDIRS += src/prg/c/src

EXTRA_DIST  = m4/gnulib-cache.m4


srctarball:
	@cwd="$${PWD}" && \
	cd @srcdir@ && \
	echo "making srctarball from @srcdir@" && \
	echo "GA	@builddir@/@PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz" \
	&& \
	git archive --format=tar.gz \
		--prefix=@PACKAGE_TARNAME@-@PACKAGE_VERSION@/ \
		--output=$${cwd}/@PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz \
		HEAD && \
	cd -
