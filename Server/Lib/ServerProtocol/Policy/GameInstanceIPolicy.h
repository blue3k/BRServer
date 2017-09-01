////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : GameInstance Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Types/BrSvrTypes.h"
#include "Types/BrGameTypes.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Policy
	{
 
		class ISvrPolicyGameInstance : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = PROTOCOLID_SVR_GAMEINSTANCE };

			// Constructor
			ISvrPolicyGameInstance( Net::Connection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// C2S: Game instance deletion
			// Cmd: Join Game
			virtual Result JoinGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const NetAddress &InGameInsSvr, const uint32_t &InTimeStamp, const GameStateID &InGameState, const uint8_t &InDay, const uint8_t &InMaxPlayer, const uint8_t &InPlayerIndex, const uint8_t &InPlayerCharacter, const PlayerRole &InRole, const uint8_t &InDead, const uint8_t &InIsNewJoin, const Array<uint8_t>& InChatHistoryData, const Array<uint8_t>& InGameLogData ) = 0;
			// S2C: Player Joined
			virtual Result PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const uint8_t &InJoinedPlayerDead, const uint8_t &InJoinedPlayerIndex, const uint8_t &InJoinedPlayerCharacter ) = 0;
			// C2S: Change configue preset
			// Cmd: Leave Game
			virtual Result LeaveGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// S2C: Player left
			virtual Result PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID ) = 0;
			// Cmd: Kick player
			virtual Result KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// S2C: Player kicked
			virtual Result PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID ) = 0;
			// Cmd: Assign role
			virtual Result AssignRoleRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// S2C: Assign role
			virtual Result RoleAssignedS2CEvt( const RouteContext &InRouteContext, const PlayerRole &InRole ) = 0;
			// C2S: Chatting message
			// Cmd: Advance game
			virtual Result AdvanceGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// S2C: The game state is advanced
			virtual Result GameAdvancedS2CEvt( const RouteContext &InRouteContext, const uint32_t &InTimeStamp, const GameStateID &InGameState, const uint8_t &InDay ) = 0;
			// S2C: Game is ended
			virtual Result GameEndedS2CEvt( const RouteContext &InRouteContext, const GameWinner &InWinner, const uint32_t &InGainedExp, const uint32_t &InGainedGameMoney, const PlayerRole &InPlayedRole, const uint8_t &InIsWon ) = 0;
			// Cmd: *Vote game advance
			virtual Result VoteGameAdvanceRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// S2C: *GameAdvance is Voted
			virtual Result GameAdvanceVotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter ) = 0;
			// Cmd: Assign role
			virtual Result VoteRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// S2C: Player Voted
			virtual Result VotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget ) = 0;
			// S2C: Player Voted
			virtual Result VoteEndS2CEvt( const RouteContext &InRouteContext, const Array<PlayerID>& InVoted ) = 0;
			// S2C: Player Voted
			virtual Result PlayerRevealedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason ) = 0;
			// S2C: Player Killed
			virtual Result PlayerKilledS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason ) = 0;
			// Cmd: Play again with the current players
			virtual Result GamePlayAgainRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InReplayMemberCount ) = 0;
			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			virtual Result GamePlayAgainS2CEvt( const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const PartyUID &InPartyUID, const PlayerID &InLeadPlayer ) = 0;
			// Cmd: Player. revive himself
			virtual Result GameRevealPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole ) = 0;
			// Cmd: Player. revive himself
			virtual Result GamePlayerReviveRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult ) = 0;
			// S2C: Player is revived
			virtual Result GamePlayerRevivedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID ) = 0;

		}; // class ISvrPolicyGameInstance : public Net::INetPolicy


		class IPolicyGameInstance : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = PROTOCOLID_GAMEINSTANCE };

			// Constructor
			IPolicyGameInstance( Net::Connection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// C2S: Game instance deletion
			virtual Result DeleteGameC2SEvt( const RouteContext &InRouteContext ) = 0;
			// Cmd: Join Game
			virtual Result JoinGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const PlayerRole &InRequestedRole ) = 0;
			// S2C: Player Joined
			// C2S: Change configue preset
			virtual Result SetConfigPresetC2SEvt( const RouteContext &InRouteContext, const uint32_t &InPresetID ) = 0;
			// Cmd: Leave Game
			virtual Result LeaveGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID ) = 0;
			// S2C: Player left
			// Cmd: Kick player
			virtual Result KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick ) = 0;
			// S2C: Player kicked
			// Cmd: Assign role
			virtual Result AssignRoleCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID ) = 0;
			// S2C: Assign role
			// C2S: Chatting message
			virtual Result ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerRole &InRole, const char* InChatMessage ) = 0;
			// Cmd: Advance game
			virtual Result AdvanceGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID ) = 0;
			// S2C: The game state is advanced
			// S2C: Game is ended
			// Cmd: *Vote game advance
			virtual Result VoteGameAdvanceCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID ) = 0;
			// S2C: *GameAdvance is Voted
			// Cmd: Assign role
			virtual Result VoteCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial ) = 0;
			// S2C: Player Voted
			// S2C: Player Voted
			// S2C: Player Voted
			// S2C: Player Killed
			// Cmd: Play again with the current players
			virtual Result GamePlayAgainCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InLeadPlayer, const PartyUID &InPartyUID ) = 0;
			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			// Cmd: Player. revive himself
			virtual Result GameRevealPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID ) = 0;
			// Cmd: Player. revive himself
			virtual Result GamePlayerReviveCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID ) = 0;
			// S2C: Player is revived

		}; // class IPolicyGameInstance : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


