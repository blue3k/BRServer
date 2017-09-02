////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : ClusterServer Message parser definitions
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
 		namespace ClusterServer
		{
 			// Cmd: Cluster member list query
			class GetClusterMemberListCmd : public MessageBase
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
				ClusterID m_ClusterID;
			public:
				GetClusterMemberListCmd()
					{}

				GetClusterMemberListCmd( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class GetClusterMemberListCmd : public MessageBase

			class GetClusterMemberListRes : public MessageBase
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
				LinkedArray<ServiceInformation> m_MemberList;
			public:
				GetClusterMemberListRes()
					{}

				GetClusterMemberListRes( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const Array<ServiceInformation>& GetMemberList() const	{ return m_MemberList; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class GetClusterMemberListRes : public MessageBase

			// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
			class JoinClusterCmd : public MessageBase
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
					HasSender = 1,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				uint16_t m_RouteHopCount;
				uint64_t m_Sender;
				NetClass m_SenderNetClass;
				NetAddress m_SenderAddress;
				ClusterID m_ClusterID;
				ClusterType m_ClusterType;
				ClusterMembership m_ClusterMembership;
			public:
				JoinClusterCmd()
					{}

				JoinClusterCmd( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const uint64_t& GetSender() const	{ return m_Sender; };
				const NetClass& GetSenderNetClass() const	{ return m_SenderNetClass; };
				const NetAddress& GetSenderAddress() const	{ return m_SenderAddress; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };
				const ClusterType& GetClusterType() const	{ return m_ClusterType; };
				const ClusterMembership& GetClusterMembership() const	{ return m_ClusterMembership; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const uint64_t &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class JoinClusterCmd : public MessageBase

			class JoinClusterRes : public MessageBase
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
				LinkedArray<ServiceInformation> m_MemberList;
			public:
				JoinClusterRes()
					{}

				JoinClusterRes( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const Array<ServiceInformation>& GetMemberList() const	{ return m_MemberList; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class JoinClusterRes : public MessageBase

			// C2S: Do not let it broadcasted while it's manual broadcast packet
			class NewServerServiceJoinedC2SEvt : public MessageBase
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
				uint64_t m_JoinedServiceUID;
				NetClass m_JoinedServiceNetClass;
				NetAddress m_JoinedServiceAddress;
				ClusterID m_ClusterID;
				ClusterType m_ClusterType;
				ClusterMembership m_JoinedServiceMembership;
			public:
				NewServerServiceJoinedC2SEvt()
					{}

				NewServerServiceJoinedC2SEvt( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const uint64_t& GetJoinedServiceUID() const	{ return m_JoinedServiceUID; };
				const NetClass& GetJoinedServiceNetClass() const	{ return m_JoinedServiceNetClass; };
				const NetAddress& GetJoinedServiceAddress() const	{ return m_JoinedServiceAddress; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };
				const ClusterType& GetClusterType() const	{ return m_ClusterType; };
				const ClusterMembership& GetJoinedServiceMembership() const	{ return m_JoinedServiceMembership; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class NewServerServiceJoinedC2SEvt : public MessageBase

			// C2S: Sync cluster service informations
			class SyncClusterServiceC2SEvt : public MessageBase
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
				ClusterID m_ClusterID;
				ClusterType m_ClusterType;
				LinkedArray<ServiceInformation> m_MemberList;
			public:
				SyncClusterServiceC2SEvt()
					{}

				SyncClusterServiceC2SEvt( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };
				const ClusterType& GetClusterType() const	{ return m_ClusterType; };
				const Array<ServiceInformation>& GetMemberList() const	{ return m_MemberList; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class SyncClusterServiceC2SEvt : public MessageBase

			// Cmd: Join to the cluster
			class RequestDataSyncCmd : public MessageBase
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
				ClusterID m_ClusterID;
			public:
				RequestDataSyncCmd()
					{}

				RequestDataSyncCmd( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterDataRead; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class RequestDataSyncCmd : public MessageBase

			class RequestDataSyncRes : public MessageBase
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
				RequestDataSyncRes()
					{}

				RequestDataSyncRes( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterDataRead; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class RequestDataSyncRes : public MessageBase

			// S2C: Master instance of the cluster is assigned
			class ClusterMasterAssignedS2CEvt : public MessageBase
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
					HasSender = 1,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint16_t m_RouteHopCount;
				uint64_t m_Sender;
				ClusterID m_ClusterID;
				uint64_t m_MasterUID;
			public:
				ClusterMasterAssignedS2CEvt()
					{}

				ClusterMasterAssignedS2CEvt( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusWrite; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const uint64_t& GetSender() const	{ return m_Sender; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };
				const uint64_t& GetMasterUID() const	{ return m_MasterUID; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InSender, const ClusterID &InClusterID, const uint64_t &InMasterUID );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class ClusterMasterAssignedS2CEvt : public MessageBase

			// C2S: Master vote
			class ClusterMasterVoteC2SEvt : public MessageBase
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
				ClusterID m_ClusterID;
				uint64_t m_VoteToUID;
				uint64_t m_VotedUpTime;
			public:
				ClusterMasterVoteC2SEvt()
					{}

				ClusterMasterVoteC2SEvt( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };
				const uint64_t& GetVoteToUID() const	{ return m_VoteToUID; };
				const uint64_t& GetVotedUpTime() const	{ return m_VotedUpTime; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const uint64_t &InVoteToUID, const uint64_t &InVotedUpTime );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class ClusterMasterVoteC2SEvt : public MessageBase

			// C2S: Update cluster service status
			class ClusterUpdateStatusC2SEvt : public MessageBase
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
					HasSender = 1,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint16_t m_RouteHopCount;
				uint64_t m_Sender;
				ClusterID m_ClusterID;
				ServiceStatus m_MemberStatus;
			public:
				ClusterUpdateStatusC2SEvt()
					{}

				ClusterUpdateStatusC2SEvt( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusWrite; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const uint64_t& GetSender() const	{ return m_Sender; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };
				const ServiceStatus& GetMemberStatus() const	{ return m_MemberStatus; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class ClusterUpdateStatusC2SEvt : public MessageBase

			// C2S: Update cluster service workload
			class ClusterUpdateWorkloadC2SEvt : public MessageBase
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
					HasSender = 1,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint16_t m_RouteHopCount;
				uint64_t m_Sender;
				ClusterID m_ClusterID;
				uint32_t m_Workload;
			public:
				ClusterUpdateWorkloadC2SEvt()
					{}

				ClusterUpdateWorkloadC2SEvt( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusWrite; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const uint64_t& GetSender() const	{ return m_Sender; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };
				const uint32_t& GetWorkload() const	{ return m_Workload; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InSender, const ClusterID &InClusterID, const uint32_t &InWorkload );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class ClusterUpdateWorkloadC2SEvt : public MessageBase

			// Cmd: Get lowest workloaded cluster member
			class GetLowestWorkloadClusterMemberCmd : public MessageBase
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
				ClusterID m_ClusterID;
			public:
				GetLowestWorkloadClusterMemberCmd()
					{}

				GetLowestWorkloadClusterMemberCmd( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class GetLowestWorkloadClusterMemberCmd : public MessageBase

			class GetLowestWorkloadClusterMemberRes : public MessageBase
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
				ServiceInformation m_Member;
			public:
				GetLowestWorkloadClusterMemberRes()
					{}

				GetLowestWorkloadClusterMemberRes( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const ServiceInformation& GetMember() const	{ return m_Member; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const ServiceInformation &InMember );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class GetLowestWorkloadClusterMemberRes : public MessageBase

			// C2S: Called when a player entity is created
			class GamePlayerEntityCreatedC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				TransactionID GetTransactionID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint16_t m_RouteHopCount;
				PlayerID m_PlayerID;
				uint64_t m_PlayerUID;
			public:
				GamePlayerEntityCreatedC2SEvt()
					{}

				GamePlayerEntityCreatedC2SEvt( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterDataWrite; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const uint64_t& GetPlayerUID() const	{ return m_PlayerUID; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const uint64_t &InPlayerUID );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class GamePlayerEntityCreatedC2SEvt : public MessageBase

			// C2S: Called when a player entity is deleted
			class GamePlayerEntityDeletedC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				TransactionID GetTransactionID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint16_t m_RouteHopCount;
				PlayerID m_PlayerID;
				uint64_t m_PlayerUID;
			public:
				GamePlayerEntityDeletedC2SEvt()
					{}

				GamePlayerEntityDeletedC2SEvt( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterDataWrite; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const uint64_t& GetPlayerUID() const	{ return m_PlayerUID; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const uint64_t &InPlayerUID );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class GamePlayerEntityDeletedC2SEvt : public MessageBase





		}; // namespace ClusterServer
	}; // namespace Message
}; // namespace SF


