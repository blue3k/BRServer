////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : ChatChannel Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/ChatChannelMsgDebug.h"
#include "Protocol/ChatChannelMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// ChatChannel Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceChatChannel()
		{
 			// Cmd: Request to join chat channel
			MessageDebugTraceMap.insert(std::make_pair(Message::ChatChannel::JoinCmd::MID.IDSeq.MsgID,&Message::ChatChannel::JoinCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::ChatChannel::JoinRes::MID.IDSeq.MsgID,&Message::ChatChannel::JoinRes::TraceOut));
			// S2C: Server envent to notify joined player information
			MessageDebugTraceMap.insert(std::make_pair(Message::ChatChannel::PlayerJoinedS2CEvt::MID.IDSeq.MsgID,&Message::ChatChannel::PlayerJoinedS2CEvt::TraceOut));
			// S2C: Chat channel leader changed
			MessageDebugTraceMap.insert(std::make_pair(Message::ChatChannel::LeaderChangedS2CEvt::MID.IDSeq.MsgID,&Message::ChatChannel::LeaderChangedS2CEvt::TraceOut));
			// Cmd: Leave chat channel
			MessageDebugTraceMap.insert(std::make_pair(Message::ChatChannel::LeaveCmd::MID.IDSeq.MsgID,&Message::ChatChannel::LeaveCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::ChatChannel::LeaveRes::MID.IDSeq.MsgID,&Message::ChatChannel::LeaveRes::TraceOut));
			// S2C: Notification event when a player left
			MessageDebugTraceMap.insert(std::make_pair(Message::ChatChannel::PlayerLeftS2CEvt::MID.IDSeq.MsgID,&Message::ChatChannel::PlayerLeftS2CEvt::TraceOut));
			// Cmd: Kick a player
			MessageDebugTraceMap.insert(std::make_pair(Message::ChatChannel::KickPlayerCmd::MID.IDSeq.MsgID,&Message::ChatChannel::KickPlayerCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::ChatChannel::KickPlayerRes::MID.IDSeq.MsgID,&Message::ChatChannel::KickPlayerRes::TraceOut));
			// S2C: Notification event when a player kicked
			MessageDebugTraceMap.insert(std::make_pair(Message::ChatChannel::PlayerKickedS2CEvt::MID.IDSeq.MsgID,&Message::ChatChannel::PlayerKickedS2CEvt::TraceOut));
			// C2S: Sending a chatting message
			MessageDebugTraceMap.insert(std::make_pair(Message::ChatChannel::ChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::ChatChannel::ChatMessageC2SEvt::TraceOut));
			// S2C: brocasting event for a chatting message
			MessageDebugTraceMap.insert(std::make_pair(Message::ChatChannel::ChatMessageS2CEvt::MID.IDSeq.MsgID,&Message::ChatChannel::ChatMessageS2CEvt::TraceOut));
		}; // void RegisterDebugTraceChatChannel()


	}; // namespace Protocol
}; // namespace SF


