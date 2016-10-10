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
			Result AddPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo );
			// Cmd: Remove a player to ranking
			Result RemovePlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Remove a player to ranking
			Result GetPlayerRankingCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Update a player to ranking
			Result UpdatePlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo );
			// Cmd: Remove a player to ranking
			Result GetRankingCmd( const TransactionID &InTransactionID, const UINT32 &InFrom, const UINT32 &InCount );

		}; // class RankingService : public ServerServiceBase


	}; // namespace Svr
}; // namespace BR


