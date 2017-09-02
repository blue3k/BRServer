﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : Server Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/ServerMsgDebug.h"
#include "Protocol/Message/ServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// Server Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceServer()
		{
 			// Cmd: 
			MessageDebugTraceMap.insert(std::make_pair(Message::Server::GenericFailureCmd::MID.IDSeq.MsgID,&Message::Server::GenericFailureCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::Server::GenericFailureRes::MID.IDSeq.MsgID,&Message::Server::GenericFailureRes::TraceOut));
			// C2S: Server Started or Connected
			MessageDebugTraceMap.insert(std::make_pair(Message::Server::ServerConnectedC2SEvt::MID.IDSeq.MsgID,&Message::Server::ServerConnectedC2SEvt::TraceOut));
		}; // void RegisterDebugTraceServer()


	}; // namespace Protocol
}; // namespace SF


