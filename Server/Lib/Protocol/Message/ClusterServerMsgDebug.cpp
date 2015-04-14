////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : ClusterServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/Message/ClusterServerMsgDebug.h"
#include "Protocol/Message/ClusterServerMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// ClusterServer Debug trace mappping
			static std::unordered_map<UINT,std::function<HRESULT(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapClusterServer;

			void RegisterDebugTraceClusterServer()
			{
 				// Cmd: Cluster member list query
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::GetClusterMemberListCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   ClusterServer::GetClusterMemberListCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::GetClusterMemberListRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   ClusterServer::GetClusterMemberListRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::JoinClusterCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   ClusterServer::JoinClusterCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::JoinClusterRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   ClusterServer::JoinClusterRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// C2S: Do not let it broadcasted while it's manual broadcast packet
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::NewServerServiceJoinedC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   ClusterServer::NewServerServiceJoinedC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// C2S: Sync cluster service informations
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::SyncClusterServiceC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   ClusterServer::SyncClusterServiceC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Join to the cluster
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::RequestDataSyncCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   ClusterServer::RequestDataSyncCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::RequestDataSyncRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   ClusterServer::RequestDataSyncRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Master instance of the cluster is assigned
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::ClusterMasterAssignedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   ClusterServer::ClusterMasterAssignedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// C2S: Master vote
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::ClusterMasterVoteC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   ClusterServer::ClusterMasterVoteC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// C2S: Update cluster service status
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::ClusterUpdateStatusC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   ClusterServer::ClusterUpdateStatusC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// C2S: Update cluster service workload
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::ClusterUpdateWorkloadC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   ClusterServer::ClusterUpdateWorkloadC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Get lowest workloaded cluster member
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::GetLowestWorkloadClusterMemberCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   ClusterServer::GetLowestWorkloadClusterMemberCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::GetLowestWorkloadClusterMemberRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   ClusterServer::GetLowestWorkloadClusterMemberRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// C2S: Called when a player entity is created
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::GamePlayerEntityCreatedC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   ClusterServer::GamePlayerEntityCreatedC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// C2S: Called when a player entity is deleted
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::GamePlayerEntityDeletedC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   ClusterServer::GamePlayerEntityDeletedC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
			}; // void RegisterDebugTraceClusterServer()


			///////////////////////////////////////////////////////////////
			// ClusterServer Debug trace
			HRESULT DebugOutClusterServer( const char *Prefix, MessageData *pMsg )
			{
 
				HRESULT hr = S_OK;
				auto itFount = MessageDebugTraceMapClusterServer.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// ClusterServer Debug trace
				itFount = MessageDebugTraceMapClusterServer.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapClusterServer.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // HRESULT DebugOutClusterServer( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


