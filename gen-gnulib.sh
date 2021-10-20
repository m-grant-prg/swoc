#! /bin/bash
#########################################################################
#									#
#	gen-gnulib.sh is automatically generated,			#
#		please do not modify!					#
#									#
#########################################################################

#########################################################################
#									#
# Script ID: gen-gnulib.sh						#
# Author: Copyright (C) 2018, 2021  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
# To simplify the running of daily apt-get maintenance commands.	#
#									#
# Syntax:	gen-gnulib.sh [ -h || --help ] || [ -V || --version ]	#
#									#
# Exit codes used:-							#
# Bash standard Exit Codes:	0 - success				#
#				1 - general failure			#
# User-defined exit code range is 64 - 113				#
#	C/C++ Semi-standard exit codes from sysexits.h range is 64 - 78	#
#		EX_USAGE	64	command line usage error	#
#		EX_DATAERR	65	data format error		#
#		EX_NOINPUT	66	cannot open input		#
#		EX_NOUSER	67	addressee unknown		#
#		EX_NOHOST	68	host name unknown		#
#		EX_UNAVAILABLE	69	service unavailable		#
#		EX_SOFTWARE	70	internal software error		#
#		EX_OSERR	71	system error (e.g., can't fork)	#
#		EX_OSFILE	72	critical OS file missing	#
#		EX_CANTCREAT	73	can't create (user) output file	#
#		EX_IOERR	74	input/output error		#
#		EX_TEMPFAIL	75	temp failure; user is invited	#
#					to retry			#
#		EX_PROTOCOL	76	remote error in protocol	#
#		EX_NOPERM	77	permission denied		#
#		EX_CONFIG	78	configuration error		#
#	User-defined (here) exit codes range 79 - 113:			#
#		None							#
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
# 19/10/2021	MG	1.0.2	Major overhaul.				#
#									#
#########################################################################


##################
# Init variables #
##################
readonly version=1.0.2				# Script version
readonly outputprefix="$(basename $0): "
readonly packageversion=v1.1.0		# Package version

basedir="."


#############
# Functions #
#############

# -h --help output.
# No parameters
# No return value
usage ()
{
cat << EOF
Usage is $0 [OPTIONS]
	[OPTIONS] are:-
	-h or --help Displays usage information
	-V or --version Displays version information
EOF
}

# Standard function to emit messages depending on various parameters.
# Parameters -	$1 What:-	The message to emit.
#		$2 Where:-	stdout == 0
#				stderr == 1
# No return value.
output()
{
	if (( !$2 )); then
		printf "%s\n" "$1"
	else
		printf "%s\n" "$1" 1>&2
	fi
}

# Standard function to tidy up and return exit code.
# Parameters - 	$1 is the exit code.
# No return value.
script_exit()
{
	exit $1
}

# Standard function to test command error and exit if non-zero.
# Parameters - 	$1 is the exit code, (normally $? from the preceeding command).
# No return value.
std_cmd_err_handler()
{
	if (( $1 )); then
		script_exit $1
	fi
}

# Standard trap exit function.
# No parameters.
# No return value.
trap_exit()
{
	local -i exit_code=$?
	local msg

	msg="Script terminating with exit code $exit_code due to trap received."
	output "$msg" 1
	script_exit $exit_code
}

# Setup trap.
trap trap_exit SIGHUP SIGINT SIGQUIT SIGTERM

# Process command line arguments with GNU getopt.
# Parameters -	$1 is the command line.
# No return value.
proc_CL()
{
	local GETOPTTEMP
	local tmpGETOPTTEMP

	tmpGETOPTTEMP="getopt -o hV --long help,version"
	GETOPTTEMP=$($tmpGETOPTTEMP -n "$0" -- "$@")
	std_cmd_err_handler $?

	eval set -- "$GETOPTTEMP"
	std_cmd_err_handler $?

	while true; do
		case "$1" in
		-h|--help)
			usage
			shift
			script_exit 0
			;;
		-V|--version)
			printf "Script version %s\n" $version
			printf "Package version %s\n" $packageversion
			shift
			script_exit 0
			;;
		--)	shift
			break
			;;
		*)	output "Internal error." 1
			script_exit 64
			;;
		esac
	done

	# Pass anything after the -- straight to the commands.
	if (( $# )); then
		basedir=$@
	fi
}


########
# Main #
########

proc_CL "$@"

gnulib-tool --import --dir=$basedir --source-base=src/prg/c/gen/lib \
	--no-conditional-dependencies --no-libtool --no-vc-files configmake
status=$?
std_cmd_err_handler $status

script_exit 0

