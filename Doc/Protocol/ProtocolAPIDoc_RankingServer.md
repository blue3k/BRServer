***
 
 CopyRight (c) 2018 StormForge
 
 Description : RankingServer Message Protocol API Document

***



Game ranking Server Protocol

namespace SF::Message::RankingServer


# Protocol interface class NetPolicyRankingServer
## AddPlayer Request
Add a player to ranking

1. Command interface

        Result AddPlayerCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerInfo: PlayerInformation type. PlayerInformation to add

		- OutInRankingScore: uint64 type. Ranking score

2. Result interface

C++: Cast message to AddPlayerRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutRanking: uint32 type. Ranking


## RemovePlayer Request
Remove a player to ranking

1. Command interface

        Result RemovePlayerCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. Player ID to remove

2. Result interface

C++: Cast message to RemovePlayerRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## GetPlayerRanking Request
Get player to ranking

1. Command interface

        Result GetPlayerRankingCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. Requester player ID

2. Result interface

C++: Cast message to GetPlayerRankingRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutRanking: uint32 type. My ranking


## UpdatePlayerScore Request
Update a player to ranking

1. Command interface

        Result UpdatePlayerScoreCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRankingScore: uint64 type. new score

		- OutInPlayerInfo: PlayerInformation type. Player information to update

		- OutInCount: uint16 type. Number of playerinformation to query

2. Result interface

C++: Cast message to UpdatePlayerScoreRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutRanking: TotalRankingPlayerInformation type. ranking player information


## GetRanking Request
Get ranking list

1. Command interface

        Result GetRankingCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRankingType: RankingType type. To separate ranking type

		- OutInBaseRanking: uint16 type. Base ranking to query, 0 based ranking order

		- OutInCount: uint16 type. Number of playerinformation to query

2. Result interface

C++: Cast message to GetRankingRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutRanking: TotalRankingPlayerInformation type. ranking player information


## DebugPrintALLRanking Request
Debug test

1. Command interface

        Result DebugPrintALLRankingCmd(const RouteContext &InRouteContext, const TransactionID &InTransactionID, const char* InFileName)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInFileName: String type. 

2. Result interface

C++: Cast message to DebugPrintALLRankingRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 








