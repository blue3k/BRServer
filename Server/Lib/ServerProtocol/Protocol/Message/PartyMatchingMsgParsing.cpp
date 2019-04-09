////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 Kyungkun Ko
// 
// Author : Generated
// 
// Description : PartyMatching Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
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
 			// S2C: Event message will be sent from matching srvice when the pary game mathcing has done.
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatching::PartyGameMatchedS2CEvt::MID.IDSeq.MsgID,&Message::PartyMatching::PartyGameMatchedS2CEvt::ParseMessageToMessageBase));
			// S2C: Event message will be sent when a player game matching has successfully finished.
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatching::PlayerGameMatchedS2CEvt::MID.IDSeq.MsgID,&Message::PartyMatching::PlayerGameMatchedS2CEvt::ParseMessageToMessageBase));
		}; // void RegisterParserPartyMatching()


	}; // namespace Protocol
}; // namespace SF


