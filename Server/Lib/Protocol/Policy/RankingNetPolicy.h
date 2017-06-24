////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Ranking Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/RankingIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyRanking : public IPolicyRanking
		{
 			public:
			// Constructor
			NetPolicyRanking ( Net::IConnection* pConn ) : IPolicyRanking(pConn)
			{}

			// Cmd: Add a player to ranking
			Result AddPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo );
			// Cmd: Remove a player to ranking
			Result RemovePlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Remove a player to ranking
			Result GetPlayerRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Update a player to ranking
			Result UpdatePlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo );
			// Cmd: Remove a player to ranking
			Result GetRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint32_t &InFrom, const uint32_t &InCount );

		}; // class NetPolicyRanking : public IPolicyRanking


		class NetSvrPolicyRanking : public ISvrPolicyRanking
		{
 			public:
			// Constructor
			NetSvrPolicyRanking ( Net::IConnection* pConn ) : ISvrPolicyRanking(pConn)
			{}

			// Cmd: Add a player to ranking
			Result AddPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking );
			// Cmd: Remove a player to ranking
			Result RemovePlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// Cmd: Remove a player to ranking
			Result GetPlayerRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking );
			// Cmd: Update a player to ranking
			Result UpdatePlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking );
			// Cmd: Remove a player to ranking
			Result GetRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InPlayerRanking );

		}; // class NetSvrPolicyRanking : public ISvrPolicyRanking


	}; // namespace Policy
}; // namespace BR


