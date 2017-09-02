////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
// 
// Author : Generated
// 
// Description : GameParty Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Net/NetDef.h"
#include "Net/Message.h"
#include "Net/Connection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Join Party
		Result NetPolicyGameParty::JoinPartyCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::JoinPartyCmd::Create(m_pConnection->GetMemoryManager(), InRouteContext, InTransactionID, InInviterID, InInvitedPlayer);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameParty::JoinPartyCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
		// Cmd: Kick player
		Result NetPolicyGameParty::LeavePartyCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::LeavePartyCmd::Create(m_pConnection->GetMemoryManager(), InRouteContext, InTransactionID, InPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameParty::LeavePartyCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Kick player
		Result NetPolicyGameParty::KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::KickPlayerCmd::Create(m_pConnection->GetMemoryManager(), InRouteContext, InTransactionID, InPlayerID, InPlayerToKick);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameParty::KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// C2S: Chatting message
		Result NetPolicyGameParty::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::ChatMessageC2SEvt::Create(m_pConnection->GetMemoryManager(), InRouteContext, InPlayerID, InChatMessage);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameParty::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage )
		// C2S: Quick Chatting message
		Result NetPolicyGameParty::QuickChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint32_t &InQuickChatID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::QuickChatMessageC2SEvt::Create(m_pConnection->GetMemoryManager(), InRouteContext, InPlayerID, InQuickChatID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameParty::QuickChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint32_t &InQuickChatID )
		// Cmd: Match
		Result NetPolicyGameParty::StartGameMatchCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::StartGameMatchCmd::Create(m_pConnection->GetMemoryManager(), InRouteContext, InTransactionID, InPlayerID, InMaxGamePlayers);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameParty::StartGameMatchCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers )
		// Cmd: Match
		Result NetPolicyGameParty::CancelGameMatchCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::CancelGameMatchCmd::Create(m_pConnection->GetMemoryManager(), InRouteContext, InTransactionID, InPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameParty::CancelGameMatchCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )


		// Cmd: Join Party
		Result NetSvrPolicyGameParty::JoinPartyRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const PlayerID &InPartyLeaderID, const Array<uint8_t>& InChatHistoryData )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::JoinPartyRes::Create(m_pConnection->GetMemoryManager(), InRouteContext, InTransactionID, InResult, InPartyLeaderID, InChatHistoryData);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::JoinPartyRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const PlayerID &InPartyLeaderID, const Array<uint8_t>& InChatHistoryData )
		// S2C: Player Joined
		Result NetSvrPolicyGameParty::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::PlayerJoinedS2CEvt::Create(m_pConnection->GetMemoryManager(), InRouteContext, InJoinedPlayer);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )
		// S2C: Player Joined
		Result NetSvrPolicyGameParty::PartyLeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::PartyLeaderChangedS2CEvt::Create(m_pConnection->GetMemoryManager(), InRouteContext, InNewLeaderID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::PartyLeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )
		// Cmd: Kick player
		Result NetSvrPolicyGameParty::LeavePartyRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::LeavePartyRes::Create(m_pConnection->GetMemoryManager(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::LeavePartyRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// S2C: Player left
		Result NetSvrPolicyGameParty::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::PlayerLeftS2CEvt::Create(m_pConnection->GetMemoryManager(), InRouteContext, InLeftPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
		// Cmd: Kick player
		Result NetSvrPolicyGameParty::KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::KickPlayerRes::Create(m_pConnection->GetMemoryManager(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// S2C: Player kicked
		Result NetSvrPolicyGameParty::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::PlayerKickedS2CEvt::Create(m_pConnection->GetMemoryManager(), InRouteContext, InKickedPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
		// S2C: Chatting message
		Result NetSvrPolicyGameParty::ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::ChatMessageS2CEvt::Create(m_pConnection->GetMemoryManager(), InRouteContext, InSenderID, InSenderName, InChatMessage);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
		// S2C: Quick Chatting message
		Result NetSvrPolicyGameParty::QuickChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const uint32_t &InQuickChatID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::QuickChatMessageS2CEvt::Create(m_pConnection->GetMemoryManager(), InRouteContext, InSenderID, InQuickChatID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::QuickChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const uint32_t &InQuickChatID )
		// Cmd: Match
		Result NetSvrPolicyGameParty::StartGameMatchRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::StartGameMatchRes::Create(m_pConnection->GetMemoryManager(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::StartGameMatchRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// S2C: Enqueued at a game matching
		Result NetSvrPolicyGameParty::QueuedGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::QueuedGameMatchingS2CEvt::Create(m_pConnection->GetMemoryManager(), InRouteContext, InMatchingQueueTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::QueuedGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
		// Cmd: Match
		Result NetSvrPolicyGameParty::CancelGameMatchRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::CancelGameMatchRes::Create(m_pConnection->GetMemoryManager(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::CancelGameMatchRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// S2C: Canceled at a game matching
		Result NetSvrPolicyGameParty::CanceledGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::CanceledGameMatchingS2CEvt::Create(m_pConnection->GetMemoryManager(), InRouteContext, InMatchingQueueTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::CanceledGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
		// S2C: Enqueued item is dequeued
		Result NetSvrPolicyGameParty::MatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::MatchingItemDequeuedS2CEvt::Create(m_pConnection->GetMemoryManager(), InRouteContext, InMatchingTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::MatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket )


	}; // namespace Policy
}; // namespace SF


