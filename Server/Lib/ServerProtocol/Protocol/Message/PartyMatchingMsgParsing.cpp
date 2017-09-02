////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : Generated
// 
// Description : PartyMatching Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/PartyMatchingMsgParsing.h"
#include "Protocol/Message/PartyMatchingMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// PartyMatching message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserPartyMatching()
		{
 			// S2C: 
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatching::PartyGameMatchedS2CEvt::MID.IDSeq.MsgID,&Message::PartyMatching::PartyGameMatchedS2CEvt::ParseMessageToMessageBase));
			// S2C: 
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatching::PlayerGameMatchedS2CEvt::MID.IDSeq.MsgID,&Message::PartyMatching::PlayerGameMatchedS2CEvt::ParseMessageToMessageBase));
		}; // void RegisterParserPartyMatching()


	}; // namespace Protocol
}; // namespace SF


