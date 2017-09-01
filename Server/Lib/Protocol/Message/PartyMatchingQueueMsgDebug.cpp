////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/Message/PartyMatchingQueueMsgDebug.h"
#include "Protocol/Message/PartyMatchingQueueMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// PartyMatchingQueue Debug trace mappping
			static std::unordered_map<UINT,std::function<Result(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapPartyMatchingQueue;

			void RegisterDebugTracePartyMatchingQueue()
			{
 				// Cmd: Register match by party
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::RegisterPartyMatchingCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::RegisterPartyMatchingCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::RegisterPartyMatchingRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::RegisterPartyMatchingRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Register match alone
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::RegisterPlayerMatchingCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::RegisterPlayerMatchingCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::RegisterPlayerMatchingRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::RegisterPlayerMatchingRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: update registration information
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::UpdateMatchingEntityUIDCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::UpdateMatchingEntityUIDCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::UpdateMatchingEntityUIDRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::UpdateMatchingEntityUIDRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: calcel registration
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::UnregisterMatchingCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::UnregisterMatchingCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::UnregisterMatchingRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::UnregisterMatchingRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: For some reason, matching is canceled
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::PartyMatchingCanceledS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::PartyMatchingCanceledS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: For some reason, matching is canceled
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::PlayerMatchingCanceledS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::PlayerMatchingCanceledS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Enqueued item is dequeued
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Enqueued item is dequeued
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Reserve a item
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::ReserveItemCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::ReserveItemCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::ReserveItemRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::ReserveItemRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Reserve a item
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::ReserveItemsCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::ReserveItemsCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::ReserveItemsRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::ReserveItemsRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Cancel reservation
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::CancelReservationCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::CancelReservationCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::CancelReservationRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::CancelReservationRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Cancel reservation
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::CancelReservationsCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::CancelReservationsCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::CancelReservationsRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::CancelReservationsRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Dequeue a reserved item
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::DequeueItemCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::DequeueItemCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::DequeueItemRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::DequeueItemRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Item error you should delete it
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::MatchingItemErrorC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatchingQueue::MatchingItemErrorC2SEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
			}; // void RegisterDebugTracePartyMatchingQueue()


			///////////////////////////////////////////////////////////////
			// PartyMatchingQueue Debug trace
			Result DebugOutPartyMatchingQueue( const char *Prefix, MessageData *pMsg )
			{
 
				Result hr;
				auto itFount = MessageDebugTraceMapPartyMatchingQueue.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// PartyMatchingQueue Debug trace
				itFount = MessageDebugTraceMapPartyMatchingQueue.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapPartyMatchingQueue.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // Result DebugOutPartyMatchingQueue( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


