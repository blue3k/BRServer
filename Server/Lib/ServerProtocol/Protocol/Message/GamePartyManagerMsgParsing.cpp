////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : Generated
// 
// Description : GamePartyManager Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/GamePartyManagerMsgParsing.h"
#include "Protocol/Message/GamePartyManagerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// GamePartyManager message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserGamePartyManager()
		{
 			// Cmd: Create a party instance
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GamePartyManager::CreatePartyCmd::MID.IDSeq.MsgID,&Message::GamePartyManager::CreatePartyCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GamePartyManager::CreatePartyRes::MID.IDSeq.MsgID,&Message::GamePartyManager::CreatePartyRes::ParseMessageToMessageBase));
			// C2S: Party instance notify of deletion
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GamePartyManager::PartyDeletedC2SEvt::MID.IDSeq.MsgID,&Message::GamePartyManager::PartyDeletedC2SEvt::ParseMessageToMessageBase));
		}; // void RegisterParserGamePartyManager()


	}; // namespace Protocol
}; // namespace SF


