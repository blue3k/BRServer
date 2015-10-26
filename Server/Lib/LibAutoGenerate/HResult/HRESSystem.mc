;
;////////////////////////////////////////////////////////////////////////////////
;// 
;// CopyRight (c) 2015 The Braves
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
              
FacilityNames=(	System=0xFFF:FACILITY_SYSTEM
				BR=0x701:FACILITY_BR
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


MessageId= Severity=Success Facility=System SymbolicName=S_FALSE
Language=English

.

MessageId= Severity=Error Facility=System SymbolicName=E_FAIL
Language=English
E_Fail
.

MessageId= Severity=Error Facility=System SymbolicName=E_NOTIMPL
Language=English
Not implemented
.

MessageId= Severity=Error Facility=System SymbolicName=E_OUTOFMEMORY
Language=English
Out of memory
.

MessageId= Severity=Error Facility=System SymbolicName=E_UNEXPECTED
Language=English

.

MessageId= Severity=Error Facility=System SymbolicName=E_POINTER
Language=English

.


MessageId= Severity=Error Facility=System SymbolicName=E_INVALIDARG
Language=English

.



