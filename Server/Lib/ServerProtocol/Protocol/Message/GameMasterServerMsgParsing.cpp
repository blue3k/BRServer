////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : Generated
// 
// Description : GameMasterServer Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/GameMasterServerMsgParsing.h"
#include "Protocol/Message/GameMasterServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// GameMasterServer message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageTo> MessageParseToVariableMap;
		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserGameMasterServer()
		{
 			// C2S: Player entered
			MessageParseToVariableMap.insert(std::make_pair(Message::GameMasterServer::PlayerEnteredC2SEvt::MID.IDSeq.MsgID,&Message::GameMasterServer::PlayerEnteredC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameMasterServer::PlayerEnteredC2SEvt::MID.IDSeq.MsgID,&Message::GameMasterServer::PlayerEnteredC2SEvt::ParseMessageToMessageBase));
			// C2S: Player leaved
			MessageParseToVariableMap.insert(std::make_pair(Message::GameMasterServer::PlayerLeavedC2SEvt::MID.IDSeq.MsgID,&Message::GameMasterServer::PlayerLeavedC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameMasterServer::PlayerLeavedC2SEvt::MID.IDSeq.MsgID,&Message::GameMasterServer::PlayerLeavedC2SEvt::ParseMessageToMessageBase));
		}; // void RegisterParserGameMasterServer()


	}; // namespace Protocol
}; // namespace SF


