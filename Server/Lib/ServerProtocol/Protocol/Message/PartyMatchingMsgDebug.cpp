////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : PartyMatching Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/PartyMatchingMsgDebug.h"
#include "Protocol/Message/PartyMatchingMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// PartyMatching Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTracePartyMatching()
		{
 			// S2C: 
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatching::PartyGameMatchedS2CEvt::MID.IDSeq.MsgID,&Message::PartyMatching::PartyGameMatchedS2CEvt::TraceOut));
			// S2C: 
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatching::PlayerGameMatchedS2CEvt::MID.IDSeq.MsgID,&Message::PartyMatching::PlayerGameMatchedS2CEvt::TraceOut));
		}; // void RegisterDebugTracePartyMatching()


	}; // namespace Protocol
}; // namespace SF


