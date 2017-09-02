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

		extern std::unordered_map<uint32_t,HandleParseMessageTo> MessageParseToVariableMap;
		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserGameParty()
		{
 			// Cmd: Join Party
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::JoinPartyCmd::MID.IDSeq.MsgID,&Message::GameParty::JoinPartyCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::JoinPartyCmd::MID.IDSeq.MsgID,&Message::GameParty::JoinPartyCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::JoinPartyRes::MID.IDSeq.MsgID,&Message::GameParty::JoinPartyRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::JoinPartyRes::MID.IDSeq.MsgID,&Message::GameParty::JoinPartyRes::ParseMessageToMessageBase));
			// S2C: Player Joined
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::PlayerJoinedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PlayerJoinedS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::PlayerJoinedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PlayerJoinedS2CEvt::ParseMessageToMessageBase));
			// S2C: Player Joined
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::PartyLeaderChangedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PartyLeaderChangedS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::PartyLeaderChangedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PartyLeaderChangedS2CEvt::ParseMessageToMessageBase));
			// Cmd: Kick player
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::LeavePartyCmd::MID.IDSeq.MsgID,&Message::GameParty::LeavePartyCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::LeavePartyCmd::MID.IDSeq.MsgID,&Message::GameParty::LeavePartyCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::LeavePartyRes::MID.IDSeq.MsgID,&Message::GameParty::LeavePartyRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::LeavePartyRes::MID.IDSeq.MsgID,&Message::GameParty::LeavePartyRes::ParseMessageToMessageBase));
			// S2C: Player left
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::PlayerLeftS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PlayerLeftS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::PlayerLeftS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PlayerLeftS2CEvt::ParseMessageToMessageBase));
			// Cmd: Kick player
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::KickPlayerCmd::MID.IDSeq.MsgID,&Message::GameParty::KickPlayerCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::KickPlayerCmd::MID.IDSeq.MsgID,&Message::GameParty::KickPlayerCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::KickPlayerRes::MID.IDSeq.MsgID,&Message::GameParty::KickPlayerRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::KickPlayerRes::MID.IDSeq.MsgID,&Message::GameParty::KickPlayerRes::ParseMessageToMessageBase));
			// S2C: Player kicked
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::PlayerKickedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PlayerKickedS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::PlayerKickedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PlayerKickedS2CEvt::ParseMessageToMessageBase));
			// C2S: Chatting message
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::ChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameParty::ChatMessageC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::ChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameParty::ChatMessageC2SEvt::ParseMessageToMessageBase));
			// S2C: Chatting message
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::ChatMessageS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::ChatMessageS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::ChatMessageS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::ChatMessageS2CEvt::ParseMessageToMessageBase));
			// C2S: Quick Chatting message
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::QuickChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameParty::QuickChatMessageC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::QuickChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameParty::QuickChatMessageC2SEvt::ParseMessageToMessageBase));
			// S2C: Quick Chatting message
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::QuickChatMessageS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::QuickChatMessageS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::QuickChatMessageS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::QuickChatMessageS2CEvt::ParseMessageToMessageBase));
			// Cmd: Match
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::StartGameMatchCmd::MID.IDSeq.MsgID,&Message::GameParty::StartGameMatchCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::StartGameMatchCmd::MID.IDSeq.MsgID,&Message::GameParty::StartGameMatchCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::StartGameMatchRes::MID.IDSeq.MsgID,&Message::GameParty::StartGameMatchRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::StartGameMatchRes::MID.IDSeq.MsgID,&Message::GameParty::StartGameMatchRes::ParseMessageToMessageBase));
			// S2C: Enqueued at a game matching
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::QueuedGameMatchingS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::QueuedGameMatchingS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::QueuedGameMatchingS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::QueuedGameMatchingS2CEvt::ParseMessageToMessageBase));
			// Cmd: Match
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::CancelGameMatchCmd::MID.IDSeq.MsgID,&Message::GameParty::CancelGameMatchCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::CancelGameMatchCmd::MID.IDSeq.MsgID,&Message::GameParty::CancelGameMatchCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::CancelGameMatchRes::MID.IDSeq.MsgID,&Message::GameParty::CancelGameMatchRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::CancelGameMatchRes::MID.IDSeq.MsgID,&Message::GameParty::CancelGameMatchRes::ParseMessageToMessageBase));
			// S2C: Canceled at a game matching
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::CanceledGameMatchingS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::CanceledGameMatchingS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::CanceledGameMatchingS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::CanceledGameMatchingS2CEvt::ParseMessageToMessageBase));
			// S2C: Enqueued item is dequeued
			MessageParseToVariableMap.insert(std::make_pair(Message::GameParty::MatchingItemDequeuedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::MatchingItemDequeuedS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::MatchingItemDequeuedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::MatchingItemDequeuedS2CEvt::ParseMessageToMessageBase));
		}; // void RegisterParserGameParty()


	}; // namespace Protocol
}; // namespace SF


