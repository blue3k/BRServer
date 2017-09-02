////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
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
#include "Protocol/Message/ClusterServerMsgDebug.h"
#include "Protocol/Message/ClusterServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// ClusterServer Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceClusterServer()
		{
 			// Cmd: Cluster member list query
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::GetClusterMemberListCmd::MID.IDSeq.MsgID,&Message::ClusterServer::GetClusterMemberListCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::GetClusterMemberListRes::MID.IDSeq.MsgID,&Message::ClusterServer::GetClusterMemberListRes::TraceOut));
			// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::JoinClusterCmd::MID.IDSeq.MsgID,&Message::ClusterServer::JoinClusterCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::JoinClusterRes::MID.IDSeq.MsgID,&Message::ClusterServer::JoinClusterRes::TraceOut));
			// C2S: Do not let it broadcasted while it's manual broadcast packet
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::NewServerServiceJoinedC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::NewServerServiceJoinedC2SEvt::TraceOut));
			// C2S: Sync cluster service informations
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::SyncClusterServiceC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::SyncClusterServiceC2SEvt::TraceOut));
			// Cmd: Join to the cluster
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::RequestDataSyncCmd::MID.IDSeq.MsgID,&Message::ClusterServer::RequestDataSyncCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::RequestDataSyncRes::MID.IDSeq.MsgID,&Message::ClusterServer::RequestDataSyncRes::TraceOut));
			// S2C: Master instance of the cluster is assigned
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::ClusterMasterAssignedS2CEvt::MID.IDSeq.MsgID,&Message::ClusterServer::ClusterMasterAssignedS2CEvt::TraceOut));
			// C2S: Master vote
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::ClusterMasterVoteC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::ClusterMasterVoteC2SEvt::TraceOut));
			// C2S: Update cluster service status
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::ClusterUpdateStatusC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::ClusterUpdateStatusC2SEvt::TraceOut));
			// C2S: Update cluster service workload
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::ClusterUpdateWorkloadC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::ClusterUpdateWorkloadC2SEvt::TraceOut));
			// Cmd: Get lowest workloaded cluster member
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::GetLowestWorkloadClusterMemberCmd::MID.IDSeq.MsgID,&Message::ClusterServer::GetLowestWorkloadClusterMemberCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::GetLowestWorkloadClusterMemberRes::MID.IDSeq.MsgID,&Message::ClusterServer::GetLowestWorkloadClusterMemberRes::TraceOut));
			// C2S: Called when a player entity is created
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::GamePlayerEntityCreatedC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::GamePlayerEntityCreatedC2SEvt::TraceOut));
			// C2S: Called when a player entity is deleted
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::GamePlayerEntityDeletedC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::GamePlayerEntityDeletedC2SEvt::TraceOut));
		}; // void RegisterDebugTraceClusterServer()


	}; // namespace Protocol
}; // namespace SF


