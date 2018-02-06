#! /usr/bin/env bash
#########################################################################
#									#
#	gen-gnulib.sh is automatically generated,			#
#		please do not modify!					#
#									#
#########################################################################

#########################################################################
#									#
# Script ID: gen-gnulib.sh						#
# Author: Copyright (C) 2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
# To instantiate the gnulib options.					#
#									#
# Syntax:	gen-gnulib.sh						#
#									#
# Exit Codes:	0 - success						#
#		1 - failure.						#
#									#
# Further Info:								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 06/02/2018	MG	1.0.1	Created.				#
#									#
#########################################################################

##################
# Init variables #
##################
script_exit_code=0
version="1.0.1"			# set version variable
packageversion=v1.2.2-1-g9ae5094	# Version of the complete package


#############
# Functions #
#############

# Standard function to cleanup and return exit code
script_exit()
{
	exit $script_exit_code
}

# Standard trap exit function
trap_exit()
{
script_exit_code=1
output "Script terminating due to trap received." 1
script_exit
}

# Setup trap
trap trap_exit SIGHUP SIGINT SIGTERM

########
# Main #
########
gnulib-tool --import --source-base=src/prg/c/gen/lib \
	--no-conditional-dependencies --no-libtool --no-vc-files configmake
script_exit_code=$?
script_exit
