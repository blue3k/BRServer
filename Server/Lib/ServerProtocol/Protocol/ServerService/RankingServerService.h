////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : RankingServer Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Protocol/Policy/RankingServerNetPolicy.h"



namespace SF
{
 	namespace Svr
	{
 		class RankingServerService : public ServerServiceBase
		{
 			public:
			// Constructor
			RankingServerService ( ServerServiceInformation* pService );

			// Cmd: Add a player to ranking
			Result AddPlayerCmd( const TransactionID &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore );
			// Cmd: Remove a player to ranking
			Result RemovePlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Remove a player to ranking
			Result GetPlayerRankingCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Update a player to ranking
			Result UpdatePlayerScoreCmd( const TransactionID &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount );
			// Cmd: Get ranking list
			Result GetRankingCmd( const TransactionID &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount );
			// Cmd: Debug test
			Result DebugPrintALLRankingCmd( const TransactionID &InTransactionID, const char* InFileName );

		}; // class RankingServerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


