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
		HRESULT NetPolicyRanking::AddPlayerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::AddPlayerCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID, InRankingScore, InPlayerInfo));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyRanking::AddPlayerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
		// Cmd: Remove a player to ranking
		HRESULT NetPolicyRanking::RemovePlayerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::RemovePlayerCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyRanking::RemovePlayerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
		// Cmd: Remove a player to ranking
		HRESULT NetPolicyRanking::GetPlayerRankingCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::GetPlayerRankingCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyRanking::GetPlayerRankingCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
		// Cmd: Update a player to ranking
		HRESULT NetPolicyRanking::UpdatePlayerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::UpdatePlayerCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID, InRankingScore, InPlayerInfo));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyRanking::UpdatePlayerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
		// Cmd: Remove a player to ranking
		HRESULT NetPolicyRanking::GetRankingCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT32 &InFrom, const UINT32 &InCount )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::GetRankingCmd::BuildIMsg(pMsg, InContext, InRouteContext, InFrom, InCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyRanking::GetRankingCmd( const Context &InContext, const RouteContext &InRouteContext, const UINT32 &InFrom, const UINT32 &InCount )


		// Cmd: Add a player to ranking
		HRESULT NetSvrPolicyRanking::AddPlayerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InRanking )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::AddPlayerRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext, InRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyRanking::AddPlayerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InRanking )
		// Cmd: Remove a player to ranking
		HRESULT NetSvrPolicyRanking::RemovePlayerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::RemovePlayerRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyRanking::RemovePlayerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		// Cmd: Remove a player to ranking
		HRESULT NetSvrPolicyRanking::GetPlayerRankingRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InRanking )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::GetPlayerRankingRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext, InRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyRanking::GetPlayerRankingRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InRanking )
		// Cmd: Update a player to ranking
		HRESULT NetSvrPolicyRanking::UpdatePlayerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InRanking )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::UpdatePlayerRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext, InRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyRanking::UpdatePlayerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InRanking )
		// Cmd: Remove a player to ranking
		HRESULT NetSvrPolicyRanking::GetRankingRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InPlayerRanking )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Ranking::GetRankingRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext, InPlayerRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyRanking::GetRankingRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InPlayerRanking )


	}; // namespace Policy
}; // namespace BR


