////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : ClusterServer Message parser definitions
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
					HasContext = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				ClusterID m_ClusterID;
			public:
				GetClusterMemberListCmd()
					{}

				GetClusterMemberListCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class GetClusterMemberListCmd : public MessageBase

			class GetClusterMemberListRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasContext = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
				LinkedArray<ServiceInformation> m_MemberList;
			public:
				GetClusterMemberListRes()
					{}

				GetClusterMemberListRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const Array<ServiceInformation>& GetMemberList() const	{ return m_MemberList; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<ServiceInformation>& InMemberList );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

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
					HasContext = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 1,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				EntityUID m_Sender;
				NetClass m_SenderNetClass;
				NetAddress m_SenderAddress;
				ClusterID m_ClusterID;
				ClusterType m_ClusterType;
				ClusterMembership m_ClusterMembership;
			public:
				JoinClusterCmd()
					{}

				JoinClusterCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const EntityUID& GetSender() const	{ return m_Sender; };
				const NetClass& GetSenderNetClass() const	{ return m_SenderNetClass; };
				const NetAddress& GetSenderAddress() const	{ return m_SenderAddress; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };
				const ClusterType& GetClusterType() const	{ return m_ClusterType; };
				const ClusterMembership& GetClusterMembership() const	{ return m_ClusterMembership; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class JoinClusterCmd : public MessageBase

			class JoinClusterRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasContext = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
				LinkedArray<ServiceInformation> m_MemberList;
			public:
				JoinClusterRes()
					{}

				JoinClusterRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const Array<ServiceInformation>& GetMemberList() const	{ return m_MemberList; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<ServiceInformation>& InMemberList );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

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
					HasContext = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				Context GetContext() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				EntityUID m_JoinedServiceUID;
				NetClass m_JoinedServiceNetClass;
				NetAddress m_JoinedServiceAddress;
				ClusterID m_ClusterID;
				ClusterType m_ClusterType;
				ClusterMembership m_JoinedServiceMembership;
			public:
				NewServerServiceJoinedC2SEvt()
					{}

				NewServerServiceJoinedC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const EntityUID& GetJoinedServiceUID() const	{ return m_JoinedServiceUID; };
				const NetClass& GetJoinedServiceNetClass() const	{ return m_JoinedServiceNetClass; };
				const NetAddress& GetJoinedServiceAddress() const	{ return m_JoinedServiceAddress; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };
				const ClusterType& GetClusterType() const	{ return m_ClusterType; };
				const ClusterMembership& GetJoinedServiceMembership() const	{ return m_JoinedServiceMembership; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

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
					HasContext = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				Context GetContext() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				ClusterID m_ClusterID;
				ClusterType m_ClusterType;
				LinkedArray<ServiceInformation> m_MemberList;
			public:
				SyncClusterServiceC2SEvt()
					{}

				SyncClusterServiceC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };
				const ClusterType& GetClusterType() const	{ return m_ClusterType; };
				const Array<ServiceInformation>& GetMemberList() const	{ return m_MemberList; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

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
					HasContext = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				ClusterID m_ClusterID;
			public:
				RequestDataSyncCmd()
					{}

				RequestDataSyncCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterDataRead; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class RequestDataSyncCmd : public MessageBase

			class RequestDataSyncRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasContext = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				RequestDataSyncRes()
					{}

				RequestDataSyncRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterDataRead; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

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
					HasContext = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 1,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				Context GetContext() { return 0; }
			private:
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				EntityUID m_Sender;
				ClusterID m_ClusterID;
				EntityUID m_MasterUID;
			public:
				ClusterMasterAssignedS2CEvt()
					{}

				ClusterMasterAssignedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusWrite; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const EntityUID& GetSender() const	{ return m_Sender; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };
				const EntityUID& GetMasterUID() const	{ return m_MasterUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const EntityUID &InMasterUID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

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
					HasContext = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				Context GetContext() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				ClusterID m_ClusterID;
				EntityUID m_VoteToUID;
				UINT64 m_VotedUpTime;
			public:
				ClusterMasterVoteC2SEvt()
					{}

				ClusterMasterVoteC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };
				const EntityUID& GetVoteToUID() const	{ return m_VoteToUID; };
				const UINT64& GetVotedUpTime() const	{ return m_VotedUpTime; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const UINT64 &InVotedUpTime );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

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
					HasContext = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 1,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				Context GetContext() { return 0; }
			private:
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				EntityUID m_Sender;
				ClusterID m_ClusterID;
				ServiceStatus m_MemberStatus;
			public:
				ClusterUpdateStatusC2SEvt()
					{}

				ClusterUpdateStatusC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusWrite; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const EntityUID& GetSender() const	{ return m_Sender; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };
				const ServiceStatus& GetMemberStatus() const	{ return m_MemberStatus; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

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
					HasContext = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 1,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				Context GetContext() { return 0; }
			private:
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				EntityUID m_Sender;
				ClusterID m_ClusterID;
				UINT32 m_Workload;
			public:
				ClusterUpdateWorkloadC2SEvt()
					{}

				ClusterUpdateWorkloadC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusWrite; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const EntityUID& GetSender() const	{ return m_Sender; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };
				const UINT32& GetWorkload() const	{ return m_Workload; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const UINT32 &InWorkload );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

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
					HasContext = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				ClusterID m_ClusterID;
			public:
				GetLowestWorkloadClusterMemberCmd()
					{}

				GetLowestWorkloadClusterMemberCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const ClusterID& GetClusterID() const	{ return m_ClusterID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const ClusterID &InClusterID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class GetLowestWorkloadClusterMemberCmd : public MessageBase

			class GetLowestWorkloadClusterMemberRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasContext = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
				ServiceInformation m_Member;
			public:
				GetLowestWorkloadClusterMemberRes()
					{}

				GetLowestWorkloadClusterMemberRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterStatusRead; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const ServiceInformation& GetMember() const	{ return m_Member; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const ServiceInformation &InMember );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

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
					HasContext = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				Context GetContext() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				PlayerID m_PlayerID;
				EntityUID m_PlayerUID;
			public:
				GamePlayerEntityCreatedC2SEvt()
					{}

				GamePlayerEntityCreatedC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterDataWrite; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const EntityUID& GetPlayerUID() const	{ return m_PlayerUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

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
					HasContext = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				Context GetContext() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				PlayerID m_PlayerID;
				EntityUID m_PlayerUID;
			public:
				GamePlayerEntityDeletedC2SEvt()
					{}

				GamePlayerEntityDeletedC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_ClusterDataWrite; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const EntityUID& GetPlayerUID() const	{ return m_PlayerUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class GamePlayerEntityDeletedC2SEvt : public MessageBase





		}; // namespace ClusterServer
	}; // namespace Message
}; // namespace BR


