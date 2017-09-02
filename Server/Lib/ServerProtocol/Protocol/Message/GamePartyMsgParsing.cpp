////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : Generated
// 
// Description : GameParty Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/GamePartyMsgParsing.h"
#include "Protocol/Message/GamePartyMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// GameParty message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserGameParty()
		{
 			// Cmd: Join Party
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::JoinPartyCmd::MID.IDSeq.MsgID,&Message::GameParty::JoinPartyCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::JoinPartyRes::MID.IDSeq.MsgID,&Message::GameParty::JoinPartyRes::ParseMessageToMessageBase));
			// S2C: Player Joined
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::PlayerJoinedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PlayerJoinedS2CEvt::ParseMessageToMessageBase));
			// S2C: Player Joined
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::PartyLeaderChangedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PartyLeaderChangedS2CEvt::ParseMessageToMessageBase));
			// Cmd: Kick player
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::LeavePartyCmd::MID.IDSeq.MsgID,&Message::GameParty::LeavePartyCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::LeavePartyRes::MID.IDSeq.MsgID,&Message::GameParty::LeavePartyRes::ParseMessageToMessageBase));
			// S2C: Player left
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::PlayerLeftS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PlayerLeftS2CEvt::ParseMessageToMessageBase));
			// Cmd: Kick player
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::KickPlayerCmd::MID.IDSeq.MsgID,&Message::GameParty::KickPlayerCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::KickPlayerRes::MID.IDSeq.MsgID,&Message::GameParty::KickPlayerRes::ParseMessageToMessageBase));
			// S2C: Player kicked
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::PlayerKickedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PlayerKickedS2CEvt::ParseMessageToMessageBase));
			// C2S: Chatting message
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::ChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameParty::ChatMessageC2SEvt::ParseMessageToMessageBase));
			// S2C: Chatting message
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::ChatMessageS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::ChatMessageS2CEvt::ParseMessageToMessageBase));
			// C2S: Quick Chatting message
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::QuickChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameParty::QuickChatMessageC2SEvt::ParseMessageToMessageBase));
			// S2C: Quick Chatting message
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::QuickChatMessageS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::QuickChatMessageS2CEvt::ParseMessageToMessageBase));
			// Cmd: Match
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::StartGameMatchCmd::MID.IDSeq.MsgID,&Message::GameParty::StartGameMatchCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::StartGameMatchRes::MID.IDSeq.MsgID,&Message::GameParty::StartGameMatchRes::ParseMessageToMessageBase));
			// S2C: Enqueued at a game matching
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::QueuedGameMatchingS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::QueuedGameMatchingS2CEvt::ParseMessageToMessageBase));
			// Cmd: Match
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::CancelGameMatchCmd::MID.IDSeq.MsgID,&Message::GameParty::CancelGameMatchCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::CancelGameMatchRes::MID.IDSeq.MsgID,&Message::GameParty::CancelGameMatchRes::ParseMessageToMessageBase));
			// S2C: Canceled at a game matching
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::CanceledGameMatchingS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::CanceledGameMatchingS2CEvt::ParseMessageToMessageBase));
			// S2C: Enqueued item is dequeued
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::MatchingItemDequeuedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::MatchingItemDequeuedS2CEvt::ParseMessageToMessageBase));
		}; // void RegisterParserGameParty()


	}; // namespace Protocol
}; // namespace SF


