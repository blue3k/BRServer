////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 Kyungkun Ko
// 
// Author : Generated
// 
// Description : ChatChannel Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/ChatChannelMsgParsing.h"
#include "Protocol/ChatChannelMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// ChatChannel message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserChatChannel()
		{
 			// Cmd: Request to join chat channel
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ChatChannel::JoinCmd::MID.IDSeq.MsgID,&Message::ChatChannel::JoinCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ChatChannel::JoinRes::MID.IDSeq.MsgID,&Message::ChatChannel::JoinRes::ParseMessageToMessageBase));
			// S2C: Server envent to notify joined player information
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ChatChannel::PlayerJoinedS2CEvt::MID.IDSeq.MsgID,&Message::ChatChannel::PlayerJoinedS2CEvt::ParseMessageToMessageBase));
			// S2C: Chat channel leader changed
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ChatChannel::LeaderChangedS2CEvt::MID.IDSeq.MsgID,&Message::ChatChannel::LeaderChangedS2CEvt::ParseMessageToMessageBase));
			// Cmd: Leave chat channel
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ChatChannel::LeaveCmd::MID.IDSeq.MsgID,&Message::ChatChannel::LeaveCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ChatChannel::LeaveRes::MID.IDSeq.MsgID,&Message::ChatChannel::LeaveRes::ParseMessageToMessageBase));
			// S2C: Notification event when a player left
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ChatChannel::PlayerLeftS2CEvt::MID.IDSeq.MsgID,&Message::ChatChannel::PlayerLeftS2CEvt::ParseMessageToMessageBase));
			// Cmd: Kick a player
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ChatChannel::KickPlayerCmd::MID.IDSeq.MsgID,&Message::ChatChannel::KickPlayerCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ChatChannel::KickPlayerRes::MID.IDSeq.MsgID,&Message::ChatChannel::KickPlayerRes::ParseMessageToMessageBase));
			// S2C: Notification event when a player kicked
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ChatChannel::PlayerKickedS2CEvt::MID.IDSeq.MsgID,&Message::ChatChannel::PlayerKickedS2CEvt::ParseMessageToMessageBase));
			// C2S: Sending a chatting message
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ChatChannel::ChatMessageC2SEvt::MID.IDSeq.MsgID,&Message::ChatChannel::ChatMessageC2SEvt::ParseMessageToMessageBase));
			// S2C: brocasting event for a chatting message
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ChatChannel::ChatMessageS2CEvt::MID.IDSeq.MsgID,&Message::ChatChannel::ChatMessageS2CEvt::ParseMessageToMessageBase));
		}; // void RegisterParserChatChannel()


	}; // namespace Protocol
}; // namespace SF


