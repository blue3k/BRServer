////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
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
#include "Net/SFMessageEndpoint.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	// Cmd: Join party
	Result NetPolicyGameParty::JoinPartyCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::JoinPartyCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InInviterID, InInvitedPlayer);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameParty::JoinPartyCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
	// Cmd: Event for player left.
	Result NetPolicyGameParty::LeavePartyCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::LeavePartyCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameParty::LeavePartyCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	// Cmd: Kick player
	Result NetPolicyGameParty::KickPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::KickPlayerCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID, InPlayerToKick);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameParty::KickPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
	// C2S: Send chat message to server.
	Result NetPolicyGameParty::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::ChatMessageC2SEvt::Create(GetSystemHeap(), InRouteContext, InPlayerID, InChatMessage);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameParty::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage )
	// C2S: Quick Chatting message
	Result NetPolicyGameParty::QuickChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint32_t &InQuickChatID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::QuickChatMessageC2SEvt::Create(GetSystemHeap(), InRouteContext, InPlayerID, InQuickChatID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameParty::QuickChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint32_t &InQuickChatID )
	// Cmd: Start party game matching
	Result NetPolicyGameParty::StartGameMatchCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::StartGameMatchCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID, InMaxGamePlayers);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameParty::StartGameMatchCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers )
	// Cmd: Cancel game matching
	Result NetPolicyGameParty::CancelGameMatchCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::CancelGameMatchCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameParty::CancelGameMatchCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )


	// Cmd: Join party
	Result NetSvrPolicyGameParty::JoinPartyRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const PlayerID &InPartyLeaderID, const Array<uint8_t>& InChatHistoryData )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::JoinPartyRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult, InPartyLeaderID, InChatHistoryData);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameParty::JoinPartyRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const PlayerID &InPartyLeaderID, const Array<uint8_t>& InChatHistoryData )
	// S2C: Player joined event.
	Result NetSvrPolicyGameParty::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::PlayerJoinedS2CEvt::Create(GetSystemHeap(), InRouteContext, InJoinedPlayer);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameParty::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )
	// S2C: Event for party leader is changed
	Result NetSvrPolicyGameParty::PartyLeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::PartyLeaderChangedS2CEvt::Create(GetSystemHeap(), InRouteContext, InNewLeaderID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameParty::PartyLeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )
	// Cmd: Event for player left.
	Result NetSvrPolicyGameParty::LeavePartyRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::LeavePartyRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameParty::LeavePartyRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	// S2C: Player left
	Result NetSvrPolicyGameParty::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::PlayerLeftS2CEvt::Create(GetSystemHeap(), InRouteContext, InLeftPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameParty::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
	// Cmd: Kick player
	Result NetSvrPolicyGameParty::KickPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::KickPlayerRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameParty::KickPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	// S2C: Event for Player kicked.
	Result NetSvrPolicyGameParty::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::PlayerKickedS2CEvt::Create(GetSystemHeap(), InRouteContext, InKickedPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameParty::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
	// S2C: Brocast chatting message
	Result NetSvrPolicyGameParty::ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::ChatMessageS2CEvt::Create(GetSystemHeap(), InRouteContext, InSenderID, InSenderName, InChatMessage);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameParty::ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
	// S2C: Quick Chatting message
	Result NetSvrPolicyGameParty::QuickChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const uint32_t &InQuickChatID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::QuickChatMessageS2CEvt::Create(GetSystemHeap(), InRouteContext, InSenderID, InQuickChatID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameParty::QuickChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const uint32_t &InQuickChatID )
	// Cmd: Start party game matching
	Result NetSvrPolicyGameParty::StartGameMatchRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::StartGameMatchRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameParty::StartGameMatchRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	// S2C: Event sent when the party is queued for game matching
	Result NetSvrPolicyGameParty::QueuedGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::QueuedGameMatchingS2CEvt::Create(GetSystemHeap(), InRouteContext, InMatchingQueueTicket);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameParty::QueuedGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
	// Cmd: Cancel game matching
	Result NetSvrPolicyGameParty::CancelGameMatchRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::CancelGameMatchRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameParty::CancelGameMatchRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	// S2C: Event sent when the game matching is canceled
	Result NetSvrPolicyGameParty::CanceledGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::CanceledGameMatchingS2CEvt::Create(GetSystemHeap(), InRouteContext, InMatchingQueueTicket);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameParty::CanceledGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
	// S2C: Event sent when the the matching has dequeued.
	Result NetSvrPolicyGameParty::MatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameParty::MatchingItemDequeuedS2CEvt::Create(GetSystemHeap(), InRouteContext, InMatchingTicket);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameParty::MatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket )


}; // namespace SF


