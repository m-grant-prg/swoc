#########################################################################
#									#
# Script ID: ./configure.ac						#
# Author: Copyright (C) 2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	Top level AutoConf script file to configure full Server Wait On	#
# Clients (swoc) project including sub-projects.			#
#									#
# cmd line syntax:							#
# ./configure	[--enable-debug=<yes|no> [default: no]]			#
# Any command line arguments are passed to the sub-project configure's.	#
# This accepts any sub-project supported arguments.			#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 11/01/2018	MG	1.0.1	First release.				#
#									#
#########################################################################

AC_REVISION([$Revision: 1.0.1 $])

AC_PREREQ([2.65])


########################
#
# The following two macros use git to provide versioning information.
# If git is not initialised or no commit has been made, then 'Pre-Release'
# will be used.
# If no Tag has been created then just a commit identifier will be used.
# If a tag has been created then the most recent tag, plus number of commits
# since the tag, plus a commit identifier will be used.
#
########################

AC_INIT([[Server Wait on Clients system - all projects.]], m4_esyscmd([ \
		git describe --always 1>/dev/null 2>/dev/null \
		&& git describe --always | tr -d '\n' \
		|| echo 'Pre-Release' | tr -d '\n']),
		[m.grant.prg@gmail.com], [swoc])

AC_SUBST([pkgversion], [m4_esyscmd([ \
		git describe --always 1>/dev/null 2>/dev/null \
		&& git describe --always | tr -d '\n' \
		|| echo 'Pre-Release' | tr -d '\n'])])


########################
#
# The following allows for an enable-debug=yes/no command line option. The
# conditional DEBUG is made available to the makefiles.
#
########################

AC_ARG_ENABLE([debug],
	[  --enable-debug    Compile for debugging],
	[case "${enableval}" in
		yes) debug=true ;;
		no)  debug=false ;;
		*) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
	esac],[debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

if test "x${debug}" = xtrue; then
	AC_MSG_NOTICE([Build in debug mode.])
fi


########################
#
# Now set up a sed sequence to substitute bookmarks in script files with either
# standard GNU Directory Variables, or with programmer-defined variables set up
# using an AC_SUBST macro above. This sed sequence will be invoked at programmer
# discretion at sub-directory Makefile.am level.
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
# containing the source.
# This also checks :-	BSD install	make variable set	mkdir -p
#			gawk
if test "x${debug}" = xtrue; then
	AC_MSG_NOTICE([Running am init automake.])
fi

AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects])

if test "x${debug}" = xtrue; then
	AC_MSG_NOTICE([Finished running am init automake.])
fi


# Checks the c++ compiler to use, stores it in the cxx variable and checks its
# capabilities. (Required by ltmain.sh).
if test "x${debug}" = xtrue; then
	AC_MSG_NOTICE([Running ac prog cxx. (Required by ltmain.sh).])
fi

AC_PROG_CXX

if test "x${debug}" = xtrue; then
	AC_MSG_NOTICE([Finished running ac prog cxx.])
fi


# Check for ranlib recommended by autoscan for ltmain.sh but rendered obsolete
# by lt init.


# Check for libtool minimum version.
if test "x${debug}" = xtrue; then
	AC_MSG_NOTICE([Running lt prereq.])
fi

LT_PREREQ([2.4.6])

if test "x${debug}" = xtrue; then
	AC_MSG_NOTICE([Finished running lt prereq.])
fi


# Turn on shared libraries.
# Also checks for fgrep, linker capabilities, ranlib, sed.
# Also checks for:-	ANSI C header files	sys/types.h	sys/stat.h
#			stdlib.h	string.h	memory.h
#			strings.h	inttypes.h	stdint.h
#			unistd.h	dlfcn.h
if test "x${debug}" = xtrue; then
	AC_MSG_NOTICE([Running lt init.])
fi

LT_INIT

if test "x${debug}" = xtrue; then
	AC_MSG_NOTICE([Finished running lt init.])
fi



########################
#
# Standard macros will be automatically placed in m4.
# Programmer defined macros should be placed in, say, m4extra which allows us
# to exclude the standard macros from being git tracked. This other directory
# must be specified as an include option in the Makefile.am ACLOCAL_AMFLAGS
#
########################

if test "x${debug}" = xtrue; then
	AC_MSG_NOTICE([Running ac config macro dir.])
fi


AC_CONFIG_MACRO_DIR([m4])

if test "x${debug}" = xtrue; then
	AC_MSG_NOTICE([Finished running ac config macro dir.])
fi


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

AC_MSG_NOTICE([checking for generic programs required by this application])

# Checks for libraries.

# Checks for C header files.

# Checks for C++ header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Sub-project configure.ac locations
AC_CONFIG_SUBDIRS([com-dev com-lib
		cli-dev cli-lib cli-prg
		srv-dev srv-lib srv-prg
		doc])

# Project files to create.
AC_CONFIG_FILES([Makefile])

AC_OUTPUT
