
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

