﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : RankingServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"



namespace SF
{
    class MessageEndpoint;
   class VariableTable;
   struct ActorMovement;


	class NetPolicyRankingServer 
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetPolicyRankingServer (const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Add a player to ranking
		Result AddPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore );
		// Cmd: Remove a player to ranking
		Result RemovePlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID );
		// Cmd: Get player to ranking
		Result GetPlayerRankingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID );
		// Cmd: Update a player to ranking
		Result UpdatePlayerScoreCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount );
		// Cmd: Get ranking list
		Result GetRankingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount );
		// Cmd: Debug test
		Result DebugPrintALLRankingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const char* InFileName );

	}; // class NetPolicyRankingServer 


	class NetSvrPolicyRankingServer
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetSvrPolicyRankingServer ( const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Add a player to ranking
		Result AddPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InRanking );
		// Cmd: Remove a player to ranking
		Result RemovePlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
		// Cmd: Get player to ranking
		Result GetPlayerRankingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InRanking );
		// Cmd: Update a player to ranking
		Result UpdatePlayerScoreRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking );
		// Cmd: Get ranking list
		Result GetRankingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking );
		// Cmd: Debug test
		Result DebugPrintALLRankingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

	}; // class NetSvrPolicyRankingServer


}; // namespace SF


