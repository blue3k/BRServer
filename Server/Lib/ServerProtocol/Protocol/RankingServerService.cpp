////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Kyungkun Ko
// 
// Author : Generated
// 
// Description : RankingServer Server service
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFMessageEndpoint.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Protocol/RankingServerService.h"
#include "SvrTrace.h"



namespace SF
{
 	RankingServerService::RankingServerService( ServerServiceInformation* pService)
		: ServerServiceBase(pService)
	{
	}


	// Cmd: Add a player to ranking
	Result RankingServerService::AddPlayerCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyRankingServer(GetTargetEndpoint()).AddPlayerCmd( InRouteContext, InTransactionID, InPlayerInfo, InRankingScore ) );

		return hr;

	}; // Result RankingServerService::AddPlayerCmd( const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore )
	// Cmd: Remove a player to ranking
	Result RankingServerService::RemovePlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyRankingServer(GetTargetEndpoint()).RemovePlayerCmd( InRouteContext, InTransactionID, InPlayerID ) );

		return hr;

	}; // Result RankingServerService::RemovePlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	// Cmd: Get player to ranking
	Result RankingServerService::GetPlayerRankingCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyRankingServer(GetTargetEndpoint()).GetPlayerRankingCmd( InRouteContext, InTransactionID, InPlayerID ) );

		return hr;

	}; // Result RankingServerService::GetPlayerRankingCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	// Cmd: Update a player to ranking
	Result RankingServerService::UpdatePlayerScoreCmd( const uint64_t &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyRankingServer(GetTargetEndpoint()).UpdatePlayerScoreCmd( InRouteContext, InTransactionID, InRankingScore, InPlayerInfo, InCount ) );

		return hr;

	}; // Result RankingServerService::UpdatePlayerScoreCmd( const uint64_t &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount )
	// Cmd: Get ranking list
	Result RankingServerService::GetRankingCmd( const uint64_t &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyRankingServer(GetTargetEndpoint()).GetRankingCmd( InRouteContext, InTransactionID, InRankingType, InBaseRanking, InCount ) );

		return hr;

	}; // Result RankingServerService::GetRankingCmd( const uint64_t &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount )
	// Cmd: Debug test
	Result RankingServerService::DebugPrintALLRankingCmd( const uint64_t &InTransactionID, const char* InFileName )
	{
 		ScopeContext hr;

		RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
		svrCheck(NetPolicyRankingServer(GetTargetEndpoint()).DebugPrintALLRankingCmd( InRouteContext, InTransactionID, InFileName ) );

		return hr;

	}; // Result RankingServerService::DebugPrintALLRankingCmd( const uint64_t &InTransactionID, const char* InFileName )


}; // namespace SF


