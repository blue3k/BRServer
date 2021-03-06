﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : GameServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFMessageEndpoint.h"
#include "Actor/Movement/SFActorMovement.h"
#include "Protocol/GameServerNetPolicy.h"
#include "Protocol/GameServerMsgClass.h"



namespace SF
{
 	// Cmd: Register player so that the player can connection and join to the game server.
	Result NetPolicyGameServer::RegisterPlayerToJoinGameServerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameServer::RegisterPlayerToJoinGameServerCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID, InTicket, InFBUserID, InShardID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetPolicyGameServer::RegisterPlayerToJoinGameServerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID, const uint32_t &InShardID )
	// Cmd: Same to RegisterPlayerToJoinGameServer, but can run on player entity. only works when the player entity exists. If player disconnected for some reason, you can run this method to connect.
	Result NetPolicyGameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID, InTicket, InFBUserID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetPolicyGameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const FacebookUID &InFBUserID )
	// C2S: Chatting message event.
	Result NetPolicyGameServer::ChatMessageC2SEvt( const RouteContext &InRouteContext, const AccountID &InSenderID, const uint8_t &InRole, const char* InSenderName, const char* InChatMessage )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameServer::ChatMessageC2SEvt::Create(GetSystemHeap(), InRouteContext, InSenderID, InRole, InSenderName, InChatMessage);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetPolicyGameServer::ChatMessageC2SEvt( const RouteContext &InRouteContext, const AccountID &InSenderID, const uint8_t &InRole, const char* InSenderName, const char* InChatMessage )
	// C2S: Notification event for P2P
	Result NetPolicyGameServer::NotifyC2SEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameServer::NotifyC2SEvt::Create(GetSystemHeap(), InRouteContext, InDestPlayerID, InNotificationID, InMessageID, InMessageParam0, InMessageParam1, InMessageText, InTimeStamp);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetPolicyGameServer::NotifyC2SEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint64_t &InTimeStamp )
	// C2S: Friend Accepted
	Result NetPolicyGameServer::FriendAcceptedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameServer::FriendAcceptedC2SEvt::Create(GetSystemHeap(), InRouteContext, InDestPlayerID, InAccepter);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetPolicyGameServer::FriendAcceptedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const ServerFriendInformation &InAccepter )
	// C2S: Friend Remove
	Result NetPolicyGameServer::FriendRemovedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameServer::FriendRemovedC2SEvt::Create(GetSystemHeap(), InRouteContext, InDestPlayerID, InRemoverID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetPolicyGameServer::FriendRemovedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRemoverID )
	// C2S: Request Player Status Update
	Result NetPolicyGameServer::RequestPlayerStatusUpdateC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameServer::RequestPlayerStatusUpdateC2SEvt::Create(GetSystemHeap(), InRouteContext, InDestPlayerID, InRequesterID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetPolicyGameServer::RequestPlayerStatusUpdateC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const PlayerID &InRequesterID )
	// C2S: Notify Player Status Updated
	Result NetPolicyGameServer::NotifyPlayerStatusUpdatedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameServer::NotifyPlayerStatusUpdatedC2SEvt::Create(GetSystemHeap(), InRouteContext, InDestPlayerID, InLatestActiveTime, InIsInGame);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetPolicyGameServer::NotifyPlayerStatusUpdatedC2SEvt( const RouteContext &InRouteContext, const AccountID &InDestPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame )
	// C2S: Party invite Notification
	Result NetPolicyGameServer::NotifyPartyInviteC2SEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const uint64_t &InPartyUID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameServer::NotifyPartyInviteC2SEvt::Create(GetSystemHeap(), InRouteContext, InDestPlayerID, InInviterID, InInviterName, InPartyUID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetPolicyGameServer::NotifyPartyInviteC2SEvt( const RouteContext &InRouteContext, const PlayerID &InDestPlayerID, const PlayerID &InInviterID, const char* InInviterName, const uint64_t &InPartyUID )


	// Cmd: Register player so that the player can connection and join to the game server.
	Result NetSvrPolicyGameServer::RegisterPlayerToJoinGameServerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const char* InPublicAddress, const char* InPublicAddressV6, const uint32_t &InPort )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameServer::RegisterPlayerToJoinGameServerRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult, InPublicAddress, InPublicAddressV6, InPort);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetSvrPolicyGameServer::RegisterPlayerToJoinGameServerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const char* InPublicAddress, const char* InPublicAddressV6, const uint32_t &InPort )
	// Cmd: Same to RegisterPlayerToJoinGameServer, but can run on player entity. only works when the player entity exists. If player disconnected for some reason, you can run this method to connect.
	Result NetSvrPolicyGameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetSvrPolicyGameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )


}; // namespace SF


