////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : ChatChannelManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/Message/ChatChannelManagerMsgDebug.h"
#include "Protocol/Message/ChatChannelManagerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// ChatChannelManager Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceChatChannelManager()
		{
 			// Cmd: Create a channel instance
			MessageDebugTraceMap.insert(std::make_pair(Message::ChatChannelManager::CreateChannelCmd::MID.IDSeq.MsgID,&Message::ChatChannelManager::CreateChannelCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::ChatChannelManager::CreateChannelRes::MID.IDSeq.MsgID,&Message::ChatChannelManager::CreateChannelRes::TraceOut));
			// Cmd: Find a channel instance with name
			MessageDebugTraceMap.insert(std::make_pair(Message::ChatChannelManager::FindChannelCmd::MID.IDSeq.MsgID,&Message::ChatChannelManager::FindChannelCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::ChatChannelManager::FindChannelRes::MID.IDSeq.MsgID,&Message::ChatChannelManager::FindChannelRes::TraceOut));
			// C2S: Notification that a chat channel instance has deleted
			MessageDebugTraceMap.insert(std::make_pair(Message::ChatChannelManager::ChatChannelDeletedC2SEvt::MID.IDSeq.MsgID,&Message::ChatChannelManager::ChatChannelDeletedC2SEvt::TraceOut));
		}; // void RegisterDebugTraceChatChannelManager()


	}; // namespace Protocol
}; // namespace SF


