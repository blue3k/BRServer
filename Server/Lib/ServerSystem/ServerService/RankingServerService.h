////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : RankingServer Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/RankingServerIPolicy.h"



namespace BR
{
 	namespace Svr
	{
 		class RankingServerService : public ServerServiceBase
		{
 			private:
				Policy::IPolicyRankingServer* GetPolicyRankingServer() { return GetPolicy<Policy::IPolicyRankingServer>(); }
			public:
				enum { ID_SERVICEPOLICY = Policy::IPolicyRankingServer::ID_POLICY };
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
}; // namespace BR


