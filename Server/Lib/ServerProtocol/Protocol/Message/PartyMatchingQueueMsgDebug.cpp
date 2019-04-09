////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/Message/PartyMatchingQueueMsgDebug.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// PartyMatchingQueue Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTracePartyMatchingQueue()
		{
 			// Cmd: Register a party for matching
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::RegisterPartyMatchingCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::RegisterPartyMatchingCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::RegisterPartyMatchingRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::RegisterPartyMatchingRes::TraceOut));
			// Cmd: Register a player for matching
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::RegisterPlayerMatchingCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::RegisterPlayerMatchingCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::RegisterPlayerMatchingRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::RegisterPlayerMatchingRes::TraceOut));
			// Cmd: [Deprecated] update registration information
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::UpdateMatchingEntityUIDCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::UpdateMatchingEntityUIDRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::UpdateMatchingEntityUIDRes::TraceOut));
			// Cmd: calcel registration
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::UnregisterMatchingCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::UnregisterMatchingCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::UnregisterMatchingRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::UnregisterMatchingRes::TraceOut));
			// S2C: Event when the matching is canceled, for party
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::PartyMatchingCanceledS2CEvt::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::PartyMatchingCanceledS2CEvt::TraceOut));
			// S2C: Event whan the matching is canceled, for a player
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::PlayerMatchingCanceledS2CEvt::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::PlayerMatchingCanceledS2CEvt::TraceOut));
			// S2C: Matching item has been dequeued for matching
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt::TraceOut));
			// S2C: Matching item has been dequeued for matching
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt::TraceOut));
			// Cmd: Reserve a item
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::ReserveItemCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::ReserveItemCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::ReserveItemRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::ReserveItemRes::TraceOut));
			// Cmd: Reserve a item
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::ReserveItemsCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::ReserveItemsCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::ReserveItemsRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::ReserveItemsRes::TraceOut));
			// Cmd: Cancel reservation
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::CancelReservationCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::CancelReservationCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::CancelReservationRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::CancelReservationRes::TraceOut));
			// Cmd: Cancel reservation
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::CancelReservationsCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::CancelReservationsCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::CancelReservationsRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::CancelReservationsRes::TraceOut));
			// Cmd: Dequeue a reserved item
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::DequeueItemCmd::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::DequeueItemCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::DequeueItemRes::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::DequeueItemRes::TraceOut));
			// C2S: Item error you should delete it
			MessageDebugTraceMap.insert(std::make_pair(Message::PartyMatchingQueue::MatchingItemErrorC2SEvt::MID.IDSeq.MsgID,&Message::PartyMatchingQueue::MatchingItemErrorC2SEvt::TraceOut));
		}; // void RegisterDebugTracePartyMatchingQueue()


	}; // namespace Protocol
}; // namespace SF


