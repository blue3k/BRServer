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
			// Cmd: Join to the cluster
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::RequestDataSyncCmd::MID.IDSeq.MsgID,&Message::ClusterServer::RequestDataSyncCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::RequestDataSyncRes::MID.IDSeq.MsgID,&Message::ClusterServer::RequestDataSyncRes::ParseMessageToMessageBase));
			// C2S: Called when a player entity is created
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::GamePlayerEntityCreatedC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::GamePlayerEntityCreatedC2SEvt::ParseMessageToMessageBase));
			// C2S: Called when a player entity is deleted
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::ClusterServer::GamePlayerEntityDeletedC2SEvt::MID.IDSeq.MsgID,&Message::ClusterServer::GamePlayerEntityDeletedC2SEvt::ParseMessageToMessageBase));
		}; // void RegisterParserClusterServer()


	}; // namespace Protocol
}; // namespace SF


