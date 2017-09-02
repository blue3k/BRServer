////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : EntityServer Message parser definitions
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
 		namespace EntityServer
		{
 			// Cmd: Register entity
			class RegisterEntityCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				TransactionID m_TransactionID;
				EntityID m_LocalEntID;
				const char* m_EntName;
			public:
				RegisterEntityCmd()
				:m_EntName(nullptr)
					{}

				RegisterEntityCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_EntName(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const EntityID& GetLocalEntID() const	{ return m_LocalEntID; };
				const char* GetEntName() const	{ return m_EntName; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName );

			}; // class RegisterEntityCmd : public MessageBase

			class RegisterEntityRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				TransactionID m_TransactionID;
				Result m_Result;
				uint64_t m_EntUID;
			public:
				RegisterEntityRes()
					{}

				RegisterEntityRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const uint64_t& GetEntUID() const	{ return m_EntUID; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InEntUID );

			}; // class RegisterEntityRes : public MessageBase

			// Cmd: Find Entity
			class UnregisterEntityCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				TransactionID m_TransactionID;
				uint64_t m_EntUID;
			public:
				UnregisterEntityCmd()
					{}

				UnregisterEntityCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint64_t& GetEntUID() const	{ return m_EntUID; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const uint64_t &InEntUID );

			}; // class UnregisterEntityCmd : public MessageBase

			class UnregisterEntityRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				TransactionID m_TransactionID;
				Result m_Result;
			public:
				UnregisterEntityRes()
					{}

				UnregisterEntityRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const Result &InResult );

			}; // class UnregisterEntityRes : public MessageBase

			// Cmd: Find Entity
			class FindEntityCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				TransactionID m_TransactionID;
				EntityID m_LocalEntID;
			public:
				FindEntityCmd()
					{}

				FindEntityCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const EntityID& GetLocalEntID() const	{ return m_LocalEntID; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const EntityID &InLocalEntID );

			}; // class FindEntityCmd : public MessageBase

			class FindEntityRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				TransactionID m_TransactionID;
				Result m_Result;
				uint64_t m_EntUID;
			public:
				FindEntityRes()
					{}

				FindEntityRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const uint64_t& GetEntUID() const	{ return m_EntUID; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InEntUID );

			}; // class FindEntityRes : public MessageBase





		}; // namespace EntityServer
	}; // namespace Message
}; // namespace SF


