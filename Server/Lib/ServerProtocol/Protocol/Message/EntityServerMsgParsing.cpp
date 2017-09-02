////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : Generated
// 
// Description : EntityServer Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/EntityServerMsgParsing.h"
#include "Protocol/Message/EntityServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// EntityServer message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageTo> MessageParseToVariableMap;
		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserEntityServer()
		{
 			// Cmd: Register entity
			MessageParseToVariableMap.insert(std::make_pair(Message::EntityServer::RegisterEntityCmd::MID.IDSeq.MsgID,&Message::EntityServer::RegisterEntityCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::EntityServer::RegisterEntityCmd::MID.IDSeq.MsgID,&Message::EntityServer::RegisterEntityCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::EntityServer::RegisterEntityRes::MID.IDSeq.MsgID,&Message::EntityServer::RegisterEntityRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::EntityServer::RegisterEntityRes::MID.IDSeq.MsgID,&Message::EntityServer::RegisterEntityRes::ParseMessageToMessageBase));
			// Cmd: Find Entity
			MessageParseToVariableMap.insert(std::make_pair(Message::EntityServer::UnregisterEntityCmd::MID.IDSeq.MsgID,&Message::EntityServer::UnregisterEntityCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::EntityServer::UnregisterEntityCmd::MID.IDSeq.MsgID,&Message::EntityServer::UnregisterEntityCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::EntityServer::UnregisterEntityRes::MID.IDSeq.MsgID,&Message::EntityServer::UnregisterEntityRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::EntityServer::UnregisterEntityRes::MID.IDSeq.MsgID,&Message::EntityServer::UnregisterEntityRes::ParseMessageToMessageBase));
			// Cmd: Find Entity
			MessageParseToVariableMap.insert(std::make_pair(Message::EntityServer::FindEntityCmd::MID.IDSeq.MsgID,&Message::EntityServer::FindEntityCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::EntityServer::FindEntityCmd::MID.IDSeq.MsgID,&Message::EntityServer::FindEntityCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::EntityServer::FindEntityRes::MID.IDSeq.MsgID,&Message::EntityServer::FindEntityRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::EntityServer::FindEntityRes::MID.IDSeq.MsgID,&Message::EntityServer::FindEntityRes::ParseMessageToMessageBase));
		}; // void RegisterParserEntityServer()


	}; // namespace Protocol
}; // namespace SF


