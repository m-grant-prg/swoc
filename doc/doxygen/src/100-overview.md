Overview					{#mainpage}
========
\tableofcontents
\section general General

The Server Wait On Client (SWOC) system provides a simple lock-flag based method
of synchronising activities between 1 or more clients and a server.

Communication is via TCP which can optionally be secured by using an SSH tunnel.
\n

\section components Components
The system is split into 9 packages providing the various elememts:

> __libswoccommon__\n
> Library containing functions common to server and client.

\n

> __libswoccommon-dev__\n
> Development files for the libswoccommon library (Include files, man
> pages, etc).

\n

> __libswocclient__\n
> Library containing client-side functions.

\n

> __libswocclient-dev__\n
> Development files for the libswocclient library (Include files, man
> pages, etc).

\n

> __swocclient-c__\n
> The command line program for controlling locks from the client side.

\n

> __libswocserver__\n
> Library containing server-side functions.

\n

> __libswocserver-dev__\n
> Development files for the libswocserver library (Include files, man
> pages, etc).

\n

> __swocserver-c__\n
> The command line program for controlling locks from the server-side.

\n

> __swoc-doc__\n
> This documentation package.
\n

\section requirements Requirements
The system requires the following packages:-
\n
\subsection reqforruntime For Run-Time
- The MGE General C Library (libmgec).

- The MGE System Utilities Library (libmgesysutils).

- The SSH library (libssh). The distro package comes in multiple flavours
depending on which encryption library libssh has been linked against. An example
on Debian would be libssh-gcrypt-4.
\n
\subsection reqfordev For Development
- The MGE General C Library development package(libmgec-dev).

- The MGE System Utilities Library development package (libmgesysutils-dev).

- The SSH library development package. Use the development package associated
with the choice above. In this case on Debian it would be libssh-gcrypt-dev.

- Doxygen.
\n
