////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
// 
// Author : Generated
// 
// Description : GameParty Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFConnection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Join party
		Result NetPolicyGameParty::JoinPartyCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::JoinPartyCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InInviterID, InInvitedPlayer);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameParty::JoinPartyCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
		// Cmd: Event for player left.
		Result NetPolicyGameParty::LeavePartyCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::LeavePartyCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameParty::LeavePartyCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Kick player
		Result NetPolicyGameParty::KickPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::KickPlayerCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID, InPlayerToKick);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameParty::KickPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// C2S: Send chat message to server.
		Result NetPolicyGameParty::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::ChatMessageC2SEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InPlayerID, InChatMessage);
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

			 pMessage = SF::Message::GameParty::QuickChatMessageC2SEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InPlayerID, InQuickChatID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameParty::QuickChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint32_t &InQuickChatID )
		// Cmd: Start party game matching
		Result NetPolicyGameParty::StartGameMatchCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::StartGameMatchCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID, InMaxGamePlayers);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameParty::StartGameMatchCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers )
		// Cmd: Cancel game matching
		Result NetPolicyGameParty::CancelGameMatchCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::CancelGameMatchCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameParty::CancelGameMatchCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )


		// Cmd: Join party
		Result NetSvrPolicyGameParty::JoinPartyRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const PlayerID &InPartyLeaderID, const Array<uint8_t>& InChatHistoryData )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::JoinPartyRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InPartyLeaderID, InChatHistoryData);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::JoinPartyRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const PlayerID &InPartyLeaderID, const Array<uint8_t>& InChatHistoryData )
		// S2C: Player joined event.
		Result NetSvrPolicyGameParty::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::PlayerJoinedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InJoinedPlayer);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )
		// S2C: Event for party leader is changed
		Result NetSvrPolicyGameParty::PartyLeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::PartyLeaderChangedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InNewLeaderID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::PartyLeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )
		// Cmd: Event for player left.
		Result NetSvrPolicyGameParty::LeavePartyRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::LeavePartyRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::LeavePartyRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// S2C: Player left
		Result NetSvrPolicyGameParty::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::PlayerLeftS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InLeftPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
		// Cmd: Kick player
		Result NetSvrPolicyGameParty::KickPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::KickPlayerRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::KickPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// S2C: Event for Player kicked.
		Result NetSvrPolicyGameParty::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::PlayerKickedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InKickedPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
		// S2C: Brocast chatting message
		Result NetSvrPolicyGameParty::ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::ChatMessageS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InSenderID, InSenderName, InChatMessage);
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

			 pMessage = SF::Message::GameParty::QuickChatMessageS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InSenderID, InQuickChatID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::QuickChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const uint32_t &InQuickChatID )
		// Cmd: Start party game matching
		Result NetSvrPolicyGameParty::StartGameMatchRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::StartGameMatchRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::StartGameMatchRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// S2C: Event sent when the party is queued for game matching
		Result NetSvrPolicyGameParty::QueuedGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::QueuedGameMatchingS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InMatchingQueueTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::QueuedGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
		// Cmd: Cancel game matching
		Result NetSvrPolicyGameParty::CancelGameMatchRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::CancelGameMatchRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::CancelGameMatchRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
		// S2C: Event sent when the game matching is canceled
		Result NetSvrPolicyGameParty::CanceledGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::CanceledGameMatchingS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InMatchingQueueTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::CanceledGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
		// S2C: Event sent when the the matching has dequeued.
		Result NetSvrPolicyGameParty::MatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameParty::MatchingItemDequeuedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InMatchingTicket);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameParty::MatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket )


	}; // namespace Policy
}; // namespace SF


