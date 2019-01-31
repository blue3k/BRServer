////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GamePartyManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/Message/GamePartyManagerMsgDebug.h"
#include "Protocol/Message/GamePartyManagerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// GamePartyManager Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceGamePartyManager()
		{
 			// Cmd: Create a party instance
			MessageDebugTraceMap.insert(std::make_pair(Message::GamePartyManager::CreatePartyCmd::MID.IDSeq.MsgID,&Message::GamePartyManager::CreatePartyCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::GamePartyManager::CreatePartyRes::MID.IDSeq.MsgID,&Message::GamePartyManager::CreatePartyRes::TraceOut));
			// C2S: Party instance notify of deletion. Sent by party instance
			MessageDebugTraceMap.insert(std::make_pair(Message::GamePartyManager::PartyDeletedC2SEvt::MID.IDSeq.MsgID,&Message::GamePartyManager::PartyDeletedC2SEvt::TraceOut));
		}; // void RegisterDebugTraceGamePartyManager()


	}; // namespace Protocol
}; // namespace SF


