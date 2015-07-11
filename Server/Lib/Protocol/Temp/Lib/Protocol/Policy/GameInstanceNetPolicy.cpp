////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameInstance Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/GameInstanceIPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// C2S: Game instance deletion
		HRESULT NetPolicyGameInstance::DeleteGameC2SEvt( const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::DeleteGameC2SEvt::BuildIMsg(pMsg, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameInstance::DeleteGameC2SEvt( const RouteContext &InRouteContext )
		// Cmd: Join Game
		HRESULT NetPolicyGameInstance::JoinGameCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const PlayerRole &InRequestedRole )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::JoinGameCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayer, InTicket, InRequestedRole));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameInstance::JoinGameCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const PlayerRole &InRequestedRole )
		// C2S: Change configue preset
		HRESULT NetPolicyGameInstance::SetConfigPresetC2SEvt( const RouteContext &InRouteContext, const UINT32 &InPresetID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::SetConfigPresetC2SEvt::BuildIMsg(pMsg, InRouteContext, InPresetID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameInstance::SetConfigPresetC2SEvt( const RouteContext &InRouteContext, const UINT32 &InPresetID )
		// Cmd: Leave Game
		HRESULT NetPolicyGameInstance::LeaveGameCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::LeaveGameCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameInstance::LeaveGameCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
		// Cmd: Kick player
		HRESULT NetPolicyGameInstance::KickPlayerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::KickPlayerCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID, InPlayerToKick));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameInstance::KickPlayerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// Cmd: Assign role
		HRESULT NetPolicyGameInstance::AssignRoleCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::AssignRoleCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameInstance::AssignRoleCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
		// C2S: Chatting message
		HRESULT NetPolicyGameInstance::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerRole &InRole, const char* InChatMessage )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::ChatMessageC2SEvt::BuildIMsg(pMsg, InRouteContext, InPlayerID, InRole, InChatMessage));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameInstance::ChatMessageC2SEvt( const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerRole &InRole, const char* InChatMessage )
		// Cmd: Advance game
		HRESULT NetPolicyGameInstance::AdvanceGameCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::AdvanceGameCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameInstance::AdvanceGameCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
		// Cmd: *Vote game advance
		HRESULT NetPolicyGameInstance::VoteGameAdvanceCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::VoteGameAdvanceCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameInstance::VoteGameAdvanceCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
		// Cmd: Assign role
		HRESULT NetPolicyGameInstance::VoteCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const UINT32 &InActionSerial )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::VoteCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID, InVoteTarget, InActionSerial));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameInstance::VoteCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const UINT32 &InActionSerial )
		// Cmd: Play again with the current players
		HRESULT NetPolicyGameInstance::GamePlayAgainCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InLeadPlayer, const PartyUID &InPartyUID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::GamePlayAgainCmd::BuildIMsg(pMsg, InContext, InRouteContext, InLeadPlayer, InPartyUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameInstance::GamePlayAgainCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InLeadPlayer, const PartyUID &InPartyUID )
		// Cmd: Player. revive himself
		HRESULT NetPolicyGameInstance::GameRevealPlayerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::GameRevealPlayerCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID, InTargetPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameInstance::GameRevealPlayerCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
		// Cmd: Player. revive himself
		HRESULT NetPolicyGameInstance::GamePlayerReviveCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::GamePlayerReviveCmd::BuildIMsg(pMsg, InContext, InRouteContext, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGameInstance::GamePlayerReviveCmd( const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )


		// Cmd: Join Game
		HRESULT NetSvrPolicyGameInstance::JoinGameRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const NetAddress &InGameInsSvr, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const BYTE &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const BYTE &InIsNewJoin, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::JoinGameRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext, InGameInsSvr, InTimeStamp, InGameState, InDay, InMaxPlayer, InPlayerIndex, InPlayerCharacter, InRole, InDead, InIsNewJoin, InChatHistoryData, InGameLogData));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::JoinGameRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const NetAddress &InGameInsSvr, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const BYTE &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const BYTE &InIsNewJoin, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData )
		// S2C: Player Joined
		HRESULT NetSvrPolicyGameInstance::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::PlayerJoinedS2CEvt::BuildIMsg(pMsg, InRouteContext, InJoinedPlayer, InJoinedPlayerRole, InJoinedPlayerDead, InJoinedPlayerIndex, InJoinedPlayerCharacter));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::PlayerJoinedS2CEvt( const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter )
		// Cmd: Leave Game
		HRESULT NetSvrPolicyGameInstance::LeaveGameRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::LeaveGameRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::LeaveGameRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		// S2C: Player left
		HRESULT NetSvrPolicyGameInstance::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::PlayerLeftS2CEvt::BuildIMsg(pMsg, InRouteContext, InLeftPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::PlayerLeftS2CEvt( const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
		// Cmd: Kick player
		HRESULT NetSvrPolicyGameInstance::KickPlayerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::KickPlayerRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::KickPlayerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		// S2C: Player kicked
		HRESULT NetSvrPolicyGameInstance::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::PlayerKickedS2CEvt::BuildIMsg(pMsg, InRouteContext, InKickedPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::PlayerKickedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
		// Cmd: Assign role
		HRESULT NetSvrPolicyGameInstance::AssignRoleRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::AssignRoleRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::AssignRoleRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		// S2C: Assign role
		HRESULT NetSvrPolicyGameInstance::RoleAssignedS2CEvt( const RouteContext &InRouteContext, const PlayerRole &InRole )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::RoleAssignedS2CEvt::BuildIMsg(pMsg, InRouteContext, InRole));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::RoleAssignedS2CEvt( const RouteContext &InRouteContext, const PlayerRole &InRole )
		// Cmd: Advance game
		HRESULT NetSvrPolicyGameInstance::AdvanceGameRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::AdvanceGameRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::AdvanceGameRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		// S2C: The game state is advanced
		HRESULT NetSvrPolicyGameInstance::GameAdvancedS2CEvt( const RouteContext &InRouteContext, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::GameAdvancedS2CEvt::BuildIMsg(pMsg, InRouteContext, InTimeStamp, InGameState, InDay));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::GameAdvancedS2CEvt( const RouteContext &InRouteContext, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay )
		// S2C: Game is ended
		HRESULT NetSvrPolicyGameInstance::GameEndedS2CEvt( const RouteContext &InRouteContext, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney, const PlayerRole &InPlayedRole, const BYTE &InIsWon )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::GameEndedS2CEvt::BuildIMsg(pMsg, InRouteContext, InWinner, InGainedExp, InGainedGameMoney, InPlayedRole, InIsWon));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::GameEndedS2CEvt( const RouteContext &InRouteContext, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney, const PlayerRole &InPlayedRole, const BYTE &InIsWon )
		// Cmd: *Vote game advance
		HRESULT NetSvrPolicyGameInstance::VoteGameAdvanceRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::VoteGameAdvanceRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::VoteGameAdvanceRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		// S2C: *GameAdvance is Voted
		HRESULT NetSvrPolicyGameInstance::GameAdvanceVotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::GameAdvanceVotedS2CEvt::BuildIMsg(pMsg, InRouteContext, InVoter));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::GameAdvanceVotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter )
		// Cmd: Assign role
		HRESULT NetSvrPolicyGameInstance::VoteRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::VoteRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::VoteRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		// S2C: Player Voted
		HRESULT NetSvrPolicyGameInstance::VotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::VotedS2CEvt::BuildIMsg(pMsg, InRouteContext, InVoter, InVotedTarget));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::VotedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget )
		// S2C: Player Voted
		HRESULT NetSvrPolicyGameInstance::VoteEndS2CEvt( const RouteContext &InRouteContext, const Array<PlayerID>& InVoted )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::VoteEndS2CEvt::BuildIMsg(pMsg, InRouteContext, InVoted));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::VoteEndS2CEvt( const RouteContext &InRouteContext, const Array<PlayerID>& InVoted )
		// S2C: Player Voted
		HRESULT NetSvrPolicyGameInstance::PlayerRevealedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::PlayerRevealedS2CEvt::BuildIMsg(pMsg, InRouteContext, InRevealedPlayerID, InRole, InReason));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::PlayerRevealedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason )
		// S2C: Player Killed
		HRESULT NetSvrPolicyGameInstance::PlayerKilledS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::PlayerKilledS2CEvt::BuildIMsg(pMsg, InRouteContext, InKilledPlayer, InReason));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::PlayerKilledS2CEvt( const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason )
		// Cmd: Play again with the current players
		HRESULT NetSvrPolicyGameInstance::GamePlayAgainRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InReplayMemberCount )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::GamePlayAgainRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext, InReplayMemberCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::GamePlayAgainRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InReplayMemberCount )
		// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
		HRESULT NetSvrPolicyGameInstance::GamePlayAgainS2CEvt( const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const PartyUID &InPartyUID, const PlayerID &InLeadPlayer )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::GamePlayAgainS2CEvt::BuildIMsg(pMsg, InRouteContext, InTargetPlayer, InPartyUID, InLeadPlayer));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::GamePlayAgainS2CEvt( const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const PartyUID &InPartyUID, const PlayerID &InLeadPlayer )
		// Cmd: Player. revive himself
		HRESULT NetSvrPolicyGameInstance::GameRevealPlayerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::GameRevealPlayerRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext, InRevealedPlayerID, InRevealedRole));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::GameRevealPlayerRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole )
		// Cmd: Player. revive himself
		HRESULT NetSvrPolicyGameInstance::GamePlayerReviveRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::GamePlayerReviveRes::BuildIMsg(pMsg, InContext, InResult, InRouteContext));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::GamePlayerReviveRes( const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
		// S2C: Player is revived
		HRESULT NetSvrPolicyGameInstance::GamePlayerRevivedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::GameInstance::GamePlayerRevivedS2CEvt::BuildIMsg(pMsg, InRouteContext, InRevivedPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGameInstance::GamePlayerRevivedS2CEvt( const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID )


	}; // namespace Policy
}; // namespace BR


