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
#include "Net/NetDef.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/RankingIPolicy.h"
#include "Protocol/Policy/RankingNetPolicy.h"
#include "Protocol/Message/RankingMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: Add a player to ranking
		Result NetPolicyRanking::AddPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::AddPlayerCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InPlayerID, InRankingScore, InPlayerInfo));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyRanking::AddPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo )
		// Cmd: Remove a player to ranking
		Result NetPolicyRanking::RemovePlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::RemovePlayerCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyRanking::RemovePlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Remove a player to ranking
		Result NetPolicyRanking::GetPlayerRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::GetPlayerRankingCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyRanking::GetPlayerRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Update a player to ranking
		Result NetPolicyRanking::UpdatePlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::UpdatePlayerCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InPlayerID, InRankingScore, InPlayerInfo));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyRanking::UpdatePlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo )
		// Cmd: Remove a player to ranking
		Result NetPolicyRanking::GetRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint32_t &InFrom, const uint32_t &InCount )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::GetRankingCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InFrom, InCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyRanking::GetRankingCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint32_t &InFrom, const uint32_t &InCount )


		// Cmd: Add a player to ranking
		Result NetSvrPolicyRanking::AddPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::AddPlayerRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult, InRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyRanking::AddPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )
		// Cmd: Remove a player to ranking
		Result NetSvrPolicyRanking::RemovePlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::RemovePlayerRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyRanking::RemovePlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// Cmd: Remove a player to ranking
		Result NetSvrPolicyRanking::GetPlayerRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::GetPlayerRankingRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult, InRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyRanking::GetPlayerRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )
		// Cmd: Update a player to ranking
		Result NetSvrPolicyRanking::UpdatePlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::UpdatePlayerRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult, InRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyRanking::UpdatePlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking )
		// Cmd: Remove a player to ranking
		Result NetSvrPolicyRanking::GetRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InPlayerRanking )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::GetRankingRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult, InPlayerRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyRanking::GetRankingRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InPlayerRanking )


	}; // namespace Policy
}; // namespace BR


