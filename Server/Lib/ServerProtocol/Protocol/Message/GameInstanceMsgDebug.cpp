////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameInstance Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/Message/GameInstanceMsgDebug.h"
#include "Protocol/Message/GameInstanceMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// GameInstance Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceGameInstance()
		{
 			// Cmd: Join to a game instance. You can call multiple times, but it would be a waste
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::JoinGameInstanceCmd::MID.IDSeq.MsgID,&Message::GameInstance::JoinGameInstanceCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::JoinGameInstanceRes::MID.IDSeq.MsgID,&Message::GameInstance::JoinGameInstanceRes::TraceOut));
			// Cmd: Leave game instance.
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::LeaveGameInstanceCmd::MID.IDSeq.MsgID,&Message::GameInstance::LeaveGameInstanceCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::LeaveGameInstanceRes::MID.IDSeq.MsgID,&Message::GameInstance::LeaveGameInstanceRes::TraceOut));
			// C2S: Player Movement
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::PlayerMovementC2SEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerMovementC2SEvt::TraceOut));
			// S2C: Player Movement
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::PlayerMovementS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerMovementS2CEvt::TraceOut));
			// Cmd: Kick player with given ID
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::KickPlayerCmd::MID.IDSeq.MsgID,&Message::GameInstance::KickPlayerCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::KickPlayerRes::MID.IDSeq.MsgID,&Message::GameInstance::KickPlayerRes::TraceOut));
			// S2C: Player kicked event. this event will be brocasted when a player kicked.
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::PlayerKickedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerKickedS2CEvt::TraceOut));
			// Cmd: Join to a game instance. You can call multiple times, but it would be waste
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::JoinGameCmd::MID.IDSeq.MsgID,&Message::GameInstance::JoinGameCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::JoinGameRes::MID.IDSeq.MsgID,&Message::GameInstance::JoinGameRes::TraceOut));
			// Cmd: Assign new roles to all players.
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::AssignRoleCmd::MID.IDSeq.MsgID,&Message::GameInstance::AssignRoleCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::AssignRoleRes::MID.IDSeq.MsgID,&Message::GameInstance::AssignRoleRes::TraceOut));
			// S2C: Event for role assigned
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::RoleAssignedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::RoleAssignedS2CEvt::TraceOut));
			// C2S: in-game chatting message.
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::ChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameInstance::ChatMessageC2SEvt::TraceOut));
			// Cmd: Advance game
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::AdvanceGameCmd::MID.IDSeq.MsgID,&Message::GameInstance::AdvanceGameCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::AdvanceGameRes::MID.IDSeq.MsgID,&Message::GameInstance::AdvanceGameRes::TraceOut));
			// S2C: The game state is advanced
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::GameAdvancedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GameAdvancedS2CEvt::TraceOut));
			// S2C: Game is ended
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::GameEndedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GameEndedS2CEvt::TraceOut));
			// Cmd: *Vote game advance
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::VoteGameAdvanceCmd::MID.IDSeq.MsgID,&Message::GameInstance::VoteGameAdvanceCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::VoteGameAdvanceRes::MID.IDSeq.MsgID,&Message::GameInstance::VoteGameAdvanceRes::TraceOut));
			// S2C: *GameAdvance is Voted
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::GameAdvanceVotedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GameAdvanceVotedS2CEvt::TraceOut));
			// Cmd: Assign role
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::VoteCmd::MID.IDSeq.MsgID,&Message::GameInstance::VoteCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::VoteRes::MID.IDSeq.MsgID,&Message::GameInstance::VoteRes::TraceOut));
			// S2C: Player Voted
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::VotedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::VotedS2CEvt::TraceOut));
			// S2C: Player Voted
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::VoteEndS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::VoteEndS2CEvt::TraceOut));
			// S2C: Player Voted
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::PlayerRevealedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerRevealedS2CEvt::TraceOut));
			// S2C: Player Killed
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::PlayerKilledS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerKilledS2CEvt::TraceOut));
			// Cmd: Play again with the current players
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::GamePlayAgainCmd::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayAgainCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::GamePlayAgainRes::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayAgainRes::TraceOut));
			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::GamePlayAgainS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayAgainS2CEvt::TraceOut));
			// Cmd: Player. revive himself
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::GameRevealPlayerCmd::MID.IDSeq.MsgID,&Message::GameInstance::GameRevealPlayerCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::GameRevealPlayerRes::MID.IDSeq.MsgID,&Message::GameInstance::GameRevealPlayerRes::TraceOut));
			// Cmd: Player. revive himself
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::GamePlayerReviveCmd::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerReviveCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::GamePlayerReviveRes::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerReviveRes::TraceOut));
			// S2C: Player is revived
			MessageDebugTraceMap.insert(std::make_pair(Message::GameInstance::GamePlayerRevivedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerRevivedS2CEvt::TraceOut));
		}; // void RegisterDebugTraceGameInstance()


	}; // namespace Protocol
}; // namespace SF


