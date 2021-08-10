#########################################################################
#									#
# Script ID: doc/doxygen/Makefile.am					#
# Author: Copyright (C) 2018-2021  Mark Grant				#
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
# 06/11/2019	MG	1.0.5	Do not register with doc-base or	#
#				dochelp when running make distcheck.	#
#				Reduce Doxygen output when silent	#
#				rules are enabled.			#
# 27/06/2020	MG	1.0.6	Split into API and full internal	#
#				documentation.				#
# 09/08/2021	MG	1.0.7	Only register documents if building	#
#				with ATONLY.				#
#									#
#########################################################################


# Build API documentation.

docpkg = $(PACKAGE_TARNAME)-doxy-$(PACKAGE_VERSION).tar.gz

# The dependencies below start with Doxyfile which is produced by configure, so
# thereafter no updates to these documents will happen by make alone. Re-run
# configure and make, or, delete the stamp file and run make to force an update.

$(docpkg): doxygen.stamp
	tar -chz --format=posix -f $@ html

doxygen.stamp: Doxyfile
	if $(AM_V_P); then \
		DOXYGEN_QUIET=NO \
			DOXYGEN_PROJECT_NAME="Server Wait On Clients System - API Documentation" \
			DOXYGEN_HTML_OUTPUT=html \
			DOXYGEN_ENABLED_SECTIONS= \
			DOXYGEN_EXCLUDE_PATTERNS="*internal*" \
			doxygen $< ; \
	else \
		DOXYGEN_QUIET=YES \
			DOXYGEN_PROJECT_NAME="Server Wait On Clients System - API Documentation" \
			DOXYGEN_HTML_OUTPUT=html \
			DOXYGEN_ENABLED_SECTIONS= \
			DOXYGEN_EXCLUDE_PATTERNS="*internal*" \
			doxygen $< ; \
	fi
	echo Timestamp > $@


# Build full internal documentation.

intdocpkg = $(PACKAGE_TARNAME)-int-doxy-$(PACKAGE_VERSION).tar.gz

$(intdocpkg): doxygen1.stamp
	tar -chz --format=posix -f $@ html-int

doxygen1.stamp: Doxyfile
	if $(AM_V_P); then \
		DOXYGEN_QUIET=NO \
			DOXYGEN_PROJECT_NAME="Server Wait On Clients System - Full Internal Documentation" \
			DOXYGEN_HTML_OUTPUT=html-int \
			DOXYGEN_ENABLED_SECTIONS=INTERNAL \
			DOXYGEN_EXCLUDE_PATTERNS= \
			doxygen $< ; \
	else \
		DOXYGEN_QUIET=YES \
			DOXYGEN_PROJECT_NAME="Server Wait On Clients System - Full Internal Documentation" \
			DOXYGEN_HTML_OUTPUT=html-int \
			DOXYGEN_ENABLED_SECTIONS=INTERNAL \
			DOXYGEN_EXCLUDE_PATTERNS= \
			doxygen $< ; \
	fi
	echo Timestamp > $@


apidir = $(htmldir)/api
intdir = $(htmldir)/int

api_DATA = $(docpkg)
int_DATA = $(intdocpkg)


swoc swoc-int: Makefile
	mkdir -p $(@D)
	rm -f $@
	$(edit) $(srcdir)/$(@F).in > $@
swoc: $(srcdir)/swoc.in
swoc-int: $(srcdir)/swoc-int.in


# If doc-base is installed register the documents then update dochelp if it is
# installed. Registering is dependent on ATONLY.

docbasedir = @docbaseloc@

docbase_DATA = swoc swoc-int

install-data-hook:
if !DISTCHECKFAKE
	cd $(DESTDIR)$(apidir) && tar xf $(docpkg)
	cd $(DESTDIR)$(intdir) && tar xf $(intdocpkg)
if ATONLY
	if command -v install-docs >/dev/null 2>&1; then \
		install-docs -vi $(docbasedir)/swoc; \
		install-docs -vi $(docbasedir)/swoc-int; \
	fi
	if command -v dochelp >/dev/null 2>&1; then \
		dochelp update; \
	fi
endif
endif

uninstall-local:
if !DISTCHECKFAKE
if ATONLY
	if command -v install-docs >/dev/null 2>&1; then \
		install-docs -vr swoc; \
		install-docs -vr swoc-int; \
	fi
	if command -v dochelp >/dev/null 2>&1; then \
		dochelp update; \
	fi
endif
	cd $(DESTDIR)$(apidir) && rm -rf html
	cd $(DESTDIR)$(intdir) && rm -rf html-int
endif


EXTRA_DIST = $(docpkg) $(intdocpkg) $(srcdir)/swoc.in $(srcdir)/swoc-int.in


CLEANFILES = doxywarn.txt doxygen.stamp doxygen1.stamp $(docpkg) $(intdocpkg) \
	swoc swoc-int

clean-local:
	rm -rf html html-int

