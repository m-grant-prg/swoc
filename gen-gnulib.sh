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
# To perform initial gnulib setup for the project.			#
#									#
# Syntax:	bootstrap.sh	[ -h || --help ] ||			#
#				[ -V || --version ] ||			#
#				[ Project root directory ]		#
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
# 07/02/2018	MG	1.0.1	Created.				#
#									#
#########################################################################

##################
# Init variables #
##################
script_exit_code=0
version="1.0.1"			# set version variable
packageversion=v1.0.1-4-g7222bec	# Version of the complete package

basedir="."


#############
# Functions #
#############

# Output $1 to stdout or stderr depending on $2.
output()
{
	if [ $2 = 0 ]
	then
		echo "$1"
	else
		echo "$1" 1>&2
	fi
}

# Standard function to test command error ($1 is $?) and exit if non-zero
std_cmd_err_handler()
{
	if [ $1 != 0 ]
	then
		script_exit_code=$1
		script_exit
	fi
}

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
# Process command line arguments with GNU getopt.
GETOPTTEMP=`getopt -o hV --long help,version -n "$0" -- "$@"`
std_cmd_err_handler $?

eval set -- "$GETOPTTEMP"
std_cmd_err_handler $?

while true
do
	case "$1" in
	-h|--help)
		echo "Usage is [ Option ] [ Project root directory ]:-"
		echo "	-h or --help displays usage information"
		echo "	-V or --version displays version information"
		echo "	Project root directory, defaults to current directory."
		shift
		script_exit_code=0
		script_exit
		;;
	-V|--version)
		echo "$0 Script version "$version
		echo "$0 Package version "$packageversion
		shift
		script_exit_code=0
		script_exit
		;;
	--)	shift
		break
		;;
	*)	script_exit_code=1
		output "Internal error." 1
		script_exit
		;;
	esac
done

# Script can accept 1 other argument, the base directory.
if [ $# -gt 1 ]
then
	script_exit_code=1
	output "Invalid argument." 1
	script_exit
fi

if [ $# -eq 1 ]
then
	basedir=$1
fi


gnulib-tool --import --dir=$basedir --source-base=src/prg/c/gen/lib \
	--no-conditional-dependencies --no-libtool --no-vc-files configmake
script_exit_code=$?
script_exit
