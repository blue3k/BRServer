////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: Kick
		HRESULT NetPolicyGameServer::RegisterPlayerToJoinGameServerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const UINT32 &InShardID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameServer::RegisterPlayerToJoinGameServerCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID, InTicket, InFBUserID, InShardID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameServer::RegisterPlayerToJoinGameServerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const UINT32 &InShardID )
		// Cmd: Kick
		HRESULT NetPolicyGameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID, InTicket, InFBUserID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )
		// C2S: Chatting message
		HRESULT NetPolicyGameServer::ChatMessageC2SEvt( const RouteContext &InRouteContext, const AccountID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameServer::ChatMessageC2SEvt::BuildIMsg(pMsg, InRouteContext, InSenderID, InRole, InSenderName, InChatMessage));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameServer::ChatMessageC2SEvt( const RouteContext &InRouteContext, const AccountID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )
		// C2S: Notification
		HRESULT NetPolicyGameServer::NotifyC2SEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const UINT64 &InTimeStamp )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameServer::NotifyC2SEvt::BuildIMsg(pMsg, InRouteContext, InDestPlayerID, InNotificationID, InMessageID, InMessageParam0, InMessageParam1, InMessageText, InTimeStamp));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameServer::NotifyC2SEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const UINT64 &InTimeStamp )
		// C2S: Friend Accept
		HRESULT NetPolicyGameServer::FriendAcceptedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameServer::FriendAcceptedC2SEvt::BuildIMsg(pMsg, InRouteContext, InDestPlayerID, InAccepter));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameServer::FriendAcceptedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )
		// C2S: Friend Remove
		HRESULT NetPolicyGameServer::FriendRemovedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameServer::FriendRemovedC2SEvt::BuildIMsg(pMsg, InRouteContext, InDestPlayerID, InRemoverID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameServer::FriendRemovedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )
		// C2S: Request Player Status Update
		HRESULT NetPolicyGameServer::RequestPlayerStatusUpdateC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameServer::RequestPlayerStatusUpdateC2SEvt::BuildIMsg(pMsg, InRouteContext, InDestPlayerID, InRequesterID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameServer::RequestPlayerStatusUpdateC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )
		// C2S: Notify Player Status Updated
		HRESULT NetPolicyGameServer::NotifyPlayerStatusUpdatedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt::BuildIMsg(pMsg, InRouteContext, InDestPlayerID, InLatestActiveTime, InIsInGame));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameServer::NotifyPlayerStatusUpdatedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame )
		// C2S: Notification
		HRESULT NetPolicyGameServer::NotifyPartyInviteC2SEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const EntityUID &InPartyUID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameServer::NotifyPartyInviteC2SEvt::BuildIMsg(pMsg, InRouteContext, InDestPlayerID, InInviterID, InInviterName, InPartyUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameServer::NotifyPartyInviteC2SEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const EntityUID &InPartyUID )


		// Cmd: Kick
		HRESULT NetSvrPolicyGameServer::RegisterPlayerToJoinGameServerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const NetAddress &InPublicAddress, const NetAddress &InPublicAddressIPV4 )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameServer::RegisterPlayerToJoinGameServerRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext, InPublicAddress, InPublicAddressIPV4));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameServer::RegisterPlayerToJoinGameServerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const NetAddress &InPublicAddress, const NetAddress &InPublicAddressIPV4 )
		// Cmd: Kick
		HRESULT NetSvrPolicyGameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )


	}; // namespace Policy
}; // namespace BR


