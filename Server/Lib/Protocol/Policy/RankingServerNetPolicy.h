////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : RankingServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/RankingServerIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyRankingServer : public IPolicyRankingServer
		{
 			public:
			// Constructor
			NetPolicyRankingServer ( Net::IConnection* pConn ) : IPolicyRankingServer(pConn)
			{}

			// Cmd: Add a player to ranking
			Result AddPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore );
			// Cmd: Remove a player to ranking
			Result RemovePlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Remove a player to ranking
			Result GetPlayerRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Update a player to ranking
			Result UpdatePlayerScoreCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount );
			// Cmd: Get ranking list
			Result GetRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount );
			// Cmd: Debug test
			Result DebugPrintALLRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const char* InFileName );

		}; // class NetPolicyRankingServer : public IPolicyRankingServer


		class NetSvrPolicyRankingServer : public ISvrPolicyRankingServer
		{
 			public:
			// Constructor
			NetSvrPolicyRankingServer ( Net::IConnection* pConn ) : ISvrPolicyRankingServer(pConn)
			{}

			// Cmd: Add a player to ranking
			Result AddPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking );
			// Cmd: Remove a player to ranking
			Result RemovePlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// Cmd: Remove a player to ranking
			Result GetPlayerRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking );
			// Cmd: Update a player to ranking
			Result UpdatePlayerScoreRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking );
			// Cmd: Get ranking list
			Result GetRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking );
			// Cmd: Debug test
			Result DebugPrintALLRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

		}; // class NetSvrPolicyRankingServer : public ISvrPolicyRankingServer


	}; // namespace Policy
}; // namespace BR


