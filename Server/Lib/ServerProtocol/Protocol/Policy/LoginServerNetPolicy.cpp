////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
// 
// Author : Generated
// 
// Description : LoginServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Net/NetDef.h"
#include "Net/Message.h"
#include "Net/Connection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/LoginServerNetPolicy.h"
#include "Protocol/Message/LoginServerMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Notify user joind and see it's valid authticket instance
		Result NetPolicyLoginServer::PlayerJoinedToGameServerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::LoginServer::PlayerJoinedToGameServerCmd::Create(m_pConnection->GetMemoryManager(), InRouteContext, InTransactionID, InPlayerID, InAuthTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyLoginServer::PlayerJoinedToGameServerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket )
		// Cmd: Kick logged in player
		Result NetPolicyLoginServer::KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InKickedPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::LoginServer::KickPlayerCmd::Create(m_pConnection->GetMemoryManager(), InRouteContext, InTransactionID, InKickedPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyLoginServer::KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InKickedPlayerID )


		// Cmd: Notify user joind and see it's valid authticket instance
		Result NetSvrPolicyLoginServer::PlayerJoinedToGameServerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::LoginServer::PlayerJoinedToGameServerRes::Create(m_pConnection->GetMemoryManager(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyLoginServer::PlayerJoinedToGameServerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// Cmd: Kick logged in player
		Result NetSvrPolicyLoginServer::KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::LoginServer::KickPlayerRes::Create(m_pConnection->GetMemoryManager(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyLoginServer::KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )


	}; // namespace Policy
}; // namespace SF


