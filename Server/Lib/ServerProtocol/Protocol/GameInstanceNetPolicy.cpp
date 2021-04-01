////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : GameInstance Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFMessageEndpoint.h"
#include "Actor/Movement/SFActorMovement.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/GameInstanceNetPolicy.h"
#include "Protocol/GameInstanceMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	// Cmd: Join to a game instance. You can call multiple times, but it would be a waste
	Result NetPolicyGameInstance::JoinGameInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const VariableTable &InCharacterVisual, const VariableTable &InCharacterAttribute )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::JoinGameInstanceCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayer, InCharacterVisual, InCharacterAttribute);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameInstance::JoinGameInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const VariableTable &InCharacterVisual, const VariableTable &InCharacterAttribute )
	// Cmd: Leave game instance.
	Result NetPolicyGameInstance::LeaveGameInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::LeaveGameInstanceCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameInstance::LeaveGameInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	// C2S: Player Movement
	Result NetPolicyGameInstance::PlayerMovementC2SEvt( const RouteContext &InRouteContext, const uint64_t &InGameInsUID, const PlayerID &InPlayerID, const VariableTable &InAttributes )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::PlayerMovementC2SEvt::Create(GetSystemHeap(), InRouteContext, InGameInsUID, InPlayerID, InAttributes);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameInstance::PlayerMovementC2SEvt( const RouteContext &InRouteContext, const uint64_t &InGameInsUID, const PlayerID &InPlayerID, const VariableTable &InAttributes )
	// Cmd: Kick player with given ID
	Result NetPolicyGameInstance::KickPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::KickPlayerCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID, InPlayerToKick);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameInstance::KickPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
	// Cmd: Join to a game instance. You can call multiple times, but it would be waste
	Result NetPolicyGameInstance::JoinGameCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const uint8_t &InRequestedRole )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::JoinGameCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayer, InRequestedRole);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameInstance::JoinGameCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const uint8_t &InRequestedRole )
	// Cmd: Assign new roles to all players.
	Result NetPolicyGameInstance::AssignRoleCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::AssignRoleCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameInstance::AssignRoleCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	// C2S: in-game chatting message.
	Result NetPolicyGameInstance::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::ChatMessageC2SEvt::Create(GetSystemHeap(), InRouteContext, InPlayerID, InRole, InChatMessage);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameInstance::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage )
	// Cmd: Advance game
	Result NetPolicyGameInstance::AdvanceGameCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::AdvanceGameCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameInstance::AdvanceGameCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	// Cmd: *Vote game advance
	Result NetPolicyGameInstance::VoteGameAdvanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::VoteGameAdvanceCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameInstance::VoteGameAdvanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	// Cmd: Assign role
	Result NetPolicyGameInstance::VoteCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::VoteCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID, InVoteTarget, InActionSerial);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameInstance::VoteCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial )
	// Cmd: Play again with the current players
	Result NetPolicyGameInstance::GamePlayAgainCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::GamePlayAgainCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InLeadPlayer, InPartyUID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameInstance::GamePlayAgainCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID )
	// Cmd: Player. revive himself
	Result NetPolicyGameInstance::GameRevealPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::GameRevealPlayerCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID, InTargetPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameInstance::GameRevealPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
	// Cmd: Player. revive himself
	Result NetPolicyGameInstance::GamePlayerReviveCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::GamePlayerReviveCmd::Create(GetSystemHeap(), InRouteContext, InTransactionID, InPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetPolicyGameInstance::GamePlayerReviveCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )


	// Cmd: Join to a game instance. You can call multiple times, but it would be a waste
	Result NetSvrPolicyGameInstance::JoinGameInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const NetAddress &InGameInsSvr4, const NetAddress &InGameInsSvr6, const uint32_t &InTimeStamp )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::JoinGameInstanceRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult, InGameInsSvr4, InGameInsSvr6, InTimeStamp);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::JoinGameInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const NetAddress &InGameInsSvr4, const NetAddress &InGameInsSvr6, const uint32_t &InTimeStamp )
	// Cmd: Leave game instance.
	Result NetSvrPolicyGameInstance::LeaveGameInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::LeaveGameInstanceRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::LeaveGameInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	// S2C: Player left event.
	Result NetSvrPolicyGameInstance::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::PlayerLeftS2CEvt::Create(GetSystemHeap(), InRouteContext, InLeftPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
	// S2C: Player Movement
	Result NetSvrPolicyGameInstance::PlayerMovementS2CEvt( const RouteContext &InRouteContext, const uint64_t &InGameInsUID, const PlayerID &InPlayerID, const VariableTable &InAttributes )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::PlayerMovementS2CEvt::Create(GetSystemHeap(), InRouteContext, InGameInsUID, InPlayerID, InAttributes);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::PlayerMovementS2CEvt( const RouteContext &InRouteContext, const uint64_t &InGameInsUID, const PlayerID &InPlayerID, const VariableTable &InAttributes )
	// Cmd: Kick player with given ID
	Result NetSvrPolicyGameInstance::KickPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::KickPlayerRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::KickPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	// Cmd: Join to a game instance. You can call multiple times, but it would be waste
	Result NetSvrPolicyGameInstance::JoinGameRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const NetAddress &InGameInsSvr, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay, const uint8_t &InMaxPlayer, const uint8_t &InPlayerIndex, const uint8_t &InPlayerCharacter, const uint8_t &InRole, const uint8_t &InDead, const uint8_t &InIsNewJoin, const Array<uint8_t>& InChatHistoryData, const Array<uint8_t>& InGameLogData )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::JoinGameRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult, InGameInsSvr, InTimeStamp, InGameState, InDay, InMaxPlayer, InPlayerIndex, InPlayerCharacter, InRole, InDead, InIsNewJoin, InChatHistoryData, InGameLogData);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::JoinGameRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const NetAddress &InGameInsSvr, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay, const uint8_t &InMaxPlayer, const uint8_t &InPlayerIndex, const uint8_t &InPlayerCharacter, const uint8_t &InRole, const uint8_t &InDead, const uint8_t &InIsNewJoin, const Array<uint8_t>& InChatHistoryData, const Array<uint8_t>& InGameLogData )
	// Cmd: Assign new roles to all players.
	Result NetSvrPolicyGameInstance::AssignRoleRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::AssignRoleRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::AssignRoleRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	// S2C: Event for role assigned
	Result NetSvrPolicyGameInstance::RoleAssignedS2CEvt( const RouteContext &InRouteContext, const uint8_t &InRole )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::RoleAssignedS2CEvt::Create(GetSystemHeap(), InRouteContext, InRole);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::RoleAssignedS2CEvt( const RouteContext &InRouteContext, const uint8_t &InRole )
	// Cmd: Advance game
	Result NetSvrPolicyGameInstance::AdvanceGameRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::AdvanceGameRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::AdvanceGameRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	// S2C: The game state is advanced
	Result NetSvrPolicyGameInstance::GameAdvancedS2CEvt( const RouteContext &InRouteContext, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::GameAdvancedS2CEvt::Create(GetSystemHeap(), InRouteContext, InTimeStamp, InGameState, InDay);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::GameAdvancedS2CEvt( const RouteContext &InRouteContext, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay )
	// S2C: Game is ended
	Result NetSvrPolicyGameInstance::GameEndedS2CEvt( const RouteContext &InRouteContext, const uint8_t &InWinner, const uint32_t &InGainedExp, const uint32_t &InGainedGameMoney, const uint8_t &InPlayedRole, const uint8_t &InIsWon )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::GameEndedS2CEvt::Create(GetSystemHeap(), InRouteContext, InWinner, InGainedExp, InGainedGameMoney, InPlayedRole, InIsWon);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::GameEndedS2CEvt( const RouteContext &InRouteContext, const uint8_t &InWinner, const uint32_t &InGainedExp, const uint32_t &InGainedGameMoney, const uint8_t &InPlayedRole, const uint8_t &InIsWon )
	// Cmd: *Vote game advance
	Result NetSvrPolicyGameInstance::VoteGameAdvanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::VoteGameAdvanceRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::VoteGameAdvanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	// S2C: *GameAdvance is Voted
	Result NetSvrPolicyGameInstance::GameAdvanceVotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::GameAdvanceVotedS2CEvt::Create(GetSystemHeap(), InRouteContext, InVoter);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::GameAdvanceVotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter )
	// Cmd: Assign role
	Result NetSvrPolicyGameInstance::VoteRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::VoteRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::VoteRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	// S2C: Player Voted
	Result NetSvrPolicyGameInstance::VotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::VotedS2CEvt::Create(GetSystemHeap(), InRouteContext, InVoter, InVotedTarget);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::VotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget )
	// S2C: Player Voted
	Result NetSvrPolicyGameInstance::VoteEndS2CEvt( const RouteContext &InRouteContext, const Array<PlayerID>& InVoted )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::VoteEndS2CEvt::Create(GetSystemHeap(), InRouteContext, InVoted);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::VoteEndS2CEvt( const RouteContext &InRouteContext, const Array<PlayerID>& InVoted )
	// S2C: Player Voted
	Result NetSvrPolicyGameInstance::PlayerRevealedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const uint8_t &InRole, const uint8_t &InReason )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::PlayerRevealedS2CEvt::Create(GetSystemHeap(), InRouteContext, InRevealedPlayerID, InRole, InReason);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::PlayerRevealedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const uint8_t &InRole, const uint8_t &InReason )
	// S2C: Player Killed
	Result NetSvrPolicyGameInstance::PlayerKilledS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const uint8_t &InReason )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::PlayerKilledS2CEvt::Create(GetSystemHeap(), InRouteContext, InKilledPlayer, InReason);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::PlayerKilledS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const uint8_t &InReason )
	// Cmd: Play again with the current players
	Result NetSvrPolicyGameInstance::GamePlayAgainRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InReplayMemberCount )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::GamePlayAgainRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult, InReplayMemberCount);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::GamePlayAgainRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InReplayMemberCount )
	// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
	Result NetSvrPolicyGameInstance::GamePlayAgainS2CEvt( const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const uint64_t &InPartyUID, const PlayerID &InLeadPlayer )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::GamePlayAgainS2CEvt::Create(GetSystemHeap(), InRouteContext, InTargetPlayer, InPartyUID, InLeadPlayer);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::GamePlayAgainS2CEvt( const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const uint64_t &InPartyUID, const PlayerID &InLeadPlayer )
	// Cmd: Player. revive himself
	Result NetSvrPolicyGameInstance::GameRevealPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<uint8_t>& InRevealedRole )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::GameRevealPlayerRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult, InRevealedPlayerID, InRevealedRole);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::GameRevealPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<uint8_t>& InRevealedRole )
	// Cmd: Player. revive himself
	Result NetSvrPolicyGameInstance::GamePlayerReviveRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::GamePlayerReviveRes::Create(GetSystemHeap(), InRouteContext, InTransactionID, InResult);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::GamePlayerReviveRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
	// S2C: Player is revived
	Result NetSvrPolicyGameInstance::GamePlayerRevivedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID )
	{
 		ScopeContext hr;

		 MessageDataPtr pMessage;
		 protocolCheckPtr(m_Endpoint);

		 pMessage = SF::Message::GameInstance::GamePlayerRevivedS2CEvt::Create(GetSystemHeap(), InRouteContext, InRevivedPlayerID);
		 protocolCheckPtr(*pMessage);

		 return m_Endpoint->Send( pMessage );

		return hr;

	}; // Result NetSvrPolicyGameInstance::GamePlayerRevivedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID )


}; // namespace SF


