////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : RankingServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
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
		Result RankingServerService::AddPlayerCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyRankingServer(GetConnection()).AddPlayerCmd( InRouteContext, InTransactionID, InPlayerInfo, InRankingScore ) );

			return hr;

		}; // Result RankingServerService::AddPlayerCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore )
		// Cmd: Remove a player to ranking
		Result RankingServerService::RemovePlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyRankingServer(GetConnection()).RemovePlayerCmd( InRouteContext, InTransactionID, InPlayerID ) );

			return hr;

		}; // Result RankingServerService::RemovePlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Get player to ranking
		Result RankingServerService::GetPlayerRankingCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyRankingServer(GetConnection()).GetPlayerRankingCmd( InRouteContext, InTransactionID, InPlayerID ) );

			return hr;

		}; // Result RankingServerService::GetPlayerRankingCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Update a player to ranking
		Result RankingServerService::UpdatePlayerScoreCmd( const uint64_t &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyRankingServer(GetConnection()).UpdatePlayerScoreCmd( InRouteContext, InTransactionID, InRankingScore, InPlayerInfo, InCount ) );

			return hr;

		}; // Result RankingServerService::UpdatePlayerScoreCmd( const uint64_t &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount )
		// Cmd: Get ranking list
		Result RankingServerService::GetRankingCmd( const uint64_t &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyRankingServer(GetConnection()).GetRankingCmd( InRouteContext, InTransactionID, InRankingType, InBaseRanking, InCount ) );

			return hr;

		}; // Result RankingServerService::GetRankingCmd( const uint64_t &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount )
		// Cmd: Debug test
		Result RankingServerService::DebugPrintALLRankingCmd( const uint64_t &InTransactionID, const char* InFileName )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrCheck(Policy::NetPolicyRankingServer(GetConnection()).DebugPrintALLRankingCmd( InRouteContext, InTransactionID, InFileName ) );

			return hr;

		}; // Result RankingServerService::DebugPrintALLRankingCmd( const uint64_t &InTransactionID, const char* InFileName )


	}; // namespace Svr
}; // namespace SF


