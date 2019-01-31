////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : Server Message parser definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol/SFProtocol.h"
#include "Net/SFMessage.h"
#include "Types/SFEngineTypedefs.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace Server
		{
 			// Cmd: Generic failure message
			class GenericFailureCmd : public MessageBase
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
			public:
				GenericFailureCmd()
					{}

				GenericFailureCmd( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };

				static Result TraceOut(const char* prefix, MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class GenericFailureCmd : public MessageBase

			class GenericFailureRes : public MessageBase
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
				GenericFailureRes()
					{}

				GenericFailureRes( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(const char* prefix, MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class GenericFailureRes : public MessageBase

			// C2S: Server Started or Connected
			class ServerConnectedC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint32_t m_StartUpTime;
				NetAddress m_PrivateAddress;
			public:
				ServerConnectedC2SEvt()
					{}

				ServerConnectedC2SEvt( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint32_t& GetStartUpTime() const	{ return m_StartUpTime; };
				const NetAddress& GetPrivateAddress() const	{ return m_PrivateAddress; };

				static Result TraceOut(const char* prefix, MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class ServerConnectedC2SEvt : public MessageBase





		}; // namespace Server
	}; // namespace Message
}; // namespace SF


