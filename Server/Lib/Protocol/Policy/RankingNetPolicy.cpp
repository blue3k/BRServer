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
		HRESULT NetPolicyRanking::AddPlayerCmd( const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::AddPlayerCmd::BuildIMsg(pMsg, InRouteContext, InContext, InPlayerID, InRankingScore, InPlayerInfo));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyRanking::AddPlayerCmd( const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
		// Cmd: Remove a player to ranking
		HRESULT NetPolicyRanking::RemovePlayerCmd( const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::RemovePlayerCmd::BuildIMsg(pMsg, InRouteContext, InContext, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyRanking::RemovePlayerCmd( const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID )
		// Cmd: Remove a player to ranking
		HRESULT NetPolicyRanking::GetPlayerRankingCmd( const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::GetPlayerRankingCmd::BuildIMsg(pMsg, InRouteContext, InContext, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyRanking::GetPlayerRankingCmd( const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID )
		// Cmd: Update a player to ranking
		HRESULT NetPolicyRanking::UpdatePlayerCmd( const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::UpdatePlayerCmd::BuildIMsg(pMsg, InRouteContext, InContext, InPlayerID, InRankingScore, InPlayerInfo));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyRanking::UpdatePlayerCmd( const RouteContext &InRouteContext, const Context &InContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
		// Cmd: Remove a player to ranking
		HRESULT NetPolicyRanking::GetRankingCmd( const RouteContext &InRouteContext, const Context &InContext, const UINT32 &InFrom, const UINT32 &InCount )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::GetRankingCmd::BuildIMsg(pMsg, InRouteContext, InContext, InFrom, InCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyRanking::GetRankingCmd( const RouteContext &InRouteContext, const Context &InContext, const UINT32 &InFrom, const UINT32 &InCount )


		// Cmd: Add a player to ranking
		HRESULT NetSvrPolicyRanking::AddPlayerRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const UINT32 &InRanking )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::AddPlayerRes::BuildIMsg(pMsg, InRouteContext, InContext, InResult, InRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyRanking::AddPlayerRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const UINT32 &InRanking )
		// Cmd: Remove a player to ranking
		HRESULT NetSvrPolicyRanking::RemovePlayerRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::RemovePlayerRes::BuildIMsg(pMsg, InRouteContext, InContext, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyRanking::RemovePlayerRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult )
		// Cmd: Remove a player to ranking
		HRESULT NetSvrPolicyRanking::GetPlayerRankingRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const UINT32 &InRanking )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::GetPlayerRankingRes::BuildIMsg(pMsg, InRouteContext, InContext, InResult, InRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyRanking::GetPlayerRankingRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const UINT32 &InRanking )
		// Cmd: Update a player to ranking
		HRESULT NetSvrPolicyRanking::UpdatePlayerRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const UINT32 &InRanking )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::UpdatePlayerRes::BuildIMsg(pMsg, InRouteContext, InContext, InResult, InRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyRanking::UpdatePlayerRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const UINT32 &InRanking )
		// Cmd: Remove a player to ranking
		HRESULT NetSvrPolicyRanking::GetRankingRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const UINT32 &InPlayerRanking )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::GetRankingRes::BuildIMsg(pMsg, InRouteContext, InContext, InResult, InPlayerRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyRanking::GetRankingRes( const RouteContext &InRouteContext, const Context &InContext, const HRESULT &InResult, const UINT32 &InPlayerRanking )


	}; // namespace Policy
}; // namespace BR


