////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GamePartyManager Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/GamePartyManagerMsgParsing.h"
#include "Protocol/GamePartyManagerMsgClass.h"



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
			// C2S: Party instance notify of deletion. Sent by party instance
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GamePartyManager::PartyDeletedC2SEvt::MID.IDSeq.MsgID,&Message::GamePartyManager::PartyDeletedC2SEvt::ParseMessageToMessageBase));
		}; // void RegisterParserGamePartyManager()


	}; // namespace Protocol
}; // namespace SF


