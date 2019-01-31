﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
// 
// Author : Generated
// 
// Description : GameInstance Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFConnection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// C2S: Nitify that a game instance is deleted. Game instance send this message to manager before it destroy itself.
		Result NetPolicyGameInstance::DeleteGameC2SEvt( const RouteContext &InRouteContext )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::DeleteGameC2SEvt::Create(m_pConnection->GetIOHeap(), InRouteContext);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameInstance::DeleteGameC2SEvt( const RouteContext &InRouteContext )
		// Cmd: Join to a game instance. You can call multiple times, but it would be waste
		Result NetPolicyGameInstance::JoinGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerInformation &InPlayer, const uint8_t &InRequestedRole )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::JoinGameCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayer, InRequestedRole);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameInstance::JoinGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerInformation &InPlayer, const uint8_t &InRequestedRole )
		// C2S: For debug purpose, change configue preset. There is a game setting table. you can switch between those setting value.
		Result NetPolicyGameInstance::SetConfigPresetC2SEvt( const RouteContext &InRouteContext, const uint32_t &InPresetID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::SetConfigPresetC2SEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InPresetID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameInstance::SetConfigPresetC2SEvt( const RouteContext &InRouteContext, const uint32_t &InPresetID )
		// Cmd: Leave game instance.
		Result NetPolicyGameInstance::LeaveGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::LeaveGameCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameInstance::LeaveGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Kick player with given ID
		Result NetPolicyGameInstance::KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::KickPlayerCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID, InPlayerToKick);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameInstance::KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// Cmd: Assign new roles to all players.
		Result NetPolicyGameInstance::AssignRoleCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::AssignRoleCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameInstance::AssignRoleCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// C2S: in-game chatting message.
		Result NetPolicyGameInstance::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::ChatMessageC2SEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InPlayerID, InRole, InChatMessage);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameInstance::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage )
		// Cmd: Advance game
		Result NetPolicyGameInstance::AdvanceGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::AdvanceGameCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameInstance::AdvanceGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: *Vote game advance
		Result NetPolicyGameInstance::VoteGameAdvanceCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::VoteGameAdvanceCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameInstance::VoteGameAdvanceCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Assign role
		Result NetPolicyGameInstance::VoteCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::VoteCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID, InVoteTarget, InActionSerial);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameInstance::VoteCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial )
		// Cmd: Play again with the current players
		Result NetPolicyGameInstance::GamePlayAgainCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::GamePlayAgainCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InLeadPlayer, InPartyUID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameInstance::GamePlayAgainCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID )
		// Cmd: Player. revive himself
		Result NetPolicyGameInstance::GameRevealPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::GameRevealPlayerCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID, InTargetPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameInstance::GameRevealPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
		// Cmd: Player. revive himself
		Result NetPolicyGameInstance::GamePlayerReviveCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::GamePlayerReviveCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyGameInstance::GamePlayerReviveCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )


		// Cmd: Join to a game instance. You can call multiple times, but it would be waste
		Result NetSvrPolicyGameInstance::JoinGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const NetAddress &InGameInsSvr, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay, const uint8_t &InMaxPlayer, const uint8_t &InPlayerIndex, const uint8_t &InPlayerCharacter, const uint8_t &InRole, const uint8_t &InDead, const uint8_t &InIsNewJoin, const Array<uint8_t>& InChatHistoryData, const Array<uint8_t>& InGameLogData )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::JoinGameRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InGameInsSvr, InTimeStamp, InGameState, InDay, InMaxPlayer, InPlayerIndex, InPlayerCharacter, InRole, InDead, InIsNewJoin, InChatHistoryData, InGameLogData);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::JoinGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const NetAddress &InGameInsSvr, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay, const uint8_t &InMaxPlayer, const uint8_t &InPlayerIndex, const uint8_t &InPlayerCharacter, const uint8_t &InRole, const uint8_t &InDead, const uint8_t &InIsNewJoin, const Array<uint8_t>& InChatHistoryData, const Array<uint8_t>& InGameLogData )
		// S2C: Player joined event. This event is brocasted when a player joined
		Result NetSvrPolicyGameInstance::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer, const uint8_t &InJoinedPlayerRole, const uint8_t &InJoinedPlayerDead, const uint8_t &InJoinedPlayerIndex, const uint8_t &InJoinedPlayerCharacter )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::PlayerJoinedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InJoinedPlayer, InJoinedPlayerRole, InJoinedPlayerDead, InJoinedPlayerIndex, InJoinedPlayerCharacter);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer, const uint8_t &InJoinedPlayerRole, const uint8_t &InJoinedPlayerDead, const uint8_t &InJoinedPlayerIndex, const uint8_t &InJoinedPlayerCharacter )
		// Cmd: Leave game instance.
		Result NetSvrPolicyGameInstance::LeaveGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::LeaveGameRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::LeaveGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// S2C: Player left event.
		Result NetSvrPolicyGameInstance::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::PlayerLeftS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InLeftPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
		// Cmd: Kick player with given ID
		Result NetSvrPolicyGameInstance::KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::KickPlayerRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// S2C: Player kicked event. this event will be brocasted when a player kicked.
		Result NetSvrPolicyGameInstance::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::PlayerKickedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InKickedPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
		// Cmd: Assign new roles to all players.
		Result NetSvrPolicyGameInstance::AssignRoleRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::AssignRoleRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::AssignRoleRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// S2C: Event for role assigned
		Result NetSvrPolicyGameInstance::RoleAssignedS2CEvt( const RouteContext &InRouteContext, const uint8_t &InRole )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::RoleAssignedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InRole);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::RoleAssignedS2CEvt( const RouteContext &InRouteContext, const uint8_t &InRole )
		// Cmd: Advance game
		Result NetSvrPolicyGameInstance::AdvanceGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::AdvanceGameRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::AdvanceGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// S2C: The game state is advanced
		Result NetSvrPolicyGameInstance::GameAdvancedS2CEvt( const RouteContext &InRouteContext, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::GameAdvancedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InTimeStamp, InGameState, InDay);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::GameAdvancedS2CEvt( const RouteContext &InRouteContext, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay )
		// S2C: Game is ended
		Result NetSvrPolicyGameInstance::GameEndedS2CEvt( const RouteContext &InRouteContext, const uint8_t &InWinner, const uint32_t &InGainedExp, const uint32_t &InGainedGameMoney, const uint8_t &InPlayedRole, const uint8_t &InIsWon )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::GameEndedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InWinner, InGainedExp, InGainedGameMoney, InPlayedRole, InIsWon);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::GameEndedS2CEvt( const RouteContext &InRouteContext, const uint8_t &InWinner, const uint32_t &InGainedExp, const uint32_t &InGainedGameMoney, const uint8_t &InPlayedRole, const uint8_t &InIsWon )
		// Cmd: *Vote game advance
		Result NetSvrPolicyGameInstance::VoteGameAdvanceRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::VoteGameAdvanceRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::VoteGameAdvanceRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// S2C: *GameAdvance is Voted
		Result NetSvrPolicyGameInstance::GameAdvanceVotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::GameAdvanceVotedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InVoter);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::GameAdvanceVotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter )
		// Cmd: Assign role
		Result NetSvrPolicyGameInstance::VoteRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::VoteRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::VoteRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// S2C: Player Voted
		Result NetSvrPolicyGameInstance::VotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::VotedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InVoter, InVotedTarget);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::VotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget )
		// S2C: Player Voted
		Result NetSvrPolicyGameInstance::VoteEndS2CEvt( const RouteContext &InRouteContext, const Array<PlayerID>& InVoted )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::VoteEndS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InVoted);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::VoteEndS2CEvt( const RouteContext &InRouteContext, const Array<PlayerID>& InVoted )
		// S2C: Player Voted
		Result NetSvrPolicyGameInstance::PlayerRevealedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const uint8_t &InRole, const uint8_t &InReason )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::PlayerRevealedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InRevealedPlayerID, InRole, InReason);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::PlayerRevealedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const uint8_t &InRole, const uint8_t &InReason )
		// S2C: Player Killed
		Result NetSvrPolicyGameInstance::PlayerKilledS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const uint8_t &InReason )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::PlayerKilledS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InKilledPlayer, InReason);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::PlayerKilledS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const uint8_t &InReason )
		// Cmd: Play again with the current players
		Result NetSvrPolicyGameInstance::GamePlayAgainRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InReplayMemberCount )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::GamePlayAgainRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InReplayMemberCount);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::GamePlayAgainRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InReplayMemberCount )
		// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
		Result NetSvrPolicyGameInstance::GamePlayAgainS2CEvt( const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const uint64_t &InPartyUID, const PlayerID &InLeadPlayer )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::GamePlayAgainS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InTargetPlayer, InPartyUID, InLeadPlayer);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::GamePlayAgainS2CEvt( const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const uint64_t &InPartyUID, const PlayerID &InLeadPlayer )
		// Cmd: Player. revive himself
		Result NetSvrPolicyGameInstance::GameRevealPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<uint8_t>& InRevealedRole )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::GameRevealPlayerRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InRevealedPlayerID, InRevealedRole);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::GameRevealPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<uint8_t>& InRevealedRole )
		// Cmd: Player. revive himself
		Result NetSvrPolicyGameInstance::GamePlayerReviveRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::GamePlayerReviveRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::GamePlayerReviveRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
		// S2C: Player is revived
		Result NetSvrPolicyGameInstance::GamePlayerRevivedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::GameInstance::GamePlayerRevivedS2CEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InRevivedPlayerID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGameInstance::GamePlayerRevivedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID )


	}; // namespace Policy
}; // namespace SF


