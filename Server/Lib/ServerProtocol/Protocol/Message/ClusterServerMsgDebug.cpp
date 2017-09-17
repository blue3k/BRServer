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
			// Cmd: Join to the cluster
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::RequestDataSyncCmd::MID.IDSeq.MsgID,&Message::ClusterServer::RequestDataSyncCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::RequestDataSyncRes::MID.IDSeq.MsgID,&Message::ClusterServer::RequestDataSyncRes::TraceOut));
			// C2S: Called when a player entity is created
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::GamePlayerEntityCreatedC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::GamePlayerEntityCreatedC2SEvt::TraceOut));
			// C2S: Called when a player entity is deleted
			MessageDebugTraceMap.insert(std::make_pair(Message::ClusterServer::GamePlayerEntityDeletedC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::GamePlayerEntityDeletedC2SEvt::TraceOut));
		}; // void RegisterDebugTraceClusterServer()


	}; // namespace Protocol
}; // namespace SF


