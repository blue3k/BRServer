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
;// Game Error Codes
;//
;/////////////////////////////////////////////////////////////////////////
;

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_SERVICE_FAILED
Language=English
Server service failed
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_TIMEOUT
Language=English
Server service failed
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_TRANSACTION_INVALID_STATE
Language=English
Transaction has invalid state
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_INVALID_TRANSITION
Language=English
Transaction has invalid state transition
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_TRANSACTION_TIMEOUT
Language=English
Transaction process Time out
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_ROUTEMSG_FAILED
Language=English
Failed to route message
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_NO_RESULT_HANDLER
Language=English
No result handler
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_NO_MESSAGE_HANDLER
Language=English
No message handler
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_INVALID_SERVERID
Language=English
Invalid server ID
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_PLAYER_NOT_FOUND
Language=English
Player not found
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_INVALID_PLAYER_STATE
Language=English
Player state is invalid
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_INVALID_PLAYER_GAMEDB
Language=English
Invalid player game db data
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_NOTEXPECTED_MESSAGE
Language=English
Not expected server message
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_DB_INVALID_CONFIG
Language=English
Database connection failed
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_DB_CONNECTION_FAILED
Language=English
Database connection failed
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_DB_CONNECTION_LOST
Language=English
Database connection lost
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_DB_STATEMENT_PREPARE_FAILED
Language=English
Database statement prepare is failed
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_DB_PARAMETER_BIND_FAILED
Language=English
Database parameter binding is failed
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_DB_RESULT_PATCH_FAILED
Language=English
Database result patch is failed
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_DB_RESULT_COUNT_MISMATCH
Language=English
Database result column count is mismatched
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_DB_RESULT_FIELDTYPE_MISMATCH
Language=English
Database result column type is mismatched
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_DB_DUPLICATED_SOURCE
Language=English
Duplicated Database source is specified
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_DB_INVALID_PARTITIONINGID
Language=English
Invalid partitioning ID

.
MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_INVALID_ENTITYUID
Language=English
Invalid entity UID
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_CLUSTER_NOTREADY
Language=English
Cluster service not ready
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_INVALID_CLUSTERTYPE
Language=English
Invalid cluster type
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_INVALID_CLUSTERID
Language=English
Invalid cluster ID
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_INVALID_QUEUEITEM
Language=English
Invalid queue item
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_QUEUEITEM_CANCELED
Language=English
Queue item is canceled
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_NOITEM_INQUEUE
Language=English
There is no item in queue
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_ALREADY_INQUEUE
Language=English
Already in queue
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_RESERVED_QUEUEITEM
Language=English
Queue item is reserved
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_CLUSTER_NOMASTER
Language=English
Cluster doesn't have any assigned master
.

MessageId= Severity=Success Facility=SvrSys SymbolicName=S_SVR_QUEUEITEM_CANCEL_PENDED
Language=English
Queue item is reserved for cancel
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_INVALID_THREAD
Language=English
Thread is wrong, running on wrong thread?
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_PURCHASE_CANCELED
Language=English
Purchase canceled
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_INVALID_PURCHASE_INFO
Language=English
Invalid receipt info
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_INVALID_PURCHASE_DUPLICATED
Language=English
Invalid duplicated purchase
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_INVALID_EXTERNAL_AUTH
Language=English
External auth fail
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_EXTERNAL_SERVER_UNAVALIABLE
Language=English
External server is not available
.

MessageId= Severity=Error Facility=SvrSys SymbolicName=E_SVR_INVALID_PURCHASE_FORMAT
Language=English
Invalid receipt result from server
.




