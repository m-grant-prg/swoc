#########################################################################
#									#
# Script ID: ./configure.ac						#
# Author: Copyright (C) 2015-2019  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoConf script file to configure SWOCServer-C project.		#
#									#
# cmd line syntax:							#
# ./configure	[--enable-debug=<yes|no> [default: no]]			#
#		[--enable-headercheck=<yes|no> [default: no]]		#
#		[--enable-sparse=<yes|no> [default: no]]		#
#		[--enable-systemd=yes|no> [default: no]]		#
#									#
# Layout: (Only relevant sections will exist).				#
# ChangeLog : High level ChangeLog					#
# SECTION  1: Initialisation & Setup					#
# SECTION  2: Architecture, target and host checks			#
# SECTION  3: Command line arguments					#
# SECTION  4: Compiler setup						#
# SECTION  5: Check for programs					#
# SECTION  6: Check for libraries					#
# SECTION  7: Check for header files					#
# SECTION  8: Check for types						#
# SECTION  9: Check for structures					#
# SECTION 10: Check for compiler characteristics			#
# SECTION 11: Check for library functions				#
# SECTION 12: Check for pkg-config files				#
# SECTION 13: Check for system services					#
# SECTION 14: Additional checks & variables				#
# SECTION 15: Specify the output files					#
#									#
#########################################################################

#########################################################################
#									#
# ChangeLog								#
#									#
# Date		Author	Version	Description				#
#									#
# 22/06/2015	MG	1.0.1	First release.				#
# 02/07/2015	MG	1.0.2	Remove redundant c++ compiler check.	#
# 06/07/2015	MG	1.0.3	Make libconffile pkg-config check	#
#				display name more meaningful.		#
# 12/07/2015	MG	1.0.4	Change to use libmgesysutils.		#
# 27/09/2015	MG	1.0.5	Add check for txt2manwrap as part of	#
#				changes to conform to AutoTools		#
#				Template version 1.0.4.			#
# 24/10/2015	MG	1.0.6	Restructure and change to conform to	#
#				AutoTools General Template v1.0.6.	#
# 10/05/2016	MG	1.0.7	Move header files to include directory.	#
# 28/05/2016	MG	1.0.8	Modified following introduction of	#
#				libswocserver library.			#
# 13/06/2016	MG	1.0.9	Use new more unique function name in	#
#				AC_SEARCH_LIBS libswocserver library	#
#				check.					#
# 20/09/2016	MG	1.0.10	Add swocserverd daemon directory.	#
# 02/02/2017	MG	1.1.0	Add library check for libmgesysutils.	#
#				Add header checks for above.		#
#				Add enable-debug option.		#
#				Add libmgec library support.		#
#				Add header checks for mgebuffer,	#
#				mgememory and mgemessage.		#
#				Add library and pkg-config check for	#
#				libswoccommon.				#
#				Add command line controlled systemd	#
#				service build.				#
#				Change to empty CFLAGS here and leave	#
#				setting to the Makefile.am's.		#
#				Add header checks for bstree.h & 	#
#				mge-errno.h				#
# 19/05/2017	MG	1.1.1	Add support for temporary header	#
#				directory.				#
#				Add Makefiles for man 5 pages and bash.	#
# 07/06/2017	MG	1.1.2	Add support for temporary libraries.	#
# 09/09/2017	MG	1.1.3	Correct function name in error string	#
#				for AC_SEARCH_LIBs on libswoccommon.	#
# 20/10/2017	MG	1.1.4	Add search for temporary libraries and	#
#				store result in a variable for AutoMake	#
#				consumption.				#
# 12/01/2018	MG	1.1.5	Add SPDX license tags to source files.	#
# 16/01/2018	MG	1.1.6	Correct generation of config.h		#
#				Update minimum AutoTools version.	#
#				Update AC macros and comments.		#
#				Change to manual package version	#
#				numbering on adoption of project /	#
#				sub-project structure.			#
# 05/02/2018	NG	1.1.7	Change AC_CONFIG_SRCDIR to sensible	#
#				value.					#
# 12/02/2018	MG	1.1.8	Make AutoMake use posix format for tar.	#
# 22/03/2018	MG	1.1.9	Add header check for configfile.h	#
# 24/03/2018	MG	1.1.10	Add support for an enable-sparse CLA	#
#				and passing a suitable variable to the	#
#				Makefile.am's.				#
# 10/05/2018	MG	1.1.11	Add check for stdbool.h			#
# 24/06/2018	MG	1.1.12	Build setup.sh in AC_CONFIG_FILES.	#
#				Build swocserverd.conf in		#
#				AC_CONFIG_FILES.			#
#				Build swocserverd.service in		#
#				AC_CONFIG_FILES.			#
#				Remove no longer needed AC_SUBST	#
#				variable pkgversion.			#
#				AC_DEFINE SWOCSERVER_SOURCE_VERSION for	#
#				use in version.c via config.h		#
#				AC_DEFINE SWOCSERVERD_SOURCE_VERSION	#
#				for use in version.c via config.h	#
# 27/06/2018	MG	1.1.13	Do not use AC_CONFIG_FILES for		#
#				substitution of Installation Directory	#
#				Variables. Reverse build of setup.sh to	#
#				the Makefile. Reverse build of		#
#				swocserverd.service to the Makefile.	#
# 13/08/2018	MG	1.1.14	Add support for an enable-headercheck	#
#				CLA and passing a suitable variable to	#
#				the Makefile.am's.			#
#				Build suitable CFLAGS based on compiler	#
#				version.				#
# 23/09/2018	MG	1.1.15	Remove check for deprecated and		#
#				replaced bzero().			#
# 05/10/2018	MG	1.1.16	Add program check for pkg-config.	#
# 18/05/2019	MG	1.2.1	Re-vamp to new standard layout.		#
#									#
#########################################################################


#########################################
#					#
# SECTION  1: Initialisation & Setup	#
#					#
#########################################

AC_REVISION($Revision: 1.2.1 $)

AC_PREREQ(2.69)

#
# The following macros set all top level versioning for this project. All
# information can be propagated via AC_CONFIG_HEADERS and AC_CONFIG_FILES.
#
AC_INIT(Server Wait On Client Package, v1.1.8, m.grant.prg@gmail.com, swoc)

AC_MSG_NOTICE(initialisation and setup)

AC_SUBST(package_version, $PACKAGE_VERSION)

AC_DEFINE(COMMON_SOURCE_VERSION, "1.1.6", \
	Define to the version of source code for the common library.)

AC_SUBST(common_lib_version_info, "2:1:1")

AC_DEFINE(CLIENT_SOURCE_VERSION, "1.1.4", \
	Define to the version of source code for the client library.)

AC_SUBST(client_lib_version_info, "1:1:1")

AC_DEFINE(SERVER_SOURCE_VERSION, "1.1.6", \
	Define to the version of source code for the server library.)

AC_SUBST(server_lib_version_info, "2:1:1")

AC_DEFINE(SWOCCLIENT_SOURCE_VERSION, "1.1.3", \
	Define to the version of source code for the client program.)

AC_DEFINE(SWOCSERVER_SOURCE_VERSION, "1.1.4", \
	Define to the version of source code for the server program.)

AC_DEFINE(SWOCSERVERD_SOURCE_VERSION, "1.0.5", \
	Define to the version of source code for the server daemon.)

#
# Passes the options to all am files. Puts objects in the sub-directory
# containing the source. Forces tar to use --format=posix in make dist.
# This also checks :-	BSD install	make variable set	mkdir -p
#			gawk
#
AM_INIT_AUTOMAKE(-Wall -Werror subdir-objects tar-pax)

#
# Standard macros will be automatically placed in m4.
# Programmer defined macros should be placed in, say, m4extra which allows us
# to exclude the standard macros from being git tracked. This other directory
# must be specified as an include option in the Makefile.am ACLOCAL_AMFLAGS
# e.g.
# ACLOCAL_AMFLAGS = -I m4 -I m4extra
#
AC_CONFIG_MACRO_DIR(m4)


#########################################
#					#
# SECTION  3: Command line arguments	#
#					#
#########################################

AC_MSG_NOTICE(processing command line arguments)

#
# The following allows for an enable-debug=yes/no command line option. The
# conditional DEBUG is made available to the makefiles.
#
AC_ARG_ENABLE(debug,
	--enable-debug    Compile for debugging,
	[case "${enableval}" in
		yes) debug=true ;;
		no)  debug=false ;;
		*) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
	esac],debug=false)
AM_CONDITIONAL(DEBUG, test x$debug = xtrue)

#
# CFLAGS is usually set to -g -O2, but for debugging we will probably
# want -g -Og and for production we will probably want -O2. So empty CFLAGS here
# and leave the selection to the Makefile.am's.
#
: ${CFLAGS=""}

if test "x${debug}" = xtrue; then
	AC_MSG_NOTICE(Using compiler debug flags.)
fi

#
# The following allows for an enable-headercheck=yes/no command line option.
# The conditional HEADERCHECK is made available to the makefiles.
#
AC_ARG_ENABLE(headercheck,
	--enable-headercheck    Build to display include stack,
	[case "${enableval}" in
		yes) headercheck=true ;;
		no)  headercheck=false ;;
		*) AC_MSG_ERROR(bad value ${enableval} in enable-headercheck);;
	esac],headercheck=false)
AM_CONDITIONAL(HEADERCHECK, test x$headercheck = xtrue)

if test "x${headercheck}" = xtrue; then
	AC_MSG_NOTICE(Building to display include stack depth.)
fi

#
# The following allows for an enable-sparse=yes/no command line option. The
# conditional SPARSE is made available to the makefiles.
#
AC_ARG_ENABLE(sparse,
	--enable-sparse    Build with sparse,
	[case "${enableval}" in
		yes) sparse=true ;;
		no)  sparse=false ;;
		*) AC_MSG_ERROR(bad value ${enableval} for --enable-sparse);;
	esac],sparse=false)
AM_CONDITIONAL(SPARSE, test x$sparse = xtrue)

if test "x${sparse}" = xtrue; then
	AC_MSG_NOTICE(Building using sparse.)
fi

#
# The following allows for an enable-systemd=yes/no command line option. The
# conditional SYSD is made available to the makefiles.
#
AC_ARG_ENABLE(systemd,
	--enable-systemd    Build for systemd,
	[case "${enableval}" in
		yes) systemd=true ;;
		no)  systemd=false ;;
		*) AC_MSG_ERROR(bad value ${enableval} for --enable-systemd);;
	esac],systemd=false)
AM_CONDITIONAL(SYSD, test x$systemd = xtrue)

if test "x${systemd}" = xtrue; then
	AC_MSG_NOTICE(Building for systemd.)
fi


#################################
#				#
# SECTION  4: Compiler setup	#
#				#
#################################

AC_MSG_NOTICE(compiler setup)

# Set language for compilation tests.
AC_LANG(C)

#
# Checks the compiler to use from either a standard list or one to use for
# sparse, stores it in the CC variable and checks its capabilities.
#
if test "x$sparse" = "xtrue" ; then
	compiler_list="cgcc gcc cc"
else
	compiler_list="gcc cc"
fi
AC_PROG_CC($compiler_list)

#
# GNULIB macro. Place just after AC_PROG_CC.
# Also checks for ar, grep, egrep, ranlib and how to run the cpp.
#
gl_EARLY

#
# Checks the c pre-processor to use, stores it in the cpp variable and checks
# its capabilities.
#
AC_PROG_CPP

#
# Checks the c++ compiler to use, stores it in the cxx variable and checks its
# capabilities. (Required by ltmain.sh).
#
AC_PROG_CXX

# GNULIB gl_INIT macro. Must appear after other checks which may affect the
# compiler invocation.
gl_INIT

# Must use this macro if the ar archiver is used. ar is mostly now used for
# static libraries.
AM_PROG_AR

# Check for libtool minimum version.
LT_PREREQ(2.4.6)

# Turn on shared libraries.
# Also checks for fgrep, linker capabilities, ranlib, sed.
# Also checks for:-	ANSI C header files	sys/types.h	sys/stat.h
#			stdlib.h	string.h	memory.h
#			strings.h	inttypes.h	stdint.h
#			unistd.h	dlfcn.h
LT_INIT


#########################################
#					#
# SECTION  5: Check for programs	#
#					#
#########################################

AC_MSG_NOTICE(checking for basic programs that have AutoConf checks)

#
# The small set of tools you can expect to find on any machine.
#
# awk		date		grep		od
# basename	diff		join		rm
# cat		dirname		ln		rmdir
# cc		egrep		ls		sed
# chgrp		expr		mkdir		sleep
# chown		expr (‘|’)	mkfifo		sort
# chmod		expr (‘:’)	mknod		tar
# cmp		fgrep		mktemp		touch
# cp		find		mv		tr
#
# (For further information including limitations to the above, see Section 11.15
# in the GNU AutoConf Manual).
#
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
# Basic and AutoTools program checks.
#
AC_PROG_AWK
AC_PROG_EGREP
AC_PROG_FGREP
AC_PROG_GREP
AC_PROG_LN_S
AC_PROG_MKDIR_P
AC_PROG_SED

AC_MSG_NOTICE(checking for some standard AutoTools required programs)
AC_PROG_INSTALL

AC_MSG_NOTICE(checking for particular programs required by this build)
AC_CHECK_PROG(DIA, dia, yes, no)
if test "x$DIA" = "xno" ; then
	AC_MSG_ERROR(dia not found)
fi

AC_CHECK_PROG(DOT, dot, yes, no)
if test "x$DOT" = "xno" ; then
	AC_MSG_ERROR(dot not found)
fi

AC_CHECK_PROG(DOXYGEN, doxygen, yes, no)
if test "x$DOXYGEN" = "xno" ; then
	AC_MSG_ERROR(doxygen not found)
fi

AC_CHECK_PROG(TAR, tar, yes, no)
if test "x$TAR" = "xno" ; then
	AC_MSG_ERROR(tar not found)
fi

AC_CHECK_PROG(TXT2MAN, txt2man, yes, no)
if test "x$TXT2MAN" = "xno" ; then
   	AC_MSG_ERROR(txt2man not found)
fi

AC_CHECK_PROG(TXT2MANWRAP, txt2manwrap, yes, no)
if test "x$TXT2MANWRAP" = "xno" ; then
   	AC_MSG_ERROR(txt2manwrap not found)
fi

AC_CHECK_PROG(PKGCONFIG, pkg-config, yes, no)
if test "x$PKGCONFIG" = "xno" ; then
	AC_MSG_ERROR(pkg-config not found)
fi

AC_MSG_NOTICE(checking for particular programs required at build and run time)
AC_CHECK_PROG(BASH, bash, yes, no)
if test "x$BASH" = "xno" ; then
   	AC_MSG_ERROR(bash not found)
fi

AC_MSG_NOTICE(checking for particular programs required at run time)
AC_CHECK_PROG(GETOPT, getopt, yes, no)
if test "x$GETOPT" = "xno" ; then
	AC_MSG_WARN(getopt not found)
fi


#########################################
#					#
# SECTION  6: Check for libraries	#
#					#
#########################################

AC_MSG_NOTICE(checking for libraries required by this application)

AC_SEARCH_LIBS(parsefile, mgesysutils, , \
	AC_MSG_ERROR(function parsefile not found in libmgesysutils))
AC_SEARCH_LIBS(add_bst_node, mgec, , \
	AC_MSG_ERROR(function add_bst_node not found in libmgec))
AC_SEARCH_LIBS(ssh_new, ssh, , \
	AC_MSG_ERROR(function ssh_new not found in libssh))


#########################################
#					#
# SECTION  7: Check for header files	#
#					#
#########################################

AC_MSG_NOTICE(checking for header files required by this application)

AC_CHECK_HEADERS(stdio.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(stdlib.h, , AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(stdarg.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(stdbool.h, , AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(sysexits.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(signal.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(limits.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(netinet/in.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(netdb.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(sys/stat.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(sys/types.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(string.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(ctype.h, , AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(unistd.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(syslog.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(errno.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(getopt.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(libssh/libssh.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(portability.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(mgebuffer.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(mgememory.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(mgemessage.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(bstree.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(mge-errno.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(configfile.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(remsyslog.h, , AC_MSG_ERROR(header not found))


#################################
#				#
# SECTION  8: Check for types	#
#				#
#################################

AC_MSG_NOTICE(checking for types required by this application)

AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T


#########################################################
#							#
# SECTION 10: Check for compiler characteristics	#
#							#
#########################################################

AC_MSG_NOTICE(checking for compiler characteristics)

# Check that the compiler recognizes the restrict keyword.
AC_C_RESTRICT

# Check for threading libraries, compiler and linker flags.
AX_PTHREAD(, AC_MSG_ERROR(thread information not found))

# Build a suitable CFLAGS list depending on compiler version.
BUILD_COMPILER_VERSION_CFLAGS(mg_cflags)


#################################################
#						#
# SECTION 11: Check for library functions	#
#						#
#################################################

AC_MSG_NOTICE(checking for GNU compatible library functions required)

AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS(memset socket strerror)


#################################################
#						#
# SECTION 12: Check for pkg-config files	#
#						#
#################################################

AC_MSG_NOTICE(checking for pkg-config files)

PKG_CHECK_MODULES(LIBMGESYSUTILS, libmgesysutils, [], \
		AC_MSG_ERROR(pkg-config file not found))
PKG_CHECK_MODULES(LIBMGEC, libmgec, [], \
		AC_MSG_ERROR(pkg-config file not found))
PKG_CHECK_MODULES(LIBSSH, libssh, [], \
		AC_MSG_ERROR(pkg-config file not found))


#################################################
#						#
# SECTION 14: Additional checks and variables	#
#						#
#################################################

AC_MSG_NOTICE(processing additional checks and variables)

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
AC_SUBST([edit], ["$SED \
		-e 's|@package_version[@]|\$(package_version)|g' \
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
AC_SUBST(tmpheaders)

for entry in `dirname $0`/src/prg/c/inc-tmp/*.h
do
	if test -f "$entry" ;then
		tmpheaders=$tmpheaders" "$(basename "$entry")
	fi
done

#
# This creates an AutoConf substitution variable for temporary libraries.
# Temporary libraries are development copies of libraries from other packages
# which are being tested / modified in conjunction with this package.
# This variable can be consumed in the AutoMake XXX_LDADD line.
# The use of `dirname $0` to get the configure invocation directory is critical
# for parallel builds and make distcheck as the relative path always stays the
# same but configure may be invoked from different directories.
#
AC_SUBST(tmplibraries)

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

#
# Setup substitution variables for use in the pkg-config file.
#
# The Common library.
AC_SUBST(com_pc_requires)
# LIBMGEC is a Requires AOT Requires.private because it exposes mge_errno.
com_pc_requires="libmgec"

# The Requires.private field
AC_SUBST(com_pc_requires_private)
com_pc_requires_private="libmgesysutils"

# The Client library.
AC_SUBST(cli_pc_requires)
cli_pc_requires="libmgec"

AC_SUBST(cli_pc_requires_private)
cli_pc_requires_private="libmgesysutils libswoccommon"

# The Server library.
AC_SUBST(srv_pc_requires)
srv_pc_requires="libmgec"

AC_SUBST(srv_pc_requires_private)
srv_pc_requires_private="libswoccommon"

# Check a source file to ensure no command line --srcdir typo.
AC_CONFIG_SRCDIR(src/prg/c/src/srv-prg/swocserver/version.c)


#########################################
#					#
# SECTION 15: Specify the output files	#
#					#
#########################################

AC_MSG_NOTICE(processing output files)

#
# Non-Makefile files to generate.
# AC_CONFIG_FILES should not be used to substitute AutoConf Installation
# Directory Variables, (bindir and friends), except in Makefiles and pkg-config
# files. See GNU AutoConf manual section 4.8.2 Installation Directory Variables.
#
AC_CONFIG_FILES(doc/doxygen/Doxyfile)
AC_CONFIG_FILES(src/conf/etc/com-lib/swoc.conf)
AC_CONFIG_FILES(src/conf/etc/srv-prg/swocserverd.conf)
AC_CONFIG_FILES(src/prg/c/pkg-config/cli-dev/libswocclient.pc)
AC_CONFIG_FILES(src/prg/c/pkg-config/com-dev/libswoccommon.pc)
AC_CONFIG_FILES(src/prg/c/pkg-config/srv-dev/libswocserver.pc)

# Causes AC_OUTPUT to create a c preprocessor file with #define statements about
# the configuration.
AC_CONFIG_HEADERS(src/prg/c/gen/inc/config.h)


# Project Makefiles to create.
AC_CONFIG_FILES(Makefile
		doc/doxygen/Makefile
		src/conf/etc/com-lib/Makefile
		src/conf/etc/srv-prg/Makefile
		src/man/3/cli-dev/Makefile
		src/man/3/com-dev/messages/Makefile
		src/man/3/com-dev/ssh/Makefile
		src/man/3/com-dev/tcp/Makefile
		src/man/3/com-dev/validateconfig/Makefile
		src/man/3/srv-dev/Makefile
		src/man/5/com-lib/Makefile
		src/man/5/srv-prg/Makefile
		src/man/8/cli-prg/Makefile
		src/man/8/srv-prg/Makefile
		src/prg/bash/com-lib/Makefile
		src/prg/bash/srv-prg/Makefile
		src/prg/c/gen/lib/Makefile
		src/prg/c/inc/cli-dev/Makefile
		src/prg/c/inc/com-dev/Makefile
		src/prg/c/inc/srv-dev/Makefile
		src/prg/c/inc/srv-prg/Makefile
		src/prg/c/inc-tmp/Makefile
		src/prg/c/lib-tmp/Makefile
		src/prg/c/pkg-config/cli-dev/Makefile
		src/prg/c/pkg-config/com-dev/Makefile
		src/prg/c/pkg-config/srv-dev/Makefile
		src/prg/c/src/cli-lib/libswocclient/Makefile
		src/prg/c/src/cli-lib/test-optionproc/Makefile
		src/prg/c/src/cli-prg/Makefile
		src/prg/c/src/com-lib/libswoccommon/Makefile
		src/prg/c/src/com-lib/test-messages/Makefile
		src/prg/c/src/com-lib/test-ssh/Makefile
		src/prg/c/src/com-lib/test-tcp/Makefile
		src/prg/c/src/com-lib/test-validate/Makefile
		src/prg/c/src/srv-lib/libswocserver/Makefile
		src/prg/c/src/srv-lib/test-optionproc/Makefile
		src/prg/c/src/srv-prg/swocserver/Makefile
		src/prg/c/src/srv-prg/swocserverd/Makefile
		src/systemd/srv-prg/Makefile)

AC_OUTPUT

