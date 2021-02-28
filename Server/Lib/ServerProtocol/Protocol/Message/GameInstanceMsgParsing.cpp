﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameInstance Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/Message/GameInstanceMsgParsing.h"
#include "Protocol/Message/GameInstanceMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// GameInstance message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserGameInstance()
		{
 			// Cmd: Join to a game instance. You can call multiple times, but it would be a waste
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::JoinGameInstanceCmd::MID.IDSeq.MsgID,&Message::GameInstance::JoinGameInstanceCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::JoinGameInstanceRes::MID.IDSeq.MsgID,&Message::GameInstance::JoinGameInstanceRes::ParseMessageToMessageBase));
			// Cmd: Leave game instance.
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::LeaveGameInstanceCmd::MID.IDSeq.MsgID,&Message::GameInstance::LeaveGameInstanceCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::LeaveGameInstanceRes::MID.IDSeq.MsgID,&Message::GameInstance::LeaveGameInstanceRes::ParseMessageToMessageBase));
			// S2C: Player left event.
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::PlayerLeftS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerLeftS2CEvt::ParseMessageToMessageBase));
			// C2S: Player Movement
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::PlayerMovementC2SEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerMovementC2SEvt::ParseMessageToMessageBase));
			// S2C: Player Movement
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::PlayerMovementS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerMovementS2CEvt::ParseMessageToMessageBase));
			// Cmd: Kick player with given ID
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::KickPlayerCmd::MID.IDSeq.MsgID,&Message::GameInstance::KickPlayerCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::KickPlayerRes::MID.IDSeq.MsgID,&Message::GameInstance::KickPlayerRes::ParseMessageToMessageBase));
			// Cmd: Join to a game instance. You can call multiple times, but it would be waste
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::JoinGameCmd::MID.IDSeq.MsgID,&Message::GameInstance::JoinGameCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::JoinGameRes::MID.IDSeq.MsgID,&Message::GameInstance::JoinGameRes::ParseMessageToMessageBase));
			// Cmd: Assign new roles to all players.
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::AssignRoleCmd::MID.IDSeq.MsgID,&Message::GameInstance::AssignRoleCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::AssignRoleRes::MID.IDSeq.MsgID,&Message::GameInstance::AssignRoleRes::ParseMessageToMessageBase));
			// S2C: Event for role assigned
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::RoleAssignedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::RoleAssignedS2CEvt::ParseMessageToMessageBase));
			// C2S: in-game chatting message.
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::ChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameInstance::ChatMessageC2SEvt::ParseMessageToMessageBase));
			// Cmd: Advance game
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::AdvanceGameCmd::MID.IDSeq.MsgID,&Message::GameInstance::AdvanceGameCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::AdvanceGameRes::MID.IDSeq.MsgID,&Message::GameInstance::AdvanceGameRes::ParseMessageToMessageBase));
			// S2C: The game state is advanced
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GameAdvancedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GameAdvancedS2CEvt::ParseMessageToMessageBase));
			// S2C: Game is ended
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GameEndedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GameEndedS2CEvt::ParseMessageToMessageBase));
			// Cmd: *Vote game advance
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::VoteGameAdvanceCmd::MID.IDSeq.MsgID,&Message::GameInstance::VoteGameAdvanceCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::VoteGameAdvanceRes::MID.IDSeq.MsgID,&Message::GameInstance::VoteGameAdvanceRes::ParseMessageToMessageBase));
			// S2C: *GameAdvance is Voted
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GameAdvanceVotedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GameAdvanceVotedS2CEvt::ParseMessageToMessageBase));
			// Cmd: Assign role
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::VoteCmd::MID.IDSeq.MsgID,&Message::GameInstance::VoteCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::VoteRes::MID.IDSeq.MsgID,&Message::GameInstance::VoteRes::ParseMessageToMessageBase));
			// S2C: Player Voted
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::VotedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::VotedS2CEvt::ParseMessageToMessageBase));
			// S2C: Player Voted
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::VoteEndS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::VoteEndS2CEvt::ParseMessageToMessageBase));
			// S2C: Player Voted
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::PlayerRevealedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerRevealedS2CEvt::ParseMessageToMessageBase));
			// S2C: Player Killed
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::PlayerKilledS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerKilledS2CEvt::ParseMessageToMessageBase));
			// Cmd: Play again with the current players
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GamePlayAgainCmd::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayAgainCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GamePlayAgainRes::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayAgainRes::ParseMessageToMessageBase));
			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GamePlayAgainS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayAgainS2CEvt::ParseMessageToMessageBase));
			// Cmd: Player. revive himself
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GameRevealPlayerCmd::MID.IDSeq.MsgID,&Message::GameInstance::GameRevealPlayerCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GameRevealPlayerRes::MID.IDSeq.MsgID,&Message::GameInstance::GameRevealPlayerRes::ParseMessageToMessageBase));
			// Cmd: Player. revive himself
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GamePlayerReviveCmd::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerReviveCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GamePlayerReviveRes::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerReviveRes::ParseMessageToMessageBase));
			// S2C: Player is revived
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GamePlayerRevivedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerRevivedS2CEvt::ParseMessageToMessageBase));
		}; // void RegisterParserGameInstance()


	}; // namespace Protocol
}; // namespace SF


