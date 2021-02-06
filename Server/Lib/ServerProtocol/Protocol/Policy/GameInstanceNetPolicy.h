////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : Generated
// 
// Description : GameInstance Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"



namespace SF
{
    class MessageEndpoint;
   class VariableTable;


	class NetPolicyGameInstance 
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetPolicyGameInstance (const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// C2S: Nitify that a game instance is deleted. Game instance send this message to manager before it destroy itself.
		Result DeleteGameC2SEvt( const RouteContext &InRouteContext );
		// Cmd: Join to a game instance. You can call multiple times, but it would be waste
		Result JoinGameInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayer );
		// C2S: For debug purpose, change configue preset. There is a game setting table. you can switch between those setting value.
		Result SetConfigPresetC2SEvt( const RouteContext &InRouteContext, const uint32_t &InPresetID );
		// Cmd: Leave game instance.
		Result LeaveGameInstanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID );
		// C2S: Player Movement
		Result PlayerMovementC2SEvt( const RouteContext &InRouteContext, const uint64_t &InGameInsUID, const PlayerID &InPlayerID, const VariableTable &InAttributes );
		// Cmd: Kick player with given ID
		Result KickPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
		// Cmd: Join to a game instance. You can call multiple times, but it would be waste
		Result JoinGameCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const uint8_t &InRequestedRole );
		// Cmd: Assign new roles to all players.
		Result AssignRoleCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID );
		// C2S: in-game chatting message.
		Result ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage );
		// Cmd: Advance game
		Result AdvanceGameCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID );
		// Cmd: *Vote game advance
		Result VoteGameAdvanceCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID );
		// Cmd: Assign role
		Result VoteCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial );
		// Cmd: Play again with the current players
		Result GamePlayAgainCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID );
		// Cmd: Player. revive himself
		Result GameRevealPlayerCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID );
		// Cmd: Player. revive himself
		Result GamePlayerReviveCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID );

	}; // class NetPolicyGameInstance 


	class NetSvrPolicyGameInstance
	{
 		private:
			SharedPointerT<MessageEndpoint> m_Endpoint;
		public:
		// Constructor
		NetSvrPolicyGameInstance ( const SharedPointerT<MessageEndpoint>& pEndpoint ) : m_Endpoint(pEndpoint)
		{}

		// Cmd: Join to a game instance. You can call multiple times, but it would be waste
		Result JoinGameInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const NetAddress &InGameInsSvr, const uint32_t &InTimeStamp );
		// S2C: Player joined event. This event is brocasted when a player joined
		Result PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer );
		// Cmd: Leave game instance.
		Result LeaveGameInstanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
		// S2C: Player left event.
		Result PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID );
		// S2C: Player Movement
		Result PlayerMovementS2CEvt( const RouteContext &InRouteContext, const uint64_t &InGameInsUID, const PlayerID &InPlayerID, const VariableTable &InAttributes );
		// Cmd: Kick player with given ID
		Result KickPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
		// S2C: Player kicked event. this event will be brocasted when a player kicked.
		Result PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID );
		// Cmd: Join to a game instance. You can call multiple times, but it would be waste
		Result JoinGameRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const NetAddress &InGameInsSvr, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay, const uint8_t &InMaxPlayer, const uint8_t &InPlayerIndex, const uint8_t &InPlayerCharacter, const uint8_t &InRole, const uint8_t &InDead, const uint8_t &InIsNewJoin, const Array<uint8_t>& InChatHistoryData, const Array<uint8_t>& InGameLogData );
		// Cmd: Assign new roles to all players.
		Result AssignRoleRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
		// S2C: Event for role assigned
		Result RoleAssignedS2CEvt( const RouteContext &InRouteContext, const uint8_t &InRole );
		// Cmd: Advance game
		Result AdvanceGameRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
		// S2C: The game state is advanced
		Result GameAdvancedS2CEvt( const RouteContext &InRouteContext, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay );
		// S2C: Game is ended
		Result GameEndedS2CEvt( const RouteContext &InRouteContext, const uint8_t &InWinner, const uint32_t &InGainedExp, const uint32_t &InGainedGameMoney, const uint8_t &InPlayedRole, const uint8_t &InIsWon );
		// Cmd: *Vote game advance
		Result VoteGameAdvanceRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
		// S2C: *GameAdvance is Voted
		Result GameAdvanceVotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter );
		// Cmd: Assign role
		Result VoteRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
		// S2C: Player Voted
		Result VotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget );
		// S2C: Player Voted
		Result VoteEndS2CEvt( const RouteContext &InRouteContext, const Array<PlayerID>& InVoted );
		// S2C: Player Voted
		Result PlayerRevealedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const uint8_t &InRole, const uint8_t &InReason );
		// S2C: Player Killed
		Result PlayerKilledS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const uint8_t &InReason );
		// Cmd: Play again with the current players
		Result GamePlayAgainRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InReplayMemberCount );
		// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
		Result GamePlayAgainS2CEvt( const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const uint64_t &InPartyUID, const PlayerID &InLeadPlayer );
		// Cmd: Player. revive himself
		Result GameRevealPlayerRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<uint8_t>& InRevealedRole );
		// Cmd: Player. revive himself
		Result GamePlayerReviveRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
		// S2C: Player is revived
		Result GamePlayerRevivedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID );

	}; // class NetSvrPolicyGameInstance


}; // namespace SF


