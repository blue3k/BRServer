////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : KyungKun Ko
//
// Description : Server Config
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "SFEngineToolPCH.h"
#include "SFServerConfig.h"
#include "String/FixedString32.h"

#include <libxml/parser.h>
#include <libxml/tree.h>



namespace SF
{

	ServerConfig::ServerConfig(IMemoryManager& memoryManager)
		: m_MemoryManager(memoryManager)
		, m_DBInstances(memoryManager)
		, m_DBClusters(memoryManager)
		, m_Servers(memoryManager)
		, m_GameClusters(memoryManager)
	{
		
	}

	ServerConfig::~ServerConfig()
	{
		for (auto& itItem : m_DBInstances)
		{
			IMemoryManager::Delete(itItem);
		}
		m_DBInstances.Clear();

		for (auto& itItem : m_DBClusters)
		{
			IMemoryManager::Delete(itItem);
		}
		m_DBClusters.Clear();

		for (auto& itItem : m_Servers)
		{
			IMemoryManager::Delete(itItem);
		}
		m_Servers.Clear();

		for (auto& itItem : m_GameClusters)
		{
			IMemoryManager::Delete(itItem);
		}
		m_GameClusters.Clear();
	}

	const ServerConfig::DBCluster* ServerConfig::GetDBCluster(FixedString clusterName) const
	{
		for (auto& itItem : m_DBClusters)
		{
			if (clusterName == FixedString(itItem->ClusterName))
				return itItem;
		}

		return nullptr;
	}

	ServerConfig::DBCluster* ServerConfig::GetDBCluster(FixedString clusterName)
	{
		for (auto& itItem : m_DBClusters)
		{
			if (clusterName == FixedString(itItem->ClusterName))
				return itItem;
		}

		return nullptr;
	}



	bool ServerConfig::operator == (const ServerConfig& src) const
	{
		// TODO:
		//for (auto& itItem : m_DBInstances)
		//{
		//	bool bFound = false;
		//	for (auto& srcItem : src.GetDBInstances())
		//	{
		//		if (*itItem == *srcItem)
		//		{
		//			bFound = true;
		//			break;
		//		}
		//	}

		//	if (!bFound)
		//		return false;
		//}

		return true;
	}

}


