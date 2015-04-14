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
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
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
			static std::unordered_map<UINT,std::function<HRESULT(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapPartyMatchingQueue;

			void RegisterDebugTracePartyMatchingQueue()
			{
 				// Cmd: Register match by party
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::RegisterPartyMatchingCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::RegisterPartyMatchingCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::RegisterPartyMatchingRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::RegisterPartyMatchingRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Register match alone
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::RegisterPlayerMatchingCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::RegisterPlayerMatchingCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::RegisterPlayerMatchingRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::RegisterPlayerMatchingRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: update registration information
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::UpdateMatchingEntityUIDCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::UpdateMatchingEntityUIDCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::UpdateMatchingEntityUIDRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::UpdateMatchingEntityUIDRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: calcel registration
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::UnregisterMatchingCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::UnregisterMatchingCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::UnregisterMatchingRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::UnregisterMatchingRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: For some reason, matching is canceled
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::PartyMatchingCanceledS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::PartyMatchingCanceledS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: For some reason, matching is canceled
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::PlayerMatchingCanceledS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::PlayerMatchingCanceledS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Enqueued item is dequeued
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Enqueued item is dequeued
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Reserve a item
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::ReserveItemCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::ReserveItemCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::ReserveItemRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::ReserveItemRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Reserve a item
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::ReserveItemsCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::ReserveItemsCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::ReserveItemsRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::ReserveItemsRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Cancel reservation
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::CancelReservationCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::CancelReservationCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::CancelReservationRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::CancelReservationRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Cancel reservation
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::CancelReservationsCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::CancelReservationsCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::CancelReservationsRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::CancelReservationsRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Dequeue a reserved item
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::DequeueItemCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::DequeueItemCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::DequeueItemRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::DequeueItemRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// C2S: Item error you should delete it
				MessageDebugTraceMapPartyMatchingQueue.insert(std::make_pair(PartyMatchingQueue::MatchingItemErrorC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   PartyMatchingQueue::MatchingItemErrorC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
			}; // void RegisterDebugTracePartyMatchingQueue()


			///////////////////////////////////////////////////////////////
			// PartyMatchingQueue Debug trace
			HRESULT DebugOutPartyMatchingQueue( const char *Prefix, MessageData *pMsg )
			{
 
				HRESULT hr = S_OK;
				auto itFount = MessageDebugTraceMapPartyMatchingQueue.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// PartyMatchingQueue Debug trace
				itFount = MessageDebugTraceMapPartyMatchingQueue.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapPartyMatchingQueue.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // HRESULT DebugOutPartyMatchingQueue( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


