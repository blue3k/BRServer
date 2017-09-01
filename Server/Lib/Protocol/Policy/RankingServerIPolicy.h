////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : RankingServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Policy
	{
 
		class ISvrPolicyRankingServer : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = PROTOCOLID_SVR_RANKINGSERVER };

			// Constructor
			ISvrPolicyRankingServer( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Add a player to ranking
			virtual Result AddPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking ) = 0;
			// Cmd: Remove a player to ranking
			virtual Result RemovePlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// Cmd: Remove a player to ranking
			virtual Result GetPlayerRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking ) = 0;
			// Cmd: Update a player to ranking
			virtual Result UpdatePlayerScoreRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking ) = 0;
			// Cmd: Get ranking list
			virtual Result GetRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking ) = 0;
			// Cmd: Debug test
			virtual Result DebugPrintALLRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;

		}; // class ISvrPolicyRankingServer : public Net::INetPolicy


		class IPolicyRankingServer : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = PROTOCOLID_RANKINGSERVER };

			// Constructor
			IPolicyRankingServer( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Add a player to ranking
			virtual Result AddPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore ) = 0;
			// Cmd: Remove a player to ranking
			virtual Result RemovePlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID ) = 0;
			// Cmd: Remove a player to ranking
			virtual Result GetPlayerRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID ) = 0;
			// Cmd: Update a player to ranking
			virtual Result UpdatePlayerScoreCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount ) = 0;
			// Cmd: Get ranking list
			virtual Result GetRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount ) = 0;
			// Cmd: Debug test
			virtual Result DebugPrintALLRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const char* InFileName ) = 0;

		}; // class IPolicyRankingServer : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


