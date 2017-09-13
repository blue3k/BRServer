﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : Generated
// 
// Description : GameInstanceManager Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/GameInstanceManagerMsgParsing.h"
#include "Protocol/Message/GameInstanceManagerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// GameInstanceManager message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserGameInstanceManager()
		{
 			// Cmd: Create a game instance
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstanceManager::CreateGameCmd::MID.IDSeq.MsgID,&Message::GameInstanceManager::CreateGameCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstanceManager::CreateGameRes::MID.IDSeq.MsgID,&Message::GameInstanceManager::CreateGameRes::ParseMessageToMessageBase));
			// C2S: Game instance notify of deletion
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstanceManager::GameDeletedC2SEvt::MID.IDSeq.MsgID,&Message::GameInstanceManager::GameDeletedC2SEvt::ParseMessageToMessageBase));
		}; // void RegisterParserGameInstanceManager()


	}; // namespace Protocol
}; // namespace SF

