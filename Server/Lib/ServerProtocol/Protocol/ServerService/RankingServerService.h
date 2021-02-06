////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : RankingServer Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/SFNetDef.h"
#include "Protocol/Policy/RankingServerNetPolicy.h"



namespace SF
{
 	class RankingServerService : public ServerServiceBase
	{
 		public:
		// Constructor
		RankingServerService ( ServerServiceInformation* pService );

		// Cmd: Add a player to ranking
		Result AddPlayerCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore );
		// Cmd: Remove a player to ranking
		Result RemovePlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID );
		// Cmd: Get player to ranking
		Result GetPlayerRankingCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID );
		// Cmd: Update a player to ranking
		Result UpdatePlayerScoreCmd( const uint64_t &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount );
		// Cmd: Get ranking list
		Result GetRankingCmd( const uint64_t &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount );
		// Cmd: Debug test
		Result DebugPrintALLRankingCmd( const uint64_t &InTransactionID, const char* InFileName );

	}; // class RankingServerService : public ServerServiceBase


}; // namespace SF


