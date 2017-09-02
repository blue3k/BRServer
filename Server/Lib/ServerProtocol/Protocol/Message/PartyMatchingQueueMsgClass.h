////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Message parser definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol/Protocol.h"
#include "Net/Message.h"
#include "Container/VariableMapBuilder.h"



namespace SF
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
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				uint16_t m_RouteHopCount;
				LinkedArray<MatchingPlayerInformation> m_Players;
			public:
				RegisterPartyMatchingCmd()
					{}

				RegisterPartyMatchingCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const Array<MatchingPlayerInformation>& GetPlayers() const	{ return m_Players; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class RegisterPartyMatchingCmd : public MessageBase

			class RegisterPartyMatchingRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
				MatchingQueueTicket m_MatchingTicket;
			public:
				RegisterPartyMatchingRes()
					{}

				RegisterPartyMatchingRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class RegisterPartyMatchingRes : public MessageBase

			// Cmd: Register match alone
			class RegisterPlayerMatchingCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				uint16_t m_RouteHopCount;
				PlayerID m_PlayerID;
			public:
				RegisterPlayerMatchingCmd()
					{}

				RegisterPlayerMatchingCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class RegisterPlayerMatchingCmd : public MessageBase

			class RegisterPlayerMatchingRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
				MatchingQueueTicket m_MatchingTicket;
			public:
				RegisterPlayerMatchingRes()
					{}

				RegisterPlayerMatchingRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class RegisterPlayerMatchingRes : public MessageBase

			// Cmd: update registration information
			class UpdateMatchingEntityUIDCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				uint16_t m_RouteHopCount;
				MatchingQueueTicket m_MatchingTicket;
				EntityUID m_PreviousUID;
			public:
				UpdateMatchingEntityUIDCmd()
					{}

				UpdateMatchingEntityUIDCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };
				const EntityUID& GetPreviousUID() const	{ return m_PreviousUID; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InPreviousUID );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class UpdateMatchingEntityUIDCmd : public MessageBase

			class UpdateMatchingEntityUIDRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
			public:
				UpdateMatchingEntityUIDRes()
					{}

				UpdateMatchingEntityUIDRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class UpdateMatchingEntityUIDRes : public MessageBase

			// Cmd: calcel registration
			class UnregisterMatchingCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				uint16_t m_RouteHopCount;
				MatchingQueueTicket m_MatchingTicket;
			public:
				UnregisterMatchingCmd()
					{}

				UnregisterMatchingCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class UnregisterMatchingCmd : public MessageBase

			class UnregisterMatchingRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
			public:
				UnregisterMatchingRes()
					{}

				UnregisterMatchingRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class UnregisterMatchingRes : public MessageBase

			// S2C: For some reason, matching is canceled
			class PartyMatchingCanceledS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint16_t m_RouteHopCount;
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
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PartyUID& GetDestPartyUID() const	{ return m_DestPartyUID; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PartyUID &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class PartyMatchingCanceledS2CEvt : public MessageBase

			// S2C: For some reason, matching is canceled
			class PlayerMatchingCanceledS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint16_t m_RouteHopCount;
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
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetDestPlayerID() const	{ return m_DestPlayerID; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class PlayerMatchingCanceledS2CEvt : public MessageBase

			// S2C: Enqueued item is dequeued
			class PartyMatchingItemDequeuedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint16_t m_RouteHopCount;
				MatchingQueueTicket m_MatchingTicket;
			public:
				PartyMatchingItemDequeuedS2CEvt()
					{}

				PartyMatchingItemDequeuedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class PartyMatchingItemDequeuedS2CEvt : public MessageBase

			// S2C: Enqueued item is dequeued
			class PlayerMatchingItemDequeuedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint16_t m_RouteHopCount;
				MatchingQueueTicket m_MatchingTicket;
			public:
				PlayerMatchingItemDequeuedS2CEvt()
					{}

				PlayerMatchingItemDequeuedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class PlayerMatchingItemDequeuedS2CEvt : public MessageBase

			// Cmd: Reserve a item
			class ReserveItemCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				uint16_t m_RouteHopCount;
			public:
				ReserveItemCmd()
					{}

				ReserveItemCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class ReserveItemCmd : public MessageBase

			class ReserveItemRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
				uint32_t m_NumberOfPlayersInTheItem;
				MatchingQueueTicket m_MatchingTicket;
			public:
				ReserveItemRes()
					{}

				ReserveItemRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const uint32_t& GetNumberOfPlayersInTheItem() const	{ return m_NumberOfPlayersInTheItem; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class ReserveItemRes : public MessageBase

			// Cmd: Reserve a item
			class ReserveItemsCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				uint16_t m_RouteHopCount;
				uint32_t m_NumberOfItemsToReserve;
			public:
				ReserveItemsCmd()
					{}

				ReserveItemsCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const uint32_t& GetNumberOfItemsToReserve() const	{ return m_NumberOfItemsToReserve; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InNumberOfItemsToReserve );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class ReserveItemsCmd : public MessageBase

			class ReserveItemsRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
				LinkedArray<uint32_t> m_NumberOfPlayersInTheItem;
				LinkedArray<MatchingQueueTicket> m_MatchingTicket;
			public:
				ReserveItemsRes()
					{}

				ReserveItemsRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const Array<uint32_t>& GetNumberOfPlayersInTheItem() const	{ return m_NumberOfPlayersInTheItem; };
				const Array<MatchingQueueTicket>& GetMatchingTicket() const	{ return m_MatchingTicket; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<uint32_t>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class ReserveItemsRes : public MessageBase

			// Cmd: Cancel reservation
			class CancelReservationCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				uint16_t m_RouteHopCount;
				MatchingQueueTicket m_TicketToCancel;
			public:
				CancelReservationCmd()
					{}

				CancelReservationCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const MatchingQueueTicket& GetTicketToCancel() const	{ return m_TicketToCancel; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class CancelReservationCmd : public MessageBase

			class CancelReservationRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
			public:
				CancelReservationRes()
					{}

				CancelReservationRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class CancelReservationRes : public MessageBase

			// Cmd: Cancel reservation
			class CancelReservationsCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				uint16_t m_RouteHopCount;
				LinkedArray<MatchingQueueTicket> m_TicketToCancel;
			public:
				CancelReservationsCmd()
					{}

				CancelReservationsCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const Array<MatchingQueueTicket>& GetTicketToCancel() const	{ return m_TicketToCancel; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class CancelReservationsCmd : public MessageBase

			class CancelReservationsRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
			public:
				CancelReservationsRes()
					{}

				CancelReservationsRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class CancelReservationsRes : public MessageBase

			// Cmd: Dequeue a reserved item
			class DequeueItemCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				uint16_t m_RouteHopCount;
				MatchingQueueTicket m_MatchingTicket;
			public:
				DequeueItemCmd()
					{}

				DequeueItemCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class DequeueItemCmd : public MessageBase

			class DequeueItemRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
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

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };
				const EntityUID& GetRegisterUID() const	{ return m_RegisterUID; };
				const PlayerID& GetRegisterID() const	{ return m_RegisterID; };
				const Array<MatchingPlayerInformation>& GetPlayers() const	{ return m_Players; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket, const EntityUID &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class DequeueItemRes : public MessageBase

			// C2S: Item error you should delete it
			class MatchingItemErrorC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint16_t m_RouteHopCount;
				MatchingQueueTicket m_MatchingTicket;
			public:
				MatchingItemErrorC2SEvt()
					{}

				MatchingItemErrorC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const MatchingQueueTicket& GetMatchingTicket() const	{ return m_MatchingTicket; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class MatchingItemErrorC2SEvt : public MessageBase





		}; // namespace PartyMatchingQueue
	}; // namespace Message
}; // namespace SF


