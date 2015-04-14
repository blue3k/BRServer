﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Message parser definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/PolicyID.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"



namespace BR
{
 	namespace Message
	{
 		namespace PartyMatchingQueue
		{
 			// Cmd: Register match by party
			class RegisterPartyMatchingCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				LinkedArray<MatchingPlayerInformation> m_Players;
			public:
				RegisterPartyMatchingCmd()
					{}

				RegisterPartyMatchingCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const Array<MatchingPlayerInformation>& GetPlayers() const	{ return m_Players; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class RegisterPartyMatchingCmd : public MessageBase

			class RegisterPartyMatchingRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
				MatchingQueueTicket m_MatchingTicket;
			public:
				RegisterPartyMatchingRes()
					{}

				RegisterPartyMatchingRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class RegisterPartyMatchingRes : public MessageBase

			// Cmd: Register match alone
			class RegisterPlayerMatchingCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				PlayerID m_PlayerID;
			public:
				RegisterPlayerMatchingCmd()
					{}

				RegisterPlayerMatchingCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class RegisterPlayerMatchingCmd : public MessageBase

			class RegisterPlayerMatchingRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
				MatchingQueueTicket m_MatchingTicket;
			public:
				RegisterPlayerMatchingRes()
					{}

				RegisterPlayerMatchingRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class RegisterPlayerMatchingRes : public MessageBase

			// Cmd: update registration information
			class UpdateMatchingEntityUIDCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				MatchingQueueTicket m_MatchingTicket;
				EntityUID m_PreviousUID;
			public:
				UpdateMatchingEntityUIDCmd()
					{}

				UpdateMatchingEntityUIDCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };
				const EntityUID& GetPreviousUID() const	{ return m_PreviousUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InPreviousUID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class UpdateMatchingEntityUIDCmd : public MessageBase

			class UpdateMatchingEntityUIDRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				UpdateMatchingEntityUIDRes()
					{}

				UpdateMatchingEntityUIDRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class UpdateMatchingEntityUIDRes : public MessageBase

			// Cmd: calcel registration
			class UnregisterMatchingCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				MatchingQueueTicket m_MatchingTicket;
			public:
				UnregisterMatchingCmd()
					{}

				UnregisterMatchingCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class UnregisterMatchingCmd : public MessageBase

			class UnregisterMatchingRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				UnregisterMatchingRes()
					{}

				UnregisterMatchingRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class UnregisterMatchingRes : public MessageBase

			// S2C: For some reason, matching is canceled
			class PartyMatchingCanceledS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				PartyUID m_DestPartyUID;
				MatchingQueueTicket m_MatchingTicket;
			public:
				PartyMatchingCanceledS2CEvt()
					{}

				PartyMatchingCanceledS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PartyUID& GetDestPartyUID() const	{ return m_DestPartyUID; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PartyUID &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class PartyMatchingCanceledS2CEvt : public MessageBase

			// S2C: For some reason, matching is canceled
			class PlayerMatchingCanceledS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				PlayerID m_DestPlayerID;
				MatchingQueueTicket m_MatchingTicket;
			public:
				PlayerMatchingCanceledS2CEvt()
					{}

				PlayerMatchingCanceledS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetDestPlayerID() const	{ return m_DestPlayerID; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class PlayerMatchingCanceledS2CEvt : public MessageBase

			// S2C: Enqueued item is dequeued
			class PartyMatchingItemDequeuedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				MatchingQueueTicket m_MatchingTicket;
			public:
				PartyMatchingItemDequeuedS2CEvt()
					{}

				PartyMatchingItemDequeuedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class PartyMatchingItemDequeuedS2CEvt : public MessageBase

			// S2C: Enqueued item is dequeued
			class PlayerMatchingItemDequeuedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				MatchingQueueTicket m_MatchingTicket;
			public:
				PlayerMatchingItemDequeuedS2CEvt()
					{}

				PlayerMatchingItemDequeuedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class PlayerMatchingItemDequeuedS2CEvt : public MessageBase

			// Cmd: Reserve a item
			class ReserveItemCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
			public:
				ReserveItemCmd()
					{}

				ReserveItemCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class ReserveItemCmd : public MessageBase

			class ReserveItemRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
				UINT32 m_NumberOfPlayersInTheItem;
				MatchingQueueTicket m_MatchingTicket;
			public:
				ReserveItemRes()
					{}

				ReserveItemRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT32& GetNumberOfPlayersInTheItem() const	{ return m_NumberOfPlayersInTheItem; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class ReserveItemRes : public MessageBase

			// Cmd: Reserve a item
			class ReserveItemsCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				UINT32 m_NumberOfItemsToReserve;
			public:
				ReserveItemsCmd()
					{}

				ReserveItemsCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const UINT32& GetNumberOfItemsToReserve() const	{ return m_NumberOfItemsToReserve; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const UINT32 &InNumberOfItemsToReserve );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class ReserveItemsCmd : public MessageBase

			class ReserveItemsRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
				LinkedArray<UINT32> m_NumberOfPlayersInTheItem;
				LinkedArray<MatchingQueueTicket> m_MatchingTicket;
			public:
				ReserveItemsRes()
					{}

				ReserveItemsRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const Array<UINT32>& GetNumberOfPlayersInTheItem() const	{ return m_NumberOfPlayersInTheItem; };
				const Array<MatchingQueueTicket>& GetMatchingTicket() const	{ return m_MatchingTicket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<UINT32>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class ReserveItemsRes : public MessageBase

			// Cmd: Cancel reservation
			class CancelReservationCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				MatchingQueueTicket m_TicketToCancel;
			public:
				CancelReservationCmd()
					{}

				CancelReservationCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const MatchingQueueTicket& GetTicketToCancel() const	{ return m_TicketToCancel; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class CancelReservationCmd : public MessageBase

			class CancelReservationRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				CancelReservationRes()
					{}

				CancelReservationRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class CancelReservationRes : public MessageBase

			// Cmd: Cancel reservation
			class CancelReservationsCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				LinkedArray<MatchingQueueTicket> m_TicketToCancel;
			public:
				CancelReservationsCmd()
					{}

				CancelReservationsCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const Array<MatchingQueueTicket>& GetTicketToCancel() const	{ return m_TicketToCancel; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class CancelReservationsCmd : public MessageBase

			class CancelReservationsRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				CancelReservationsRes()
					{}

				CancelReservationsRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class CancelReservationsRes : public MessageBase

			// Cmd: Dequeue a reserved item
			class DequeueItemCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				MatchingQueueTicket m_MatchingTicket;
			public:
				DequeueItemCmd()
					{}

				DequeueItemCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class DequeueItemCmd : public MessageBase

			class DequeueItemRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
				MatchingQueueTicket m_MatchingTicket;
				EntityUID m_RegisterUID;
				PlayerID m_RegisterID;
				LinkedArray<MatchingPlayerInformation> m_Players;
			public:
				DequeueItemRes()
					{}

				DequeueItemRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };
				const EntityUID& GetRegisterUID() const	{ return m_RegisterUID; };
				const PlayerID& GetRegisterID() const	{ return m_RegisterID; };
				const Array<MatchingPlayerInformation>& GetPlayers() const	{ return m_Players; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class DequeueItemRes : public MessageBase

			// C2S: Item error you should delete it
			class MatchingItemErrorC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				MatchingQueueTicket m_MatchingTicket;
			public:
				MatchingItemErrorC2SEvt()
					{}

				MatchingItemErrorC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class MatchingItemErrorC2SEvt : public MessageBase





		}; // namespace PartyMatchingQueue
	}; // namespace Message
}; // namespace BR


