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
;// Login Error Codes
;//
;/////////////////////////////////////////////////////////////////////////
;


MessageId= Severity=Error Facility=Login SymbolicName=E_INVALID_USERNAME
Language=English
Invalid username
.

MessageId= Severity=Error Facility=Login SymbolicName=E_INVALID_PASSWORD
Language=English
Invalid Password
.


MessageId= Severity=Error Facility=Login SymbolicName=E_ALREADY_LOGGEDIN
Language=English
Already logged in
.

MessageId= Severity=Error Facility=Login SymbolicName=E_ALREADY_LOGGEDIN_OTHERPLACE
Language=English
Already logged in at another place, and failed to kick
.

MessageId= Severity=Error Facility=Login SymbolicName=E_INVALID_SIGNATURE
Language=English
Invalid user signature
.

