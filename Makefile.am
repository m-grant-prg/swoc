#########################################################################
#									#
# Script ID: ./Makefile.am						#
# Author: Copyright (C) 2015-2021  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate SWOCServer-C project from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 22/06/2015	MG	1.0.1	First release.				#
# 24/10/2015	MG	1.0.2	Restructure and change to conform to	#
#				AutoTools General Template v1.0.6.	#
# 10/05/2016	MG	1.0.3	Move header files to include directory.	#
# 28/05/2016	MG	1.0.4	Modified following libswocserver	#
#				library introduction.			#
# 20/09/2016	MG	1.0.5	Add swocserverd daemon directory.	#
# 23/09/2016	MG	1.0.6	Add bash directory following move from	#
#				libswocserver package.			#
# 10/01/2017	MG	1.1.0	Move bash config file setup script back	#
#				to libswocserver package as that	#
#				library can exist without this server	#
#				package.				#
#				Add systemd service build. This only	#
#				gets executed if configure is run with	#
#				--enable-systemd=yes.	 		#
# 19/05/2017	MG	1.1.1	Add support for temporary header	#
#				directory.				#
#				Add Make locations for man 5 pages and	#
#				bash.					#
# 07/06/2017	MG	1.1.2	Add support for temporary libraries.	#
# 12/01/2018	MG	1.1.3	Add SPDX license tags to source files.	#
# 16/01/2018	MG	1.1.4	Correct generation of config.h		#
# 05/02/2018	MG	1.1.5	Remove unnecessary -I m4extra.		#
# 26/04/2018	MG	1.1.6	Add srctarball target to build a source	#
#				tarball in the build directory. Uses	#
#				git archive to archive HEAD.		#
# 08/06/2018	MG	1.1.7	Add info messages to srctarball target.	#
# 26/10/2018	MG	1.1.8	Change srctarball target to build with	#
#				src between package name and version in	#
#				filename.				#
# 19/05/2019	MG	1.1.9	Merge sub-projects into one.		#
#				Make source tarball name == GitHub.	#
#				Add install of standard GNU files such	#
#				as AUTHORS etc.				#
#				Remove shebang.				#
# 06/11/2019	MG	1.1.10	To enable make distcheck to work, any	#
#				hard-coded paths requiring permissions	#
#				such as /etc or fixed locations such as	#
#				/usr/share/java must be accommodated,	#
#				so set the configure flags to be used	#
#				by make distcheck.			#
# 27/03/2020	MG	1.1.11	Move the cli-prg source into the	#
#				swocclient sub-directory as the		#
#				directory hierarchy needs to be the	#
#				same accross the source tree for	#
#				temporary libraries to work based on	#
#				the search in configure.ac.		#
# 14/8/2021	MG	1.1.12	Add systemd action ordering comment.	#
#				Add library install post-processing	#
#				comments.				#
#				Add doc/var and misc to DIST.		#
#									#
#########################################################################

#########################################################################
#									#
# N.B.									#
# ====									#
# The sequence of the build is important for systemd, see below.	#
# See requirements for library install post-processing below.		#
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
SUBDIRS += src/prg/c/inc/com-dev
SUBDIRS += src/prg/c/pkg-config/com-dev
SUBDIRS += src/prg/c/src/com-lib/libswoccommon
SUBDIRS += src/prg/c/src/com-lib/test-messages
SUBDIRS += src/prg/c/src/com-lib/test-ssh
SUBDIRS += src/prg/c/src/com-lib/test-tcp
SUBDIRS += src/prg/c/src/com-lib/test-validate
SUBDIRS += src/prg/c/inc/cli-dev
SUBDIRS += src/prg/c/pkg-config/cli-dev
SUBDIRS += src/prg/c/src/cli-lib/libswocclient
SUBDIRS += src/prg/c/src/cli-lib/test-optionproc
SUBDIRS += src/prg/c/inc/srv-dev
SUBDIRS += src/prg/c/pkg-config/srv-dev

# The last library installed / uninstalled must perform a ldconfig
# Install hook
# 	ldconfig
# Uninstall hook
# 	ldconfig
SUBDIRS += src/prg/c/src/srv-lib/libswocserver
SUBDIRS += src/prg/c/src/srv-lib/test-optionproc
SUBDIRS += src/prg/c/src/cli-prg/swocclient
SUBDIRS += src/prg/c/inc/srv-prg

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
SUBDIRS += doc/doxygen


doc_DATA = AUTHORS ChangeLog COPYING NEWS README


EXTRA_DIST = $(srcdir)/AUTHORS $(srcdir)/ChangeLog $(srcdir)/COPYING \
		$(srcdir)/NEWS $(srcdir)/README
EXTRA_DIST += m4/gnulib-cache.m4
EXTRA_DIST += doc/var misc


srctarball:
	@cwd="$${PWD}" && \
	targ="$${cwd}/@PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz" && \
	cd @srcdir@ && \
	echo "making srctarball from @srcdir@" && \
	echo "GA	$${targ}" && \
	git archive --format=tar.gz \
		--prefix=@PACKAGE_TARNAME@-@PACKAGE_VERSION@/ \
		--output=$${targ} \
		HEAD && \
	cd -

