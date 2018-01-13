Configuration File
==================
[TOC]
\section parsefile Parse Configuration File

The parsefile function in the libmgesysutils library processes a configuration
file consisting of sections and parameters of key / value pairs.

Whitespace lines and empty lines are ignored.

Lines with a '#' as the first non-whitespace character are ignored as comment
lines.

The first parameter line must be a section header.

Section headers are enclosed in [], [ must be the first non-whitespace character
on the line and anything after the first ] is ignored.

All other lines are assumed to be parameter lines and after any initial
whitespace characters they must have a parameter name, optional whitespace, a
mandatory '=', optional whitespace and finishing with an optional word of
characters and / or numbers.

If a parameter is repeated then the last occurrence prevails.

An array of struct confsection's is provided by the library user to specify
valid parameters and whether they are mandatory. A mandatory Key / Value pair
for an optional Section is only enforced if the Section does appear, (is
defined), in the config file.

    EXAMPLE
	\# Section General.
	[General]
	\# pollint is the polling interval in seconds.
	pollint=5 <== This is legal.
	large pollint=5 <== This is legal.
	pollint = <== This is legal.
	pollint	<== This is illegal.


	Use a pointer to something like the following array. The struct
	confsection can be declared in myapp.h.
	struct confsection sections[] = {
		{"General", 0, 0, {
			{"pollint", 0, 0, ""}
			}
		},
		{"Full", 0, 0, {
			{"Server", 0, 0, ""},
			{"DayOfWeek", 1, 0, ""}
			}
		},
		{"Incremental", 0, 0, {
			{"Server", 0, 0, ""},
			{"DayOfWeek", 0, 0, ""}
			}
		},
		{"Output", 1, 0, {
			{"Verbose", 1, 0, ""},
			{"Bold", 0, 0, ""}
			}
		}
	};
	N.B. There appear to be more braces than should be required
	and this is so, however, removing the extra braces results
	in compiler warnings.
