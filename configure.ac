#########################################################################
#									#
# Script ID: srv-lib/configure.ac					#
# Author: Copyright (C) 2016-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoConf script file to configure libswocserver project.	#
#									#
# cmd line syntax:							#
# ./configure	[--enable-debug=<yes|no> [default: no]]			#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 25/05/2016	MG	1.0.1	First release.				#
# 28/05/2016	MG	1.0.2	Remove unnecessary program checks	#
#				eg txt2man.				#
# 29/05/2016	MG	1.0.3	Add library check for libmgesysutils.	#
# 17/12/2016	MG	1.0.4	Add checks for swoccommon library.	#
#				Add support for building man5 pages.	#
# 14/01/2017	MG	1.0.5	Change to empty CFLAGS here and leave	#
#				setting to the Makefile.am's.		#
# 08/02/2017	MG	1.0.6	Remove unnecessary sysexits.h check.	#
#				Add mge-errno.h check.			#
# 11/03/2017	MG	1.0.7	Add configure message output if debug	#
#				enabled.				#
#				Add full variable substitutions.	#
#				Adopt new code formatting.		#
# 27/04/2017	MG	1.0.8	Change AC_INIT and pkgversion AC_SUBST	#
#				to multi-line.				#
#				Add support for temporary headers	#
#				directory.				#
# 16/05/2017	MG	1.0.9	Add support for config file.		#
# 09/06/2017	MG	1.0.10	Add support for temporary libraries.	#
# 17/10/2017	MG	1.0.11	Remove config file man page after move	#
#				to libswoccommon.			#
# 20/10/2017	MG	1.0.12	Add search for temporary libraries and	#
#				store result in a variable for AutoMake	#
#				consumption.				#
# 12/01/2018	MG	1.0.13	Add SPDX license tags to source files.	#
# 15/01/2018	MG	1.0.14	Correct generation of config.h		#
#									#
#########################################################################

AC_REVISION([$Revision: 1.0.14 $])

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

AC_INIT([Server Wait On Client Server side library], m4_esyscmd([ \
	git describe --always 1>/dev/null 2>/dev/null \
	&& git describe --always | tr -d '\n' \
	|| echo 'Pre-Release' | tr -d '\n']),
	[m.grant.prg@gmail.com], [libswocserver])

AC_SUBST([pkgversion], [m4_esyscmd([ \
	git describe --always 1>/dev/null 2>/dev/null \
	&& git describe --always | tr -d '\n' \
	|| echo 'Pre-Release' | tr -d '\n'])])


########################
#
# This creates an AutoConf substitution variable for temporary header files.
# Temporary headers might be copies of -dev package header files brought into a
# library package for development and modification. Whilst there it should be
# used. When finished it must be moved back to the -dev package for release.
# This variable and the inc-tmp/Makefile.am allow for this conditional use and
# even conditional presence in inc-tmp.
# The use of `dirname $0` to get the configure invocation directory is critical
# for parallel builds and make distcheck as the relative path always stays the
# same but configure may be invoked from different directories.
#
########################

AC_SUBST([tmpheaders])

for entry in `dirname $0`/src/prg/c/inc-tmp/*.h
do
	if test -f "$entry" ;then
		tmpheaders=$tmpheaders" "$(basename "$entry")
	fi
done


########################
#
# This creates an AutoConf substitution variable for temporary libraries.
# Temporary libraries are development copies of libraries from other packages
# which are being tested / modified in conjunction with this package.
# This variable can be consumed in the AutoMake XXX_LDADD line.
# The use of `dirname $0` to get the configure invocation directory is critical
# for parallel builds and make distcheck as the relative path always stays the
# same but configure may be invoked from different directories.
#
########################

AC_SUBST([tmplibraries])

for direntry in `dirname $0`/src/prg/c/lib-tmp/*
do
	if test -d "$direntry" ;then
		for entry in `dirname $0`\
/src/prg/c/lib-tmp/$(basename "$direntry")/*.la
		do
			if test -f "$entry" ;then
				tmplibraries=$tmplibraries\
" \$(srcdir)/../../lib-tmp/"$(basename "$direntry")"/"$(basename "$entry")
			fi
		done
	fi
done


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

# CFLAGS is usually set to -g -O2, but for debugging we will probably
# want -g -Og and for production we will probably want -O2. So empty CFLAGS here
# and leave the selection to the Makefile.am's.

: ${CFLAGS=""}

if test "x${debug}" = xtrue; then
	AC_MSG_NOTICE([Using compiler debug flags.])
fi


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
# containing the source.
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects])

# Checks the compiler to use and stores it in the cc variable.
AC_PROG_CC

# GNULIB macros.
gl_EARLY
gl_INIT

# Must use this macro if the ar archiver is used. ar is mostly now used for
# static libraries.
AM_PROG_AR

# Turns on shared libraries.
LT_INIT

# Causes AC_OUTPUT to create a c preprocessor file with #define statements about
# the configuration.
AC_CONFIG_HEADERS([src/prg/c/gen/inc/config.h])


########################
#
# Standard macros will be automatically placed in m4.
# Programmer defined macros should be placed in, say, m4extra which allows us
# to exclude the standard macros from being git tracked. This other directory
# must be specified as an include option in the Makefile.am ACLOCAL_AMFLAGS
#
########################

AC_CONFIG_MACRO_DIR([m4])


########################
#
# Checks for programs.
# install, mkdir and gawk are checked by AM_INIT_AUTOMAKE.
# grep, egrep and ranlib are also automatically checked.
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


# Checks for libraries.
AC_SEARCH_LIBS([clear_msg], [mgec], , \
	[AC_MSG_ERROR([function clear_msg not found in libmgec])])
AC_SEARCH_LIBS([prep_recv_sock], [swoccommon], , \
	[AC_MSG_ERROR([function prep_recv_sock not found in libswoccommon])])

# Checks for C header files.
AC_CHECK_HEADERS([stdio.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([stdlib.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([stdarg.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([sys/stat.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([sys/types.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([string.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([unistd.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([syslog.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([ctype.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([limits.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([portability.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([configfile.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([mge-errno.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([mgemessage.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([libswoccommon.h], [], [AC_MSG_ERROR([header not found])])
AC_CHECK_HEADERS([libswocserver.h], [], [AC_MSG_ERROR([header not found])])

# Checks for C++ header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Checks for pkg-config files.
PKG_CHECK_MODULES(LIBMGEC, libmgec, [], \
		[AC_MSG_ERROR([pkg-config file not found])])
PKG_CHECK_MODULES(LIBSWOCCOMMON, libswoccommon, [], \
		[AC_MSG_ERROR([pkg-config file not found])])

# Project Makefiles to create.
AC_CONFIG_FILES([Makefile
		src/prg/c/inc-tmp/Makefile
		src/prg/c/lib-tmp/Makefile
		src/prg/c/src/libswocserver/Makefile
		src/prg/c/src/test-optionproc/Makefile
		src/prg/c/gen/lib/Makefile])

AC_OUTPUT
