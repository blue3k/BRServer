////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
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
#include "Net/SFConnection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/RankingServerNetPolicy.h"
#include "Protocol/Message/RankingServerMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Add a player to ranking
		Result NetPolicyRankingServer::AddPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::RankingServer::AddPlayerCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerInfo, InRankingScore);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyRankingServer::AddPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore )
		// Cmd: Remove a player to ranking
		Result NetPolicyRankingServer::RemovePlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::RankingServer::RemovePlayerCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyRankingServer::RemovePlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Get player to ranking
		Result NetPolicyRankingServer::GetPlayerRankingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::RankingServer::GetPlayerRankingCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyRankingServer::GetPlayerRankingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Update a player to ranking
		Result NetPolicyRankingServer::UpdatePlayerScoreCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::RankingServer::UpdatePlayerScoreCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRankingScore, InPlayerInfo, InCount);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyRankingServer::UpdatePlayerScoreCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount )
		// Cmd: Get ranking list
		Result NetPolicyRankingServer::GetRankingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::RankingServer::GetRankingCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRankingType, InBaseRanking, InCount);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyRankingServer::GetRankingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount )
		// Cmd: Debug test
		Result NetPolicyRankingServer::DebugPrintALLRankingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const char* InFileName )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::RankingServer::DebugPrintALLRankingCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InFileName);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyRankingServer::DebugPrintALLRankingCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const char* InFileName )


		// Cmd: Add a player to ranking
		Result NetSvrPolicyRankingServer::AddPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InRanking )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::RankingServer::AddPlayerRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InRanking);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyRankingServer::AddPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InRanking )
		// Cmd: Remove a player to ranking
		Result NetSvrPolicyRankingServer::RemovePlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::RankingServer::RemovePlayerRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyRankingServer::RemovePlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// Cmd: Get player to ranking
		Result NetSvrPolicyRankingServer::GetPlayerRankingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InRanking )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::RankingServer::GetPlayerRankingRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InRanking);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyRankingServer::GetPlayerRankingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InRanking )
		// Cmd: Update a player to ranking
		Result NetSvrPolicyRankingServer::UpdatePlayerScoreRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::RankingServer::UpdatePlayerScoreRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InRanking);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyRankingServer::UpdatePlayerScoreRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
		// Cmd: Get ranking list
		Result NetSvrPolicyRankingServer::GetRankingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::RankingServer::GetRankingRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InRanking);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyRankingServer::GetRankingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
		// Cmd: Debug test
		Result NetSvrPolicyRankingServer::DebugPrintALLRankingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::RankingServer::DebugPrintALLRankingRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyRankingServer::DebugPrintALLRankingRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )


	}; // namespace Policy
}; // namespace SF


