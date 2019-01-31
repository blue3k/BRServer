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
#include "Protocol/SFProtocol.h"
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityInformation.h"
#include "Protocol/ServerService/RankingServerService.h"
#include "SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		RankingServerService::RankingServerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService)
		{
		}


		// Cmd: Add a player to ranking
		Result RankingServerService::AddPlayerCmd( const TransactionID &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyRankingServer(GetConnection()).AddPlayerCmd( InRouteContext, InTransactionID, InPlayerInfo, InRankingScore ) );

		Proc_End:

			return hr;

		}; // Result RankingServerService::AddPlayerCmd( const TransactionID &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore )
		// Cmd: Remove a player to ranking
		Result RankingServerService::RemovePlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyRankingServer(GetConnection()).RemovePlayerCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result RankingServerService::RemovePlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Get player to ranking
		Result RankingServerService::GetPlayerRankingCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyRankingServer(GetConnection()).GetPlayerRankingCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result RankingServerService::GetPlayerRankingCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Update a player to ranking
		Result RankingServerService::UpdatePlayerScoreCmd( const TransactionID &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyRankingServer(GetConnection()).UpdatePlayerScoreCmd( InRouteContext, InTransactionID, InRankingScore, InPlayerInfo, InCount ) );

		Proc_End:

			return hr;

		}; // Result RankingServerService::UpdatePlayerScoreCmd( const TransactionID &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount )
		// Cmd: Get ranking list
		Result RankingServerService::GetRankingCmd( const TransactionID &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyRankingServer(GetConnection()).GetRankingCmd( InRouteContext, InTransactionID, InRankingType, InBaseRanking, InCount ) );

		Proc_End:

			return hr;

		}; // Result RankingServerService::GetRankingCmd( const TransactionID &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount )
		// Cmd: Debug test
		Result RankingServerService::DebugPrintALLRankingCmd( const TransactionID &InTransactionID, const char* InFileName )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyRankingServer(GetConnection()).DebugPrintALLRankingCmd( InRouteContext, InTransactionID, InFileName ) );

		Proc_End:

			return hr;

		}; // Result RankingServerService::DebugPrintALLRankingCmd( const TransactionID &InTransactionID, const char* InFileName )


	}; // namespace Svr
}; // namespace SF


