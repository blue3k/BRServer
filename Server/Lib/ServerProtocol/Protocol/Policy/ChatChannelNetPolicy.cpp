﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
// 
// Author : Generated
// 
// Description : ChatChannel Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
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
		Result NetPolicyChatChannel::JoinCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::JoinCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPasscode, InJoiningPlayer);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyChatChannel::JoinCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer )
		// Cmd: Leave chat channel
		Result NetPolicyChatChannel::LeaveCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::LeaveCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyChatChannel::LeaveCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Kick a player
		Result NetPolicyChatChannel::KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::KickPlayerCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID, InPlayerToKick);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyChatChannel::KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// C2S: Sending a chatting message
		Result NetPolicyChatChannel::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InChatMessage )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::ChatMessageC2SEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InSenderID, InChatMessage);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyChatChannel::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InChatMessage )


		// Cmd: Request to join chat channel
		Result NetSvrPolicyChatChannel::JoinRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const PlayerID &InChatChannelLeaderID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::JoinRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InChatChannelLeaderID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyChatChannel::JoinRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const PlayerID &InChatChannelLeaderID )
		// S2C: Server envent to notify joined player information
		Result NetSvrPolicyChatChannel::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::PlayerJoinedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InJoinedPlayer);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyChatChannel::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )
		// S2C: Chat channel leader changed
		Result NetSvrPolicyChatChannel::LeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::LeaderChangedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InNewLeaderID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyChatChannel::LeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )
		// Cmd: Leave chat channel
		Result NetSvrPolicyChatChannel::LeaveRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::LeaveRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyChatChannel::LeaveRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// S2C: Notification event when a player left
		Result NetSvrPolicyChatChannel::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::PlayerLeftS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InLeftPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyChatChannel::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
		// Cmd: Kick a player
		Result NetSvrPolicyChatChannel::KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::KickPlayerRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyChatChannel::KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// S2C: Notification event when a player kicked
		Result NetSvrPolicyChatChannel::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::PlayerKickedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InKickedPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyChatChannel::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
		// S2C: brocasting event for a chatting message
		Result NetSvrPolicyChatChannel::ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannel::ChatMessageS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InSenderID, InSenderName, InChatMessage);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyChatChannel::ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )


	}; // namespace Policy
}; // namespace SF

