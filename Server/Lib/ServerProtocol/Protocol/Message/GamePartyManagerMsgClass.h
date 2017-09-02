////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GamePartyManager Message parser definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol/Protocol.h"
#include "Net/Message.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace GamePartyManager
		{
 			// Cmd: Create a party instance
			class CreatePartyCmd : public MessageBase
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
				PlayerInformation m_Creator;
			public:
				CreatePartyCmd()
					{}

				CreatePartyCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerInformation& GetCreator() const	{ return m_Creator; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const PlayerInformation &InCreator );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class CreatePartyCmd : public MessageBase

			class CreatePartyRes : public MessageBase
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
				CreatePartyRes()
					{}

				CreatePartyRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class CreatePartyRes : public MessageBase

			// C2S: Party instance notify of deletion
			class PartyDeletedC2SEvt : public MessageBase
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
			public:
				PartyDeletedC2SEvt()
					{}

				PartyDeletedC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class PartyDeletedC2SEvt : public MessageBase





		}; // namespace GamePartyManager
	}; // namespace Message
}; // namespace SF


