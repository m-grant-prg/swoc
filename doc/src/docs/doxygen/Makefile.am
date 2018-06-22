#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: doc/source/docs/doxygen/Makefile.am			#
# Author: Copyright (C) 2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to produce doxygen html documentation.	#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 11/01/2018	MG	1.0.0	First release.				#
# 12/02/2018	MG	1.0.1	Make tar use the Posix format for	#
#				longer filenames.			#
# 22/06/2018	MG	1.0.2	Remove make, (leave install), of	#
#				swoc doc-base file as it is now	built	#
#				using AC_CONFIG_FILES.			#
#									#
#########################################################################


docpkg = $(PACKAGE_TARNAME)-doxy-$(PACKAGE_VERSION).tar.gz

docbasedir = $(datarootdir)/doc-base


# The dependencies below start with Doxyfile which is produced by configure, so
# thereafter no updates to these documents will happen by make alone. Re-run
# configure and make, or, delete the stamp file and run make to force an update.
#
# The test for the -lib and -dev sources ensures that the Doxygen documentation
# and resulting tarball are updated only when the sources are present. The
# importance of this is that during make install for this -doc package the
# sources are not present and make all would would produce basic empty Doxygen
# documentation which would then be installed.

$(docpkg): doxygen.stamp
	if [ -d $(top_srcdir)/../com-lib/src/ \
		-a -d $(top_srcdir)/../com-dev/src/ ]; \
		then tar -chz --format=posix -f $@ html; fi

doxygen.stamp: Doxyfile
	if [ -d $(top_srcdir)/../com-lib/src/ \
		-a -d $(top_srcdir)/../com-dev/src/ ]; \
		then doxygen $<; fi
	echo Timestamp > $@


html_DATA = $(docpkg)


docbase_DATA = swoc


# If doc-base is installed register the documents then update dochelp if it is
# installed.
install-data-hook:
	cd $(DESTDIR)$(htmldir) && tar xf $(docpkg)
	if command -v install-docs >/dev/null 2>&1; then \
		install-docs -vi $(docbasedir)/libmgesysutils; \
	fi
	if command -v dochelp >/dev/null 2>&1; then \
		dochelp update; \
	fi

uninstall-hook:
	cd $(DESTDIR)$(htmldir) && rm -rf html
	if command -v install-docs >/dev/null 2>&1; then \
		install-docs -vr libmgesysutils; \
	fi
	if command -v dochelp >/dev/null 2>&1; then \
		dochelp update; \
	fi


EXTRA_DIST = $(docpkg) swoc.in


CLEANFILES = doxywarn.txt doxygen.stamp $(docpkg) swoc

clean-local:
	rm -rf html
