////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : Generated
// 
// Description : ClusterServer Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/ClusterServerMsgParsing.h"
#include "Protocol/Message/ClusterServerMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// ClusterServer message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserClusterServer()
		{
 			// Cmd: Cluster member list query
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::GetClusterMemberListCmd::MID.IDSeq.MsgID,&Message::ClusterServer::GetClusterMemberListCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::GetClusterMemberListRes::MID.IDSeq.MsgID,&Message::ClusterServer::GetClusterMemberListRes::ParseMessageToMessageBase));
			// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::JoinClusterCmd::MID.IDSeq.MsgID,&Message::ClusterServer::JoinClusterCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::JoinClusterRes::MID.IDSeq.MsgID,&Message::ClusterServer::JoinClusterRes::ParseMessageToMessageBase));
			// C2S: Do not let it broadcasted while it's manual broadcast packet
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::NewServerServiceJoinedC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::NewServerServiceJoinedC2SEvt::ParseMessageToMessageBase));
			// C2S: Sync cluster service informations
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::SyncClusterServiceC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::SyncClusterServiceC2SEvt::ParseMessageToMessageBase));
			// Cmd: Join to the cluster
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::RequestDataSyncCmd::MID.IDSeq.MsgID,&Message::ClusterServer::RequestDataSyncCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::RequestDataSyncRes::MID.IDSeq.MsgID,&Message::ClusterServer::RequestDataSyncRes::ParseMessageToMessageBase));
			// S2C: Master instance of the cluster is assigned
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::ClusterMasterAssignedS2CEvt::MID.IDSeq.MsgID,&Message::ClusterServer::ClusterMasterAssignedS2CEvt::ParseMessageToMessageBase));
			// C2S: Master vote
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::ClusterMasterVoteC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::ClusterMasterVoteC2SEvt::ParseMessageToMessageBase));
			// C2S: Update cluster service status
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::ClusterUpdateStatusC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::ClusterUpdateStatusC2SEvt::ParseMessageToMessageBase));
			// C2S: Update cluster service workload
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::ClusterUpdateWorkloadC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::ClusterUpdateWorkloadC2SEvt::ParseMessageToMessageBase));
			// Cmd: Get lowest workloaded cluster member
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::GetLowestWorkloadClusterMemberCmd::MID.IDSeq.MsgID,&Message::ClusterServer::GetLowestWorkloadClusterMemberCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::GetLowestWorkloadClusterMemberRes::MID.IDSeq.MsgID,&Message::ClusterServer::GetLowestWorkloadClusterMemberRes::ParseMessageToMessageBase));
			// C2S: Called when a player entity is created
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::GamePlayerEntityCreatedC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::GamePlayerEntityCreatedC2SEvt::ParseMessageToMessageBase));
			// C2S: Called when a player entity is deleted
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::GamePlayerEntityDeletedC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::GamePlayerEntityDeletedC2SEvt::ParseMessageToMessageBase));
		}; // void RegisterParserClusterServer()


	}; // namespace Protocol
}; // namespace SF


