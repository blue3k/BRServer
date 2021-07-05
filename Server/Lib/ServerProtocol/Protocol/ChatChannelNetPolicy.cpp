////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : ChatChannel Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFMessageEndpoint.h"
#include "Actor/Movement/SFActorMovement.h"
#include "Protocol/ChatChannelNetPolicy.h"
#include "Protocol/ChatChannelMsgClass.h"



namespace SF
{
 	// Cmd: Request to join chat channel
	Result NetPolicyChatChannel::JoinCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ChatChannel::JoinCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPasscode, InJoiningPlayer);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetPolicyChatChannel::JoinCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer )
	// Cmd: Leave chat channel
	Result NetPolicyChatChannel::LeaveCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ChatChannel::LeaveCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetPolicyChatChannel::LeaveCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	// Cmd: Kick a player
	Result NetPolicyChatChannel::KickPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ChatChannel::KickPlayerCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID, InPlayerToKick);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetPolicyChatChannel::KickPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
	// C2S: Sending a chatting message
	Result NetPolicyChatChannel::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InChatMessage )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ChatChannel::ChatMessageC2SEvt::Create(GetSystemHeap(), InRouteContext, InSenderID, InChatMessage);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetPolicyChatChannel::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InChatMessage )


	// Cmd: Request to join chat channel
	Result NetSvrPolicyChatChannel::JoinRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const PlayerID &InChatChannelLeaderID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ChatChannel::JoinRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult, InChatChannelLeaderID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetSvrPolicyChatChannel::JoinRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const PlayerID &InChatChannelLeaderID )
	// S2C: Server envent to notify joined player information
	Result NetSvrPolicyChatChannel::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ChatChannel::PlayerJoinedS2CEvt::Create(GetSystemHeap(), InRouteContext, InJoinedPlayer);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetSvrPolicyChatChannel::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )
	// S2C: Chat channel leader changed
	Result NetSvrPolicyChatChannel::LeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ChatChannel::LeaderChangedS2CEvt::Create(GetSystemHeap(), InRouteContext, InNewLeaderID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetSvrPolicyChatChannel::LeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )
	// Cmd: Leave chat channel
	Result NetSvrPolicyChatChannel::LeaveRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ChatChannel::LeaveRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetSvrPolicyChatChannel::LeaveRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	// S2C: Notification event when a player left
	Result NetSvrPolicyChatChannel::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ChatChannel::PlayerLeftS2CEvt::Create(GetSystemHeap(), InRouteContext, InLeftPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetSvrPolicyChatChannel::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
	// Cmd: Kick a player
	Result NetSvrPolicyChatChannel::KickPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ChatChannel::KickPlayerRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetSvrPolicyChatChannel::KickPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	// S2C: Notification event when a player kicked
	Result NetSvrPolicyChatChannel::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ChatChannel::PlayerKickedS2CEvt::Create(GetSystemHeap(), InRouteContext, InKickedPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetSvrPolicyChatChannel::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
	// S2C: brocasting event for a chatting message
	Result NetSvrPolicyChatChannel::ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::ChatChannel::ChatMessageS2CEvt::Create(GetSystemHeap(), InRouteContext, InSenderID, InSenderName, InChatMessage);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

	}; // Result NetSvrPolicyChatChannel::ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )


}; // namespace SF


