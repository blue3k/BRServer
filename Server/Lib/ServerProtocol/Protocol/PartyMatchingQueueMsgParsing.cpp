////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 Kyungkun Ko
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/PartyMatchingQueueMsgParsing.h"
#include "Protocol/PartyMatchingQueueMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// PartyMatchingQueue message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserPartyMatchingQueue()
		{
 			// Cmd: Register a party for matching
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::RegisterPartyMatchingCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::RegisterPartyMatchingCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::RegisterPartyMatchingRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::RegisterPartyMatchingRes::ParseMessageToMessageBase));
			// Cmd: Register a player for matching
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::RegisterPlayerMatchingCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::RegisterPlayerMatchingCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::RegisterPlayerMatchingRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::RegisterPlayerMatchingRes::ParseMessageToMessageBase));
			// Cmd: [Deprecated] update registration information
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::UpdateMatchingEntityUIDRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::UpdateMatchingEntityUIDRes::ParseMessageToMessageBase));
			// Cmd: calcel registration
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::UnregisterMatchingCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::UnregisterMatchingCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::UnregisterMatchingRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::UnregisterMatchingRes::ParseMessageToMessageBase));
			// S2C: Event when the matching is canceled, for party
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::PartyMatchingCanceledS2CEvt::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::PartyMatchingCanceledS2CEvt::ParseMessageToMessageBase));
			// S2C: Event whan the matching is canceled, for a player
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::PlayerMatchingCanceledS2CEvt::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::PlayerMatchingCanceledS2CEvt::ParseMessageToMessageBase));
			// S2C: Matching item has been dequeued for matching
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt::ParseMessageToMessageBase));
			// S2C: Matching item has been dequeued for matching
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt::ParseMessageToMessageBase));
			// Cmd: Reserve a item
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::ReserveItemCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::ReserveItemCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::ReserveItemRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::ReserveItemRes::ParseMessageToMessageBase));
			// Cmd: Reserve a item
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::ReserveItemsCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::ReserveItemsCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::ReserveItemsRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::ReserveItemsRes::ParseMessageToMessageBase));
			// Cmd: Cancel reservation
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::CancelReservationCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::CancelReservationCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::CancelReservationRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::CancelReservationRes::ParseMessageToMessageBase));
			// Cmd: Cancel reservation
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::CancelReservationsCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::CancelReservationsCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::CancelReservationsRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::CancelReservationsRes::ParseMessageToMessageBase));
			// Cmd: Dequeue a reserved item
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::DequeueItemCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::DequeueItemCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::DequeueItemRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::DequeueItemRes::ParseMessageToMessageBase));
			// C2S: Item error you should delete it
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::PartyMatchingQueue::MatchingItemErrorC2SEvt::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::MatchingItemErrorC2SEvt::ParseMessageToMessageBase));
		}; // void RegisterParserPartyMatchingQueue()


	}; // namespace Protocol
}; // namespace SF


