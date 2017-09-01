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
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
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
			static std::unordered_map<UINT,std::function<Result(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapClusterServer;

			void RegisterDebugTraceClusterServer()
			{
 				// Cmd: Cluster member list query
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::GetClusterMemberListCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   ClusterServer::GetClusterMemberListCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::GetClusterMemberListRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   ClusterServer::GetClusterMemberListRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::JoinClusterCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   ClusterServer::JoinClusterCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::JoinClusterRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   ClusterServer::JoinClusterRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Do not let it broadcasted while it's manual broadcast packet
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::NewServerServiceJoinedC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   ClusterServer::NewServerServiceJoinedC2SEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Sync cluster service informations
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::SyncClusterServiceC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   ClusterServer::SyncClusterServiceC2SEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Join to the cluster
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::RequestDataSyncCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   ClusterServer::RequestDataSyncCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::RequestDataSyncRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   ClusterServer::RequestDataSyncRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Master instance of the cluster is assigned
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::ClusterMasterAssignedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   ClusterServer::ClusterMasterAssignedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Master vote
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::ClusterMasterVoteC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   ClusterServer::ClusterMasterVoteC2SEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Update cluster service status
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::ClusterUpdateStatusC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   ClusterServer::ClusterUpdateStatusC2SEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Update cluster service workload
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::ClusterUpdateWorkloadC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   ClusterServer::ClusterUpdateWorkloadC2SEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Get lowest workloaded cluster member
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::GetLowestWorkloadClusterMemberCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   ClusterServer::GetLowestWorkloadClusterMemberCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::GetLowestWorkloadClusterMemberRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   ClusterServer::GetLowestWorkloadClusterMemberRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Called when a player entity is created
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::GamePlayerEntityCreatedC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   ClusterServer::GamePlayerEntityCreatedC2SEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Called when a player entity is deleted
				MessageDebugTraceMapClusterServer.insert(std::make_pair(ClusterServer::GamePlayerEntityDeletedC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   ClusterServer::GamePlayerEntityDeletedC2SEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
			}; // void RegisterDebugTraceClusterServer()


			///////////////////////////////////////////////////////////////
			// ClusterServer Debug trace
			Result DebugOutClusterServer( const char *Prefix, MessageData *pMsg )
			{
 
				Result hr;
				auto itFount = MessageDebugTraceMapClusterServer.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// ClusterServer Debug trace
				itFount = MessageDebugTraceMapClusterServer.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapClusterServer.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // Result DebugOutClusterServer( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


