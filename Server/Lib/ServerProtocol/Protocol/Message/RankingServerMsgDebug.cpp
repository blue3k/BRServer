////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : RankingServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/Message/RankingServerMsgDebug.h"
#include "Protocol/Message/RankingServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// RankingServer Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceRankingServer()
		{
 			// Cmd: Add a player to ranking
			MessageDebugTraceMap.insert(std::make_pair(Message::RankingServer::AddPlayerCmd::MID.IDSeq.MsgID,&Message::RankingServer::AddPlayerCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::RankingServer::AddPlayerRes::MID.IDSeq.MsgID,&Message::RankingServer::AddPlayerRes::TraceOut));
			// Cmd: Remove a player to ranking
			MessageDebugTraceMap.insert(std::make_pair(Message::RankingServer::RemovePlayerCmd::MID.IDSeq.MsgID,&Message::RankingServer::RemovePlayerCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::RankingServer::RemovePlayerRes::MID.IDSeq.MsgID,&Message::RankingServer::RemovePlayerRes::TraceOut));
			// Cmd: Get player to ranking
			MessageDebugTraceMap.insert(std::make_pair(Message::RankingServer::GetPlayerRankingCmd::MID.IDSeq.MsgID,&Message::RankingServer::GetPlayerRankingCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::RankingServer::GetPlayerRankingRes::MID.IDSeq.MsgID,&Message::RankingServer::GetPlayerRankingRes::TraceOut));
			// Cmd: Update a player to ranking
			MessageDebugTraceMap.insert(std::make_pair(Message::RankingServer::UpdatePlayerScoreCmd::MID.IDSeq.MsgID,&Message::RankingServer::UpdatePlayerScoreCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::RankingServer::UpdatePlayerScoreRes::MID.IDSeq.MsgID,&Message::RankingServer::UpdatePlayerScoreRes::TraceOut));
			// Cmd: Get ranking list
			MessageDebugTraceMap.insert(std::make_pair(Message::RankingServer::GetRankingCmd::MID.IDSeq.MsgID,&Message::RankingServer::GetRankingCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::RankingServer::GetRankingRes::MID.IDSeq.MsgID,&Message::RankingServer::GetRankingRes::TraceOut));
			// Cmd: Debug test
			MessageDebugTraceMap.insert(std::make_pair(Message::RankingServer::DebugPrintALLRankingCmd::MID.IDSeq.MsgID,&Message::RankingServer::DebugPrintALLRankingCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::RankingServer::DebugPrintALLRankingRes::MID.IDSeq.MsgID,&Message::RankingServer::DebugPrintALLRankingRes::TraceOut));
		}; // void RegisterDebugTraceRankingServer()


	}; // namespace Protocol
}; // namespace SF


