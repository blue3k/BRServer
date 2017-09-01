////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Monitoring Message parser definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol/Protocol.h"
#include "Net/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"



namespace BR
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
				PlayerID GetPlayerID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				TransactionID m_TransactionID;
			public:
				GetInstanceListCmd()
					{}

				GetInstanceListCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID );

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
				PlayerID GetPlayerID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				TransactionID m_TransactionID;
				Result m_Result;
				LinkedArray<PerformanceCounterInstanceInfo> m_CounterInstances;
				uint32_t m_TotalInstanceCount;
			public:
				GetInstanceListRes()
					{}

				GetInstanceListRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const Array<PerformanceCounterInstanceInfo>& GetCounterInstances() const	{ return m_CounterInstances; };
				const uint32_t& GetTotalInstanceCount() const	{ return m_TotalInstanceCount; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount );

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
				PlayerID GetPlayerID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				TransactionID m_TransactionID;
				EntityUID m_InstanceUID;
			public:
				RequestCounterValuesCmd()
					{}

				RequestCounterValuesCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const EntityUID& GetInstanceUID() const	{ return m_InstanceUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const EntityUID &InInstanceUID );

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
				PlayerID GetPlayerID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				TransactionID m_TransactionID;
				Result m_Result;
				EntityUID m_InstanceUID;
				LinkedArray<uint64_t> m_CounterValues;
			public:
				RequestCounterValuesRes()
					{}

				RequestCounterValuesRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const EntityUID& GetInstanceUID() const	{ return m_InstanceUID; };
				const Array<uint64_t>& GetCounterValues() const	{ return m_CounterValues; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues );

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
				PlayerID GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				const char* m_InstanceName;
				EntityUID m_InstanceUID;
				LinkedArray<PerformanceCounterInfo> m_NewCounters;
			public:
				PerformanceCounterNewC2SEvt()
				:m_InstanceName(nullptr)
					{}

				PerformanceCounterNewC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_InstanceName(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const char* GetInstanceName() const	{ return m_InstanceName; };
				const EntityUID& GetInstanceUID() const	{ return m_InstanceUID; };
				const Array<PerformanceCounterInfo>& GetNewCounters() const	{ return m_NewCounters; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters );

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
				PlayerID GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				LinkedArray<EntityUID> m_FreeInstances;
			public:
				PerformanceCounterFreeC2SEvt()
					{}

				PerformanceCounterFreeC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Array<EntityUID>& GetFreeInstances() const	{ return m_FreeInstances; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const Array<EntityUID>& InFreeInstances );

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
				PlayerID GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				EntityUID m_InstanceUID;
				LinkedArray<uint64_t> m_CounterValues;
			public:
				PerformanceCounterUpdateC2SEvt()
					{}

				PerformanceCounterUpdateC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const EntityUID& GetInstanceUID() const	{ return m_InstanceUID; };
				const Array<uint64_t>& GetCounterValues() const	{ return m_CounterValues; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues );

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
				PlayerID GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				EntityUID m_InstanceUID;
			public:
				PerformanceCounterUpdateCounterInfoS2CEvt()
					{}

				PerformanceCounterUpdateCounterInfoS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const EntityUID& GetInstanceUID() const	{ return m_InstanceUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const EntityUID &InInstanceUID );

			}; // class PerformanceCounterUpdateCounterInfoS2CEvt : public MessageBase





		}; // namespace Monitoring
	}; // namespace Message
}; // namespace BR


