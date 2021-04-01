////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : RelayServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/RelayServerMsgDebug.h"
#include "Protocol/RelayServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// RelayServer Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceRelayServer()
		{
 			// Cmd: Relay Instance
			MessageDebugTraceMap.insert(std::make_pair(Message::RelayServer::CreateRelayInstanceCmd::MID.IDSeq.MsgID,&Message::RelayServer::CreateRelayInstanceCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::RelayServer::CreateRelayInstanceRes::MID.IDSeq.MsgID,&Message::RelayServer::CreateRelayInstanceRes::TraceOut));
			// Cmd: Add a player to Relay
			MessageDebugTraceMap.insert(std::make_pair(Message::RelayServer::AddPlayerCmd::MID.IDSeq.MsgID,&Message::RelayServer::AddPlayerCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::RelayServer::AddPlayerRes::MID.IDSeq.MsgID,&Message::RelayServer::AddPlayerRes::TraceOut));
			// Cmd: Remove a player to Relay
			MessageDebugTraceMap.insert(std::make_pair(Message::RelayServer::RemovePlayerCmd::MID.IDSeq.MsgID,&Message::RelayServer::RemovePlayerCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::RelayServer::RemovePlayerRes::MID.IDSeq.MsgID,&Message::RelayServer::RemovePlayerRes::TraceOut));
		}; // void RegisterDebugTraceRelayServer()


	}; // namespace Protocol
}; // namespace SF


