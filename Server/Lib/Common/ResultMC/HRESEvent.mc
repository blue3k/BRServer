;
;////////////////////////////////////////////////////////////////////////////////
;// 
;// CopyRight (c) 2013 The Braves
;// 
;// Author : KyungKun Ko
;//
;// Description : Server Event definitions. 
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



MessageId= Severity=Success Facility=BR SymbolicName=BR_EVENT_SUCCESS
Language=English
Operation Successed %1 : %2
.

MessageId= Severity=Success Facility=BR SymbolicName=BR_EVENT_S_INSTALL
Language=English
%1 Service instaled : %2
.

MessageId= Severity=Success Facility=BR SymbolicName=BR_EVENT_S_UNINSTALL
Language=English
%1 Service uninstaled : %2
.

MessageId= Severity=Error Facility=BR SymbolicName=BR_EVENT_E_INSTALL
Language=English
%1 Service install failed : %2
.

MessageId= Severity=Error Facility=BR SymbolicName=BR_EVENT_E_UNINSTALL
Language=English
%1 Service uninstall failed : %2
.

MessageId= Severity=Error Facility=Generic SymbolicName=BR_EVENT_ERROR
Language=English
Log Error %1 %2.
.

MessageId= Severity=Warning Facility=Generic SymbolicName=BR_EVENT_WARNING
Language=English
Log Warning %1 %2.
.

MessageId= Severity=Error Facility=Generic SymbolicName=BR_EVENT_ASSERT
Language=English
Log Assert %1 %2.
.

MessageId= Severity=Informational Facility=Generic SymbolicName=BR_EVENT_TRACE
Language=English
Log Trace %1 %2.
.

MessageId= Severity=Informational Facility=Generic SymbolicName=BR_EVENT_INFO
Language=English
Log Info %1 %2.
.

MessageId= Severity=Error Facility=Generic SymbolicName=BR_EVENT_CFGFILE_NOT_FOUND
Language=English
The Mad server configuration file was not found at '%1!s!'.
.

MessageId= Severity=Error Facility=Generic SymbolicName=BR_EVENT_UNKNOWN_SERVICE
Language=English
The Mad configuration file does not have an entry for the '%1' service.
.

MessageId= Severity=Error Facility=Generic SymbolicName=BR_EVENT_INVALID_CFGFILE
Language=English
The Mad configuration file '%1' is invalid.
.

MessageId= Severity=Informational Facility=Generic SymbolicName=BR_EVENT_NETMGR_DISCONNECT
Language=English
The NetManager %1.
.

MessageId= Severity=Error Facility=Generic SymbolicName=BR_EVENT_NET_CONNECTFAILED
Language=English
The NetManager failed to connect to a server.
.


MessageId= Severity=Error Facility=Generic SymbolicName=BR_EVENT_SQL_INIT_FAILED
Language=English
SQL initialization failed: %1.
.

MessageId= Severity=Error Facility=Generic SymbolicName=BR_EVENT_SQL_ACCESS_FAILED
Language=English
SQL access failed: %1.
.

