////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : Generated
// 
// Description : GameInstance Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/GameInstanceMsgParsing.h"
#include "Protocol/Message/GameInstanceMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// GameInstance message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageTo> MessageParseToVariableMap;
		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserGameInstance()
		{
 			// C2S: Game instance deletion
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::DeleteGameC2SEvt::MID.IDSeq.MsgID,&Message::GameInstance::DeleteGameC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::DeleteGameC2SEvt::MID.IDSeq.MsgID,&Message::GameInstance::DeleteGameC2SEvt::ParseMessageToMessageBase));
			// Cmd: Join Game
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::JoinGameCmd::MID.IDSeq.MsgID,&Message::GameInstance::JoinGameCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::JoinGameCmd::MID.IDSeq.MsgID,&Message::GameInstance::JoinGameCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::JoinGameRes::MID.IDSeq.MsgID,&Message::GameInstance::JoinGameRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::JoinGameRes::MID.IDSeq.MsgID,&Message::GameInstance::JoinGameRes::ParseMessageToMessageBase));
			// S2C: Player Joined
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::PlayerJoinedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerJoinedS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::PlayerJoinedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerJoinedS2CEvt::ParseMessageToMessageBase));
			// C2S: Change configue preset
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::SetConfigPresetC2SEvt::MID.IDSeq.MsgID,&Message::GameInstance::SetConfigPresetC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::SetConfigPresetC2SEvt::MID.IDSeq.MsgID,&Message::GameInstance::SetConfigPresetC2SEvt::ParseMessageToMessageBase));
			// Cmd: Leave Game
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::LeaveGameCmd::MID.IDSeq.MsgID,&Message::GameInstance::LeaveGameCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::LeaveGameCmd::MID.IDSeq.MsgID,&Message::GameInstance::LeaveGameCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::LeaveGameRes::MID.IDSeq.MsgID,&Message::GameInstance::LeaveGameRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::LeaveGameRes::MID.IDSeq.MsgID,&Message::GameInstance::LeaveGameRes::ParseMessageToMessageBase));
			// S2C: Player left
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::PlayerLeftS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerLeftS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::PlayerLeftS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerLeftS2CEvt::ParseMessageToMessageBase));
			// Cmd: Kick player
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::KickPlayerCmd::MID.IDSeq.MsgID,&Message::GameInstance::KickPlayerCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::KickPlayerCmd::MID.IDSeq.MsgID,&Message::GameInstance::KickPlayerCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::KickPlayerRes::MID.IDSeq.MsgID,&Message::GameInstance::KickPlayerRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::KickPlayerRes::MID.IDSeq.MsgID,&Message::GameInstance::KickPlayerRes::ParseMessageToMessageBase));
			// S2C: Player kicked
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::PlayerKickedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerKickedS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::PlayerKickedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerKickedS2CEvt::ParseMessageToMessageBase));
			// Cmd: Assign role
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::AssignRoleCmd::MID.IDSeq.MsgID,&Message::GameInstance::AssignRoleCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::AssignRoleCmd::MID.IDSeq.MsgID,&Message::GameInstance::AssignRoleCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::AssignRoleRes::MID.IDSeq.MsgID,&Message::GameInstance::AssignRoleRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::AssignRoleRes::MID.IDSeq.MsgID,&Message::GameInstance::AssignRoleRes::ParseMessageToMessageBase));
			// S2C: Assign role
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::RoleAssignedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::RoleAssignedS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::RoleAssignedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::RoleAssignedS2CEvt::ParseMessageToMessageBase));
			// C2S: Chatting message
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::ChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameInstance::ChatMessageC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::ChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameInstance::ChatMessageC2SEvt::ParseMessageToMessageBase));
			// Cmd: Advance game
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::AdvanceGameCmd::MID.IDSeq.MsgID,&Message::GameInstance::AdvanceGameCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::AdvanceGameCmd::MID.IDSeq.MsgID,&Message::GameInstance::AdvanceGameCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::AdvanceGameRes::MID.IDSeq.MsgID,&Message::GameInstance::AdvanceGameRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::AdvanceGameRes::MID.IDSeq.MsgID,&Message::GameInstance::AdvanceGameRes::ParseMessageToMessageBase));
			// S2C: The game state is advanced
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::GameAdvancedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GameAdvancedS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GameAdvancedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GameAdvancedS2CEvt::ParseMessageToMessageBase));
			// S2C: Game is ended
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::GameEndedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GameEndedS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GameEndedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GameEndedS2CEvt::ParseMessageToMessageBase));
			// Cmd: *Vote game advance
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::VoteGameAdvanceCmd::MID.IDSeq.MsgID,&Message::GameInstance::VoteGameAdvanceCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::VoteGameAdvanceCmd::MID.IDSeq.MsgID,&Message::GameInstance::VoteGameAdvanceCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::VoteGameAdvanceRes::MID.IDSeq.MsgID,&Message::GameInstance::VoteGameAdvanceRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::VoteGameAdvanceRes::MID.IDSeq.MsgID,&Message::GameInstance::VoteGameAdvanceRes::ParseMessageToMessageBase));
			// S2C: *GameAdvance is Voted
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::GameAdvanceVotedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GameAdvanceVotedS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GameAdvanceVotedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GameAdvanceVotedS2CEvt::ParseMessageToMessageBase));
			// Cmd: Assign role
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::VoteCmd::MID.IDSeq.MsgID,&Message::GameInstance::VoteCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::VoteCmd::MID.IDSeq.MsgID,&Message::GameInstance::VoteCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::VoteRes::MID.IDSeq.MsgID,&Message::GameInstance::VoteRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::VoteRes::MID.IDSeq.MsgID,&Message::GameInstance::VoteRes::ParseMessageToMessageBase));
			// S2C: Player Voted
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::VotedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::VotedS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::VotedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::VotedS2CEvt::ParseMessageToMessageBase));
			// S2C: Player Voted
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::VoteEndS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::VoteEndS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::VoteEndS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::VoteEndS2CEvt::ParseMessageToMessageBase));
			// S2C: Player Voted
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::PlayerRevealedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerRevealedS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::PlayerRevealedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerRevealedS2CEvt::ParseMessageToMessageBase));
			// S2C: Player Killed
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::PlayerKilledS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerKilledS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::PlayerKilledS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerKilledS2CEvt::ParseMessageToMessageBase));
			// Cmd: Play again with the current players
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::GamePlayAgainCmd::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayAgainCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GamePlayAgainCmd::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayAgainCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::GamePlayAgainRes::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayAgainRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GamePlayAgainRes::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayAgainRes::ParseMessageToMessageBase));
			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::GamePlayAgainS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayAgainS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GamePlayAgainS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayAgainS2CEvt::ParseMessageToMessageBase));
			// Cmd: Player. revive himself
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::GameRevealPlayerCmd::MID.IDSeq.MsgID,&Message::GameInstance::GameRevealPlayerCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GameRevealPlayerCmd::MID.IDSeq.MsgID,&Message::GameInstance::GameRevealPlayerCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::GameRevealPlayerRes::MID.IDSeq.MsgID,&Message::GameInstance::GameRevealPlayerRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GameRevealPlayerRes::MID.IDSeq.MsgID,&Message::GameInstance::GameRevealPlayerRes::ParseMessageToMessageBase));
			// Cmd: Player. revive himself
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::GamePlayerReviveCmd::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerReviveCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GamePlayerReviveCmd::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerReviveCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::GamePlayerReviveRes::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerReviveRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GamePlayerReviveRes::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerReviveRes::ParseMessageToMessageBase));
			// S2C: Player is revived
			MessageParseToVariableMap.insert(std::make_pair(Message::GameInstance::GamePlayerRevivedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerRevivedS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GamePlayerRevivedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerRevivedS2CEvt::ParseMessageToMessageBase));
		}; // void RegisterParserGameInstance()


	}; // namespace Protocol
}; // namespace SF


