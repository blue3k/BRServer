
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : madk
//
// Description : Server Error definitions. 
//
////////////////////////////////////////////////////////////////////////////////


#pragma once



#define STATUS_SEVERITY(hr)  (((hr) >> 30) & 0x3)

// Customer Bit is set by the message compiler command line arguments

/////////////////////////////////////////////////////////////////////////
//
// Net Success Codes
//
/////////////////////////////////////////////////////////////////////////

//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_ZONE                    0x707
#define FACILITY_SVRSYS                  0x704
#define FACILITY_NET                     0x703
#define FACILITY_LOGIN                   0x705
#define FACILITY_GENERIC                 0x702
#define FACILITY_GAME                    0x706
#define FACILITY_COMMUNITY               0x708
#define FACILITY_BR                      0x701


//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: S_NET_PROCESSED_SEQUENCE
//
// MessageText:
//
// Network sequence already processed
//
#define S_NET_PROCESSED_SEQUENCE         0x07030001L


/////////////////////////////////////////////////////////////////////////
//
// Net Error Codes
//
/////////////////////////////////////////////////////////////////////////

//
// MessageId: E_NET_VERSION_NOT_AVAIL
//
// MessageText:
//
// The version not available or miss match with system.
//
#define E_NET_VERSION_NOT_AVAIL          0xC7030002L

//
// MessageId: E_NET_PROTOCOL_VERSION_MISMATCH
//
// MessageText:
//
// The version not available or miss match with system.
//
#define E_NET_PROTOCOL_VERSION_MISMATCH  0xC7030003L

//
// MessageId: E_NET_NOTINITIALIZED
//
// MessageText:
//
// Network is not initialized
//
#define E_NET_NOTINITIALIZED             0xC7030004L

//
// MessageId: E_NET_INVALID_NETCLASS
//
// MessageText:
//
// The version not available or miss match with system.
//
#define E_NET_INVALID_NETCLASS           0xC7030005L

//
// MessageId: E_NET_NOT_ENOUGH_WINDOWSPACE
//
// MessageText:
//
// Not enough space in sliding window
//
#define E_NET_NOT_ENOUGH_WINDOWSPACE     0xC7030006L

//
// MessageId: E_NET_IO_ABORTED
//
// MessageText:
//
// Network IO aborted
//
#define E_NET_IO_ABORTED                 0xC7030007L

//
// MessageId: E_NET_IO_SEND_FAIL
//
// MessageText:
//
// Network IO send failed
//
#define E_NET_IO_SEND_FAIL               0xC7030008L

//
// MessageId: E_NET_IO_RECV_FAIL
//
// MessageText:
//
// Network IO recv failed
//
#define E_NET_IO_RECV_FAIL               0xC7030009L

//
// MessageId: E_NET_SEND_FAIL
//
// MessageText:
//
// Network send failed
//
#define E_NET_SEND_FAIL                  0xC703000AL

//
// MessageId: E_NET_RECV_FAIL
//
// MessageText:
//
// Network recv failed
//
#define E_NET_RECV_FAIL                  0xC703000BL

//
// MessageId: E_NET_INVALID_SEQUENCE
//
// MessageText:
//
// Network Message sequence is invalid
//
#define E_NET_INVALID_SEQUENCE           0xC703000CL

//
// MessageId: E_NET_BADPACKET_SIZE
//
// MessageText:
//
// Network Message not expected bad packet size
//
#define E_NET_BADPACKET_SIZE             0xC703000DL

//
// MessageId: E_NET_CONNECTION_CLOSED
//
// MessageText:
//
// Network connection closed
//
#define E_NET_CONNECTION_CLOSED          0xC703000EL

//
// MessageId: E_NET_NOT_CONNECTED
//
// MessageText:
//
// Network connection is never made
//
#define E_NET_NOT_CONNECTED              0xC703000FL

//
// MessageId: E_NET_INVALID_CID
//
// MessageText:
//
// Network connection ID is invalid
//
#define E_NET_INVALID_CID                0xC7030010L

//
// MessageId: E_NET_CONNECTION_TIMEOUT
//
// MessageText:
//
// Network Connection timeout
//
#define E_NET_CONNECTION_TIMEOUT         0xC7030011L

//
// MessageId: E_NET_CONNECTION_FAIL
//
// MessageText:
//
// Network Connecte failed
//
#define E_NET_CONNECTION_FAIL            0xC7030012L

//
// MessageId: E_NET_BADPACKET_TOOBIG
//
// MessageText:
//
// Network Connecte failed
//
#define E_NET_BADPACKET_TOOBIG           0xC7030013L

//
// MessageId: E_NET_BADPACKET_NOTEXPECTED
//
// MessageText:
//
// Network Message not expected bad packet
//
#define E_NET_BADPACKET_NOTEXPECTED      0xC7030014L

//
// MessageId: E_NET_INVALID_MESSAGE_CHECKSUM
//
// MessageText:
//
// Checksum value of the message is invalid
//
#define E_NET_INVALID_MESSAGE_CHECKSUM   0xC7030015L

//
// MessageId: E_NET_INVALID_MESSAGE_ENCRYPTION
//
// MessageText:
//
// Message encryption is invalid
//
#define E_NET_INVALID_MESSAGE_ENCRYPTION 0xC7030016L

//
// MessageId: E_NET_INVALID_CONNECTION_STATE
//
// MessageText:
//
// Invalid connection state
//
#define E_NET_INVALID_CONNECTION_STATE   0xC7030017L

//
// MessageId: E_CURLE_UNSUPPORTED_PROTOCOL
//
// MessageText:
//
// The protocol is not supported.
//
#define E_CURLE_UNSUPPORTED_PROTOCOL     0xC7030018L

//
// MessageId: E_CURLE_FAILED_INIT
//
// MessageText:
//
// CURL initialization is failed.
//
#define E_CURLE_FAILED_INIT              0xC7030019L

//
// MessageId: E_CURLE_URL_MALFORMAT
//
// MessageText:
//
// Invalid URL format.
//
#define E_CURLE_URL_MALFORMAT            0xC703001AL

//
// MessageId: E_CURLE_NOT_BUILT_IN
//
// MessageText:
//
// Not a build-in feature.
//
#define E_CURLE_NOT_BUILT_IN             0xC703001BL

//
// MessageId: E_CURLE_COULDNT_RESOLVE_PROXY
//
// MessageText:
//
// Failed to resolve URL.
//
#define E_CURLE_COULDNT_RESOLVE_PROXY    0xC703001CL

//
// MessageId: E_CURLE_COULDNT_RESOLVE_HOST
//
// MessageText:
//
// Failed to resolve URL.
//
#define E_CURLE_COULDNT_RESOLVE_HOST     0xC703001DL

//
// MessageId: E_CURLE_COULDNT_CONNECT
//
// MessageText:
//
// Connection is failed
//
#define E_CURLE_COULDNT_CONNECT          0xC703001EL

//
// MessageId: E_CURLE_HTTP_POST_ERROR
//
// MessageText:
//
// HTTP Post can't be done.
//
#define E_CURLE_HTTP_POST_ERROR          0xC703001FL

//
// MessageId: E_CURLE_SSL_ENGINE_NOTFOUND
//
// MessageText:
//
// Not supported SSL type.
//
#define E_CURLE_SSL_ENGINE_NOTFOUND      0xC7030020L

//
// MessageId: E_CURLE_SSL_ENGINE_SETFAILED
//
// MessageText:
//
// Not supported SSL type.
//
#define E_CURLE_SSL_ENGINE_SETFAILED     0xC7030021L

//
// MessageId: E_CURLE_SEND_ERROR
//
// MessageText:
//
// Network send failed
//
#define E_CURLE_SEND_ERROR               0xC7030022L

//
// MessageId: E_CURLE_RECV_ERROR
//
// MessageText:
//
// Network receive failed
//
#define E_CURLE_RECV_ERROR               0xC7030023L

//
// MessageId: E_CURLE_USE_SSL_FAILED
//
// MessageText:
//
// Invalid SSL setting.
//
#define E_CURLE_USE_SSL_FAILED           0xC7030024L

//
// MessageId: E_CURLE_NO_CONNECTION_AVAILABLE
//
// MessageText:
//
// No Connection is available now.
//
#define E_CURLE_NO_CONNECTION_AVAILABLE  0xC7030025L

//
// MessageId: E_NET_SEQUENCE_OVERFLOW
//
// MessageText:
//
// Too big sequence id
//
#define E_NET_SEQUENCE_OVERFLOW          0xC7030026L

//
// MessageId: E_NET_INTR
//
// MessageText:
//
// A blocking operation was interrupted by a call to WSACancelBlockingCall.
//
#define E_NET_INTR                       0xC7030027L

//
// MessageId: E_NET_BADF
//
// MessageText:
//
// The file handle supplied is not valid.
//
#define E_NET_BADF                       0xC7030028L

//
// MessageId: E_NET_ACCES
//
// MessageText:
//
// An attempt was made to access a socket in a way forbidden by its access permissions
//
#define E_NET_ACCES                      0xC7030029L

//
// MessageId: E_NET_FAULT
//
// MessageText:
//
// The system detected an invalid pointer address in attempting to use a pointer argument in a call.
//
#define E_NET_FAULT                      0xC703002AL

//
// MessageId: E_NET_INVAL
//
// MessageText:
//
// An invalid argument was supplied.
//
#define E_NET_INVAL                      0xC703002BL

//
// MessageId: E_NET_MFILE
//
// MessageText:
//
// Too many open sockets.
//
#define E_NET_MFILE                      0xC703002CL

//
// MessageId: E_NET_WOULDBLOCK
//
// MessageText:
//
// A non-blocking socket operation could not be completed immediately.
//
#define E_NET_WOULDBLOCK                 0xC703002DL

//
// MessageId: E_NET_INPROGRESS
//
// MessageText:
//
// A blocking operation is currently executing.
//
#define E_NET_INPROGRESS                 0xC703002EL

//
// MessageId: E_NET_ALREADY
//
// MessageText:
//
// An operation was attempted on a non-blocking socket that already had an operation in progress.
//
#define E_NET_ALREADY                    0xC703002FL

//
// MessageId: E_NET_NOTSOCK
//
// MessageText:
//
// An operation was attempted on something that is not a socket.
//
#define E_NET_NOTSOCK                    0xC7030030L

//
// MessageId: E_NET_DESTADDRREQ
//
// MessageText:
//
// A required address was omitted from an operation on a socket.
//
#define E_NET_DESTADDRREQ                0xC7030031L

//
// MessageId: E_NET_NETDOWN
//
// MessageText:
//
// A socket operation encountered a dead network.
//
#define E_NET_NETDOWN                    0xC7030032L

//
// MessageId: E_NET_NETUNREACH
//
// MessageText:
//
// A socket operation was attempted to an unreachable network.
//
#define E_NET_NETUNREACH                 0xC7030033L

//
// MessageId: E_NET_NETRESET
//
// MessageText:
//
// The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress.
//
#define E_NET_NETRESET                   0xC7030034L

//
// MessageId: E_NET_CONNABORTED
//
// MessageText:
//
// An established connection was aborted by the software in your host machine.
//
#define E_NET_CONNABORTED                0xC7030035L

//
// MessageId: E_NET_CONNRESET
//
// MessageText:
//
// An existing connection was forcibly closed by the remote host.
//
#define E_NET_CONNRESET                  0xC7030036L

//
// MessageId: E_NET_NOBUFS
//
// MessageText:
//
// An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.
//
#define E_NET_NOBUFS                     0xC7030037L

//
// MessageId: E_NET_ISCONN
//
// MessageText:
//
// A connect request was made on an already connected socket.
//
#define E_NET_ISCONN                     0xC7030038L

//
// MessageId: E_NET_NOTCONN
//
// MessageText:
//
// A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied.
//
#define E_NET_NOTCONN                    0xC7030039L

//
// MessageId: E_NET_SHUTDOWN
//
// MessageText:
//
// A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call.
//
#define E_NET_SHUTDOWN                   0xC703003AL

//
// MessageId: E_NET_TOOMANYREFS
//
// MessageText:
//
// Too many references to some kernel object.
//
#define E_NET_TOOMANYREFS                0xC703003BL

//
// MessageId: E_NET_TIMEDOUT
//
// MessageText:
//
// A connection attempt failed because the connected party did not properly respond after a period of time, or established connection failed because connected host has failed to respond.
//
#define E_NET_TIMEDOUT                   0xC703003CL

//
// MessageId: E_NET_CONNECTION_REFUSSED
//
// MessageText:
//
// No connection could be made because the target machine actively refused it.
//
#define E_NET_CONNECTION_REFUSSED        0xC703003DL

//
// MessageId: E_NET_LOOP
//
// MessageText:
//
// Cannot translate name.
//
#define E_NET_LOOP                       0xC703003EL

//
// MessageId: E_NET_NAMETOOLONG
//
// MessageText:
//
// Name component or name was too long.
//
#define E_NET_NAMETOOLONG                0xC703003FL

//
// MessageId: E_NET_HOSTDOWN
//
// MessageText:
//
// A socket operation failed because the destination host was down.
//
#define E_NET_HOSTDOWN                   0xC7030040L

//
// MessageId: E_NET_HOSTUNREACH
//
// MessageText:
//
// A socket operation was attempted to an unreachable host.
//
#define E_NET_HOSTUNREACH                0xC7030041L

//
// MessageId: E_NET_NOTEMPTY
//
// MessageText:
//
// Cannot remove a directory that is not empty.
//
#define E_NET_NOTEMPTY                   0xC7030042L

//
// MessageId: E_NET_PROCLIM
//
// MessageText:
//
// A Windows Sockets implementation may have a limit on the number of applications that may use it simultaneously.
//
#define E_NET_PROCLIM                    0xC7030043L

//
// MessageId: E_NET_SYSNOTREADY
//
// MessageText:
//
// WSAStartup cannot function at this time because the underlying system it uses to provide network services is currently unavailable.
//
#define E_NET_SYSNOTREADY                0xC7030044L

//
// MessageId: E_NET_VERNOTSUPPORTED
//
// MessageText:
//
// The Windows Sockets version requested is not supported.
//
#define E_NET_VERNOTSUPPORTED            0xC7030045L

//
// MessageId: E_NET_NOTINITIALISED
//
// MessageText:
//
// Either the application has not called WSAStartup, or WSAStartup failed.
//
#define E_NET_NOTINITIALISED             0xC7030046L

//
// MessageId: E_NET_DISCON
//
// MessageText:
//
// Returned by WSARecv or WSARecvFrom to indicate the remote party has initiated a graceful shutdown sequence.
//
#define E_NET_DISCON                     0xC7030047L

//
// MessageId: E_NET_SYSCALLFAILURE
//
// MessageText:
//
// A system call has failed.
//
#define E_NET_SYSCALLFAILURE             0xC7030048L

//
// MessageId: E_NET_HOST_NOT_FOUND
//
// MessageText:
//
// A system call has failed.
//
#define E_NET_HOST_NOT_FOUND             0xC7030049L

//
// MessageId: E_NET_TRY_AGAIN
//
// MessageText:
//
// Try again
//
#define E_NET_TRY_AGAIN                  0xC703004AL

//
// MessageId: E_NET_IO_PENDING
//
// MessageText:
//
// IOPending
//
#define E_NET_IO_PENDING                 0xC703004BL

//
// MessageId: E_NET_SECURE_HOST_NOT_FOUND
//
// MessageText:
//
// No such host is known securely.
//
#define E_NET_SECURE_HOST_NOT_FOUND      0xC703004CL

//
// MessageId: E_NET_PROTO
//
// MessageText:
//
// Protocol error
//
#define E_NET_PROTO                      0xC703004DL

//
// MessageId: E_NET_PROTOTYPE
//
// MessageText:
//
// Protocol wrong type for socket
//
#define E_NET_PROTOTYPE                  0xC703004EL

//
// MessageId: E_NET_NOPROTOOPT
//
// MessageText:
//
// Protocol not available
//
#define E_NET_NOPROTOOPT                 0xC703004FL

//
// MessageId: E_NET_NONET
//
// MessageText:
//
// Machine is not on the network
//
#define E_NET_NONET                      0xC7030050L

//
// MessageId: E_NET_OPNOTSUPP
//
// MessageText:
//
// Operation not supported on socket
//
#define E_NET_OPNOTSUPP                  0xC7030051L

//
// MessageId: E_NET_BADFLAGS
//
// MessageText:
//
// An invalid value was provided for the ai_flags member of the pHints parameter.
//
#define E_NET_BADFLAGS                   0xC7030052L

//
// MessageId: E_NET_FAMILY
//
// MessageText:
//
// Bad network family
//
#define E_NET_FAMILY                     0xC7030053L

//
// MessageId: E_NET_INVALID_SERVICE
//
// MessageText:
//
// The pServiceName parameter is not supported for the specified ai_socktype member of the pHints parameter.
//
#define E_NET_INVALID_SERVICE            0xC7030054L

