////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : Monitoring Message parser definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol/Protocol.h"
#include "Net/Message.h"
#include "Types/SFEngineTypedefs.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace Monitoring
		{
 			// Cmd: Add a player to ranking
			class GetInstanceListCmd : public MessageBase
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
			public:
				GetInstanceListCmd()
					{}

				GetInstanceListCmd( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID );

			}; // class GetInstanceListCmd : public MessageBase

			class GetInstanceListRes : public MessageBase
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
				LinkedArray<PerformanceCounterInstanceInfo> m_CounterInstances;
				uint32_t m_TotalInstanceCount;
			public:
				GetInstanceListRes()
					{}

				GetInstanceListRes( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const Array<PerformanceCounterInstanceInfo>& GetCounterInstances() const	{ return m_CounterInstances; };
				const uint32_t& GetTotalInstanceCount() const	{ return m_TotalInstanceCount; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount );

			}; // class GetInstanceListRes : public MessageBase

			// Cmd: Remove a player to ranking
			class RequestCounterValuesCmd : public MessageBase
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
				uint64_t m_InstanceUID;
			public:
				RequestCounterValuesCmd()
					{}

				RequestCounterValuesCmd( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint64_t& GetInstanceUID() const	{ return m_InstanceUID; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const uint64_t &InInstanceUID );

			}; // class RequestCounterValuesCmd : public MessageBase

			class RequestCounterValuesRes : public MessageBase
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
				uint64_t m_InstanceUID;
				LinkedArray<uint64_t> m_CounterValues;
			public:
				RequestCounterValuesRes()
					{}

				RequestCounterValuesRes( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const uint64_t& GetInstanceUID() const	{ return m_InstanceUID; };
				const Array<uint64_t>& GetCounterValues() const	{ return m_CounterValues; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues );

			}; // class RequestCounterValuesRes : public MessageBase

			// C2S: Counter instance is created
			class PerformanceCounterNewC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				const char* m_InstanceName;
				uint64_t m_InstanceUID;
				LinkedArray<PerformanceCounterInfo> m_NewCounters;
			public:
				PerformanceCounterNewC2SEvt()
				:m_InstanceName(nullptr)
					{}

				PerformanceCounterNewC2SEvt( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
				,m_InstanceName(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const char* GetInstanceName() const	{ return m_InstanceName; };
				const uint64_t& GetInstanceUID() const	{ return m_InstanceUID; };
				const Array<PerformanceCounterInfo>& GetNewCounters() const	{ return m_NewCounters; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters );

			}; // class PerformanceCounterNewC2SEvt : public MessageBase

			// C2S: Counter instance is deleted
			class PerformanceCounterFreeC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				LinkedArray<uint64_t> m_FreeInstances;
			public:
				PerformanceCounterFreeC2SEvt()
					{}

				PerformanceCounterFreeC2SEvt( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Array<uint64_t>& GetFreeInstances() const	{ return m_FreeInstances; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const Array<uint64_t>& InFreeInstances );

			}; // class PerformanceCounterFreeC2SEvt : public MessageBase

			// C2S: Counter update broadcast
			class PerformanceCounterUpdateC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				uint64_t m_InstanceUID;
				LinkedArray<uint64_t> m_CounterValues;
			public:
				PerformanceCounterUpdateC2SEvt()
					{}

				PerformanceCounterUpdateC2SEvt( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const uint64_t& GetInstanceUID() const	{ return m_InstanceUID; };
				const Array<uint64_t>& GetCounterValues() const	{ return m_CounterValues; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues );

			}; // class PerformanceCounterUpdateC2SEvt : public MessageBase

			// S2C: Request from server
			class PerformanceCounterUpdateCounterInfoS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				uint64_t m_InstanceUID;
			public:
				PerformanceCounterUpdateCounterInfoS2CEvt()
					{}

				PerformanceCounterUpdateCounterInfoS2CEvt( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const uint64_t& GetInstanceUID() const	{ return m_InstanceUID; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const uint64_t &InInstanceUID );

			}; // class PerformanceCounterUpdateCounterInfoS2CEvt : public MessageBase





		}; // namespace Monitoring
	}; // namespace Message
}; // namespace SF


