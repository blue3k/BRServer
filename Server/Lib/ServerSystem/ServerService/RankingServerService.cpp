////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : RankingServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServerService/RankingServerService.h"
#include "ServerSystem/SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		RankingServerService::RankingServerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyRankingServer::ID_POLICY)
		{
			static_assert((UINT)Policy::IPolicyRankingServer::ID_POLICY == (UINT)ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyRankingServer());
		}


		// Cmd: Add a player to ranking
		Result RankingServerService::AddPlayerCmd( const TransactionID &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyRankingServer()->AddPlayerCmd( InRouteContext, InTransactionID, InPlayerInfo, InRankingScore ) );

		Proc_End:

			return hr;

		}; // Result RankingServerService::AddPlayerCmd( const TransactionID &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore )
		// Cmd: Remove a player to ranking
		Result RankingServerService::RemovePlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyRankingServer()->RemovePlayerCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result RankingServerService::RemovePlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Remove a player to ranking
		Result RankingServerService::GetPlayerRankingCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyRankingServer()->GetPlayerRankingCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result RankingServerService::GetPlayerRankingCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Update a player to ranking
		Result RankingServerService::UpdatePlayerScoreCmd( const TransactionID &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyRankingServer()->UpdatePlayerScoreCmd( InRouteContext, InTransactionID, InRankingScore, InPlayerInfo, InCount ) );

		Proc_End:

			return hr;

		}; // Result RankingServerService::UpdatePlayerScoreCmd( const TransactionID &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount )
		// Cmd: Get ranking list
		Result RankingServerService::GetRankingCmd( const TransactionID &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyRankingServer()->GetRankingCmd( InRouteContext, InTransactionID, InRankingType, InBaseRanking, InCount ) );

		Proc_End:

			return hr;

		}; // Result RankingServerService::GetRankingCmd( const TransactionID &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount )
		// Cmd: Debug test
		Result RankingServerService::DebugPrintALLRankingCmd( const TransactionID &InTransactionID, const char* InFileName )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyRankingServer()->DebugPrintALLRankingCmd( InRouteContext, InTransactionID, InFileName ) );

		Proc_End:

			return hr;

		}; // Result RankingServerService::DebugPrintALLRankingCmd( const TransactionID &InTransactionID, const char* InFileName )


	}; // namespace Svr
}; // namespace SF


