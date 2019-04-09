////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 Kyungkun Ko
// 
// Author : Generated
// 
// Description : RankingServer Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/Message/RankingServerMsgParsing.h"
#include "Protocol/Message/RankingServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// RankingServer message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserRankingServer()
		{
 			// Cmd: Add a player to ranking
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::AddPlayerCmd::MID.IDSeq.MsgID,&Message::RankingServer::AddPlayerCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::AddPlayerRes::MID.IDSeq.MsgID,&Message::RankingServer::AddPlayerRes::ParseMessageToMessageBase));
			// Cmd: Remove a player to ranking
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::RemovePlayerCmd::MID.IDSeq.MsgID,&Message::RankingServer::RemovePlayerCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::RemovePlayerRes::MID.IDSeq.MsgID,&Message::RankingServer::RemovePlayerRes::ParseMessageToMessageBase));
			// Cmd: Get player to ranking
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::GetPlayerRankingCmd::MID.IDSeq.MsgID,&Message::RankingServer::GetPlayerRankingCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::GetPlayerRankingRes::MID.IDSeq.MsgID,&Message::RankingServer::GetPlayerRankingRes::ParseMessageToMessageBase));
			// Cmd: Update a player to ranking
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::UpdatePlayerScoreCmd::MID.IDSeq.MsgID,&Message::RankingServer::UpdatePlayerScoreCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::UpdatePlayerScoreRes::MID.IDSeq.MsgID,&Message::RankingServer::UpdatePlayerScoreRes::ParseMessageToMessageBase));
			// Cmd: Get ranking list
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::GetRankingCmd::MID.IDSeq.MsgID,&Message::RankingServer::GetRankingCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::GetRankingRes::MID.IDSeq.MsgID,&Message::RankingServer::GetRankingRes::ParseMessageToMessageBase));
			// Cmd: Debug test
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::DebugPrintALLRankingCmd::MID.IDSeq.MsgID,&Message::RankingServer::DebugPrintALLRankingCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::RankingServer::DebugPrintALLRankingRes::MID.IDSeq.MsgID,&Message::RankingServer::DebugPrintALLRankingRes::ParseMessageToMessageBase));
		}; // void RegisterParserRankingServer()


	}; // namespace Protocol
}; // namespace SF


