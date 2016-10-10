////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : Ranking Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Policy
	{
 
		class ISvrPolicyRanking : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_SVR_RANKING };

			// Constructor
			ISvrPolicyRanking( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Add a player to ranking
			virtual Result AddPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const UINT32 &InRanking ) = 0;
			// Cmd: Remove a player to ranking
			virtual Result RemovePlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// Cmd: Remove a player to ranking
			virtual Result GetPlayerRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const UINT32 &InRanking ) = 0;
			// Cmd: Update a player to ranking
			virtual Result UpdatePlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const UINT32 &InRanking ) = 0;
			// Cmd: Remove a player to ranking
			virtual Result GetRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const UINT32 &InPlayerRanking ) = 0;

		}; // class ISvrPolicyRanking : public Net::INetPolicy


		class IPolicyRanking : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_RANKING };

			// Constructor
			IPolicyRanking( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Add a player to ranking
			virtual Result AddPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo ) = 0;
			// Cmd: Remove a player to ranking
			virtual Result RemovePlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID ) = 0;
			// Cmd: Remove a player to ranking
			virtual Result GetPlayerRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID ) = 0;
			// Cmd: Update a player to ranking
			virtual Result UpdatePlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo ) = 0;
			// Cmd: Remove a player to ranking
			virtual Result GetRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT32 &InFrom, const UINT32 &InCount ) = 0;

		}; // class IPolicyRanking : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


