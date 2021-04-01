////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : ClusterServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/ClusterServerMsgDebug.h"
#include "Protocol/ClusterServerMsgClass.h"



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
		}; // void RegisterDebugTraceClusterServer()


	}; // namespace Protocol
}; // namespace SF


