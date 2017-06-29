////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : RankingServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/RankingServerIPolicy.h"
#include "Protocol/Policy/RankingServerNetPolicy.h"
#include "Protocol/Message/RankingServerMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: Add a player to ranking
		Result NetPolicyRankingServer::AddPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::RankingServer::AddPlayerCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InPlayerInfo, InRankingScore));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyRankingServer::AddPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore )
		// Cmd: Remove a player to ranking
		Result NetPolicyRankingServer::RemovePlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::RankingServer::RemovePlayerCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyRankingServer::RemovePlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Remove a player to ranking
		Result NetPolicyRankingServer::GetPlayerRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::RankingServer::GetPlayerRankingCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyRankingServer::GetPlayerRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Update a player to ranking
		Result NetPolicyRankingServer::UpdatePlayerScoreCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::RankingServer::UpdatePlayerScoreCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InRankingScore, InPlayerInfo, InCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyRankingServer::UpdatePlayerScoreCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount )
		// Cmd: Get ranking list
		Result NetPolicyRankingServer::GetRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::RankingServer::GetRankingCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InRankingType, InBaseRanking, InCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyRankingServer::GetRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount )


		// Cmd: Add a player to ranking
		Result NetSvrPolicyRankingServer::AddPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::RankingServer::AddPlayerRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult, InRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyRankingServer::AddPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )
		// Cmd: Remove a player to ranking
		Result NetSvrPolicyRankingServer::RemovePlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::RankingServer::RemovePlayerRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyRankingServer::RemovePlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// Cmd: Remove a player to ranking
		Result NetSvrPolicyRankingServer::GetPlayerRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::RankingServer::GetPlayerRankingRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult, InRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyRankingServer::GetPlayerRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )
		// Cmd: Update a player to ranking
		Result NetSvrPolicyRankingServer::UpdatePlayerScoreRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::RankingServer::UpdatePlayerScoreRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult, InRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyRankingServer::UpdatePlayerScoreRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
		// Cmd: Get ranking list
		Result NetSvrPolicyRankingServer::GetRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::RankingServer::GetRankingRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult, InRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyRankingServer::GetRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )


	}; // namespace Policy
}; // namespace BR


