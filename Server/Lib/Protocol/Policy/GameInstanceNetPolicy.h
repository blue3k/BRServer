////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameInstance Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyGameInstance : public IPolicyGameInstance
		{
 			public:
			// Constructor
			NetPolicyGameInstance ( Net::IConnection* pConn ) : IPolicyGameInstance(pConn)
			{}

			// C2S: Game instance deletion
			Result DeleteGameC2SEvt( const RouteContext &InRouteContext );
			// Cmd: Join Game
			Result JoinGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const PlayerRole &InRequestedRole );
			// C2S: Change configue preset
			Result SetConfigPresetC2SEvt( const RouteContext &InRouteContext, const uint32_t &InPresetID );
			// Cmd: Leave Game
			Result LeaveGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Kick player
			Result KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
			// Cmd: Assign role
			Result AssignRoleCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// C2S: Chatting message
			Result ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerRole &InRole, const char* InChatMessage );
			// Cmd: Advance game
			Result AdvanceGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: *Vote game advance
			Result VoteGameAdvanceCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Assign role
			Result VoteCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial );
			// Cmd: Play again with the current players
			Result GamePlayAgainCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InLeadPlayer, const PartyUID &InPartyUID );
			// Cmd: Player. revive himself
			Result GameRevealPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID );
			// Cmd: Player. revive himself
			Result GamePlayerReviveCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );

		}; // class NetPolicyGameInstance : public IPolicyGameInstance


		class NetSvrPolicyGameInstance : public ISvrPolicyGameInstance
		{
 			public:
			// Constructor
			NetSvrPolicyGameInstance ( Net::IConnection* pConn ) : ISvrPolicyGameInstance(pConn)
			{}

			// Cmd: Join Game
			Result JoinGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const NetAddress &InGameInsSvr, const uint32_t &InTimeStamp, const GameStateID &InGameState, const uint8_t &InDay, const uint8_t &InMaxPlayer, const uint8_t &InPlayerIndex, const uint8_t &InPlayerCharacter, const PlayerRole &InRole, const uint8_t &InDead, const uint8_t &InIsNewJoin, const Array<uint8_t>& InChatHistoryData, const Array<uint8_t>& InGameLogData );
			// S2C: Player Joined
			Result PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const uint8_t &InJoinedPlayerDead, const uint8_t &InJoinedPlayerIndex, const uint8_t &InJoinedPlayerCharacter );
			// Cmd: Leave Game
			Result LeaveGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// S2C: Player left
			Result PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID );
			// Cmd: Kick player
			Result KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// S2C: Player kicked
			Result PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID );
			// Cmd: Assign role
			Result AssignRoleRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// S2C: Assign role
			Result RoleAssignedS2CEvt( const RouteContext &InRouteContext, const PlayerRole &InRole );
			// Cmd: Advance game
			Result AdvanceGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// S2C: The game state is advanced
			Result GameAdvancedS2CEvt( const RouteContext &InRouteContext, const uint32_t &InTimeStamp, const GameStateID &InGameState, const uint8_t &InDay );
			// S2C: Game is ended
			Result GameEndedS2CEvt( const RouteContext &InRouteContext, const GameWinner &InWinner, const uint32_t &InGainedExp, const uint32_t &InGainedGameMoney, const PlayerRole &InPlayedRole, const uint8_t &InIsWon );
			// Cmd: *Vote game advance
			Result VoteGameAdvanceRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// S2C: *GameAdvance is Voted
			Result GameAdvanceVotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter );
			// Cmd: Assign role
			Result VoteRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// S2C: Player Voted
			Result VotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget );
			// S2C: Player Voted
			Result VoteEndS2CEvt( const RouteContext &InRouteContext, const Array<PlayerID>& InVoted );
			// S2C: Player Voted
			Result PlayerRevealedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason );
			// S2C: Player Killed
			Result PlayerKilledS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason );
			// Cmd: Play again with the current players
			Result GamePlayAgainRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InReplayMemberCount );
			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			Result GamePlayAgainS2CEvt( const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const PartyUID &InPartyUID, const PlayerID &InLeadPlayer );
			// Cmd: Player. revive himself
			Result GameRevealPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole );
			// Cmd: Player. revive himself
			Result GamePlayerReviveRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );
			// S2C: Player is revived
			Result GamePlayerRevivedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID );

		}; // class NetSvrPolicyGameInstance : public ISvrPolicyGameInstance


	}; // namespace Policy
}; // namespace BR


