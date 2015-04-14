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


MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_ALREADY_IN_GAME
Language=English
User is already in a game
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_INAVLID_PLAYER
Language=English
Invalid player
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_NOTREADY
Language=English
Game is not ready
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_INVALID_ROLE
Language=English
Invalid player role
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_NOT_READY_FOR_NEXT_STATE
Language=English
Not ready for next game play state
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_INVALID_VOTETYPE
Language=English
Invalid vote type
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_INVALID_VOTE_STATE
Language=English
Invalid vote state
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_INVALID_VOTE_TARGET
Language=English
Invalid vote target
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_NO_SUFFRAGE
Language=English
No suffrage
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_INVALID_GAMESTATE
Language=English
Invalid game state
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_INVALID_PARTYID
Language=English
Invalid partyID
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_INAVLID_PLAYER_COUNT
Language=English
Invalid player count for game
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_ALREADY_IN_PARTY
Language=English
User is already in a party
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_NO_PRIVILEGE
Language=English
User doesn't have eligible privilege
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_INVALID_SHOPITEM
Language=English
Invalid shop item data
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_INVALID_SHOPITEMID
Language=English
Invalid shop item id
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_NOTENOUGH_RESOURCE
Language=English
Not enough money(gold, gem, etc)
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_LOW_STAMINA
Language=English
Doesn't have enough stamina
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_INVALID_PLAYER_STATE
Language=English
Invalid player game state
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_MAX_NOTIFICATION
Language=English
Maximum notification count
.

MessageId= Severity=Error Facility=Game SymbolicName=E_GAME_MAX_TRY
Language=English
Maximum try reached
.

