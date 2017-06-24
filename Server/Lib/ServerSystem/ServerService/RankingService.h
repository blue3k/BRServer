////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Ranking Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/RankingIPolicy.h"



namespace BR
{
 	namespace Svr
	{
 		class RankingService : public ServerServiceBase
		{
 			private:
				Policy::IPolicyRanking* GetPolicyRanking() { return GetPolicy<Policy::IPolicyRanking>(); }
			public:
				enum { ID_SERVICEPOLICY = Policy::IPolicyRanking::ID_POLICY };
			// Constructor
			RankingService ( ServerServiceInformation* pService );

			// Cmd: Add a player to ranking
			Result AddPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo );
			// Cmd: Remove a player to ranking
			Result RemovePlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Remove a player to ranking
			Result GetPlayerRankingCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Update a player to ranking
			Result UpdatePlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo );
			// Cmd: Remove a player to ranking
			Result GetRankingCmd( const TransactionID &InTransactionID, const uint32_t &InFrom, const uint32_t &InCount );

		}; // class RankingService : public ServerServiceBase


	}; // namespace Svr
}; // namespace BR


