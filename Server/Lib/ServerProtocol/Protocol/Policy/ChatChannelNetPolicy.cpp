////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
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
#include "Net/SFConnection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/ChatChannelNetPolicy.h"
#include "Protocol/Message/ChatChannelMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Request to join chat channel
		Result NetPolicyChatChannel::JoinCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::JoinCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPasscode, InJoiningPlayer);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyChatChannel::JoinCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer )
		// Cmd: Leave chat channel
		Result NetPolicyChatChannel::LeaveCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::LeaveCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyChatChannel::LeaveCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Kick a player
		Result NetPolicyChatChannel::KickPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::KickPlayerCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID, InPlayerToKick);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyChatChannel::KickPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// C2S: Sending a chatting message
		Result NetPolicyChatChannel::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InChatMessage )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::ChatMessageC2SEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InSenderID, InChatMessage);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetPolicyChatChannel::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InChatMessage )


		// Cmd: Request to join chat channel
		Result NetSvrPolicyChatChannel::JoinRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const PlayerID &InChatChannelLeaderID )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::JoinRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InChatChannelLeaderID);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyChatChannel::JoinRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const PlayerID &InChatChannelLeaderID )
		// S2C: Server envent to notify joined player information
		Result NetSvrPolicyChatChannel::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::PlayerJoinedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InJoinedPlayer);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyChatChannel::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )
		// S2C: Chat channel leader changed
		Result NetSvrPolicyChatChannel::LeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::LeaderChangedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InNewLeaderID);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyChatChannel::LeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )
		// Cmd: Leave chat channel
		Result NetSvrPolicyChatChannel::LeaveRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::LeaveRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyChatChannel::LeaveRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// S2C: Notification event when a player left
		Result NetSvrPolicyChatChannel::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::PlayerLeftS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InLeftPlayerID);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyChatChannel::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
		// Cmd: Kick a player
		Result NetSvrPolicyChatChannel::KickPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::KickPlayerRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyChatChannel::KickPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// S2C: Notification event when a player kicked
		Result NetSvrPolicyChatChannel::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::PlayerKickedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InKickedPlayerID);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyChatChannel::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
		// S2C: brocasting event for a chatting message
		Result NetSvrPolicyChatChannel::ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
		{
 			FunctionContext hr;

			 MessageDataPtr pMessage;
			 protocolCheckPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::ChatMessageS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InSenderID, InSenderName, InChatMessage);
			 protocolCheckPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

			return hr;

		}; // Result NetSvrPolicyChatChannel::ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )


	}; // namespace Policy
}; // namespace SF


