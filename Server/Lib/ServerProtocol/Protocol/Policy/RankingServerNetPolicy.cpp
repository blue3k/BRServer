////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : RankingServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFMessageEndpoint.h"
#include "Actor/Movement/SFActorMovement.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/RankingServerNetPolicy.h"
#include "Protocol/Message/RankingServerMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	// Cmd: Add a player to ranking
	Result NetPolicyRankingServer::AddPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RankingServer::AddPlayerCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerInfo, InRankingScore);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyRankingServer::AddPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore )
	// Cmd: Remove a player to ranking
	Result NetPolicyRankingServer::RemovePlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RankingServer::RemovePlayerCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyRankingServer::RemovePlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	// Cmd: Get player to ranking
	Result NetPolicyRankingServer::GetPlayerRankingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RankingServer::GetPlayerRankingCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyRankingServer::GetPlayerRankingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	// Cmd: Update a player to ranking
	Result NetPolicyRankingServer::UpdatePlayerScoreCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RankingServer::UpdatePlayerScoreCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InRankingScore, InPlayerInfo, InCount);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyRankingServer::UpdatePlayerScoreCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount )
	// Cmd: Get ranking list
	Result NetPolicyRankingServer::GetRankingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RankingServer::GetRankingCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InRankingType, InBaseRanking, InCount);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyRankingServer::GetRankingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount )
	// Cmd: Debug test
	Result NetPolicyRankingServer::DebugPrintALLRankingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const char* InFileName )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RankingServer::DebugPrintALLRankingCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InFileName);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyRankingServer::DebugPrintALLRankingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const char* InFileName )


	// Cmd: Add a player to ranking
	Result NetSvrPolicyRankingServer::AddPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InRanking )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RankingServer::AddPlayerRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult, InRanking);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyRankingServer::AddPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InRanking )
	// Cmd: Remove a player to ranking
	Result NetSvrPolicyRankingServer::RemovePlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RankingServer::RemovePlayerRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyRankingServer::RemovePlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	// Cmd: Get player to ranking
	Result NetSvrPolicyRankingServer::GetPlayerRankingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InRanking )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RankingServer::GetPlayerRankingRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult, InRanking);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyRankingServer::GetPlayerRankingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InRanking )
	// Cmd: Update a player to ranking
	Result NetSvrPolicyRankingServer::UpdatePlayerScoreRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RankingServer::UpdatePlayerScoreRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult, InRanking);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyRankingServer::UpdatePlayerScoreRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
	// Cmd: Get ranking list
	Result NetSvrPolicyRankingServer::GetRankingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RankingServer::GetRankingRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult, InRanking);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyRankingServer::GetRankingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
	// Cmd: Debug test
	Result NetSvrPolicyRankingServer::DebugPrintALLRankingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::RankingServer::DebugPrintALLRankingRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyRankingServer::DebugPrintALLRankingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )


}; // namespace SF


