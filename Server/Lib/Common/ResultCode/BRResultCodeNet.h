﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : KyungKun Ko
//
// Description : Autogenerated header, Net
//
////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>



// Network sequence already processed 
#define S_NET_PROCESSED_SEQUENCE                                      ((int32_t)0x27030000L) 

// The version not available or miss match with system. 
#define E_NET_PROTOCOL_VERSION_MISMATCH                               ((int32_t)0xE7030001L) 

// Network is not initialized 
#define E_NET_NOTINITIALIZED                                          ((int32_t)0xE7030002L) 

// The version not available or miss match with system. 
#define E_NET_INVALID_NETCLASS                                        ((int32_t)0xE7030003L) 

// Not enough space in sliding window 
#define E_NET_NOT_ENOUGH_WINDOWSPACE                                  ((int32_t)0xE7030004L) 

// Network IO aborted 
#define E_NET_IO_ABORTED                                              ((int32_t)0xE7030005L) 

// Network IO send failed 
#define E_NET_IO_SEND_FAIL                                            ((int32_t)0xE7030006L) 

// Network IO recv failed 
#define E_NET_IO_RECV_FAIL                                            ((int32_t)0xE7030007L) 

// Network send failed 
#define E_NET_SEND_FAIL                                               ((int32_t)0xE7030008L) 

// Network recv failed 
#define E_NET_RECV_FAIL                                               ((int32_t)0xE7030009L) 

// Network Message sequence is invalid 
#define E_NET_INVALID_SEQUENCE                                        ((int32_t)0xE703000AL) 

// Network Message not expected bad packet size 
#define E_NET_BADPACKET_SIZE                                          ((int32_t)0xE703000BL) 

// Network connection closed 
#define E_NET_CONNECTION_CLOSED                                       ((int32_t)0xE703000CL) 

// Network connection is never made 
#define E_NET_NOT_CONNECTED                                           ((int32_t)0xE703000DL) 

// Network connection ID is invalid 
#define E_NET_INVALID_CID                                             ((int32_t)0xE703000EL) 

// Network Connection timeout 
#define E_NET_CONNECTION_TIMEOUT                                      ((int32_t)0xE703000FL) 

// Network connection is failed 
#define E_NET_CONNECTION_FAIL                                         ((int32_t)0xE7030010L) 

// Too big packet data 
#define E_NET_BADPACKET_TOOBIG                                        ((int32_t)0xE7030011L) 

// Network Message not expected bad packet 
#define E_NET_BADPACKET_NOTEXPECTED                                   ((int32_t)0xE7030012L) 

// Checksum value of the message is invalid 
#define E_NET_INVALID_MESSAGE_CHECKSUM                                ((int32_t)0xE7030013L) 

// Message encryption is invalid 
#define E_NET_INVALID_MESSAGE_ENCRYPTION                              ((int32_t)0xE7030014L) 

// Invalid connection state 
#define E_NET_INVALID_CONNECTION_STATE                                ((int32_t)0xE7030015L) 

// Too big sequence id 
#define E_NET_SEQUENCE_OVERFLOW                                       ((int32_t)0xE7030016L) 

// A blocking operation was interrupted by a call to WSACancelBlockingCall. 
#define E_NET_INTR                                                    ((int32_t)0xE7030017L) 

// The file handle supplied is not valid. 
#define E_NET_BADF                                                    ((int32_t)0xE7030018L) 

// An attempt was made to access a socket in a way forbidden by its access permissions 
#define E_NET_ACCES                                                   ((int32_t)0xE7030019L) 

// The system detected an invalid pointer address in attempting to use a pointer argument in a call. 
#define E_NET_FAULT                                                   ((int32_t)0xE703001AL) 

// An invalid argument was supplied. 
#define E_NET_INVAL                                                   ((int32_t)0xE703001BL) 

// Too many open sockets. 
#define E_NET_MFILE                                                   ((int32_t)0xE703001CL) 

// A non-blocking socket operation could not be completed immediately. 
#define E_NET_WOULDBLOCK                                              ((int32_t)0xE703001DL) 

// A blocking operation is currently executing. 
#define E_NET_INPROGRESS                                              ((int32_t)0xE703001EL) 

// An operation was attempted on a non-blocking socket that already had an operation in progress. 
#define E_NET_ALREADY                                                 ((int32_t)0xE703001FL) 

// An operation was attempted on something that is not a socket. 
#define E_NET_NOTSOCK                                                 ((int32_t)0xE7030020L) 

// A required address was omitted from an operation on a socket. 
#define E_NET_DESTADDRREQ                                             ((int32_t)0xE7030021L) 

// A socket operation encountered a dead network. 
#define E_NET_NETDOWN                                                 ((int32_t)0xE7030022L) 

// A socket operation was attempted to an unreachable network. 
#define E_NET_NETUNREACH                                              ((int32_t)0xE7030023L) 

// The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress. 
#define E_NET_NETRESET                                                ((int32_t)0xE7030024L) 

// An established connection was aborted by the software in your host machine. 
#define E_NET_CONNABORTED                                             ((int32_t)0xE7030025L) 

// An existing connection was forcibly closed by the remote host. 
#define E_NET_CONNRESET                                               ((int32_t)0xE7030026L) 

// An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full. 
#define E_NET_NOBUFS                                                  ((int32_t)0xE7030027L) 

// A connect request was made on an already connected socket. 
#define E_NET_ISCONN                                                  ((int32_t)0xE7030028L) 

// A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied. 
#define E_NET_NOTCONN                                                 ((int32_t)0xE7030029L) 

// A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call. 
#define E_NET_SHUTDOWN                                                ((int32_t)0xE703002AL) 

// Too many references to some kernel object. 
#define E_NET_TOOMANYREFS                                             ((int32_t)0xE703002BL) 

// A connection attempt failed because the connected party did not properly respond after a period of time, or established connection failed because connected host has failed to respond. 
#define E_NET_TIMEDOUT                                                ((int32_t)0xE703002CL) 

// No connection could be made because the target machine actively refused it. 
#define E_NET_CONNECTION_REFUSSED                                     ((int32_t)0xE703002DL) 

// Cannot translate name. 
#define E_NET_LOOP                                                    ((int32_t)0xE703002EL) 

// Name component or name was too long. 
#define E_NET_NAMETOOLONG                                             ((int32_t)0xE703002FL) 

// A socket operation failed because the destination host was down. 
#define E_NET_HOSTDOWN                                                ((int32_t)0xE7030030L) 

// A socket operation was attempted to an unreachable host. 
#define E_NET_HOSTUNREACH                                             ((int32_t)0xE7030031L) 

// Cannot remove a directory that is not empty. 
#define E_NET_NOTEMPTY                                                ((int32_t)0xE7030032L) 

// A Windows Sockets implementation may have a limit on the number of applications that may use it simultaneously. 
#define E_NET_PROCLIM                                                 ((int32_t)0xE7030033L) 

// WSAStartup cannot function at this time because the underlying system it uses to provide network services is currently unavailable. 
#define E_NET_SYSNOTREADY                                             ((int32_t)0xE7030034L) 

// The Windows Sockets version requested is not supported. 
#define E_NET_VERNOTSUPPORTED                                         ((int32_t)0xE7030035L) 

// Either the application has not called WSAStartup, or WSAStartup failed. 
#define E_NET_NOTINITIALISED                                          ((int32_t)0xE7030036L) 

// Returned by WSARecv or WSARecvFrom to indicate the remote party has initiated a graceful shutdown sequence. 
#define E_NET_DISCON                                                  ((int32_t)0xE7030037L) 

// A system call has failed. 
#define E_NET_SYSCALLFAILURE                                          ((int32_t)0xE7030038L) 

// Host not found. 
#define E_NET_HOST_NOT_FOUND                                          ((int32_t)0xE7030039L) 

// Try again 
#define E_NET_TRY_AGAIN                                               ((int32_t)0xE703003AL) 

// IOPending 
#define E_NET_IO_PENDING                                              ((int32_t)0xE703003BL) 

// No such host is known securely. 
#define E_NET_SECURE_HOST_NOT_FOUND                                   ((int32_t)0xE703003CL) 

// Protocol error 
#define E_NET_PROTO                                                   ((int32_t)0xE703003DL) 

// Protocol wrong type for socket. 
#define E_NET_PROTOTYPE                                               ((int32_t)0xE703003EL) 

// Protocol not available 
#define E_NET_NOPROTOOPT                                              ((int32_t)0xE703003FL) 

// Machine is not on the network 
#define E_NET_NONET                                                   ((int32_t)0xE7030040L) 

// Operation not supported on socket 
#define E_NET_OPNOTSUPP                                               ((int32_t)0xE7030041L) 

// An invalid value was provided for the ai_flags member of the pHints parameter. 
#define E_NET_BADFLAGS                                                ((int32_t)0xE7030042L) 

// Bad network family 
#define E_NET_FAMILY                                                  ((int32_t)0xE7030043L) 

// The pServiceName parameter is not supported for the specified ai_socktype member of the pHints parameter. 
#define E_NET_INVALID_SERVICE                                         ((int32_t)0xE7030044L) 

// Invalid network address 
#define E_NET_INVALID_ADDRESS                                         ((int32_t)0xE7030045L) 


