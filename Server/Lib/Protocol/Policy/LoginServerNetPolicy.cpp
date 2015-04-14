////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : LoginServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/LoginServerIPolicy.h"
#include "Protocol/Policy/LoginServerNetPolicy.h"
#include "Protocol/Message/LoginServerMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: Notify user joind and see it's valid authticket instance
		HRESULT NetPolicyLoginServer::PlayerJoinedToGameServerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::LoginServer::PlayerJoinedToGameServerCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID, InAuthTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyLoginServer::PlayerJoinedToGameServerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket )
		// Cmd: Kick logged in player
		HRESULT NetPolicyLoginServer::KickPlayerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::LoginServer::KickPlayerCmd::BuildIMsg(pMsg, InContext, InRouteContext, InKickedPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyLoginServer::KickPlayerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )


		// Cmd: Notify user joind and see it's valid authticket instance
		HRESULT NetSvrPolicyLoginServer::PlayerJoinedToGameServerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::LoginServer::PlayerJoinedToGameServerRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyLoginServer::PlayerJoinedToGameServerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		// Cmd: Kick logged in player
		HRESULT NetSvrPolicyLoginServer::KickPlayerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::LoginServer::KickPlayerRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyLoginServer::KickPlayerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )


	}; // namespace Policy
}; // namespace BR


