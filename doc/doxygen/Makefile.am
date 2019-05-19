#########################################################################
#									#
# Script ID: doc/doxygen/Makefile.am					#
# Author: Copyright (C) 2018-2019  Mark Grant				#
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
# 27/06/2018	MG	1.0.3	Reverse 1.0.2 as AC_CONFIG_FILES cannot	#
#				be used for the substitution of		#
#				Installation Directory Variables.	#
# 19/05/2019	MG	1.0.4	Merge sub-projects into one.		#
#				Remove shebang.				#
#									#
#########################################################################


docpkg = $(PACKAGE_TARNAME)-doxy-$(PACKAGE_VERSION).tar.gz

docbasedir = $(datarootdir)/doc-base


# The dependencies below start with Doxyfile which is produced by configure, so
# thereafter no updates to these documents will happen by make alone. Re-run
# configure and make, or, delete the stamp file and run make to force an update.

$(docpkg): doxygen.stamp
	tar -chz --format=posix -f $@ html

doxygen.stamp: Doxyfile
	doxygen $<
	echo Timestamp > $@


html_DATA = $(docpkg)


swoc: Makefile
	mkdir -p $(@D)
	rm -f $@
	$(edit) $(srcdir)/$(@F).in > $@
swoc: $(srcdir)/swoc.in

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


EXTRA_DIST = $(docpkg) $(srcdir)/swoc.in


CLEANFILES = doxywarn.txt doxygen.stamp $(docpkg) swoc

clean-local:
	rm -rf html

