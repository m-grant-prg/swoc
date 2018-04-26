#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: srv-prg/Makefile.am					#
# Author: Copyright (C) 2015-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate SWOCServer-C project from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 22/06/2015	MG	1.0.1	First release.				#
# 24/10/2015	MG	1.0.2	Restructure and change to conform to	#
#				AutoTools General Template v1.0.6.	#
# 10/05/2016	MG	1.0.3	Move header files to include directory.	#
# 28/05/2016	MG	1.0.4	Modified following libswocserver	#
#				library introduction.			#
# 20/09/2016	MG	1.0.5	Add swocserverd daemon directory.	#
# 23/09/2016	MG	1.0.6	Add bash directory following move from	#
#				libswocserver package.			#
# 10/01/2017	MG	1.1.0	Move bash config file setup script back	#
#				to libswocserver package as that	#
#				library can exist without this server	#
#				package.				#
#				Add systemd service build. This only	#
#				gets executed if configure is run with	#
#				--enable-systemd=yes.	 		#
# 19/05/2017	MG	1.1.1	Add support for temporary header	#
#				directory.				#
#				Add Make locations for man 5 pages and	#
#				bash.					#
# 07/06/2017	MG	1.1.2	Add support for temporary libraries.	#
# 12/01/2018	MG	1.1.3	Add SPDX license tags to source files.	#
# 16/01/2018	MG	1.1.4	Correct generation of config.h		#
# 05/02/2018	MG	1.1.5	Remove unnecessary -I m4extra.		#
# 26/04/2018	MG	1.1.6	Add srctarball target to build a source	#
#				tarball in the build directory. Uses	#
#				git archive to archive HEAD.		#
#									#
#########################################################################

ACLOCAL_AMFLAGS = -I m4

# The sequence of the build below is important in so far as this sequence
# means that the service start-up is done by a hook at the end of systemd
# service file installation.
# It also means that the service is stopped by a local target in swocserver
# uninstall (swocserver -e is used as ExecStop).

SUBDIRS = src/conf/etc
SUBDIRS += src/man/5 src/man/8
SUBDIRS += src/prg/c/gen/lib
SUBDIRS += src/prg/bash
SUBDIRS += src/prg/c/inc src/prg/c/inc-tmp
SUBDIRS += src/prg/c/lib-tmp
SUBDIRS += src/prg/c/src/swocserver
SUBDIRS += src/prg/c/src/swocserverd

if SYSD
SUBDIRS += src/systemd
endif

EXTRA_DIST  = m4/gnulib-cache.m4


srctarball:
	@cwd="$${PWD}" && \
	cd @srcdir@ && \
	git archive --format=tar.gz \
		--prefix=@PACKAGE_TARNAME@-@PACKAGE_VERSION@/ \
		--output=$${cwd}/@PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz \
		HEAD && \
	cd -
