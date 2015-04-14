
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server Event definitions. 
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


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
// MessageId: BR_EVENT_SUCCESS
//
// MessageText:
//
// Operation Successed %1 : %2
//
#define BR_EVENT_SUCCESS                 0x07010001L

//
// MessageId: BR_EVENT_S_INSTALL
//
// MessageText:
//
// %1 Service instaled : %2
//
#define BR_EVENT_S_INSTALL               0x07010002L

//
// MessageId: BR_EVENT_S_UNINSTALL
//
// MessageText:
//
// %1 Service uninstaled : %2
//
#define BR_EVENT_S_UNINSTALL             0x07010003L

//
// MessageId: BR_EVENT_E_INSTALL
//
// MessageText:
//
// %1 Service install failed : %2
//
#define BR_EVENT_E_INSTALL               0xC7010004L

//
// MessageId: BR_EVENT_E_UNINSTALL
//
// MessageText:
//
// %1 Service uninstall failed : %2
//
#define BR_EVENT_E_UNINSTALL             0xC7010005L

//
// MessageId: BR_EVENT_ERROR
//
// MessageText:
//
// Log Error %1 %2.
//
#define BR_EVENT_ERROR                   0xC7020001L

//
// MessageId: BR_EVENT_WARNING
//
// MessageText:
//
// Log Warning %1 %2.
//
#define BR_EVENT_WARNING                 0x87020002L

//
// MessageId: BR_EVENT_ASSERT
//
// MessageText:
//
// Log Assert %1 %2.
//
#define BR_EVENT_ASSERT                  0xC7020003L

//
// MessageId: BR_EVENT_TRACE
//
// MessageText:
//
// Log Trace %1 %2.
//
#define BR_EVENT_TRACE                   0x47020004L

//
// MessageId: BR_EVENT_INFO
//
// MessageText:
//
// Log Info %1 %2.
//
#define BR_EVENT_INFO                    0x47020005L

//
// MessageId: BR_EVENT_CFGFILE_NOT_FOUND
//
// MessageText:
//
// The Mad server configuration file was not found at '%1!s!'.
//
#define BR_EVENT_CFGFILE_NOT_FOUND       0xC7020006L

//
// MessageId: BR_EVENT_UNKNOWN_SERVICE
//
// MessageText:
//
// The Mad configuration file does not have an entry for the '%1' service.
//
#define BR_EVENT_UNKNOWN_SERVICE         0xC7020007L

//
// MessageId: BR_EVENT_INVALID_CFGFILE
//
// MessageText:
//
// The Mad configuration file '%1' is invalid.
//
#define BR_EVENT_INVALID_CFGFILE         0xC7020008L

//
// MessageId: BR_EVENT_NETMGR_DISCONNECT
//
// MessageText:
//
// The NetManager %1.
//
#define BR_EVENT_NETMGR_DISCONNECT       0x47020009L

//
// MessageId: BR_EVENT_NET_CONNECTFAILED
//
// MessageText:
//
// The NetManager failed to connect to a server.
//
#define BR_EVENT_NET_CONNECTFAILED       0xC702000AL

//
// MessageId: BR_EVENT_SQL_INIT_FAILED
//
// MessageText:
//
// SQL initialization failed: %1.
//
#define BR_EVENT_SQL_INIT_FAILED         0xC702000BL

//
// MessageId: BR_EVENT_SQL_ACCESS_FAILED
//
// MessageText:
//
// SQL access failed: %1.
//
#define BR_EVENT_SQL_ACCESS_FAILED       0xC702000CL

