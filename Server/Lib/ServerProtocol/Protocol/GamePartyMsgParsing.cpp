////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameParty Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/GamePartyMsgParsing.h"
#include "Protocol/GamePartyMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// GameParty message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserGameParty()
		{
 			// Cmd: Join party
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::JoinPartyCmd::MID.IDSeq.MsgID,&Message::GameParty::JoinPartyCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::JoinPartyRes::MID.IDSeq.MsgID,&Message::GameParty::JoinPartyRes::ParseMessageToMessageBase));
			// S2C: Player joined event.
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::PlayerJoinedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PlayerJoinedS2CEvt::ParseMessageToMessageBase));
			// S2C: Event for party leader is changed
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::PartyLeaderChangedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PartyLeaderChangedS2CEvt::ParseMessageToMessageBase));
			// Cmd: Event for player left.
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::LeavePartyCmd::MID.IDSeq.MsgID,&Message::GameParty::LeavePartyCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::LeavePartyRes::MID.IDSeq.MsgID,&Message::GameParty::LeavePartyRes::ParseMessageToMessageBase));
			// S2C: Player left
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::PlayerLeftS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PlayerLeftS2CEvt::ParseMessageToMessageBase));
			// Cmd: Kick player
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::KickPlayerCmd::MID.IDSeq.MsgID,&Message::GameParty::KickPlayerCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::KickPlayerRes::MID.IDSeq.MsgID,&Message::GameParty::KickPlayerRes::ParseMessageToMessageBase));
			// S2C: Event for Player kicked.
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::PlayerKickedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PlayerKickedS2CEvt::ParseMessageToMessageBase));
			// C2S: Send chat message to server.
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::ChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameParty::ChatMessageC2SEvt::ParseMessageToMessageBase));
			// S2C: Brocast chatting message
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::ChatMessageS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::ChatMessageS2CEvt::ParseMessageToMessageBase));
			// C2S: Quick Chatting message
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::QuickChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameParty::QuickChatMessageC2SEvt::ParseMessageToMessageBase));
			// S2C: Quick Chatting message
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::QuickChatMessageS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::QuickChatMessageS2CEvt::ParseMessageToMessageBase));
			// Cmd: Start party game matching
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::StartGameMatchCmd::MID.IDSeq.MsgID,&Message::GameParty::StartGameMatchCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::StartGameMatchRes::MID.IDSeq.MsgID,&Message::GameParty::StartGameMatchRes::ParseMessageToMessageBase));
			// S2C: Event sent when the party is queued for game matching
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::QueuedGameMatchingS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::QueuedGameMatchingS2CEvt::ParseMessageToMessageBase));
			// Cmd: Cancel game matching
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::CancelGameMatchCmd::MID.IDSeq.MsgID,&Message::GameParty::CancelGameMatchCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::CancelGameMatchRes::MID.IDSeq.MsgID,&Message::GameParty::CancelGameMatchRes::ParseMessageToMessageBase));
			// S2C: Event sent when the game matching is canceled
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::CanceledGameMatchingS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::CanceledGameMatchingS2CEvt::ParseMessageToMessageBase));
			// S2C: Event sent when the the matching has dequeued.
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameParty::MatchingItemDequeuedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::MatchingItemDequeuedS2CEvt::ParseMessageToMessageBase));
		}; // void RegisterParserGameParty()


	}; // namespace Protocol
}; // namespace SF


