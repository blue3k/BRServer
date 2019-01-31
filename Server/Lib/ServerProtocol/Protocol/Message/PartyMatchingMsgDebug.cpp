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
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
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
 			// S2C: Event message will be sent from matching srvice when the pary game mathcing has done.
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatching::PartyGameMatchedS2CEvt::MID.IDSeq.MsgID,&Message::PartyMatching::PartyGameMatchedS2CEvt::TraceOut));
			// S2C: Event message will be sent when a player game matching has successfully finished.
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatching::PlayerGameMatchedS2CEvt::MID.IDSeq.MsgID,&Message::PartyMatching::PlayerGameMatchedS2CEvt::TraceOut));
		}; // void RegisterDebugTracePartyMatching()


	}; // namespace Protocol
}; // namespace SF


