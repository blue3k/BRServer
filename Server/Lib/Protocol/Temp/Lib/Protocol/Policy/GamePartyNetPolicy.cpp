////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameParty Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/GamePartyIPolicy.h"
#include "Protocol/Policy/GamePartyNetPolicy.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: Join Party
		HRESULT NetPolicyGameParty::JoinPartyCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::JoinPartyCmd::BuildIMsg(pMsg, InContext, InRouteContext, InInviterID, InInvitedPlayer));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameParty::JoinPartyCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
		// Cmd: Kick player
		HRESULT NetPolicyGameParty::LeavePartyCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::LeavePartyCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameParty::LeavePartyCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
		// Cmd: Kick player
		HRESULT NetPolicyGameParty::KickPlayerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::KickPlayerCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID, InPlayerToKick));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameParty::KickPlayerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// C2S: Chatting message
		HRESULT NetPolicyGameParty::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::ChatMessageC2SEvt::BuildIMsg(pMsg, InRouteContext, InPlayerID, InChatMessage));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameParty::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage )
		// C2S: Quick Chatting message
		HRESULT NetPolicyGameParty::QuickChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT32 &InQuickChatID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::QuickChatMessageC2SEvt::BuildIMsg(pMsg, InRouteContext, InPlayerID, InQuickChatID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameParty::QuickChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT32 &InQuickChatID )
		// Cmd: Match
		HRESULT NetPolicyGameParty::StartGameMatchCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT32 &InMaxGamePlayers )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::StartGameMatchCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID, InMaxGamePlayers));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameParty::StartGameMatchCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT32 &InMaxGamePlayers )
		// Cmd: Match
		HRESULT NetPolicyGameParty::CancelGameMatchCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::CancelGameMatchCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameParty::CancelGameMatchCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )


		// Cmd: Join Party
		HRESULT NetSvrPolicyGameParty::JoinPartyRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::JoinPartyRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext, InPartyLeaderID, InChatHistoryData));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameParty::JoinPartyRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData )
		// S2C: Player Joined
		HRESULT NetSvrPolicyGameParty::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::PlayerJoinedS2CEvt::BuildIMsg(pMsg, InRouteContext, InJoinedPlayer));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameParty::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )
		// S2C: Player Joined
		HRESULT NetSvrPolicyGameParty::PartyLeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::PartyLeaderChangedS2CEvt::BuildIMsg(pMsg, InRouteContext, InNewLeaderID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameParty::PartyLeaderChangedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )
		// Cmd: Kick player
		HRESULT NetSvrPolicyGameParty::LeavePartyRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::LeavePartyRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameParty::LeavePartyRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		// S2C: Player left
		HRESULT NetSvrPolicyGameParty::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::PlayerLeftS2CEvt::BuildIMsg(pMsg, InRouteContext, InLeftPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameParty::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
		// Cmd: Kick player
		HRESULT NetSvrPolicyGameParty::KickPlayerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::KickPlayerRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameParty::KickPlayerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		// S2C: Player kicked
		HRESULT NetSvrPolicyGameParty::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::PlayerKickedS2CEvt::BuildIMsg(pMsg, InRouteContext, InKickedPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameParty::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
		// S2C: Chatting message
		HRESULT NetSvrPolicyGameParty::ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::ChatMessageS2CEvt::BuildIMsg(pMsg, InRouteContext, InSenderID, InSenderName, InChatMessage));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameParty::ChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
		// S2C: Quick Chatting message
		HRESULT NetSvrPolicyGameParty::QuickChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const UINT32 &InQuickChatID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::QuickChatMessageS2CEvt::BuildIMsg(pMsg, InRouteContext, InSenderID, InQuickChatID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameParty::QuickChatMessageS2CEvt( const RouteContext &InRouteContext, const PlayerID &InSenderID, const UINT32 &InQuickChatID )
		// Cmd: Match
		HRESULT NetSvrPolicyGameParty::StartGameMatchRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::StartGameMatchRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameParty::StartGameMatchRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		// S2C: Enqueued at a game matching
		HRESULT NetSvrPolicyGameParty::QueuedGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::QueuedGameMatchingS2CEvt::BuildIMsg(pMsg, InRouteContext, InMatchingQueueTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameParty::QueuedGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
		// Cmd: Match
		HRESULT NetSvrPolicyGameParty::CancelGameMatchRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::CancelGameMatchRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameParty::CancelGameMatchRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		// S2C: Canceled at a game matching
		HRESULT NetSvrPolicyGameParty::CanceledGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::CanceledGameMatchingS2CEvt::BuildIMsg(pMsg, InRouteContext, InMatchingQueueTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameParty::CanceledGameMatchingS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
		// S2C: Enqueued item is dequeued
		HRESULT NetSvrPolicyGameParty::MatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameParty::MatchingItemDequeuedS2CEvt::BuildIMsg(pMsg, InRouteContext, InMatchingTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameParty::MatchingItemDequeuedS2CEvt( const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket )


	}; // namespace Policy
}; // namespace BR


