////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameParty Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/GamePartyMsgDebug.h"
#include "Protocol/GamePartyMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// GameParty Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceGameParty()
		{
 			// Cmd: Join party
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::JoinPartyCmd::MID.IDSeq.MsgID,&Message::GameParty::JoinPartyCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::JoinPartyRes::MID.IDSeq.MsgID,&Message::GameParty::JoinPartyRes::TraceOut));
			// S2C: Player joined event.
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::PlayerJoinedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PlayerJoinedS2CEvt::TraceOut));
			// S2C: Event for party leader is changed
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::PartyLeaderChangedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PartyLeaderChangedS2CEvt::TraceOut));
			// Cmd: Event for player left.
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::LeavePartyCmd::MID.IDSeq.MsgID,&Message::GameParty::LeavePartyCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::LeavePartyRes::MID.IDSeq.MsgID,&Message::GameParty::LeavePartyRes::TraceOut));
			// S2C: Player left
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::PlayerLeftS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PlayerLeftS2CEvt::TraceOut));
			// Cmd: Kick player
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::KickPlayerCmd::MID.IDSeq.MsgID,&Message::GameParty::KickPlayerCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::KickPlayerRes::MID.IDSeq.MsgID,&Message::GameParty::KickPlayerRes::TraceOut));
			// S2C: Event for Player kicked.
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::PlayerKickedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::PlayerKickedS2CEvt::TraceOut));
			// C2S: Send chat message to server.
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::ChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameParty::ChatMessageC2SEvt::TraceOut));
			// S2C: Brocast chatting message
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::ChatMessageS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::ChatMessageS2CEvt::TraceOut));
			// C2S: Quick Chatting message
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::QuickChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::GameParty::QuickChatMessageC2SEvt::TraceOut));
			// S2C: Quick Chatting message
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::QuickChatMessageS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::QuickChatMessageS2CEvt::TraceOut));
			// Cmd: Start party game matching
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::StartGameMatchCmd::MID.IDSeq.MsgID,&Message::GameParty::StartGameMatchCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::StartGameMatchRes::MID.IDSeq.MsgID,&Message::GameParty::StartGameMatchRes::TraceOut));
			// S2C: Event sent when the party is queued for game matching
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::QueuedGameMatchingS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::QueuedGameMatchingS2CEvt::TraceOut));
			// Cmd: Cancel game matching
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::CancelGameMatchCmd::MID.IDSeq.MsgID,&Message::GameParty::CancelGameMatchCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::CancelGameMatchRes::MID.IDSeq.MsgID,&Message::GameParty::CancelGameMatchRes::TraceOut));
			// S2C: Event sent when the game matching is canceled
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::CanceledGameMatchingS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::CanceledGameMatchingS2CEvt::TraceOut));
			// S2C: Event sent when the the matching has dequeued.
			MessageDebugTraceMap.insert(std::make_pair(Message::GameParty::MatchingItemDequeuedS2CEvt::MID.IDSeq.MsgID,&Message::GameParty::MatchingItemDequeuedS2CEvt::TraceOut));
		}; // void RegisterDebugTraceGameParty()


	}; // namespace Protocol
}; // namespace SF


