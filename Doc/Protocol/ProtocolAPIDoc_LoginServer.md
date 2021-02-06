***
 
 CopyRight (c) 2021 StormForge
 
 Description : LoginServer Message Protocol API Document

***



Login Server Protocol

namespace SF::Message::LoginServer


# Protocol interface class NetPolicyLoginServer
## PlayerJoinedToGameServer Request
Notify Login server that client is successfully connected and joined to game server so that login server clear the player information.

1. Command interface

        Result PlayerJoinedToGameServerCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. Player ID

		- OutInAuthTicket: AuthTicket type. Authorization ticket.

2. Result interface

C++: Cast message to PlayerJoinedToGameServerRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## KickPlayer Request
Kick logged in player, used to kick player on other login server to prevent duplicated login.

1. Command interface

        Result KickPlayerCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InKickedPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInKickedPlayerID: PlayerID type. Player ID to kick

2. Result interface

C++: Cast message to KickPlayerRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 








