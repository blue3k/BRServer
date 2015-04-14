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
;// Net Success Codes
;//
;/////////////////////////////////////////////////////////////////////////
;

MessageId= Severity=Success Facility=Net SymbolicName=S_NET_PROCESSED_SEQUENCE
Language=English
Network sequence already processed
.


;
;/////////////////////////////////////////////////////////////////////////
;//
;// Net Error Codes
;//
;/////////////////////////////////////////////////////////////////////////
;

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_VERSION_NOT_AVAIL
Language=English
The version not available or miss match with system.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_PROTOCOL_VERSION_MISMATCH
Language=English
The version not available or miss match with system.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_NOTINITIALIZED
Language=English
Network is not initialized
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_INVALID_NETCLASS
Language=English
The version not available or miss match with system.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_NOT_ENOUGH_WINDOWSPACE
Language=English
Not enough space in sliding window
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_IO_ABORTED
Language=English
Network IO aborted
.


MessageId= Severity=Error Facility=Net SymbolicName=E_NET_IO_SEND_FAIL
Language=English
Network IO send failed
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_IO_RECV_FAIL
Language=English
Network IO recv failed
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_SEND_FAIL
Language=English
Network send failed
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_RECV_FAIL
Language=English
Network recv failed
.


MessageId= Severity=Error Facility=Net SymbolicName=E_NET_INVALID_SEQUENCE
Language=English
Network Message sequence is invalid
.


MessageId= Severity=Error Facility=Net SymbolicName=E_NET_BADPACKET_SIZE
Language=English
Network Message not expected bad packet size
.


MessageId= Severity=Error Facility=Net SymbolicName=E_NET_CONNECTION_CLOSED
Language=English
Network connection closed
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_NOT_CONNECTED
Language=English
Network connection is never made
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_INVALID_CID
Language=English
Network connection ID is invalid
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_CONNECTION_TIMEOUT
Language=English
Network Connection timeout
.


MessageId= Severity=Error Facility=Net SymbolicName=E_NET_CONNECTION_FAIL
Language=English
Network Connecte failed
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_BADPACKET_TOOBIG
Language=English
Network Connecte failed
.


MessageId= Severity=Error Facility=Net SymbolicName=E_NET_BADPACKET_NOTEXPECTED
Language=English
Network Message not expected bad packet
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_INVALID_MESSAGE_CHECKSUM
Language=English
Checksum value of the message is invalid
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_INVALID_MESSAGE_ENCRYPTION
Language=English
Message encryption is invalid
.

MessageId= Severity=Error Facility=Net SymbolicName=E_NET_INVALID_CONNECTION_STATE
Language=English
Invalid connection state
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_UNSUPPORTED_PROTOCOL
Language=English
The protocol is not supported.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_FAILED_INIT
Language=English
CURL initialization is failed.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_URL_MALFORMAT
Language=English
Invalid URL format.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_NOT_BUILT_IN
Language=English
Not a build-in feature.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_COULDNT_RESOLVE_PROXY
Language=English
Failed to resolve URL.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_COULDNT_RESOLVE_HOST
Language=English
Failed to resolve URL.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_COULDNT_CONNECT
Language=English
Connection is failed
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_HTTP_POST_ERROR
Language=English
HTTP Post can't be done.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_SSL_ENGINE_NOTFOUND
Language=English
Not supported SSL type.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_SSL_ENGINE_SETFAILED
Language=English
Not supported SSL type.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_SEND_ERROR
Language=English
Network send failed
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_RECV_ERROR
Language=English
Network receive failed
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_USE_SSL_FAILED
Language=English
Invalid SSL setting.
.

MessageId= Severity=Error Facility=Net SymbolicName=E_CURLE_NO_CONNECTION_AVAILABLE
Language=English
No Connection is available now.
.


MessageId= Severity=Error Facility=Net SymbolicName=E_NET_SEQUENCE_OVERFLOW
Language=English
Too big sequence id
.


