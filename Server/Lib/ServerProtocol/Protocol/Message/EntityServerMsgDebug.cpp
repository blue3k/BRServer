////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : EntityServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/EntityServerMsgDebug.h"
#include "Protocol/Message/EntityServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// EntityServer Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceEntityServer()
		{
 			// Cmd: Register entity
			MessageDebugTraceMap.insert(std::make_pair(Message::EntityServer::RegisterEntityCmd::MID.IDSeq.MsgID,&Message::EntityServer::RegisterEntityCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::EntityServer::RegisterEntityRes::MID.IDSeq.MsgID,&Message::EntityServer::RegisterEntityRes::TraceOut));
			// Cmd: Find Entity
			MessageDebugTraceMap.insert(std::make_pair(Message::EntityServer::UnregisterEntityCmd::MID.IDSeq.MsgID,&Message::EntityServer::UnregisterEntityCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::EntityServer::UnregisterEntityRes::MID.IDSeq.MsgID,&Message::EntityServer::UnregisterEntityRes::TraceOut));
			// Cmd: Find Entity
			MessageDebugTraceMap.insert(std::make_pair(Message::EntityServer::FindEntityCmd::MID.IDSeq.MsgID,&Message::EntityServer::FindEntityCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::EntityServer::FindEntityRes::MID.IDSeq.MsgID,&Message::EntityServer::FindEntityRes::TraceOut));
		}; // void RegisterDebugTraceEntityServer()


	}; // namespace Protocol
}; // namespace SF


