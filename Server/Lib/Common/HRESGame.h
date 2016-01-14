
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
// Game Error Codes
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
// MessageId: E_GAME_ALREADY_IN_GAME
//
// MessageText:
//
// User is already in a game
//
#define E_GAME_ALREADY_IN_GAME           0xC7060001L

//
// MessageId: E_GAME_INVALID_PLAYER
//
// MessageText:
//
// Invalid player
//
#define E_GAME_INVALID_PLAYER            0xC7060002L

//
// MessageId: E_GAME_NOTREADY
//
// MessageText:
//
// Game is not ready
//
#define E_GAME_NOTREADY                  0xC7060003L

//
// MessageId: E_GAME_INVALID_ROLE
//
// MessageText:
//
// Invalid player role
//
#define E_GAME_INVALID_ROLE              0xC7060004L

//
// MessageId: E_GAME_NOT_READY_FOR_NEXT_STATE
//
// MessageText:
//
// Not ready for next game play state
//
#define E_GAME_NOT_READY_FOR_NEXT_STATE  0xC7060005L

//
// MessageId: E_GAME_INVALID_VOTETYPE
//
// MessageText:
//
// Invalid vote type
//
#define E_GAME_INVALID_VOTETYPE          0xC7060006L

//
// MessageId: E_GAME_INVALID_VOTE_STATE
//
// MessageText:
//
// Invalid vote state
//
#define E_GAME_INVALID_VOTE_STATE        0xC7060007L

//
// MessageId: E_GAME_INVALID_VOTE_TARGET
//
// MessageText:
//
// Invalid vote target
//
#define E_GAME_INVALID_VOTE_TARGET       0xC7060008L

//
// MessageId: E_GAME_NO_SUFFRAGE
//
// MessageText:
//
// No suffrage
//
#define E_GAME_NO_SUFFRAGE               0xC7060009L

//
// MessageId: E_GAME_INVALID_GAMESTATE
//
// MessageText:
//
// Invalid game state
//
#define E_GAME_INVALID_GAMESTATE         0xC706000AL

//
// MessageId: E_GAME_INVALID_PARTYID
//
// MessageText:
//
// Invalid partyID
//
#define E_GAME_INVALID_PARTYID           0xC706000BL

//
// MessageId: E_GAME_INVALID_PLAYER_COUNT
//
// MessageText:
//
// Invalid player count for game
//
#define E_GAME_INVALID_PLAYER_COUNT      0xC706000CL

//
// MessageId: E_GAME_ALREADY_IN_PARTY
//
// MessageText:
//
// User is already in a party
//
#define E_GAME_ALREADY_IN_PARTY          0xC706000DL

//
// MessageId: E_GAME_NO_PRIVILEGE
//
// MessageText:
//
// User doesn't have eligible privilege
//
#define E_GAME_NO_PRIVILEGE              0xC706000EL

//
// MessageId: E_GAME_INVALID_SHOPITEM
//
// MessageText:
//
// Invalid shop item data
//
#define E_GAME_INVALID_SHOPITEM          0xC706000FL

//
// MessageId: E_GAME_INVALID_SHOPITEMID
//
// MessageText:
//
// Invalid shop item id
//
#define E_GAME_INVALID_SHOPITEMID        0xC7060010L

//
// MessageId: E_GAME_NOTENOUGH_RESOURCE
//
// MessageText:
//
// Not enough money(gold, gem, etc)
//
#define E_GAME_NOTENOUGH_RESOURCE        0xC7060011L

//
// MessageId: E_GAME_LOW_STAMINA
//
// MessageText:
//
// Doesn't have enough stamina
//
#define E_GAME_LOW_STAMINA               0xC7060012L

//
// MessageId: E_GAME_INVALID_PLAYER_STATE
//
// MessageText:
//
// Invalid player game state
//
#define E_GAME_INVALID_PLAYER_STATE      0xC7060013L

//
// MessageId: E_GAME_MAX_NOTIFICATION
//
// MessageText:
//
// Maximum notification count
//
#define E_GAME_MAX_NOTIFICATION          0xC7060014L

//
// MessageId: E_GAME_MAX_TRY
//
// MessageText:
//
// Maximum try reached
//
#define E_GAME_MAX_TRY                   0xC7060015L

//
// MessageId: E_GAME_INVALID_COSTID
//
// MessageText:
//
// Invalid cost table id
//
#define E_GAME_INVALID_COSTID            0xC7060016L

//
// MessageId: E_GAME_INVALID_BOTTALK_TABLE
//
// MessageText:
//
// Invalid bot talk table
//
#define E_GAME_INVALID_BOTTALK_TABLE     0xC7060017L

