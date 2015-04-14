
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

