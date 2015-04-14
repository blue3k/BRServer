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

#include "Common/Typedefs.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Net/NetPolicy.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Policy
	{
 
		class ISvrPolicyGameInstance : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_SVR_GAMEINSTANCE };

			// Constructor
			ISvrPolicyGameInstance( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// C2S: Game instance deletion
			// Cmd: Join Game
			virtual HRESULT JoinGameRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const NetAddress &InGameInsSvr, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const BYTE &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const BYTE &InIsNewJoin, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData ) = 0;
			// S2C: Player Joined
			virtual HRESULT PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter ) = 0;
			// C2S: Change configue preset
			// Cmd: Leave Game
			virtual HRESULT LeaveGameRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// S2C: Player left
			virtual HRESULT PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID ) = 0;
			// Cmd: Kick player
			virtual HRESULT KickPlayerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// S2C: Player kicked
			virtual HRESULT PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID ) = 0;
			// Cmd: Assign role
			virtual HRESULT AssignRoleRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// S2C: Assign role
			virtual HRESULT RoleAssignedS2CEvt( const RouteContext &InRouteContext, const PlayerRole &InRole ) = 0;
			// C2S: Chatting message
			// Cmd: Advance game
			virtual HRESULT AdvanceGameRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// S2C: The game state is advanced
			virtual HRESULT GameAdvancedS2CEvt( const RouteContext &InRouteContext, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay ) = 0;
			// S2C: Game is ended
			virtual HRESULT GameEndedS2CEvt( const RouteContext &InRouteContext, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney, const PlayerRole &InPlayedRole, const BYTE &InIsWon ) = 0;
			// Cmd: *Vote game advance
			virtual HRESULT VoteGameAdvanceRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// S2C: *GameAdvance is Voted
			virtual HRESULT GameAdvanceVotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter ) = 0;
			// Cmd: Assign role
			virtual HRESULT VoteRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// S2C: Player Voted
			virtual HRESULT VotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget ) = 0;
			// S2C: Player Voted
			virtual HRESULT VoteEndS2CEvt( const RouteContext &InRouteContext, const Array<PlayerID>& InVoted ) = 0;
			// S2C: Player Voted
			virtual HRESULT PlayerRevealedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason ) = 0;
			// S2C: Player Killed
			virtual HRESULT PlayerKilledS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason ) = 0;
			// Cmd: Play again with the current players
			virtual HRESULT GamePlayAgainRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InReplayMemberCount ) = 0;
			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			virtual HRESULT GamePlayAgainS2CEvt( const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const PartyUID &InPartyUID, const PlayerID &InLeadPlayer ) = 0;
			// Cmd: Player. revive himself
			virtual HRESULT GameRevealPlayerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const PlayerRole &InRevealedRole ) = 0;
			// Cmd: Player. revive himself
			virtual HRESULT GamePlayerReviveRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext ) = 0;
			// S2C: Player is revived
			virtual HRESULT GamePlayerRevivedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID ) = 0;

		}; // class ISvrPolicyGameInstance : public Net::INetPolicy


		class IPolicyGameInstance : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_GAMEINSTANCE };

			// Constructor
			IPolicyGameInstance( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// C2S: Game instance deletion
			virtual HRESULT DeleteGameC2SEvt( const RouteContext &InRouteContext ) = 0;
			// Cmd: Join Game
			virtual HRESULT JoinGameCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerInformation &InPlayer, const AuthTicket &InTicket ) = 0;
			// S2C: Player Joined
			// C2S: Change configue preset
			virtual HRESULT SetConfigPresetC2SEvt( const RouteContext &InRouteContext, const UINT32 &InPresetID ) = 0;
			// Cmd: Leave Game
			virtual HRESULT LeaveGameCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID ) = 0;
			// S2C: Player left
			// Cmd: Kick player
			virtual HRESULT KickPlayerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick ) = 0;
			// S2C: Player kicked
			// Cmd: Assign role
			virtual HRESULT AssignRoleCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID ) = 0;
			// S2C: Assign role
			// C2S: Chatting message
			virtual HRESULT ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerRole &InRole, const char* InChatMessage ) = 0;
			// Cmd: Advance game
			virtual HRESULT AdvanceGameCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID ) = 0;
			// S2C: The game state is advanced
			// S2C: Game is ended
			// Cmd: *Vote game advance
			virtual HRESULT VoteGameAdvanceCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID ) = 0;
			// S2C: *GameAdvance is Voted
			// Cmd: Assign role
			virtual HRESULT VoteCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const UINT32 &InActionSerial ) = 0;
			// S2C: Player Voted
			// S2C: Player Voted
			// S2C: Player Voted
			// S2C: Player Killed
			// Cmd: Play again with the current players
			virtual HRESULT GamePlayAgainCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InLeadPlayer, const PartyUID &InPartyUID ) = 0;
			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			// Cmd: Player. revive himself
			virtual HRESULT GameRevealPlayerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerID &InTargetPlayerID ) = 0;
			// Cmd: Player. revive himself
			virtual HRESULT GamePlayerReviveCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID ) = 0;
			// S2C: Player is revived

		}; // class IPolicyGameInstance : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


