﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) StormForge
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
#include "Variable/SFVariableTable.h"
#include "Container/SFArray.h"
#include "Actor/Movement/SFActorMovement.h"



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
				uint64_t GetPlayerID() { return uint64_t{}; }
				uint32_t GetRouteHopCount() { return uint32_t{}; }
				uint64_t GetSender() { return uint64_t{}; }
			private:
				RouteContext m_RouteContext{};
				uint64_t m_TransactionID{};
			public:
				GenericFailureCmd()
					{}

				GenericFailureCmd( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID );

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
				uint64_t GetPlayerID() { return uint64_t{}; }
				uint32_t GetRouteHopCount() { return uint32_t{}; }
				uint64_t GetSender() { return uint64_t{}; }
			private:
				RouteContext m_RouteContext{};
				uint64_t m_TransactionID{};
				Result m_Result{};
			public:
				GenericFailureRes()
					{}

				GenericFailureRes( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

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
				uint64_t GetPlayerID() { return uint64_t{}; }
				uint64_t GetTransactionID() { return uint64_t{}; }
				uint32_t GetRouteHopCount() { return uint32_t{}; }
				uint64_t GetSender() { return uint64_t{}; }
			private:
				RouteContext m_RouteContext{};
				uint32_t m_StartUpTime{};
				NetAddress m_PrivateAddress{};
			public:
				ServerConnectedC2SEvt()
					{}

				ServerConnectedC2SEvt( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint32_t& GetStartUpTime() const	{ return m_StartUpTime; };
				const NetAddress& GetPrivateAddress() const	{ return m_PrivateAddress; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress );

			}; // class ServerConnectedC2SEvt : public MessageBase





		}; // namespace Server
	}; // namespace Message
}; // namespace SF


