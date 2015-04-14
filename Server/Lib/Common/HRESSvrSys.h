
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
// MessageId: E_SVR_SERVICE_FAILED
//
// MessageText:
//
// Server service failed
//
#define E_SVR_SERVICE_FAILED             0xC7040001L

//
// MessageId: E_SVR_TIMEOUT
//
// MessageText:
//
// Server service failed
//
#define E_SVR_TIMEOUT                    0xC7040002L

//
// MessageId: E_SVR_TRANSACTION_INVALID_STATE
//
// MessageText:
//
// Transaction has invalid state
//
#define E_SVR_TRANSACTION_INVALID_STATE  0xC7040003L

//
// MessageId: E_SVR_INVALID_TRANSITION
//
// MessageText:
//
// Transaction has invalid state transition
//
#define E_SVR_INVALID_TRANSITION         0xC7040004L

//
// MessageId: E_SVR_TRANSACTION_TIMEOUT
//
// MessageText:
//
// Transaction process Time out
//
#define E_SVR_TRANSACTION_TIMEOUT        0xC7040005L

//
// MessageId: E_SVR_ROUTEMSG_FAILED
//
// MessageText:
//
// Failed to route message
//
#define E_SVR_ROUTEMSG_FAILED            0xC7040006L

//
// MessageId: E_SVR_NO_RESULT_HANDLER
//
// MessageText:
//
// No result handler
//
#define E_SVR_NO_RESULT_HANDLER          0xC7040007L

//
// MessageId: E_SVR_NO_MESSAGE_HANDLER
//
// MessageText:
//
// No message handler
//
#define E_SVR_NO_MESSAGE_HANDLER         0xC7040008L

//
// MessageId: E_SVR_INVALID_SERVERID
//
// MessageText:
//
// Invalid server ID
//
#define E_SVR_INVALID_SERVERID           0xC7040009L

//
// MessageId: E_PLAYER_NOT_FOUND
//
// MessageText:
//
// Player not found
//
#define E_PLAYER_NOT_FOUND               0xC704000AL

//
// MessageId: E_SVR_INVALID_PLAYER_STATE
//
// MessageText:
//
// Player state is invalid
//
#define E_SVR_INVALID_PLAYER_STATE       0xC704000BL

//
// MessageId: E_SVR_INVALID_PLAYER_GAMEDB
//
// MessageText:
//
// Invalid player game db data
//
#define E_SVR_INVALID_PLAYER_GAMEDB      0xC704000CL

//
// MessageId: E_SVR_NOTEXPECTED_MESSAGE
//
// MessageText:
//
// Not expected server message
//
#define E_SVR_NOTEXPECTED_MESSAGE        0xC704000DL

//
// MessageId: E_DB_INVALID_CONFIG
//
// MessageText:
//
// Database connection failed
//
#define E_DB_INVALID_CONFIG              0xC704000EL

//
// MessageId: E_DB_CONNECTION_FAILED
//
// MessageText:
//
// Database connection failed
//
#define E_DB_CONNECTION_FAILED           0xC704000FL

//
// MessageId: E_DB_CONNECTION_LOST
//
// MessageText:
//
// Database connection lost
//
#define E_DB_CONNECTION_LOST             0xC7040010L

//
// MessageId: E_DB_STATEMENT_PREPARE_FAILED
//
// MessageText:
//
// Database statement prepare is failed
//
#define E_DB_STATEMENT_PREPARE_FAILED    0xC7040011L

//
// MessageId: E_DB_PARAMETER_BIND_FAILED
//
// MessageText:
//
// Database parameter binding is failed
//
#define E_DB_PARAMETER_BIND_FAILED       0xC7040012L

//
// MessageId: E_DB_RESULT_PATCH_FAILED
//
// MessageText:
//
// Database result patch is failed
//
#define E_DB_RESULT_PATCH_FAILED         0xC7040013L

//
// MessageId: E_DB_RESULT_COUNT_MISMATCH
//
// MessageText:
//
// Database result column count is mismatched
//
#define E_DB_RESULT_COUNT_MISMATCH       0xC7040014L

//
// MessageId: E_DB_RESULT_FIELDTYPE_MISMATCH
//
// MessageText:
//
// Database result column type is mismatched
//
#define E_DB_RESULT_FIELDTYPE_MISMATCH   0xC7040015L

//
// MessageId: E_DB_DUPLICATED_SOURCE
//
// MessageText:
//
// Duplicated Database source is specified
//
#define E_DB_DUPLICATED_SOURCE           0xC7040016L

//
// MessageId: E_DB_INVALID_PARTITIONINGID
//
// MessageText:
//
// Invalid partitioning ID
// 
//
#define E_DB_INVALID_PARTITIONINGID      0xC7040017L

//
// MessageId: E_SVR_INVALID_ENTITYUID
//
// MessageText:
//
// Invalid entity UID
//
#define E_SVR_INVALID_ENTITYUID          0xC7040018L

//
// MessageId: E_SVR_CLUSTER_NOTREADY
//
// MessageText:
//
// Cluster service not ready
//
#define E_SVR_CLUSTER_NOTREADY           0xC7040019L

//
// MessageId: E_SVR_INVALID_CLUSTERTYPE
//
// MessageText:
//
// Invalid cluster type
//
#define E_SVR_INVALID_CLUSTERTYPE        0xC704001AL

//
// MessageId: E_SVR_INVALID_CLUSTERID
//
// MessageText:
//
// Invalid cluster ID
//
#define E_SVR_INVALID_CLUSTERID          0xC704001BL

//
// MessageId: E_SVR_INVALID_QUEUEITEM
//
// MessageText:
//
// Invalid queue item
//
#define E_SVR_INVALID_QUEUEITEM          0xC704001CL

//
// MessageId: E_SVR_QUEUEITEM_CANCELED
//
// MessageText:
//
// Queue item is canceled
//
#define E_SVR_QUEUEITEM_CANCELED         0xC704001DL

//
// MessageId: E_SVR_NOITEM_INQUEUE
//
// MessageText:
//
// There is no item in queue
//
#define E_SVR_NOITEM_INQUEUE             0xC704001EL

//
// MessageId: E_SVR_ALREADY_INQUEUE
//
// MessageText:
//
// Already in queue
//
#define E_SVR_ALREADY_INQUEUE            0xC704001FL

//
// MessageId: E_SVR_RESERVED_QUEUEITEM
//
// MessageText:
//
// Queue item is reserved
//
#define E_SVR_RESERVED_QUEUEITEM         0xC7040020L

//
// MessageId: E_SVR_CLUSTER_NOMASTER
//
// MessageText:
//
// Cluster doesn't have any assigned master
//
#define E_SVR_CLUSTER_NOMASTER           0xC7040021L

//
// MessageId: S_SVR_QUEUEITEM_CANCEL_PENDED
//
// MessageText:
//
// Queue item is reserved for cancel
//
#define S_SVR_QUEUEITEM_CANCEL_PENDED    0x07040022L

//
// MessageId: S_SVR_INVALID_THREAD
//
// MessageText:
//
// Thread is wrong, running on wrong thread?
//
#define S_SVR_INVALID_THREAD             0x07040023L

