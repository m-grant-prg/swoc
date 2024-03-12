#########################################################################
#									#
# Author: Copyright (C) 2015-2024  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoConf script file to configure SWOCServer-C project.		#
#									#
# cmd line syntax of standard configure:				#
# ./configure --help							#
#									#
# cmd line syntax of additional functionality:				#
# ./configure	[--enable-atonly[={yes|no}]]				#
#		[--enable-debug[={yes|no}]]				#
#		[--enable-distcheckfake[={yes|no}]]			#
#		[--enable-headercheck[={yes|no}]]			#
#		[--enable-iwyu[={yes|no}]]				#
#		[--enable-scan-build[={yes|no}]]			#
#		[--enable-sparse[={yes|no}]]				#
#		[--enable-systemd[={yes|no}]]				#
# Parameter defaults:							#
#	When specified on the CL, but without the [={yes|no}], the	#
#	default is yes.							#
#	When not specified the default is no.				#
#									#
# Parameters generic description. For specifics to this configure.ac	#
# please see inline comments at the point of definition:-		#
# --enable-atonly							#
#	Used to produce a fully working package just from the AT build	#
#	when the build references a (distro / external) static location	#
#	eg another package's config file in /etc (AOT AT's		#
#	/usr/local/etc), or an externally installed jar file in		#
#	/usr/share/jave (AOT AT's /usr/local/share/java).		#
# --enable-debug							#
#	Used to set things like compiler debug flags.			#
# --enable-distcheckfake						#
#	Enables the replacement of an absolute directory path specified	#
#	in a variable, with a value based on AutoTools standard		#
#	directories. This enables the Makefile.am to use the AT		#
#	variable based value when running distcheck and the absolute	#
#	path for all else. Hence distcheck can now succeed when it	#
#	would normally fail.						#
# --enable-headercheck							#
#	Used to set a flag to output the header file nesting.		#
# --enable-iwyu								#
#	Used to check that include-what-you-use is installed.		#
# --enable-scan-build							#
#	Used to check that clang's scan-build is installed.		#
# --enable-sparse							#
#	Used to compile with the sparse static analyser.		#
# --enable-systemd							#
#	Used to provide systemd setup.					#
#									#
# Layout: (Only relevant sections will exist).				#
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
# SECTION 12: Check for pkg-config files or alternatives		#
# SECTION 13: Check for system services					#
# SECTION 14: Additional checks & variables				#
# SECTION 15: Specify the output files					#
#									#
#########################################################################


#########################################
#					#
# SECTION  1: Initialisation & Setup	#
#					#
#########################################

AC_COPYRIGHT([Copyright (C) 2015-2024  Mark Grant
	      This script is free to copy or distribute under the terms set out
	      in the package license. See COPYING for details.])

AC_REVISION($Revision: 1.4.0 $)

AC_PREREQ(2.69)

#
# The following macro uses git, if available, to provide versioning information.
# The logic is as follows:-
# If git is not available, is not initialised or no commit has been made, then
# the local variable pkg_version will be used.
# If git is available but no Tag has been created then just a commit identifier
# will be used.
# If git is available and a tag has been created then the most recent tag, plus
# number of commits since the tag, plus a commit identifier will be used.
# The package version in ac init will be used to create an output variable which
# can be used by ac output and / or the sed setup below to substitute bookmarks
# in source files.
#
# Set pkg_version for a default hard-coded version number.
m4_define(pkg_version, 1.7.1-rc2)

AC_INIT([Server Wait On Client Package], m4_esyscmd([ \
	git describe --always 1>/dev/null 2>/dev/null \
	&& sed --help 1>/dev/null 2>/dev/null \
	&& git describe --always | sed 's/v//' | tr -d '\n' \
	|| echo pkg_version | tr -d '\n']),
	[m.grant.prg@gmail.com], [swoc])

AC_MSG_NOTICE(initialisation and setup)

AC_DEFINE(COMMON_SOURCE_VERSION, "1.2.0", \
	Define to the version of source code for the common library.)

AC_SUBST(common_lib_version_info, "3:6:0")

AC_DEFINE(CLIENT_SOURCE_VERSION, "1.2.0", \
	Define to the version of source code for the client library.)

AC_SUBST(client_lib_version_info, "2:4:0")

AC_DEFINE(SERVER_SOURCE_VERSION, "1.2.0", \
	Define to the version of source code for the server library.)

AC_SUBST(server_lib_version_info, "3:4:0")

AC_DEFINE(SWOCCLIENT_SOURCE_VERSION, "1.2.0", \
	Define to the version of source code for the client program.)

AC_DEFINE(SWOCSERVER_SOURCE_VERSION, "1.2.0", \
	Define to the version of source code for the server program.)

AC_DEFINE(SWOCSERVERD_SOURCE_VERSION, "1.1.0", \
	Define to the version of source code for the server daemon.)

AC_SUBST(pkgversion, $PACKAGE_VERSION)

# Separate AutoTools generated files from project root.
AC_CONFIG_AUX_DIR([build-aux])

#
# Canonical targets are required by AX_PTHREAD.
#
AC_CANONICAL_TARGET

#
# Passes the options to all am files. Puts objects in the sub-directory
# containing the source. Forces tar to use --format=posix in make dist.
# This also checks :-	BSD install	make variable set	mkdir -p
#			gawk
#
AM_INIT_AUTOMAKE(-Wall -Wno-portability -Werror subdir-objects tar-pax)

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
# The following allows for an enable-analyzer=yes/no command line option.
# The conditional ANALAYZER is made available to the makefiles.
#
AC_ARG_ENABLE(analyzer,
	AS_HELP_STRING([--enable-analyzer], [build to display analyzer output]),
	[case "${enableval}" in
		yes) analyzer=true ;;
		no)  analyzer=false ;;
		*) AC_MSG_ERROR(bad value ${enableval} in enable-analyzer);;
	esac],analyzer=false)
AM_CONDITIONAL(ANALYZER, test x$analyzer = xtrue)

AC_MSG_CHECKING([whether to build for analyzer output])
if test "x${analyzer}" = xtrue; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi

#
# The following allows for an enable-atonly=yes/no command line option. The
# conditional ATONLY is made available to the makefiles.
# This allows for a workaround where AutoTools normally has a prefix of
# /usr/local but anything pre-installed would normally be in /usr.
#
AC_ARG_ENABLE(atonly,
	AS_HELP_STRING([--enable-atonly], \
			[build for AT only testing or installation]),
	[case "${enableval}" in
		yes) atonly=true ;;
		no)  atonly=false ;;
		*) AC_MSG_ERROR(bad value ${enableval} for --enable-atonly) ;;
	esac],atonly=false)
AM_CONDITIONAL(ATONLY, test x$atonly = xtrue)

AC_MSG_CHECKING([whether to build for AT only testing or installation])
if test "x${atonly}" = xtrue; then
	AC_SUBST(docbaseloc,"/usr/share/doc-base")
	AC_MSG_RESULT([yes])
else
	AC_SUBST(docbaseloc,"$datarootdir/doc-base")
	AC_MSG_RESULT([no])
fi

#
# The following allows for an enable-debug=yes/no command line option. The
# conditional DEBUG is made available to the makefiles.
#
AC_ARG_ENABLE(debug,
	AS_HELP_STRING([--enable-debug], [compile for debugging]),
	[case "${enableval}" in
		yes) debug=true ;;
		no)  debug=false ;;
		*) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
	esac],debug=false)
AM_CONDITIONAL(DEBUG, test x$debug = xtrue)

AC_MSG_CHECKING([whether to build in debug mode])
if test "x${debug}" = xtrue; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi

#
# If a file location has to be fixed, e.g. /etc/foo.conf and never
# /usr/local/etc/foo.conf, then make distcheck will fail. Setting this flag
# allows make distcheck to locate the file in an AT standard directory-based
# location.
# eg /usr/local/etc/foo.conf.
# and hence succeed.
#
AC_ARG_ENABLE([distcheckfake],
	AS_HELP_STRING([--enable-distcheckfake], [enable running of distcheck]),
	[case "${enableval}" in
		yes) distcheckfake=true ;;
		no)  distcheckfake=false ;;
		*) AC_MSG_ERROR([bad value ${enableval} for \
			--enable-distcheckfake]) ;;
	esac],[distcheckfake=false])
AM_CONDITIONAL([DISTCHECKFAKE], [test x$distcheckfake = xtrue])

AC_MSG_CHECKING([whether to use workarounds to facilitate distcheck])
if test "x${distcheckfake}" = xtrue; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi

#
# The following allows for an enable-headercheck=yes/no command line option.
# The conditional HEADERCHECK is made available to the makefiles.
#
AC_ARG_ENABLE(headercheck,
	AS_HELP_STRING([--enable-headercheck], [build to display header stack]),
	[case "${enableval}" in
		yes) headercheck=true ;;
		no)  headercheck=false ;;
		*) AC_MSG_ERROR(bad value ${enableval} in enable-headercheck);;
	esac],headercheck=false)
AM_CONDITIONAL(HEADERCHECK, test x$headercheck = xtrue)

AC_MSG_CHECKING([whether to print header stack depth])
if test "x${headercheck}" = xtrue; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi

#
# The following allows for an enable-iwyu=yes/no command line option.
# The result is only used in configure.
#
AC_ARG_ENABLE(iwyu,
	AS_HELP_STRING([--enable-iwyu], [include-what-you-use is selected]),
	[case "${enableval}" in
		yes) enable_iwyu=true ;;
		no)  enable_iwyu=false ;;
		*) AC_MSG_ERROR(bad value ${enableval} in enable-iwyu);;
	esac],enable_iwyu=false)

AC_MSG_CHECKING([whether include-what-you-use is selected])
if test "x${enable_iwyu}" = xtrue; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi

#
# The following allows for an enable-scan-build=yes/no command line option.
# The result is only used in configure.
#
AC_ARG_ENABLE(scan_build,
	AS_HELP_STRING([--enable-scan-build], [scan-build use is selected]),
	[case "${enableval}" in
		yes) scan_build=true ;;
		no)  scan_build=false ;;
		*) AC_MSG_ERROR(bad value ${enableval} in enable-scan-build);;
	esac],scan_build=false)

AC_MSG_CHECKING([whether scan-build is selected])
if test "x${scan_build}" = xtrue; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi

#
# The following allows for an enable-sparse=yes/no command line option. The
# conditional SPARSE is made available to the makefiles.
#
AC_ARG_ENABLE(sparse,
	AS_HELP_STRING([--enable-sparse], [build with sparse]),
	[case "${enableval}" in
		yes) sparse=true ;;
		no)  sparse=false ;;
		*) AC_MSG_ERROR(bad value ${enableval} for --enable-sparse);;
	esac],sparse=false)
AM_CONDITIONAL(SPARSE, test x$sparse = xtrue)

AC_MSG_CHECKING([whether to use sparse])
if test "x${sparse}" = xtrue; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi

#
# The following allows for an enable-systemd=yes/no command line option. The
# conditional SYSD is made available to the makefiles.
#
AC_ARG_ENABLE(systemd,
	AS_HELP_STRING([--enable-systemd], [build for systemd]),
	[case "${enableval}" in
		yes) systemd=true ;;
		no)  systemd=false ;;
		*) AC_MSG_ERROR(bad value ${enableval} for --enable-systemd);;
	esac],systemd=false)
AM_CONDITIONAL(SYSD, test x$systemd = xtrue)

AC_MSG_CHECKING([whether to build for systemd])
if test "x${systemd}" = xtrue; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi


#################################
#				#
# SECTION  4: Compiler setup	#
#				#
#################################

AC_MSG_NOTICE(compiler setup)

# Set language for compilation tests.
AC_LANG(C)

# CFLAGS is usually set to -g -O2, but for debugging we will probably
# want -g -Og and for production we will probably want -O2. So empty CFLAGS here
# and leave the selection to the Makefile.am's. Both this clearing of the flags
# and the default setting mentioned above are still overridden if CFLAGS is set
# on the command line by the user. (This must appear between AC INIT and
# AC PROG CC).
: ${CFLAGS=""}

# Checks the compiler to use, in order, from either the CC variable passed to
# configure, if sparse is specified, or a standard list. The chosen value is
# stored in the CC variable and then configure checks its capabilities.
if test -n "$CC" ; then
	compiler_list="$CC"
elif test "x$sparse" = "xtrue" ; then
	compiler_list="cgcc"
else
	compiler_list="gcc clang cc"
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
AC_CHECK_PROG(DOT, dot, yes, no)
if test "x$DOT" = "xno" ; then
	AC_MSG_ERROR(dot not found)
fi

AC_CHECK_PROG(DOXYGEN, doxygen, yes, no)
if test "x$DOXYGEN" = "xno" ; then
	AC_MSG_ERROR(doxygen not found)
fi

if test "x${enable_iwyu}" = xtrue; then
	AC_CHECK_PROG(IWYU, iwyu,yes, no)
	if test "x$IWYU" = "xno" ; then
		AC_MSG_ERROR(iwyu not found)
	fi
fi

if test "x${scan_build}" = xtrue; then
	AC_CHECK_PROG(SCAN_BUILD, scan-build, yes, no)
	if test "x$SCAN_BUILD" = "xno" ; then
		AC_MSG_ERROR(scan-build not found)
	fi
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
AC_PATH_PROG([BASH], [bash], [no])
if test "x$BASH" = "xno" ; then
	AC_MSG_ERROR([bash not found])
fi

AC_MSG_NOTICE(checking for particular programs required at run time)
AC_CHECK_PROG(GETOPT, getopt, yes, no)
if test "x$GETOPT" = "xno" ; then
	AC_MSG_WARN(getopt not found)
fi


#########################################
#					#
# SECTION  7: Check for header files	#
#					#
#########################################

AC_MSG_NOTICE(checking for header files required by this application)

AC_CHECK_HEADERS(stdio.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(stdlib.h, , AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(stdarg.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(sysexits.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(signal.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(limits.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(netinet/in.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(netdb.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(sys/socket.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(arpa/inet.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(sys/stat.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(sys/types.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(string.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(ctype.h, , AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(unistd.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(syslog.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(errno.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(getopt.h, [], AC_MSG_ERROR(header not found))
AC_CHECK_HEADERS(libssh/libssh.h, [], AC_MSG_ERROR(header not found))
if test -f `dirname $0`/src/prg/c/inc-tmp/libmgec/mge-portability.h ; then
	AC_MSG_NOTICE(mge-portability.h is in the internal inc-tmp directory)
else
	AC_CHECK_HEADERS(libmgec/mge-portability.h, [],
			 AC_MSG_ERROR(header not found))
fi
if test -f `dirname $0`/src/prg/c/inc-tmp/libmgec/mge-buffer.h ; then
	AC_MSG_NOTICE(mge-buffer.h is in the internal inc-tmp directory)
else
	AC_CHECK_HEADERS(libmgec/mge-buffer.h, [],
			 AC_MSG_ERROR(header not found))
fi
if test -f `dirname $0`/src/prg/c/inc-tmp/libmgec/mge-message.h ; then
	AC_MSG_NOTICE(mge-message.h is in the internal inc-tmp directory)
else
	AC_CHECK_HEADERS(libmgec/mge-message.h, [],
			 AC_MSG_ERROR(header not found))
fi
if test -f `dirname $0`/src/prg/c/inc-tmp/libmgec/mge-memory.h ; then
	AC_MSG_NOTICE(mge-memory.h is in the internal inc-tmp directory)
else
	AC_CHECK_HEADERS(libmgec/mge-memory.h, [],
			 AC_MSG_ERROR(header not found))
fi
if test -f `dirname $0`/src/prg/c/inc-tmp/libmgec/mge-bstree.h ; then
	AC_MSG_NOTICE(mge-bstree.h is in the internal inc-tmp directory)
else
	AC_CHECK_HEADERS(libmgec/mge-bstree.h, [],
			 AC_MSG_ERROR(header not found))
fi
if test -f `dirname $0`/src/prg/c/inc-tmp/libmgec/mge-errno.h ; then
	AC_MSG_NOTICE(mge-errno.h is in the internal inc-tmp directory)
else
	AC_CHECK_HEADERS(libmgec/mge-errno.h, [],
			 AC_MSG_ERROR(header not found))
fi
if test -f `dirname $0`/src/prg/c/inc-tmp/libmgesysutils/mge-configfile.h ; then
	AC_MSG_NOTICE(mge-configfile.h is in the internal inc-tmp directory)
else
	AC_CHECK_HEADERS(libmgesysutils/mge-configfile.h, [],
			 AC_MSG_ERROR(header not found))
fi
if test -f `dirname $0`/src/prg/c/inc-tmp/libmgesysutils/mge-remsyslog.h ; then
	AC_MSG_NOTICE(mge-remsyslog.h is in the internal inc-tmp directory)
else
	AC_CHECK_HEADERS(libmgesysutils/mge-remsyslog.h, [],
			 AC_MSG_ERROR(header not found))
fi

AC_HEADER_STDBOOL


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

# Build suitable pre-processor, compiler and linker flags.
MG_BUILD_COMPILER_VERSION_CPPFLAGS(MG_CPPFLAGS, MG_DEBUG_CPPFLAGS)
MG_BUILD_COMPILER_VERSION_CFLAGS(MG_CFLAGS, MG_DEBUG_CFLAGS)
MG_BUILD_COMPILER_VERSION_ANALYZER_CFLAGS(MG_ANALYZER_CFLAGS)
MG_BUILD_LDFLAGS(MG_LDFLAGS)


#################################################
#						#
# SECTION 11: Check for library functions	#
#						#
#################################################

AC_MSG_NOTICE(checking for GNU compatible library functions required)

AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([getpass memset socket strchr strerror strncasecmp strsignal])
AC_CHECK_FUNCS([strtol])


#################################################################
#								#
# SECTION 12: Check for pkg-config files or alternatives	#
#								#
#################################################################

AC_MSG_NOTICE(Processing required libraries)

# Use tmplibraries if present
#	else use pkg-config files if present and suitable giving XXX_CFLAGS and
#	XXX_LIBS
#		else use Search_Libs giving LIBS
# As per p274, Autotools, 2nd edition, 2020, John Calcote - Do not use
# Pkg_Check_Modules macro, it has problems under common conditions.
if [test -f `dirname $0`/src/prg/c/lib-tmp/libmgec/libmgec.la];then
	AC_MSG_NOTICE([Using tmplibraries AOT pkg-config for libmgec library])
elif pkg-config --atleast-version=1.7.0 libmgec; then
	AC_MSG_NOTICE([Using pkg-config for libmgec library])
	AC_SUBST([LIBMGEC_CFLAGS], [$(pkg-config --cflags libmgec)])
	AC_SUBST([LIBMGEC_LIBS], [$(pkg-config --libs libmgec)])
else
	AC_MSG_NOTICE([Using Search_LIBS to find libmgec library])
	AC_SEARCH_LIBS([pull_msg], [mgec], [],
		[AC_MSG_ERROR([function pull_msg not found in libmgec])])
fi

if [test -f `dirname $0`/src/prg/c/lib-tmp/libmgesysutils/libmgesysutils.la]
then
	AC_MSG_NOTICE([Using tmplibraries AOT pkg-config for libmgesysutils])
elif pkg-config --atleast-version=1.5.0 libmgesysutils; then
	AC_MSG_NOTICE([Using pkg-config for libmgesysutils library])
	AC_SUBST([LIBMGESYSUTILS_CFLAGS], [$(pkg-config --cflags libmgesysutils)])
	AC_SUBST([LIBMGESYSUTILS_LIBS], [$(pkg-config --libs libmgesysutils)])
else
	AC_MSG_NOTICE([Using Search_LIBS to find libmgesysutils library])
	AC_SEARCH_LIBS([parsefile], [mgesysutils], [],
		[AC_MSG_ERROR([function parsefile not found in libmgesysutils])])
fi

if [test -f `dirname $0`/src/prg/c/lib-tmp/libssh/libssh.la];then
	AC_MSG_NOTICE([Using tmplibraries AOT pkg-config for libssh library])
elif pkg-config --atleast-version=0.9.5 libssh; then
	AC_MSG_NOTICE([Using pkg-config for libssh library])
	AC_SUBST([LIBSSH_CFLAGS], [$(pkg-config --cflags libssh)])
	AC_SUBST([LIBSSH_LIBS], [$(pkg-config --libs libssh)])
else
	AC_MSG_NOTICE([Using Search_LIBS to find libssh library])
	AC_SEARCH_LIBS([ssh_get_server_publickey], [ssh], [],
		[AC_MSG_ERROR([ssh_get_server_publickey not found in libssh])])
fi


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
		-e 's|@cli_pc_requires[@]|\$(cli_pc_requires)|g' \
		-e 's|@cli_pc_requires_private[@]|\$(cli_pc_requires_private)|g' \
		-e 's|@com_pc_requires[@]|\$(com_pc_requires)|g' \
		-e 's|@com_pc_requires_private[@]|\$(com_pc_requires_private)|g' \
		-e 's|@srv_pc_requires[@]|\$(srv_pc_requires)|g' \
		-e 's|@srv_pc_requires_private[@]|\$(srv_pc_requires_private)|g' \
		-e 's|@pkgversion[@]|\$(pkgversion)|g' \
		-e 's|@bashlocation[@]|\$(BASH)|g' \
		-e 's|@docbaseloc[@]|\$(docbaseloc)|g' \
		-e 's|@CC[@]|\$(CC)|g' \
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

AC_MSG_CHECKING([for temporary headers])
cd `dirname $0`/src/prg/c/inc-tmp
tmpheaders=$(echo $(find -name *.h))
cd - > /dev/null
if test X"$tmpheaders" = X ; then
	AC_MSG_RESULT([none])
else
	AC_MSG_RESULT([using $tmpheaders])
fi

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

AC_MSG_CHECKING([for temporary libraries])

cd `dirname $0`
topdir=$(find ~+ -type d -name atbuild -prune -o -type d -name lib-tmp -print)
cd - > /dev/null

for direntry in $topdir/*
do
	if test -d "$direntry" ;then
		for entry in $direntry/*.la
		do
			if test -f "$entry" ;then
				tmplibraries+=" $entry"
			fi
		done
	fi
done
if test X"$tmplibraries" = X ; then
	AC_MSG_RESULT([none])
else
	AC_MSG_RESULT([using $tmplibraries])
fi

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
# This manual section mentions bindir, includedir etc but makes no mention of
# any extended forms such as pkgincludedir, and it indeed seems that they are
# not supported. Variable substitution in the Makefile.am's should be used
# instead.
#
AC_CONFIG_FILES(docs/doxygen/Doxyfile)
AC_CONFIG_FILES(src/conf/etc/com-lib/swoc.conf)
AC_CONFIG_FILES(src/conf/etc/srv-prg/swocserverd.conf)

# Causes AC_OUTPUT to create a c preprocessor file with #define statements about
# the configuration.
AC_CONFIG_HEADERS(src/prg/c/gen/inc/config.h)


# Project Makefiles to create.
AC_CONFIG_FILES(Makefile
		docs/doxygen/Makefile
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
		src/prg/c/inc/swoc/Makefile
		src/prg/c/inc-tmp/Makefile
		src/prg/c/lib-tmp/Makefile
		src/prg/c/pkg-config/Makefile
		src/prg/c/src/cli-lib/Makefile
		src/prg/c/src/cli-prg/Makefile
		src/prg/c/src/com-lib/Makefile
		src/prg/c/src/srv-lib/Makefile
		src/prg/c/src/srv-prg/swocserver/Makefile
		src/prg/c/src/srv-prg/swocserverd/Makefile
		src/prg/c/src/tests/cli-lib/Makefile
		src/prg/c/src/tests/com-lib/Makefile
		src/prg/c/src/tests/srv-lib/Makefile
		src/prg/c/src/tests/test-headers/Makefile
		src/prg/c/src/tests/run-tests/Makefile
		src/systemd/srv-prg/Makefile)

AC_OUTPUT

