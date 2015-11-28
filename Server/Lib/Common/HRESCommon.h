
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server Error definitions. 
//
////////////////////////////////////////////////////////////////////////////////


#pragma once



#define STATUS_SEVERITY(hr)  (((hr) >> 30) & 0x3)

// Customer Bit is set by the message compiler command line arguments

/////////////////////////////////////////////////////////////////////////
//
// System level event IDs
//
/////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
//
// Generic Error Codes
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
// MessageId: E_NOT_INITIALIZED
//
// MessageText:
//
// Called without initialization.
//
#define E_NOT_INITIALIZED                0xC7020001L

//
// MessageId: E_VERSION_NOT_AVAIL
//
// MessageText:
//
// The version not available or miss match with system.
//
#define E_VERSION_NOT_AVAIL              0xC7020002L

//
// MessageId: E_INVALID_CLUSTERMASTER
//
// MessageText:
//
// Invalid Cluster Master server
//
#define E_INVALID_CLUSTERMASTER          0xC7020003L

//
// MessageId: E_INVALID_STATE
//
// MessageText:
//
// The version not available or miss match with system.
//
#define E_INVALID_STATE                  0xC7020004L

//
// MessageId: E_INVALID_STR_FORMAT
//
// MessageText:
//
// Invalid string format.
//
#define E_INVALID_STR_FORMAT             0xC7020005L

//
// MessageId: E_DUPLICATED_SERVERID
//
// MessageText:
//
// ServerID is duplicated.
//
#define E_DUPLICATED_SERVERID            0xC7020006L

//
// MessageId: E_INVALID_ENTITY
//
// MessageText:
//
// Invalid entity
//
#define E_INVALID_ENTITY                 0xC7020007L

//
// MessageId: E_DUPLICATED_ENTITY
//
// MessageText:
//
// Invalid duplicated entity
//
#define E_DUPLICATED_ENTITY              0xC7020008L

//
// MessageId: E_DUPLICATED_ENTITYID
//
// MessageText:
//
// Invalid duplicated entity
//
#define E_DUPLICATED_ENTITYID            0xC7020009L

//
// MessageId: E_DUPLICATED_ACCOUNTID
//
// MessageText:
//
// Duplicated Account ID
//
#define E_DUPLICATED_ACCOUNTID           0xC702000AL

//
// MessageId: E_DUPLICATED_PLAYERID
//
// MessageText:
//
// Duplicated Player ID
//
#define E_DUPLICATED_PLAYERID            0xC702000BL

//
// MessageId: E_DUPLICATED_NAME
//
// MessageText:
//
// Duplicated name
//
#define E_DUPLICATED_NAME                0xC702000CL

//
// MessageId: E_DUPLICATED_ID
//
// MessageText:
//
// Duplicated ID
//
#define E_DUPLICATED_ID                  0xC702000DL

//
// MessageId: E_INVALID_ACCOUNTID
//
// MessageText:
//
// Invalid duplicated entity
//
#define E_INVALID_ACCOUNTID              0xC702000EL

//
// MessageId: E_INVALID_TICKET
//
// MessageText:
//
// Invalid ticket
//
#define E_INVALID_TICKET                 0xC702000FL

//
// MessageId: E_INVALID_PLAYERID
//
// MessageText:
//
// Invalid player ID
//
#define E_INVALID_PLAYERID               0xC7020010L

//
// MessageId: E_INVALID_VALUE
//
// MessageText:
//
// Invalid value
//
#define E_INVALID_VALUE                  0xC7020011L

//
// MessageId: E_INVALID_CLASS
//
// MessageText:
//
// Invalid class value
//
#define E_INVALID_CLASS                  0xC7020012L

//
// MessageId: E_INVALID_DIFFICULTY
//
// MessageText:
//
// Invalid Difficulty
//
#define E_INVALID_DIFFICULTY             0xC7020013L

//
// MessageId: E_INVALID_REGISTRATION
//
// MessageText:
//
// Invalid registration to play
//
#define E_INVALID_REGISTRATION           0xC7020014L

//
// MessageId: E_INVALID_CHANNELID
//
// MessageText:
//
// Invalid Channel ID
//
#define E_INVALID_CHANNELID              0xC7020015L

//
// MessageId: E_INVALID_INSTANCEID
//
// MessageText:
//
// Invalid Zone instance ID
//
#define E_INVALID_INSTANCEID             0xC7020016L

//
// MessageId: E_DUPLICATED_COMPONENT
//
// MessageText:
//
// Duplicated component
//
#define E_DUPLICATED_COMPONENT           0xC7020017L

//
// MessageId: E_MAX_FRIEND
//
// MessageText:
//
// Maximum friend
//
#define E_MAX_FRIEND                     0xC7020018L

//
// MessageId: E_INVALID_PLAYER_COUNT
//
// MessageText:
//
// Invalid player count for service
//
#define E_INVALID_PLAYER_COUNT           0xC7020019L

//
// MessageId: E_INVALID_PLAYER_LEVEL
//
// MessageText:
//
// Invalid player level
//
#define E_INVALID_PLAYER_LEVEL           0xC702001AL

//
// MessageId: E_INVALID_PLAYER_NICK
//
// MessageText:
//
// Invalid player nick
//
#define E_INVALID_PLAYER_NICK            0xC702001BL

//
// MessageId: E_INVALID_NOTIFICATIONID
//
// MessageText:
//
// Invalid Notification ID
//
#define E_INVALID_NOTIFICATIONID         0xC702001CL

//
// MessageId: E_INVALID_RANKING_RANGE
//
// MessageText:
//
// Invalid Ranking range
//
#define E_INVALID_RANKING_RANGE          0xC702001DL

//
// MessageId: E_TARGET_MAX_FRIEND
//
// MessageText:
//
// Target Maximum friend
//
#define E_TARGET_MAX_FRIEND              0xC702001EL

//
// MessageId: E_INVALID_STR_DATA
//
// MessageText:
//
// Invalid string data.
//
#define E_INVALID_STR_DATA               0xC702001FL

//
// MessageId: E_NOT_AUTORIZED
//
// MessageText:
//
// Not authorized
//
#define E_NOT_AUTORIZED                  0xC7020020L

//
// MessageId: E_NO_PERMITION
//
// MessageText:
//
// No permition
//
#define E_NO_PERMITION                   0xC7020021L

//
// MessageId: E_NO_FILE_OR_DIR
//
// MessageText:
//
// No file or directory
//
#define E_NO_FILE_OR_DIR                 0xC7020022L

//
// MessageId: E_NO_SUCH_PROCESS
//
// MessageText:
//
// No such process
//
#define E_NO_SUCH_PROCESS                0xC7020023L

//
// MessageId: E_INTERRUPTED_SYSCALL
//
// MessageText:
//
// Interrupted system call
//
#define E_INTERRUPTED_SYSCALL            0xC7020024L

//
// MessageId: E_IO_ERROR
//
// MessageText:
//
// I/O error
//
#define E_IO_ERROR                       0xC7020025L

//
// MessageId: E_NOT_DEVICE
//
// MessageText:
//
// No such device or address
//
#define E_NOT_DEVICE                     0xC7020026L

//
// MessageId: E_TOO_MANY_ARGLIST
//
// MessageText:
//
// Arg list too long
//
#define E_TOO_MANY_ARGLIST               0xC7020027L

//
// MessageId: E_INVALID_EXEC_FROMAT
//
// MessageText:
//
// Exec format error
//
#define E_INVALID_EXEC_FROMAT            0xC7020028L

//
// MessageId: E_INVALID_FILE_HANDLE
//
// MessageText:
//
// Bad file number
//
#define E_INVALID_FILE_HANDLE            0xC7020029L

//
// MessageId: E_NO_CHILD_PROCESS
//
// MessageText:
//
// No child process
//
#define E_NO_CHILD_PROCESS               0xC702002AL

//
// MessageId: E_TRY_AGAIN
//
// MessageText:
//
// Try again
//
#define E_TRY_AGAIN                      0xC702002BL

//
// MessageId: E_INVALID_POINTER
//
// MessageText:
//
// Bad address
//
#define E_INVALID_POINTER                0xC702002CL

//
// MessageId: E_NOT_BLOCK
//
// MessageText:
//
// Block device required
//
#define E_NOT_BLOCK                      0xC702002DL

//
// MessageId: E_BUSY
//
// MessageText:
//
// Busy
//
#define E_BUSY                           0xC702002EL

//
// MessageId: E_FILE_EXISTS
//
// MessageText:
//
// File exists
//
#define E_FILE_EXISTS                    0xC702002FL

//
// MessageId: E_CROSS_LINK
//
// MessageText:
//
// Cross-device link
//
#define E_CROSS_LINK                     0xC7020030L

//
// MessageId: E_INVALID_DEVICE
//
// MessageText:
//
// No such device
//
#define E_INVALID_DEVICE                 0xC7020031L

//
// MessageId: E_INVALID_DIRECTORY
//
// MessageText:
//
// Not a directory
//
#define E_INVALID_DIRECTORY              0xC7020032L

//
// MessageId: E_DIRECTORY
//
// MessageText:
//
// Is a directory
//
#define E_DIRECTORY                      0xC7020033L

//
// MessageId: E_INVALID_ARG
//
// MessageText:
//
// Invalid argument
//
#define E_INVALID_ARG                    0xC7020034L

//
// MessageId: E_NOT_ENOUGH_FILE_TABLE
//
// MessageText:
//
// File table overflow
//
#define E_NOT_ENOUGH_FILE_TABLE          0xC7020035L

//
// MessageId: E_TOO_MANY_OPENED_FILE
//
// MessageText:
//
// Too many open files
//
#define E_TOO_MANY_OPENED_FILE           0xC7020036L

//
// MessageId: E_INVALID_TYPEWRITER
//
// MessageText:
//
// Not a typewriter
//
#define E_INVALID_TYPEWRITER             0xC7020037L

//
// MessageId: E_BUSY_FILE
//
// MessageText:
//
// Text file busy
//
#define E_BUSY_FILE                      0xC7020038L

//
// MessageId: E_TOO_BIG_FILE
//
// MessageText:
//
// File too large
//
#define E_TOO_BIG_FILE                   0xC7020039L

//
// MessageId: E_NOT_ENOUGH_SPACE
//
// MessageText:
//
// No space left on device
//
#define E_NOT_ENOUGH_SPACE               0xC702003AL

//
// MessageId: E_INVALID_SEEK
//
// MessageText:
//
// Illegal seek
//
#define E_INVALID_SEEK                   0xC702003BL

//
// MessageId: E_READ_ONLY
//
// MessageText:
//
// Read-only file system
//
#define E_READ_ONLY                      0xC702003CL

//
// MessageId: E_TOO_MANY_LINK
//
// MessageText:
//
// Too many links
//
#define E_TOO_MANY_LINK                  0xC702003DL

//
// MessageId: E_INVALID_PIPE
//
// MessageText:
//
// Broken pipe
//
#define E_INVALID_PIPE                   0xC702003EL

//
// MessageId: E_INVALID_MATH_DOMAIN
//
// MessageText:
//
// Math argument out of domain of func
//
#define E_INVALID_MATH_DOMAIN            0xC702003FL

//
// MessageId: E_INVALID_NUMERIC
//
// MessageText:
//
// Math result not representable
//
#define E_INVALID_NUMERIC                0xC7020040L

