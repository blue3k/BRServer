;
;////////////////////////////////////////////////////////////////////////////////
;// 
;// CopyRight (c) 2013 The Braves
;// 
;// Author : madk
;//
;// Description : Server Error definitions. 
;//
;////////////////////////////////////////////////////////////////////////////////
;
;
;#pragma once
;
;

SeverityNames=(Success=0x0:STATUS_SEVERITY_SUCCESS
               Informational=0x1:STATUS_SEVERITY_INFORMATIONAL
               Warning=0x2:STATUS_SEVERITY_WARNING
               Error=0x3:STATUS_SEVERITY_ERROR
              )
              
FacilityNames=(	BR=0x701:FACILITY_BR
				Generic=0x702:FACILITY_GENERIC
				Net=0x703:FACILITY_NET
				SvrSys=0x704:FACILITY_SVRSYS
				Login=0x705:FACILITY_LOGIN
				Game=0x706:FACILITY_GAME
				Zone=0x707:FACILITY_ZONE
				Community=0x708:FACILITY_COMMUNITY)

;
;#define STATUS_SEVERITY(hr)  (((hr) >> 30) & 0x3)
;

;// Customer Bit is set by the message compiler command line arguments

;
;/////////////////////////////////////////////////////////////////////////
;//
;// Net Success Codes
;//
;/////////////////////////////////////////////////////////////////////////
;

MessageId= Severity=Success Facility=Net SymbolicName=S_NET_PROCESSED_SEQUENCE
Language=English
Network sequence already processed
.


;
;/////////////////////////////////////////////////////////////////////////
;//
;// Net Error Codes
;//
;/////////////////////////////////////////////////////////////////////////
;

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_VERSION_NOT_AVAIL
Language=English
The version not available or miss match with system.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_PROTOCOL_VERSION_MISMATCH
Language=English
The version not available or miss match with system.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_NOTINITIALIZED
Language=English
Network is not initialized
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_INVALID_NETCLASS
Language=English
The version not available or miss match with system.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_NOT_ENOUGH_WINDOWSPACE
Language=English
Not enough space in sliding window
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_IO_ABORTED
Language=English
Network IO aborted
.


MessageId= Severity=Error Facility=Net SymbolicName=E_NET_IO_SEND_FAIL
Language=English
Network IO send failed
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_IO_RECV_FAIL
Language=English
Network IO recv failed
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_SEND_FAIL
Language=English
Network send failed
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_RECV_FAIL
Language=English
Network recv failed
.


MessageId= Severity=Error Facility=Net SymbolicName=E_NET_INVALID_SEQUENCE
Language=English
Network Message sequence is invalid
.


MessageId= Severity=Error Facility=Net SymbolicName=E_NET_BADPACKET_SIZE
Language=English
Network Message not expected bad packet size
.


MessageId= Severity=Error Facility=Net SymbolicName=E_NET_CONNECTION_CLOSED
Language=English
Network connection closed
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_NOT_CONNECTED
Language=English
Network connection is never made
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_INVALID_CID
Language=English
Network connection ID is invalid
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_CONNECTION_TIMEOUT
Language=English
Network Connection timeout
.


MessageId= Severity=Error Facility=Net SymbolicName=E_NET_CONNECTION_FAIL
Language=English
Network Connecte failed
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_BADPACKET_TOOBIG
Language=English
Network Connecte failed
.


MessageId= Severity=Error Facility=Net SymbolicName=E_NET_BADPACKET_NOTEXPECTED
Language=English
Network Message not expected bad packet
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_INVALID_MESSAGE_CHECKSUM
Language=English
Checksum value of the message is invalid
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_INVALID_MESSAGE_ENCRYPTION
Language=English
Message encryption is invalid
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_INVALID_CONNECTION_STATE
Language=English
Invalid connection state
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_UNSUPPORTED_PROTOCOL
Language=English
The protocol is not supported.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_FAILED_INIT
Language=English
CURL initialization is failed.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_URL_MALFORMAT
Language=English
Invalid URL format.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_NOT_BUILT_IN
Language=English
Not a build-in feature.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_COULDNT_RESOLVE_PROXY
Language=English
Failed to resolve URL.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_COULDNT_RESOLVE_HOST
Language=English
Failed to resolve URL.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_COULDNT_CONNECT
Language=English
Connection is failed
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_HTTP_POST_ERROR
Language=English
HTTP Post can't be done.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_SSL_ENGINE_NOTFOUND
Language=English
Not supported SSL type.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_SSL_ENGINE_SETFAILED
Language=English
Not supported SSL type.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_SEND_ERROR
Language=English
Network send failed
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_RECV_ERROR
Language=English
Network receive failed
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_USE_SSL_FAILED
Language=English
Invalid SSL setting.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_NO_CONNECTION_AVAILABLE
Language=English
No Connection is available now.
.


MessageId= Severity=Error Facility=Net SymbolicName=E_NET_SEQUENCE_OVERFLOW
Language=English
Too big sequence id
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_INTR
Language=English
A blocking operation was interrupted by a call to WSACancelBlockingCall.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_BADF
Language=English
The file handle supplied is not valid.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_ACCES
Language=English
An attempt was made to access a socket in a way forbidden by its access permissions
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_FAULT
Language=English
The system detected an invalid pointer address in attempting to use a pointer argument in a call.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_INVAL
Language=English
An invalid argument was supplied.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_MFILE
Language=English
Too many open sockets.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_WOULDBLOCK
Language=English
A non-blocking socket operation could not be completed immediately.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_INPROGRESS
Language=English
A blocking operation is currently executing.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_ALREADY
Language=English
An operation was attempted on a non-blocking socket that already had an operation in progress.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_NOTSOCK
Language=English
An operation was attempted on something that is not a socket.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_DESTADDRREQ
Language=English
A required address was omitted from an operation on a socket.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_NETDOWN
Language=English
A socket operation encountered a dead network.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_NETUNREACH
Language=English
A socket operation was attempted to an unreachable network.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_NETRESET
Language=English
The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_CONNABORTED
Language=English
An established connection was aborted by the software in your host machine.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_CONNRESET
Language=English
An existing connection was forcibly closed by the remote host.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_NOBUFS
Language=English
An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_ISCONN
Language=English
A connect request was made on an already connected socket.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_NOTCONN
Language=English
A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_SHUTDOWN
Language=English
A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_TOOMANYREFS
Language=English
Too many references to some kernel object.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_TIMEDOUT
Language=English
A connection attempt failed because the connected party did not properly respond after a period of time, or established connection failed because connected host has failed to respond.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_CONNECTION_REFUSSED
Language=English
No connection could be made because the target machine actively refused it.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_LOOP
Language=English
Cannot translate name.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_NAMETOOLONG
Language=English
Name component or name was too long.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_HOSTDOWN
Language=English
A socket operation failed because the destination host was down.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_HOSTUNREACH
Language=English
A socket operation was attempted to an unreachable host.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_NOTEMPTY
Language=English
Cannot remove a directory that is not empty.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_PROCLIM
Language=English
A Windows Sockets implementation may have a limit on the number of applications that may use it simultaneously.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_SYSNOTREADY
Language=English
WSAStartup cannot function at this time because the underlying system it uses to provide network services is currently unavailable.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_VERNOTSUPPORTED
Language=English
The Windows Sockets version requested is not supported.
.


MessageId= Severity=Error Facility=Net SymbolicName=E_NET_NOTINITIALISED
Language=English
Either the application has not called WSAStartup, or WSAStartup failed.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_DISCON
Language=English
Returned by WSARecv or WSARecvFrom to indicate the remote party has initiated a graceful shutdown sequence.
.


MessageId= Severity=Error Facility=Net SymbolicName=E_NET_SYSCALLFAILURE
Language=English
A system call has failed.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_HOST_NOT_FOUND
Language=English
A system call has failed.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_TRY_AGAIN
Language=English
Try again
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_IO_PENDING
Language=English
IOPending
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_SECURE_HOST_NOT_FOUND
Language=English
No such host is known securely.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_PROTO
Language=English
Protocol error
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_PROTOTYPE
Language=English
Protocol wrong type for socket
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_NOPROTOOPT
Language=English
Protocol not available
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_NONET
Language=English
Machine is not on the network
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_OPNOTSUPP
Language=English
Operation not supported on socket
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_BADFLAGS
Language=English
An invalid value was provided for the ai_flags member of the pHints parameter.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_FAMILY
Language=English
Bad network family
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_INVALID_SERVICE
Language=English
The pServiceName parameter is not supported for the specified ai_socktype member of the pHints parameter.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_INVALID_ADDRESS
Language=English
Invalid network address
.
