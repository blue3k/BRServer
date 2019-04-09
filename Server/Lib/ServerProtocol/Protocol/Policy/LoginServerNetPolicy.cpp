////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
// 
// Author : Generated
// 
// Description : LoginServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFConnection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/LoginServerNetPolicy.h"
#include "Protocol/Message/LoginServerMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Notify Login server that client is successfully connected and joined to game server so that login server clear the player information.
		Result NetPolicyLoginServer::PlayerJoinedToGameServerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::LoginServer::PlayerJoinedToGameServerCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID, InAuthTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyLoginServer::PlayerJoinedToGameServerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket )
		// Cmd: Kick logged in player, used to kick player on other login server to prevent duplicated login.
		Result NetPolicyLoginServer::KickPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InKickedPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::LoginServer::KickPlayerCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InKickedPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyLoginServer::KickPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InKickedPlayerID )


		// Cmd: Notify Login server that client is successfully connected and joined to game server so that login server clear the player information.
		Result NetSvrPolicyLoginServer::PlayerJoinedToGameServerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::LoginServer::PlayerJoinedToGameServerRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyLoginServer::PlayerJoinedToGameServerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// Cmd: Kick logged in player, used to kick player on other login server to prevent duplicated login.
		Result NetSvrPolicyLoginServer::KickPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::LoginServer::KickPlayerRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyLoginServer::KickPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )


	}; // namespace Policy
}; // namespace SF


