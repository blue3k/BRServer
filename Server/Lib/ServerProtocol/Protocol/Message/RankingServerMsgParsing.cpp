////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : Generated
// 
// Description : RankingServer Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/RankingServerMsgParsing.h"
#include "Protocol/Message/RankingServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// RankingServer message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageTo> MessageParseToVariableMap;
		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserRankingServer()
		{
 			// Cmd: Add a player to ranking
			MessageParseToVariableMap.insert(std::make_pair(Message::RankingServer::AddPlayerCmd::MID.IDSeq.MsgID,&Message::RankingServer::AddPlayerCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::AddPlayerCmd::MID.IDSeq.MsgID,&Message::RankingServer::AddPlayerCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::RankingServer::AddPlayerRes::MID.IDSeq.MsgID,&Message::RankingServer::AddPlayerRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::AddPlayerRes::MID.IDSeq.MsgID,&Message::RankingServer::AddPlayerRes::ParseMessageToMessageBase));
			// Cmd: Remove a player to ranking
			MessageParseToVariableMap.insert(std::make_pair(Message::RankingServer::RemovePlayerCmd::MID.IDSeq.MsgID,&Message::RankingServer::RemovePlayerCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::RemovePlayerCmd::MID.IDSeq.MsgID,&Message::RankingServer::RemovePlayerCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::RankingServer::RemovePlayerRes::MID.IDSeq.MsgID,&Message::RankingServer::RemovePlayerRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::RemovePlayerRes::MID.IDSeq.MsgID,&Message::RankingServer::RemovePlayerRes::ParseMessageToMessageBase));
			// Cmd: Remove a player to ranking
			MessageParseToVariableMap.insert(std::make_pair(Message::RankingServer::GetPlayerRankingCmd::MID.IDSeq.MsgID,&Message::RankingServer::GetPlayerRankingCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::GetPlayerRankingCmd::MID.IDSeq.MsgID,&Message::RankingServer::GetPlayerRankingCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::RankingServer::GetPlayerRankingRes::MID.IDSeq.MsgID,&Message::RankingServer::GetPlayerRankingRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::GetPlayerRankingRes::MID.IDSeq.MsgID,&Message::RankingServer::GetPlayerRankingRes::ParseMessageToMessageBase));
			// Cmd: Update a player to ranking
			MessageParseToVariableMap.insert(std::make_pair(Message::RankingServer::UpdatePlayerScoreCmd::MID.IDSeq.MsgID,&Message::RankingServer::UpdatePlayerScoreCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::UpdatePlayerScoreCmd::MID.IDSeq.MsgID,&Message::RankingServer::UpdatePlayerScoreCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::RankingServer::UpdatePlayerScoreRes::MID.IDSeq.MsgID,&Message::RankingServer::UpdatePlayerScoreRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::UpdatePlayerScoreRes::MID.IDSeq.MsgID,&Message::RankingServer::UpdatePlayerScoreRes::ParseMessageToMessageBase));
			// Cmd: Get ranking list
			MessageParseToVariableMap.insert(std::make_pair(Message::RankingServer::GetRankingCmd::MID.IDSeq.MsgID,&Message::RankingServer::GetRankingCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::GetRankingCmd::MID.IDSeq.MsgID,&Message::RankingServer::GetRankingCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::RankingServer::GetRankingRes::MID.IDSeq.MsgID,&Message::RankingServer::GetRankingRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::GetRankingRes::MID.IDSeq.MsgID,&Message::RankingServer::GetRankingRes::ParseMessageToMessageBase));
			// Cmd: Debug test
			MessageParseToVariableMap.insert(std::make_pair(Message::RankingServer::DebugPrintALLRankingCmd::MID.IDSeq.MsgID,&Message::RankingServer::DebugPrintALLRankingCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::DebugPrintALLRankingCmd::MID.IDSeq.MsgID,&Message::RankingServer::DebugPrintALLRankingCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::RankingServer::DebugPrintALLRankingRes::MID.IDSeq.MsgID,&Message::RankingServer::DebugPrintALLRankingRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::DebugPrintALLRankingRes::MID.IDSeq.MsgID,&Message::RankingServer::DebugPrintALLRankingRes::ParseMessageToMessageBase));
		}; // void RegisterParserRankingServer()


	}; // namespace Protocol
}; // namespace SF


