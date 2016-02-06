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
			HRESULT DeleteGameC2SEvt( const RouteContext &InRouteContext );
			// Cmd: Join Game
			HRESULT JoinGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const PlayerRole &InRequestedRole );
			// C2S: Change configue preset
			HRESULT SetConfigPresetC2SEvt( const RouteContext &InRouteContext, const UINT32 &InPresetID );
			// Cmd: Leave Game
			HRESULT LeaveGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Kick player
			HRESULT KickPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
			// Cmd: Assign role
			HRESULT AssignRoleCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// C2S: Chatting message
			HRESULT ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerRole &InRole, const char* InChatMessage );
			// Cmd: Advance game
			HRESULT AdvanceGameCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: *Vote game advance
			HRESULT VoteGameAdvanceCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );
			// Cmd: Assign role
			HRESULT VoteCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const UINT32 &InActionSerial );
			// Cmd: Play again with the current players
			HRESULT GamePlayAgainCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InLeadPlayer, const PartyUID &InPartyUID );
			// Cmd: Player. revive himself
			HRESULT GameRevealPlayerCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID );
			// Cmd: Player. revive himself
			HRESULT GamePlayerReviveCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );

		}; // class NetPolicyGameInstance : public IPolicyGameInstance


		class NetSvrPolicyGameInstance : public ISvrPolicyGameInstance
		{
 			public:
			// Constructor
			NetSvrPolicyGameInstance ( Net::IConnection* pConn ) : ISvrPolicyGameInstance(pConn)
			{}

			// Cmd: Join Game
			HRESULT JoinGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const NetAddress &InGameInsSvr, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const BYTE &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const BYTE &InIsNewJoin, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData );
			// S2C: Player Joined
			HRESULT PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter );
			// Cmd: Leave Game
			HRESULT LeaveGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult );
			// S2C: Player left
			HRESULT PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID );
			// Cmd: Kick player
			HRESULT KickPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult );
			// S2C: Player kicked
			HRESULT PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID );
			// Cmd: Assign role
			HRESULT AssignRoleRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult );
			// S2C: Assign role
			HRESULT RoleAssignedS2CEvt( const RouteContext &InRouteContext, const PlayerRole &InRole );
			// Cmd: Advance game
			HRESULT AdvanceGameRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult );
			// S2C: The game state is advanced
			HRESULT GameAdvancedS2CEvt( const RouteContext &InRouteContext, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay );
			// S2C: Game is ended
			HRESULT GameEndedS2CEvt( const RouteContext &InRouteContext, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney, const PlayerRole &InPlayedRole, const BYTE &InIsWon );
			// Cmd: *Vote game advance
			HRESULT VoteGameAdvanceRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult );
			// S2C: *GameAdvance is Voted
			HRESULT GameAdvanceVotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter );
			// Cmd: Assign role
			HRESULT VoteRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult );
			// S2C: Player Voted
			HRESULT VotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget );
			// S2C: Player Voted
			HRESULT VoteEndS2CEvt( const RouteContext &InRouteContext, const Array<PlayerID>& InVoted );
			// S2C: Player Voted
			HRESULT PlayerRevealedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason );
			// S2C: Player Killed
			HRESULT PlayerKilledS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason );
			// Cmd: Play again with the current players
			HRESULT GamePlayAgainRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const UINT32 &InReplayMemberCount );
			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			HRESULT GamePlayAgainS2CEvt( const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const PartyUID &InPartyUID, const PlayerID &InLeadPlayer );
			// Cmd: Player. revive himself
			HRESULT GameRevealPlayerRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole );
			// Cmd: Player. revive himself
			HRESULT GamePlayerReviveRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult );
			// S2C: Player is revived
			HRESULT GamePlayerRevivedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID );

		}; // class NetSvrPolicyGameInstance : public ISvrPolicyGameInstance


	}; // namespace Policy
}; // namespace BR


