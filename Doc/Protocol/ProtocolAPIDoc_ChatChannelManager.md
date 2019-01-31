***
 
 CopyRight (c) 2018 StormForge
 
 Description : ChatChannelManager Message Protocol API Document

***



Chatting channel manager service Protocol

namespace SF::Message::ChatChannelManager


# Protocol interface class NetPolicyChatChannelManager
## CreateChannel Request
Create a channel instance

1. Command interface

        Result CreateChannelCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName, const char* InPasscode, const PlayerInformation &InCreator)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInChannelName: String type. Channel name for search

		- OutInPasscode: String type. Passcode, can be empty string for public room

		- OutInCreator: PlayerInformation type. Player information who creates

2. Result interface

C++: Cast message to CreateChannelRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutChannelUID: EntityUID type. Created channel unique ID


## FindChannel Request
Find a channel instance with name

1. Command interface

        Result FindChannelCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInChannelName: String type. Channel name to find

2. Result interface

C++: Cast message to FindChannelRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutChannelUID: EntityUID type. Channel entity UID


## ChatChannelDeletedC2SEvt
Notification that a chat channel instance has deleted

        Result ChatChannelDeletedC2SEvt(const RouteContext &InRouteContext, const uint16_t &InRouteHopCount)

		- OutInRouteContext: RouteContext type. 

		- OutInRouteHopCount: uint16 type. 








