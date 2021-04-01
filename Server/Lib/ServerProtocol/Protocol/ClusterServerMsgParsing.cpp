////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 Kyungkun Ko
// 
// Author : Generated
// 
// Description : ClusterServer Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/ClusterServerMsgParsing.h"
#include "Protocol/ClusterServerMsgClass.h"



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
		}; // void RegisterParserClusterServer()


	}; // namespace Protocol
}; // namespace SF


