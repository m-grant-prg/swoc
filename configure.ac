#########################################################################
#									#
# Script ID: com-dev/configure.ac					#
# Author: Copyright (C) 2016-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoConf script file to configure libswoccommon-dev project.	#
#									#
# configure cmd line syntax:-						#
#		./configure						#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 21/12/2016	MG	1.0.1	First release.				#
# 23/04/2017	MG	1.0.2	Add fully populated sed variable	#
#				substitution.				#
#				Make AC_INIT and pkgversion AC_SUBST	#
#				multi-line.				#
# 18/10/2017	MG	1.0.3	Add open_ssh_tunnel, close_ssh_tunnel	#
#				and validateconfig man 3 pages.		#
# 12/01/2018	MG	1.0.4	Add SPDX license tags to source files.	#
# 14/01/2018	MG	1.0.5	Remove unnecessary config.h generation.	#
#				Update AutoTools minimum version to 2.69#
#				Remove unnecessary compiler check.	#
#				Change to manual package version	#
#				numbering on adoption of project /	#
#				sub-project structure.			#
# 12/02/2018	MG	1.0.6	Make AutoMake use posix format for tar.	#
# 04/03/2018	MG	1.0.7	Add substitution variables for 		#
#				pkg-config file Requires and		#
#				Requires.private fields.		#
#									#
#########################################################################

AC_REVISION([$Revision: 1.0.7 $])

AC_PREREQ([2.69])


########################
#
# The package version in ac init will be used to create an output variable which
# can be used by ac output and / or the sed setup below to substitute bookmarks
# in source files.
#
########################

AC_INIT([[SWOC Common Library dev files]], [v1.0.2],
	[m.grant.prg@gmail.com], [libswoccommon-dev])

AC_SUBST(pkgversion, $PACKAGE_VERSION)


########################
#
# Now set up a sed sequence to substitute bookmarks in script files with either
# standard GNU Directory Variables, or with programmer-defined variables set up
# using an AC_SUBST macro above. This sed sequence will be invoked at
# programmer discretion at sub-directory Makefile.am level.
# ----------------------
#
# prefix	/usr/local (typically)
#	exec_prefix	${prefix}
#		bindir	${exec_prefix}/bin	user executables
#		sbindir	${exec_prefix}/sbin	system admin executables
#		libexecdir	${exec_prefix}/libexec	program executables
#		libdir	${exec_prefix}/lib	object code libraries
#	sysconfdir	 ${prefix}/etc	read-only single-machine data
#	sharedstatedir	${prefix}/com	modifiable architecture-independent data
#	localstatedir	${prefix}/var	modifiable single-machine data
#	runstatedir	${localstatedir}/run
#	includedir	${prefix}/include	C header files
#	oldincludedir	$usr/include	C header files
#	datarootdir	${prefix}/share	read-only arch.-independent data root
#		localedir	${datarootdir}/locale	locale-dependent data
#		datadir	${datarootdir}	read-only architecture-independent data
#		mandir	${datarootdir}/man	man documentation
#		infodir	${datarootdir}/info	info documentation
#		docdir	${datarootdir}/doc/${PACKAGE}	documentation root
#			htmldir	${docdir}	html documentation
#			dvidir	${docdir}	dvi documentation
#			pdfdir	${docdir}	pdf documentation
#			psdir	${docdir}	ps documentation
#		lispdir	${datarootdir}/emacs/site-lisp
#	pkgdatadir	${datarootdir}/${PACKAGE}
#	pkgincludedir	${includedir}/${PACKAGE}
#	pkglibdir	${libdir}/${PACKAGE}
#	pkglibexecdir	${libexecdir}/${PACKAGE}
#
########################

AC_SUBST([edit], ["sed \
		-e 's|@pkgversion[@]|\$(pkgversion)|g' \
		-e 's|@prefix[@]|\$(prefix)|g' \
		-e 's|@exec_prefix[@]|\$(exec_prefix)|g' \
		-e 's|@bindir[@]|\$(bindir)|g' \
		-e 's|@sbindir[@]|\$(sbindir)|g' \
		-e 's|@libexecdir[@]|\$(libexecdir)|g' \
		-e 's|@libdir[@]|\$(libdir)|g' \
		-e 's|@sysconfdir[@]|\$(sysconfdir)|g' \
		-e 's|@sharedstatedir[@]|\$(sharedstatedir)|g' \
		-e 's|@localstatedir[@]|\$(localstatedir)|g' \
		-e 's|@runstatedir[@]|\$(runstatedir)|g' \
		-e 's|@includedir[@]|\$(includedir)|g' \
		-e 's|@oldincludedir[@]|\$(oldincludedir)|g' \
		-e 's|@datarootdir[@]|\$(datarootdir)|g' \
		-e 's|@localedir[@]|\$(localedir)|g' \
		-e 's|@datadir[@]|\$(datadir)|g' \
		-e 's|@mandir[@]|\$(mandir)|g' \
		-e 's|@infodir[@]|\$(infodir)|g' \
		-e 's|@docdir[@]|\$(docdir)|g' \
		-e 's|@htmldir[@]|\$(htmldir)|g' \
		-e 's|@dvidir[@]|\$(dvidir)|g' \
		-e 's|@pdfdir[@]|\$(pdfdir)|g' \
		-e 's|@psdir[@]|\$(psdir)|g' \
		-e 's|@lispdir[@]|\$(lispdir)|g' \
		-e 's|@pkgdatadir[@]|\$(pkgdatadir)|g' \
		-e 's|@pkgincludedir[@]|\$(pkgincludedir)|g' \
		-e 's|@pkglibdir[@]|\$(pkglibdir)|g' \
		-e 's|@pkglibexecdir[@]|\$(pkglibexecdir)|g'"])


# Passes the options to all am files. Puts objects in the sub-directory
# containing the source. Forces tar to use --format=posix in make dist.
# This also checks :-	BSD install	make variable set	mkdir -p
#			gawk
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects tar-pax])


########################
#
# Standard macros will be automatically placed in m4.
# Programmer defined macros should be placed in, say, m4extra which allows us
# to exclude the standard macros from being git tracked. This other directory
# must be specified as an include option in the Makefile.am ACLOCAL_AMFLAGS
# e.g.
# ACLOCAL_AMFLAGS = -I m4 -I m4extra
#
########################

AC_CONFIG_MACRO_DIR([m4])


########################
#
# Setup substitution variables for use in the pkg-config file.
#
########################
# The Requires field.
AC_SUBST(pc_requires)
# LIBMGEC is a Requires AOT Requires.private because it exposes mge_errno.
pc_requires="libmgec"

# The Requires.private field
AC_SUBST(pc_requires_private)
pc_requires_private="libmgesysutils"


########################
#
# Checks for programs.
# If using am init automake, gl early or lt init the following programs are
# automatically checked:-
#
# am init automake
#	BSD install	mkdir -p	gawk
#
# gl early
#	ar	grep	egrep	ranlib
#
# lt init
#	fgrep	ranlib	sed
#
# Particular and generic program checks follow:
#
########################

AC_MSG_NOTICE([checking for particular programs required by this application])
AC_PROG_SED

AC_MSG_NOTICE([checking for generic programs required by this application])
AC_CHECK_PROG([TAR], [tar], [yes], [no])
if test "x$TAR" = "xno" ; then
	AC_MSG_ERROR([tar not found])
fi

AC_CHECK_PROG([BASH], [bash], [yes], [no])
if test "x$BASH" = "xno" ; then
   	AC_MSG_ERROR([bash not found])
fi

AC_CHECK_PROG([TXT2MAN], [txt2man], [yes], [no])
if test "x$TXT2MAN" = "xno" ; then
   	AC_MSG_ERROR([txt2man not found])
fi

AC_CHECK_PROG([TXT2MANWRAP], [txt2manwrap], [yes], [no])
if test "x$TXT2MANWRAP" = "xno" ; then
   	AC_MSG_ERROR([txt2manwrap not found])
fi

# Checks for libraries.

# Checks for C header files.
AC_CHECK_HEADERS([portability.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([sys/types.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([netinet/in.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([netdb.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([mgemessage.h], [], [AC_MSG_ERROR([header not found])])

# Checks for C++ header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_MSG_NOTICE([checking for typedefs required by this application])
AC_TYPE_SIZE_T

# Checks for library functions.

# Checks for pkg-config files.

# Project Makefiles to create.
AC_CONFIG_FILES([Makefile
		src/man/3/messages/Makefile
		src/man/3/ssh/Makefile
		src/man/3/tcp/Makefile
		src/man/3/validateconfig/Makefile
		src/prg/c/inc/Makefile
		src/prg/c/src/libswoccommon/Makefile
		src/prg/c/src/libswoccommon/libswoccommon.pc])

AC_OUTPUT
