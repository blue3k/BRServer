////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Ranking Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Common/BrBaseTypes.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServerService/RankingService.h"
#include "ServerSystem/SvrTrace.h"



namespace BR
{
 	namespace Svr
	{
 		RankingService::RankingService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyRanking::ID_POLICY)
		{
			static_assert((UINT)Policy::IPolicyRanking::ID_POLICY == (UINT)ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyRanking());
		}


		// Cmd: Add a player to ranking
		Result RankingService::AddPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyRanking()->AddPlayerCmd( InRouteContext, InTransactionID, InPlayerID, InRankingScore, InPlayerInfo ) );

		Proc_End:

			return hr;

		}; // Result RankingService::AddPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo )
		// Cmd: Remove a player to ranking
		Result RankingService::RemovePlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyRanking()->RemovePlayerCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result RankingService::RemovePlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Remove a player to ranking
		Result RankingService::GetPlayerRankingCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyRanking()->GetPlayerRankingCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result RankingService::GetPlayerRankingCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Update a player to ranking
		Result RankingService::UpdatePlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyRanking()->UpdatePlayerCmd( InRouteContext, InTransactionID, InPlayerID, InRankingScore, InPlayerInfo ) );

		Proc_End:

			return hr;

		}; // Result RankingService::UpdatePlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo )
		// Cmd: Remove a player to ranking
		Result RankingService::GetRankingCmd( const TransactionID &InTransactionID, const uint32_t &InFrom, const uint32_t &InCount )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyRanking()->GetRankingCmd( InRouteContext, InTransactionID, InFrom, InCount ) );

		Proc_End:

			return hr;

		}; // Result RankingService::GetRankingCmd( const TransactionID &InTransactionID, const uint32_t &InFrom, const uint32_t &InCount )


	}; // namespace Svr
}; // namespace BR


