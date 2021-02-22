////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameInstanceManager Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
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
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstanceManager::CreateGameInstanceCmd::MID.IDSeq.MsgID,&Message::GameInstanceManager::CreateGameInstanceCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstanceManager::CreateGameInstanceRes::MID.IDSeq.MsgID,&Message::GameInstanceManager::CreateGameInstanceRes::ParseMessageToMessageBase));
		}; // void RegisterParserGameInstanceManager()


	}; // namespace Protocol
}; // namespace SF


