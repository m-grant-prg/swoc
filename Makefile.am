#########################################################################
#									#
# Author: Copyright (C) 2015-2024  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate SWOCServer-C project from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# N.B.									#
# ====									#
# The sequence of the build is important for systemd, see below.	#
#									#
#########################################################################


ACLOCAL_AMFLAGS = -I m4 -I m4extra


AM_DISTCHECK_CONFIGURE_FLAGS = --enable-distcheckfake=yes


SUBDIRS = src/conf/etc/com-lib
SUBDIRS += src/conf/etc/srv-prg
SUBDIRS += src/man/3/cli-dev
SUBDIRS += src/man/3/com-dev/messages
SUBDIRS += src/man/3/com-dev/ssh
SUBDIRS += src/man/3/com-dev/tcp
SUBDIRS += src/man/3/com-dev/validateconfig
SUBDIRS += src/man/3/srv-dev
SUBDIRS += src/man/5/com-lib
SUBDIRS += src/man/5/srv-prg
SUBDIRS += src/man/8/cli-prg
SUBDIRS += src/man/8/srv-prg
SUBDIRS += src/prg/bash/com-lib
SUBDIRS += src/prg/bash/srv-prg
SUBDIRS += src/prg/c/inc-tmp
SUBDIRS += src/prg/c/lib-tmp
SUBDIRS += src/prg/c/gen/lib
SUBDIRS += src/prg/c/inc/swoc
SUBDIRS += src/prg/c/pkg-config
SUBDIRS += src/prg/c/src/com-lib
SUBDIRS += src/prg/c/src/cli-lib
SUBDIRS += src/prg/c/src/srv-lib
SUBDIRS += src/prg/c/src/cli-prg

# systemd requires the ordering of certain actions, so:-
# Install
#	swocserver
#	swocserverd
#	service
#	service-post (hook)
#		enable service
#		start service
# Uninstall
# 	swocserver-pre (local)
# 		stop service
# 		disable service
# 	swocserver
# 	swocserverd
# 	service
# 	service-post (hook)
# 		daemon reload
SUBDIRS += src/prg/c/src/srv-prg/swocserver
SUBDIRS += src/prg/c/src/srv-prg/swocserverd
if SYSD
SUBDIRS += src/systemd/srv-prg
endif

SUBDIRS += docs/doxygen
SUBDIRS += src/prg/c/src/tests/cli-lib
SUBDIRS += src/prg/c/src/tests/com-lib
SUBDIRS += src/prg/c/src/tests/srv-lib
SUBDIRS += src/prg/c/src/tests/test-headers
SUBDIRS += src/prg/c/src/tests/run-tests


doc_DATA = AUTHORS ChangeLog COPYING NEWS README


EXTRA_DIST = $(srcdir)/AUTHORS $(srcdir)/ChangeLog $(srcdir)/COPYING \
	     $(srcdir)/NEWS $(srcdir)/README
EXTRA_DIST += m4/gnulib-cache.m4
EXTRA_DIST += docs/misc misc


srctarball:
	@cwd="$${PWD}" && \
	varg="$$(echo @PACKAGE_VERSION@ | sed -e s/\-rc/\~rc/ )" && \
	targ="$${cwd}/@PACKAGE_TARNAME@-$${varg}.tar.gz" && \
	cd @srcdir@ && \
	echo "making srctarball from @srcdir@" && \
	echo "GA	$${targ}" && \
	git archive --format=tar.gz \
		--prefix=@PACKAGE_TARNAME@-$${varg}/ \
		--output=$${targ} \
		HEAD && \
	cd -


doxygen:
	(cd docs/doxygen && rm -f *.stamp && $(MAKE) $(AM_MAKEFLAGS) $@) \
		|| exit 1

.PHONY: doxygen

