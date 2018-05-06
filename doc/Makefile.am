#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: doc/Makefile.am						#
# Author: Copyright (C) 2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate swoc-doc project from		#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 11/01/2018	MG	1.0.1	First release.				#
# 26/04/2018	MG	1.0.2	Add srctarball target to build a source	#
#				tarball in the build directory. Uses	#
#				git archive to archive HEAD.		#
#									#
#########################################################################

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src/docs/doxygen


srctarball:
	@cwd="$${PWD}" && \
	cd @srcdir@ && \
	git archive --format=tar.gz \
		--prefix=@PACKAGE_TARNAME@-@PACKAGE_VERSION@/ \
		--output=$${cwd}/@PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz \
		HEAD && \
	cd -
