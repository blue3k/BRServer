////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : LoginServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/LoginServerMsgDebug.h"
#include "Protocol/Message/LoginServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// LoginServer Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceLoginServer()
		{
 			// Cmd: Notify user joind and see it's valid authticket instance
			MessageDebugTraceMap.insert(std::make_pair(Message::LoginServer::PlayerJoinedToGameServerCmd::MID.IDSeq.MsgID,&Message::LoginServer::PlayerJoinedToGameServerCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::LoginServer::PlayerJoinedToGameServerRes::MID.IDSeq.MsgID,&Message::LoginServer::PlayerJoinedToGameServerRes::TraceOut));
			// Cmd: Kick logged in player
			MessageDebugTraceMap.insert(std::make_pair(Message::LoginServer::KickPlayerCmd::MID.IDSeq.MsgID,&Message::LoginServer::KickPlayerCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::LoginServer::KickPlayerRes::MID.IDSeq.MsgID,&Message::LoginServer::KickPlayerRes::TraceOut));
		}; // void RegisterDebugTraceLoginServer()


	}; // namespace Protocol
}; // namespace SF


