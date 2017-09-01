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
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Net/Message.h"
#include "Types/BrSvrTypes.h"
#include "Types/BrGameTypes.h"
#include "Protocol/Policy/LoginServerIPolicy.h"
#include "Protocol/Policy/LoginServerNetPolicy.h"
#include "Protocol/Message/LoginServerMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: Notify user joind and see it's valid authticket instance
		Result NetPolicyLoginServer::PlayerJoinedToGameServerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::LoginServer::PlayerJoinedToGameServerCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InPlayerID, InAuthTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyLoginServer::PlayerJoinedToGameServerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket )
		// Cmd: Kick logged in player
		Result NetPolicyLoginServer::KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InKickedPlayerID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::LoginServer::KickPlayerCmd::BuildIMsg(pMsg, InRouteContext, InTransactionID, InKickedPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyLoginServer::KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InKickedPlayerID )


		// Cmd: Notify user joind and see it's valid authticket instance
		Result NetSvrPolicyLoginServer::PlayerJoinedToGameServerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::LoginServer::PlayerJoinedToGameServerRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyLoginServer::PlayerJoinedToGameServerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// Cmd: Kick logged in player
		Result NetSvrPolicyLoginServer::KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::LoginServer::KickPlayerRes::BuildIMsg(pMsg, InRouteContext, InTransactionID, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyLoginServer::KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )


	}; // namespace Policy
}; // namespace BR


