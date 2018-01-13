Remote SysLog
=============
[TOC]
Functions around remote syslogs.

\section sndremsyslogmsg Send Remote SysLog Message

Sends a message to a remote syslog server using an UDP Datagram on the standard
'syslog' port. Obviously the remote server has to be set up to accept incoming
syslog messages.
