
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
// Login Error Codes
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
// MessageId: E_INVALID_USERNAME
//
// MessageText:
//
// Invalid username
//
#define E_INVALID_USERNAME               0xC7050001L

//
// MessageId: E_INVALID_PASSWORD
//
// MessageText:
//
// Invalid Password
//
#define E_INVALID_PASSWORD               0xC7050002L

//
// MessageId: E_ALREADY_LOGGEDIN
//
// MessageText:
//
// Already logged in
//
#define E_ALREADY_LOGGEDIN               0xC7050003L

//
// MessageId: E_ALREADY_LOGGEDIN_OTHERPLACE
//
// MessageText:
//
// Already logged in at another place, and failed to kick
//
#define E_ALREADY_LOGGEDIN_OTHERPLACE    0xC7050004L

//
// MessageId: E_INVALID_SIGNATURE
//
// MessageText:
//
// Invalid user signature
//
#define E_INVALID_SIGNATURE              0xC7050005L

