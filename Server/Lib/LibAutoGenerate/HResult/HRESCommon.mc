;
;////////////////////////////////////////////////////////////////////////////////
;// 
;// CopyRight (c) 2013 The Braves
;// 
;// Author : KyungKun Ko
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
;// System level event IDs
;//
;/////////////////////////////////////////////////////////////////////////
;

;
;/////////////////////////////////////////////////////////////////////////
;//
;// Generic Error Codes
;//
;/////////////////////////////////////////////////////////////////////////
;

MessageId= Severity=Error Facility=Generic SymbolicName=E_NOT_INITIALIZED
Language=English
Called without initialization.
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_VERSION_NOT_AVAIL
Language=English
The version not available or miss match with system.
.


MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_CLUSTERMASTER
Language=English
Invalid Cluster Master server
.


MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_STATE
Language=English
The version not available or miss match with system.
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_STR_FORMAT
Language=English
Invalid string format.
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_DUPLICATED_SERVERID
Language=English
ServerID is duplicated.
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_ENTITY
Language=English
Invalid entity
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_DUPLICATED_ENTITY
Language=English
Invalid duplicated entity
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_DUPLICATED_ENTITYID
Language=English
Invalid duplicated entity
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_DUPLICATED_ACCOUNTID
Language=English
Duplicated Account ID
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_DUPLICATED_PLAYERID
Language=English
Duplicated Player ID
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_DUPLICATED_NAME
Language=English
Duplicated name
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_DUPLICATED_ID
Language=English
Duplicated ID
.


MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_ACCOUNTID
Language=English
Invalid duplicated entity
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_TICKET
Language=English
Invalid ticket
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_PLAYERID
Language=English
Invalid player ID
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_VALUE
Language=English
Invalid value
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_CLASS
Language=English
Invalid class value
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_DIFFICULTY
Language=English
Invalid Difficulty
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_REGISTRATION
Language=English
Invalid registration to play
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_CHANNELID
Language=English
Invalid Channel ID
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_INSTANCEID
Language=English
Invalid Zone instance ID
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_DUPLICATED_COMPONENT
Language=English
Duplicated component
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_MAX_FRIEND
Language=English
Maximum friend
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_PLAYER_COUNT
Language=English
Invalid player count for service
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_PLAYER_LEVEL
Language=English
Invalid player level
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_PLAYER_NICK
Language=English
Invalid player nick
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_NOTIFICATIONID
Language=English
Invalid Notification ID
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_RANKING_RANGE
Language=English
Invalid Ranking range
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_TARGET_MAX_FRIEND
Language=English
Target Maximum friend
.


MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_STR_DATA
Language=English
Invalid string data.
.


MessageId= Severity=Error Facility=Generic SymbolicName=E_NOT_AUTORIZED
Language=English
Not authorized
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_NO_PERMITION
Language=English
No permition
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_NO_FILE_OR_DIR
Language=English
No file or directory
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_NO_SUCH_PROCESS
Language=English
No such process
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INTERRUPTED_SYSCALL
Language=English
Interrupted system call
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_IO_ERROR
Language=English
I/O error
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_NOT_DEVICE
Language=English
No such device or address
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_TOO_MANY_ARGLIST
Language=English
Arg list too long
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_EXEC_FROMAT
Language=English
Exec format error
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_FILE_HANDLE
Language=English
Bad file number
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_NO_CHILD_PROCESS
Language=English
No child process
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_TRY_AGAIN
Language=English
Try again
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_POINTER
Language=English
Bad address
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_NOT_BLOCK
Language=English
Block device required
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_BUSY
Language=English
Busy
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_FILE_EXISTS
Language=English
File exists
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_CROSS_LINK
Language=English
Cross-device link
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_DEVICE
Language=English
No such device
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_DIRECTORY
Language=English
Not a directory
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_DIRECTORY
Language=English
Is a directory
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_ARG
Language=English
Invalid argument
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_NOT_ENOUGH_FILE_TABLE
Language=English
File table overflow
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_TOO_MANY_OPENED_FILE
Language=English
Too many open files
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_TYPEWRITER
Language=English
Not a typewriter
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_BUSY_FILE
Language=English
Text file busy
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_TOO_BIG_FILE
Language=English
File too large
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_NOT_ENOUGH_SPACE
Language=English
No space left on device
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_SEEK
Language=English
Illegal seek
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_READ_ONLY
Language=English
Read-only file system
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_TOO_MANY_LINK
Language=English
Too many links
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_PIPE
Language=English
Broken pipe
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_MATH_DOMAIN
Language=English
Math argument out of domain of func
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_NUMERIC
Language=English
Math result not representable
.

MessageId= Severity=Error Facility=Generic SymbolicName=E_INVALID_PASSWORD
Language=English
Invalid password
.




