////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 Kyungkun Ko
// 
// Author : Generated
// 
// Description : ChatChannelManager Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/Message/ChatChannelManagerMsgParsing.h"
#include "Protocol/Message/ChatChannelManagerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// ChatChannelManager message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserChatChannelManager()
		{
 			// Cmd: Create a channel instance
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ChatChannelManager::CreateChannelCmd::MID.IDSeq.MsgID,&Message::ChatChannelManager::CreateChannelCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ChatChannelManager::CreateChannelRes::MID.IDSeq.MsgID,&Message::ChatChannelManager::CreateChannelRes::ParseMessageToMessageBase));
			// Cmd: Find a channel instance with name
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ChatChannelManager::FindChannelCmd::MID.IDSeq.MsgID,&Message::ChatChannelManager::FindChannelCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ChatChannelManager::FindChannelRes::MID.IDSeq.MsgID,&Message::ChatChannelManager::FindChannelRes::ParseMessageToMessageBase));
			// C2S: Notification that a chat channel instance has deleted
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ChatChannelManager::ChatChannelDeletedC2SEvt::MID.IDSeq.MsgID,&Message::ChatChannelManager::ChatChannelDeletedC2SEvt::ParseMessageToMessageBase));
		}; // void RegisterParserChatChannelManager()


	}; // namespace Protocol
}; // namespace SF


