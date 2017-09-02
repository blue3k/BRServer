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
#include "SFServerConfigZooKeeper.h"
#include "String/FixedString32.h"

#include <libxml/parser.h>
#include <libxml/tree.h>



namespace SF
{

	ServerConfigZooKeeper::ServerConfigZooKeeper(ServerConfig& config, ZooKeeper& zkInstance)
		: m_Config(config)
		, m_zkInstance(zkInstance)
	{
		
	}

	ServerConfigZooKeeper::~ServerConfigZooKeeper()
	{
	}

	template<class Func>
	Result ServerConfigZooKeeper::ForeachChild(const String& nodePath, Func func)
	{
		StaticArray<String, 32> childList(GetMemoryManager());
		Result result = m_zkInstance.GetChildren(nodePath, childList);
		if (!result) return result;

		for (auto& itChildPath : childList)
		{
			result = func(itChildPath);
			if (!result)
				break;
		}

		return result;
	}

	Result ServerConfigZooKeeper::GetNodeValue(const String& nodePath, Json::Value& jsonValue)
	{
		StaticArray<uint8_t, 1024> valueBuffer(GetMemoryManager());
		Result result = m_zkInstance.Get(nodePath, valueBuffer, nullptr);
		if (!result) return result;

		Json::Reader reader;

		auto bRes = reader.parse(reinterpret_cast<const char*>(valueBuffer.data()), reinterpret_cast<const char*>(valueBuffer.data()) + valueBuffer.GetItemCount(), jsonValue, false);
		if (!bRes)
			return ResultCode::FAIL;

		return result;
	}

	Result ServerConfigZooKeeper::SetNodeValue(const String& nodePath, const Json::Value& jsonValue)
	{
		Json::FastWriter writer;

		auto stringValue = std::forward<std::string>(writer.write(jsonValue));

		return m_zkInstance.Set(nodePath, stringValue);
	}

	Result ServerConfigZooKeeper::CreateNode(const String& parentNodePath, const char* nodeName, const Json::Value& nodeValue)
	{
		String fullPath;
		if(nodeName != nullptr)
			fullPath.Format("{0}/{1}", parentNodePath, nodeName);
		else
			fullPath = parentNodePath;

		Json::FastWriter writer;
		auto stringValue = std::forward<std::string>(writer.write(nodeValue));

		return m_zkInstance.Create(fullPath, stringValue, nullptr, 0, fullPath);
	}


	String ServerConfigZooKeeper::GetLeapNodeName(const String& nodePath)
	{
		DynamicArray<String> pathList(GetMemoryManager());
		nodePath.Split("/", true, pathList);
		if (pathList.GetItemCount() > 0)
			return pathList[pathList.GetItemCount() - 1];
		else
			return String(GetMemoryManager());
	}


	Json::Value ServerConfigZooKeeper::ToJsonSafeString(const String& src)
	{
		static const char* NullString = "";
		auto cstr = (const char*)src;
		assert(cstr != nullptr);
		return Json::Value(cstr == nullptr ? NullString : cstr);
	}



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Node setting value parsing
	//


	Result ServerConfigZooKeeper::ParseNetPrivate(const Json::Value& itemValue, ServerConfig::NetPrivate& privateNet)
	{
		privateNet.IP = itemValue.get("IP", Json::Value("")).asCString();
		privateNet.Port = itemValue.get("Port", Json::Value("")).asUInt();

		return ResultCode::SUCCESS;
	}

	Result ServerConfigZooKeeper::ParseNetPublic(const Json::Value& itemValue, ServerConfig::NetPublic& publicNet)
	{
		publicNet.IPV4 = itemValue.get("IPV4", Json::Value("")).asCString();
		publicNet.IPV6 = itemValue.get("IPV6", Json::Value("")).asCString();
		publicNet.ListenIP = itemValue.get("ListenIP", Json::Value("")).asCString();
		publicNet.Port = itemValue.get("Port", Json::Value("")).asUInt();
		publicNet.MaxConnection = itemValue.get("MaxConnection", Json::Value("")).asUInt();

		return ResultCode::SUCCESS;
	}

	Result ServerConfigZooKeeper::ParseGenericServer(const Json::Value& itemValue, ServerConfig::GenericServer* pGenericServer)
	{
		pGenericServer->UID = itemValue.get("UID", Json::Value("")).asUInt();
		//pGenericServer->Name = itemValue["Name"].asCString();
		Result result = ParseNetPrivate(itemValue.get("NetPrivate",Json::Value(Json::objectValue)), pGenericServer->PrivateNet);
		if (!result) return result;

		return ResultCode::SUCCESS;
	}


	Result ServerConfigZooKeeper::ParseDBInstance(const Json::Value& itemValue, ServerConfig::DBInstance* pDBInstance)
	{
		//pDBInstance->InstanceName = itemValue["InstanceName"].asCString();
		pDBInstance->ConnectionString = itemValue.get("ConnectionString", Json::Value("")).asCString();
		pDBInstance->UserID = itemValue.get("UserID", Json::Value("")).asCString();
		pDBInstance->Password = itemValue.get("Password", Json::Value("")).asCString();

		return ResultCode::SUCCESS;
	}

	Result ServerConfigZooKeeper::ParseDBCluster(const Json::Value& itemValue, ServerConfig::DBCluster* pDBCluster)
	{
		pDBCluster->ClusterType = Enum<DBClusterType>().GetValue(itemValue.get("ClusterType", Json::Value("Normal")).asCString());
		pDBCluster->DBInstanceName = itemValue.get("DBInstanceName", Json::Value("")).asCString();
		pDBCluster->DBName = itemValue.get("DBName", Json::Value("")).asCString();

		return ResultCode::SUCCESS;
	}

	Result ServerConfigZooKeeper::ParseModule(const String& moduleName, const Json::Value& itemValue, ServerConfig::ServerModule* &pServerModule)
	{
		Result result;


		switch (Hash32(moduleName))
		{
		case "ModMatchingQueue_Game_8x1"_hash32:
		case "ModMatchingQueue_Game_8x2"_hash32:
		case "ModMatchingQueue_Game_8x3"_hash32:
		case "ModMatchingQueue_Game_8x4"_hash32:
		case "ModMatchingQueue_Game_8x5"_hash32:
		case "ModMatchingQueue_Game_8x6"_hash32:
		case "ModMatchingQueue_Game_8x7"_hash32:
		case "ModMatchingQueue_Game_8x1S"_hash32:
		case "ModMatchingQueue_Game_8x1W"_hash32:
		case "ModMatchingQueue_Game_4x1"_hash32:
		case "ModMatchingQueue_Game_4x2"_hash32:
		case "ModMatchingQueue_Game_4x3"_hash32:
		case "ModMatchingQueue_Game_4x1S"_hash32:
		case "ModMatchingQueue_Game_4x1W"_hash32:
		case "ModGamePartyManager"_hash32:
		case "ModMonitoring"_hash32:
		case "ModRanking"_hash32:
		{
			auto pModule = new(GetMemoryManager()) ServerConfig::ServerModule(GetMemoryManager());
			pServerModule = pModule;
		}
			break;

		case "ModMatching_Game_8"_hash32:
		{
			auto pModule = new(GetMemoryManager()) ServerConfig::ServerModuleMatching_8(GetMemoryManager());
			pModule->UseBot = itemValue.get("UseBot",Json::Value(false)).asBool();
			pServerModule = pModule;
		}
			break;
		case "ModMatching_Game_4"_hash32:
		{
			auto pModule = new(GetMemoryManager()) ServerConfig::ServerModuleMatching_4(GetMemoryManager());
			pModule->UseBot = itemValue.get("UseBot", Json::Value(false)).asBool();
			pServerModule = pModule;
		}
			break;
		case "ModPurchaseValidateGoogle"_hash32:
		{
			auto pModule = new(GetMemoryManager()) ServerConfig::ServerModuleGooglePurchaseValidate(GetMemoryManager());
			pModule->Account = itemValue.get("Account", Json::Value("")).asCString();
			pModule->P12KeyFile = itemValue.get("P12KeyFile", Json::Value("")).asCString();
			pModule->AuthScopes = itemValue.get("AuthScopes", Json::Value("")).asCString();
			pServerModule = pModule;
		}
			break;
		case "ModPurchaseValidateIOS"_hash32:
		{
			auto pModule = new(GetMemoryManager()) ServerConfig::ServerModuleIOSPurchaseValidate(GetMemoryManager());
			pModule->URL = itemValue.get("URL", Json::Value("")).asCString();
			pModule->AltURL = itemValue.get("AltURL", Json::Value("")).asCString();
			pServerModule = pModule;
		}
			break;
		case "ModLogin"_hash32:
		{
			auto pModule = new(GetMemoryManager()) ServerConfig::ServerModulePublicService(GetMemoryManager());
			result = ParseNetPublic(itemValue.get("NetPublic", Json::Value(Json::objectValue)), pModule->PublicNet);
			pServerModule = pModule;
		}
			break;
		case "ModGame"_hash32:
		{
			auto pModule = new(GetMemoryManager()) ServerConfig::ServerModulePublicService(GetMemoryManager());
			result = ParseNetPublic(itemValue.get("NetPublic", Json::Value(Json::objectValue)), pModule->PublicNet);
			pServerModule = pModule;
		}
			break;
		case "NetPrivate"_hash32:
			break;
		default:
			assert(false);
			break;
		}

		if (pServerModule != nullptr)
			pServerModule->ModuleName = moduleName;

		return result;
	}

	Result ServerConfigZooKeeper::ParseServerComponent(const String& componentName, const Json::Value& itemValue, ServerConfig::ServerComponent* &pServerComponent)
	{
		Result result;

		switch (Hash32(componentName))
		{
		case "ComponentGoogle"_hash32:
		{
			auto pComponent = new(GetMemoryManager()) ServerConfig::ServerComponentGoogle(GetMemoryManager());
			pComponent->Account = itemValue.get("Account", Json::Value("")).asCString();
			pComponent->P12KeyFile = itemValue.get("P12KeyFile", Json::Value("")).asCString();
			pComponent->AuthScopes = itemValue.get("AuthScopes", Json::Value("")).asCString();
			pServerComponent = pComponent;
		}
		break;
		case "ComponentIOS"_hash32:
		{
			auto pComponent = new(GetMemoryManager()) ServerConfig::ServerComponentIOS(GetMemoryManager());
			pComponent->URL = itemValue.get("URL", Json::Value("")).asCString();
			pComponent->AltURL = itemValue.get("AltURL", Json::Value("")).asCString();
			pServerComponent = pComponent;
		}
		break;
		case "NetPrivate"_hash32:
		case "NetPublic"_hash32:
			break;
		default:
			assert(false);
			break;
		}

		if (pServerComponent != nullptr)
			pServerComponent->ComponentName = componentName;

		return result;
	}

	Result ServerConfigZooKeeper::ParseGameServer(const Json::Value& itemValue, ServerConfig::GameServer* pGameServer)
	{
		Result result = ParseGenericServer(itemValue, pGameServer);
		if (!result)
			return result;

		result = ParseNetPublic(itemValue.get("NetPublic", Json::Value("")), pGameServer->PublicNet);

		return result;
	}

	Result ServerConfigZooKeeper::ParseGameInstanceServer(const Json::Value& itemValue, ServerConfig::GameInstanceServer* pGameInstanceServer)
	{
		Result result = ParseGenericServer(itemValue, pGameInstanceServer);
		if (!result)
			return result;

		return result;
	}

	Result ServerConfigZooKeeper::ParseGameCluster(const Json::Value& json, ServerConfig::GameCluster* pGameCluster)
	{
		//pGameCluster->GameClusterID = Enum<GameID>().GetValue(json["GameClusterID"].asCString());
		return ResultCode::SUCCESS;
	}



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Node loading
	//

	Result ServerConfigZooKeeper::LoadGameInstanceServer(const String& nodePath, ServerConfig::GameInstanceServer* &pServer)
	{
		Json::Value nodeValue;
		Result result = GetNodeValue(nodePath, nodeValue);
		if (!result)
			return result;

		pServer = new(GetMemoryManager()) ServerConfig::GameInstanceServer(GetMemoryManager());

		result = ParseGameInstanceServer(nodeValue, pServer);
		if (!result)
			return result;

		return result;
	}

	Result ServerConfigZooKeeper::LoadGameServer(const String& nodePath, ServerConfig::GameServer* &pServer)
	{
		Json::Value nodeValue;
		Result result = GetNodeValue(nodePath, nodeValue);
		if (!result)
			return result;

		pServer = new(GetMemoryManager()) ServerConfig::GameServer(GetMemoryManager());

		result = ParseGameServer(nodeValue, pServer);
		if (!result) return result;

		result = ForeachChild(nodePath, [&](const String& childName)
		{
			Json::Value childValue;
			ServerConfig::ServerComponent* pComponent = nullptr;

			String childPath(GetMemoryManager());
			childPath.Format("{0}/{1}", nodePath, childName);
			result = GetNodeValue(childPath, childValue);
			if (!result)
				return result;

			result = ParseServerComponent(childName, childValue, pComponent);
			if (!result)
				return result;

			if(pComponent != nullptr)
				pServer->Components.Append(pComponent);

			return result;
		});

		return result;
	}

	Result ServerConfigZooKeeper::LoadModuleServer(const String& nodePath, ServerConfig::ModuleServer* &pServer)
	{
		Json::Value nodeValue;
		Result result = GetNodeValue(nodePath, nodeValue);
		if (!result)
			return result;


		pServer = new(GetMemoryManager()) ServerConfig::ModuleServer(GetMemoryManager());

		result = ParseGenericServer(nodeValue, pServer);
		if (!result) return result;

		result = ForeachChild(nodePath, [&](const String& childName)
		{
			Json::Value childValue;
			ServerConfig::ServerModule* pModule = nullptr;

			String childPath(GetMemoryManager());
			childPath.Format("{0}/{1}", nodePath, childName);

			result = GetNodeValue(childPath, childValue);
			if (!result)
				return result;

			result = ParseModule(childName, childValue, pModule);
			if (!result)
				return result;

			if (pModule != nullptr)
				pServer->Modules.Append(pModule);

			return result;
		});

		return result;
	}

	Result ServerConfigZooKeeper::LoadDBClusters(const String& rootPath, Array<ServerConfig::DBCluster*>& dbClusters)
	{
		Result result = ForeachChild(rootPath, [&](const String& childName)
		{
			Json::Value nodeValue;

			String childPath(GetMemoryManager());
			childPath.Format("{0}/{1}", rootPath, childName);

			result = GetNodeValue(childPath, nodeValue);
			if (!result)
				return result;

			ServerConfig::DBCluster* pDBCluster = nullptr;
			pDBCluster = new(GetMemoryManager()) ServerConfig::DBCluster(GetMemoryManager());
			pDBCluster->ClusterName = childName;
			result = ParseDBCluster(nodeValue, pDBCluster);
			if (pDBCluster != nullptr)
				dbClusters.Append(pDBCluster);

			return result;
		});

		return result;
	}

	Result ServerConfigZooKeeper::LoadModuleServers(const String& rootPath, Array<ServerConfig::ModuleServer*>& servers)
	{
		Result result = ForeachChild(rootPath, [&](const String& childName)
		{
			String childPath(GetMemoryManager());
			childPath.Format("{0}/{1}", rootPath, childName);

			ServerConfig::ModuleServer* pModuleServer = nullptr;
			result = LoadModuleServer(childPath, pModuleServer);
			pModuleServer->Name = childName;
			if (pModuleServer != nullptr)
				servers.Append(pModuleServer);

			return result;
		});

		return result;
	}

	Result ServerConfigZooKeeper::LoadGameCluster(GameID gameClusterID, const String& nodePath, ServerConfig::GameCluster* &pGameCluster)
	{
		Result result;
		Json::Value clusterValue;
		result = GetNodeValue(nodePath, clusterValue);
		if (!result)
			return result;

		pGameCluster = new(GetMemoryManager()) ServerConfig::GameCluster(GetMemoryManager());
		pGameCluster->GameClusterID = gameClusterID;

		result = ParseGameCluster(clusterValue, pGameCluster);
		if (!result)
			return result;

		String rootPath;
		rootPath.Format("{0}/{1}", nodePath, "DBCluster");
		result = LoadDBClusters(rootPath, pGameCluster->DBClusters);
		if (!result)
			return result;

		rootPath.Format("{0}/{1}", nodePath, "Server");
		result = LoadModuleServers(rootPath, pGameCluster->ModuleServers);
		if (!result)
			return result;

		rootPath.Format("{0}/{1}", nodePath, "GameServer");
		result = ForeachChild(rootPath, [&](const String& nodeName)
		{
			String childPath(GetMemoryManager());
			childPath.Format("{0}/{1}", rootPath, nodeName);

			ServerConfig::GameServer* pServer = nullptr;
			result = LoadGameServer(childPath, pServer);
			pServer->Name = nodeName;
			if (pServer != nullptr)
				pGameCluster->GameServers.Append(pServer);

			return result;
		});
		if (!result)
			return result;

		rootPath.Format("{0}/{1}", nodePath, "GameInstanceServer");
		result = ForeachChild(rootPath, [&](const String& nodeName)
		{
			String childPath(GetMemoryManager());
			childPath.Format("{0}/{1}", rootPath, nodeName);

			ServerConfig::GameInstanceServer* pServer = nullptr;
			result = LoadGameInstanceServer(childPath, pServer);
			pServer->Name = nodeName;
			if (pServer != nullptr)
				pGameCluster->GameInstanceServers.Append(pServer);

			return result;
		});
		if (!result)
			return result;

		return result;
	}

	Result ServerConfigZooKeeper::LoadConfig(const String& nodePath)
	{
		Result result;

		String rootPath;

		rootPath.Format("{0}/{1}", nodePath, "DBInstance");
		result = ForeachChild(rootPath, [&](const String& nodeName)
		{
			Json::Value nodeValue;

			String childPath(GetMemoryManager());
			childPath.Format("{0}/{1}", rootPath, nodeName);

			result = GetNodeValue(childPath, nodeValue);
			if (!result)
				return result;

			ServerConfig::DBInstance* pDBInstance = new(GetMemoryManager()) ServerConfig::DBInstance(GetMemoryManager());
			pDBInstance->InstanceName = nodeName;
			result = ParseDBInstance(nodeValue, pDBInstance);
			if (pDBInstance != nullptr)
				m_Config.GetDBInstances().Append(pDBInstance);

			return result;
		});
		if (!result)
			return result;

		rootPath.Format("{0}/{1}", nodePath, "DBCluster");
		result = LoadDBClusters(rootPath, m_Config.GetDBClusters());
		if (!result)
			return result;

		rootPath.Format("{0}/{1}", nodePath, "Server");
		result = LoadModuleServers(rootPath, m_Config.GetServers());
		if (!result)
			return result;


		rootPath.Format("{0}/{1}", nodePath, "GameCluster");
		result = ForeachChild(rootPath, [&](const String& nodeName)
		{
			Json::Value nodeValue;
			String childPath(GetMemoryManager());
			childPath.Format("{0}/{1}", rootPath, nodeName);

			result = GetNodeValue(childPath, nodeValue);
			if (!result)
				return result;

			ServerConfig::GameCluster* pGameCluster = nullptr;
			result = LoadGameCluster(Enum<GameID>().GetValue(nodeName), childPath, pGameCluster);
			if (pGameCluster != nullptr)
			{
				m_Config.GetGameClusters().Append(pGameCluster);
			}

			return result;
		});
		if (!result)
			return result;



		return result;
	}



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Node setting value generation
	//


	Json::Value ServerConfigZooKeeper::ToJsonNetPrivate(const ServerConfig::NetPrivate& privateNet) const
	{
		Json::Value itemValue(Json::objectValue);

		itemValue["IP"] = ToJsonSafeString(privateNet.IP);
		itemValue["Port"] = Json::Value(privateNet.Port);

		return itemValue;
	}

	Json::Value ServerConfigZooKeeper::ToJsonNetPublic(const ServerConfig::NetPublic& publicNet) const
	{
		Json::Value itemValue(Json::objectValue);

		itemValue["IPV4"] = ToJsonSafeString(publicNet.IPV4);
		itemValue["IPV6"] = ToJsonSafeString(publicNet.IPV6);
		itemValue["ListenIP"] = ToJsonSafeString(publicNet.ListenIP);
		itemValue["Port"] = Json::Value(publicNet.Port);
		itemValue["MaxConnection"] = Json::Value(publicNet.MaxConnection);

		return itemValue;
	}

	Json::Value ServerConfigZooKeeper::ToJsonGenericServer(const ServerConfig::GenericServer* pGenericServer) const
	{
		Json::Value itemValue(Json::objectValue);

		itemValue["UID"] = Json::Value(pGenericServer->UID);
		//itemValue["Name"] = ToJsonSafeString(pGenericServer->Name);
		itemValue["NetPrivate"] = ToJsonNetPrivate(pGenericServer->PrivateNet);

		return itemValue;
	}

	Json::Value ServerConfigZooKeeper::ToJsonDBInstance(const ServerConfig::DBInstance* pDBInstance) const
	{
		Json::Value itemValue(Json::objectValue);

		//itemValue["InstanceName"] = ToJsonSafeString(pDBInstance->InstanceName);
		itemValue["ConnectionString"] = ToJsonSafeString(pDBInstance->ConnectionString);
		itemValue["UserID"] = ToJsonSafeString(pDBInstance->UserID);
		itemValue["Password"] = ToJsonSafeString(pDBInstance->Password);

		return itemValue;
	}

	Json::Value ServerConfigZooKeeper::ToJsonDBCluster(const ServerConfig::DBCluster* pDBCluster) const
	{
		Json::Value itemValue(Json::objectValue);

		itemValue["ClusterType"] = ToJsonSafeString(Enum<DBClusterType>().GetValueName(pDBCluster->ClusterType));
		itemValue["DBInstanceName"] = ToJsonSafeString(pDBCluster->DBInstanceName);
		itemValue["DBName"] = ToJsonSafeString(pDBCluster->DBName);

		return itemValue;
	}

	Json::Value ServerConfigZooKeeper::ToJsonModule(const ServerConfig::ServerModule* pServerModule) const
	{
		Json::Value itemValue(Json::objectValue);

		switch (Hash32((const char*)pServerModule->ModuleName))
		{
		case "ModMatchingQueue_Game_8x1"_hash32:
		case "ModMatchingQueue_Game_8x2"_hash32:
		case "ModMatchingQueue_Game_8x3"_hash32:
		case "ModMatchingQueue_Game_8x4"_hash32:
		case "ModMatchingQueue_Game_8x5"_hash32:
		case "ModMatchingQueue_Game_8x6"_hash32:
		case "ModMatchingQueue_Game_8x7"_hash32:
		case "ModMatchingQueue_Game_8x1S"_hash32:
		case "ModMatchingQueue_Game_8x1W"_hash32:
		case "ModMatchingQueue_Game_4x1"_hash32:
		case "ModMatchingQueue_Game_4x2"_hash32:
		case "ModMatchingQueue_Game_4x3"_hash32:
		case "ModMatchingQueue_Game_4x1S"_hash32:
		case "ModMatchingQueue_Game_4x1W"_hash32:
		case "ModGamePartyManager"_hash32:
			break;

		case "ModMatching_Game_8"_hash32:
			itemValue["UseBot"] = Json::Value(((ServerConfig::ServerModuleMatching_8*)pServerModule)->UseBot);
			break;
		case "ModMatching_Game_4"_hash32:
			itemValue["UseBot"] = Json::Value(((ServerConfig::ServerModuleMatching_4*)pServerModule)->UseBot);
			break;
		case "ModPurchaseValidateGoogle"_hash32:
		{
			auto pModule = static_cast<const ServerConfig::ServerModuleGooglePurchaseValidate*>(pServerModule);
			itemValue["Account"] = ToJsonSafeString(pModule->Account);
			itemValue["P12KeyFile"] = ToJsonSafeString(pModule->P12KeyFile);
			itemValue["AuthScopes"] = ToJsonSafeString(pModule->AuthScopes);
		}
		break;
		case "ModPurchaseValidateIOS"_hash32:
		{
			auto pModule = static_cast<const ServerConfig::ServerModuleIOSPurchaseValidate*>(pServerModule);
			itemValue["URL"] = ToJsonSafeString(pModule->URL);
			itemValue["AltURL"] = ToJsonSafeString(pModule->AltURL);
		}
		break;
		case "ModMonitoring"_hash32:
			break;
		case "ModLogin"_hash32:
		{
			auto pModule = static_cast<const ServerConfig::ServerModulePublicService*>(pServerModule);
			itemValue["NetPublic"] = ToJsonNetPublic(pModule->PublicNet);
		}
		break;
		case "ModRanking"_hash32:
			break;
		case "ModGame"_hash32:
		{
			auto pModule = static_cast<const ServerConfig::ServerModulePublicService*>(pServerModule);
			itemValue["NetPublic"] = ToJsonNetPublic(pModule->PublicNet);
		}
		break;
		case "NetPrivate"_hash32:
			break;
		default:
			assert(false);
			break;
		}


		return itemValue;
	}

	Json::Value ServerConfigZooKeeper::ToJsonServerComponent(const ServerConfig::ServerComponent* pServerComponent) const
	{
		Json::Value itemValue(Json::objectValue);

		switch (Hash32((const char*)pServerComponent->ComponentName))
		{
		case "ComponentGoogle"_hash32:
		{
			auto pComponent = static_cast<const ServerConfig::ServerComponentGoogle*>(pServerComponent);
			itemValue["Account"] = ToJsonSafeString(pComponent->Account);
			itemValue["P12KeyFile"] = ToJsonSafeString(pComponent->P12KeyFile);
			itemValue["AuthScopes"] = ToJsonSafeString(pComponent->AuthScopes);
		}
		break;
		case "ComponentIOS"_hash32:
		{
			auto pComponent = static_cast<const ServerConfig::ServerComponentIOS*>(pServerComponent);
			itemValue["URL"] = ToJsonSafeString(pComponent->URL);
			itemValue["AltURL"] = ToJsonSafeString(pComponent->AltURL);
		}
		break;
		case "NetPrivate"_hash32:
		case "NetPublic"_hash32:
			break;
		default:
			assert(false);
			break;
		}

		return itemValue;
	}

	Json::Value ServerConfigZooKeeper::ToJsonGameServer(const ServerConfig::GameServer* pGameServer) const
	{
		Json::Value itemValue(Json::objectValue);

		itemValue = ToJsonGenericServer(pGameServer);
		itemValue["NetPublic"] = ToJsonNetPublic(pGameServer->PublicNet);

		return itemValue;
	}

	Json::Value ServerConfigZooKeeper::ToJsonGameInstanceServer(const ServerConfig::GameInstanceServer* pGameInstanceServer) const
	{
		Json::Value itemValue(Json::objectValue);

		itemValue = ToJsonGenericServer(pGameInstanceServer);

		return itemValue;
	}

	Json::Value ServerConfigZooKeeper::ToJsonGameCluster(const ServerConfig::GameCluster* pGameCluster) const
	{
		Json::Value itemValue(Json::objectValue);

		//itemValue["GameClusterID"] = Enum<GameID>().GetValueName(pGameCluster->GameClusterID);

		return itemValue;
	}

	Result ServerConfigZooKeeper::StoreGameInstanceServer(const String& nodePath, const ServerConfig::GameInstanceServer* pServer)
	{
		Json::Value nodeValue(Json::objectValue);
		nodeValue = ToJsonGameInstanceServer(pServer);

		return CreateNode(nodePath, nullptr, nodeValue);
	}

	Result ServerConfigZooKeeper::StoreGameServer(const String& nodePath, const ServerConfig::GameServer* pServer)
	{
		Result result;
		Json::Value nodeValue(Json::objectValue);

		nodeValue = ToJsonGameServer(pServer);

		result = CreateNode(nodePath, nullptr, nodeValue);
		if (!result)
			return result;

		for (auto& itComponent : pServer->Components)
		{
			Json::Value componentValue = ToJsonServerComponent(itComponent);

			result = CreateNode(nodePath, itComponent->ComponentName, componentValue);
			if (!result)
				return result;
		}

		return result;
	}

	Result ServerConfigZooKeeper::StoreModuleServer(const String& nodePath, const ServerConfig::ModuleServer* pServer)
	{
		Result result;
		Json::Value nodeValue(Json::objectValue);

		nodeValue = ToJsonGenericServer(pServer);
		result = CreateNode(nodePath, nullptr, nodeValue);
		if (!result)
			return result;

		for (auto& itModule : pServer->Modules)
		{
			Json::Value moduleValue = ToJsonModule(itModule);

			result = CreateNode(nodePath, itModule->ModuleName, moduleValue);
			if (!result)
				return result;
		}

		return result;
	}

	Result ServerConfigZooKeeper::StoreGameCluster(const String& nodePath, const ServerConfig::GameCluster* pGameCluster)
	{
		Result result;

		// Destroy if already have it, and create new one
		if (m_zkInstance.Exists(nodePath))
			result = m_zkInstance.DeleteTree(nodePath);
		String outPath;
		result = m_zkInstance.Create(nodePath, "", nullptr, 0, outPath);
		if (!result) return result;

		String rootPath;
		rootPath.Format("{0}/{1}", nodePath, "DBCluster");
		result = m_zkInstance.Create(rootPath, "", nullptr, 0, outPath);
		if (!result)
			return result;

		Json::Value clusterValue = ToJsonGameCluster(pGameCluster);
		result = SetNodeValue(nodePath, clusterValue);
		if (!result)
			return result;

		for (auto& itItem : pGameCluster->DBClusters)
		{
			Json::Value itemValue = ToJsonDBCluster(itItem);
			result = CreateNode(rootPath, itItem->ClusterName, itemValue);
			if (!result)
				return result;
		}

		// create root for module servers
		rootPath.Format("{0}/{1}", nodePath, "Server");
		result = m_zkInstance.Create(rootPath, "", nullptr, 0, outPath);
		if (!result)
			return result;

		for (auto& itServer : pGameCluster->ModuleServers)
		{
			String childPath;
			childPath.Format("{0}/{1}", rootPath, itServer->Name);

			result = StoreModuleServer(childPath, itServer);
			if (!result)
				return result;
		}

		// create root for module servers
		rootPath.Format("{0}/{1}", nodePath, "GameServer");
		result = m_zkInstance.Create(rootPath, "", nullptr, 0, outPath);
		if (!result)
			return result;

		for (auto& itServer : pGameCluster->GameServers)
		{
			String childPath;
			childPath.Format("{0}/{1}", rootPath, itServer->Name);

			result = StoreGameServer(childPath, itServer);
			if (!result)
				return result;
		}

		// create root for module servers
		rootPath.Format("{0}/{1}", nodePath, "GameInstanceServer");
		result = m_zkInstance.Create(rootPath, "", nullptr, 0, outPath);
		if (!result)
			return result;

		for (auto& itServer : pGameCluster->GameInstanceServers)
		{
			String childPath;
			childPath.Format("{0}/{1}", rootPath, itServer->Name);

			result = StoreGameInstanceServer(childPath, itServer);
			if (!result)
				return result;
		}

		return result;
	}


	Result ServerConfigZooKeeper::StoreConfig(const String& nodePath)
	{
		Result result;

		if (m_zkInstance.Exists(nodePath))
			result = m_zkInstance.DeleteTree(nodePath);
		if (!result)
			return result;

		String outPath(GetMemoryManager());
		StaticArray<uint8_t, 16> valueBuffer(GetMemoryManager());
		result = m_zkInstance.Create(nodePath, valueBuffer, nullptr, 0, outPath);
		if (!result)
			return result;

		String rootPath;

		rootPath.Format("{0}/{1}", nodePath, "DBInstance");
		result = m_zkInstance.Create(rootPath, "", nullptr, 0, outPath);
		if (!result)
			return result;

		for (auto& itItem : m_Config.GetDBInstances())
		{
			Json::Value itemValue = ToJsonDBInstance(itItem);
			result = CreateNode(rootPath, itItem->InstanceName, itemValue);
			if (!result)
				return result;
		}


		rootPath.Format("{0}/{1}", nodePath, "DBCluster");
		result = m_zkInstance.Create(rootPath, "", nullptr, 0, outPath);
		if (!result)
			return result;

		for (auto& itItem : m_Config.GetDBClusters())
		{
			Json::Value itemValue = ToJsonDBCluster(itItem);
			result = CreateNode(rootPath, itItem->ClusterName, itemValue);
			if (!result)
				return result;
		}


		// create root for module servers
		rootPath.Format("{0}/{1}", nodePath, "Server");
		result = m_zkInstance.Create(rootPath, "", nullptr, 0, outPath);
		if (!result)
			return result;

		for (auto& itItem : m_Config.GetServers())
		{
			String childPath;
			childPath.Format("{0}/{1}", rootPath, itItem->Name);

			result = StoreModuleServer(childPath, itItem);
			if (!result)
				return result;
		}

		// create root for Game clusters
		rootPath.Format("{0}/{1}", nodePath, "GameCluster");
		result = m_zkInstance.Create(rootPath, "", nullptr, 0, outPath);
		if (!result)
			return result;

		for (auto& itItem : m_Config.GetGameClusters())
		{
			String childPath;
			childPath.Format("{0}/{1}", rootPath, Enum<GameID>().GetValueName(itItem->GameClusterID));

			result = StoreGameCluster(childPath, itItem);
			if (!result)
				return result;
		}


		return result;
	}


	// Load config from server
	Result ServerConfigZooKeeper::LoadConfig(const char* configNodeRoot)
	{
		return LoadConfig(String(GetMemoryManager(), configNodeRoot));
	}

	// Store config to server
	Result ServerConfigZooKeeper::StoreConfig(const char* configNodeRoot)
	{
		return StoreConfig(String(GetMemoryManager(), configNodeRoot));
	}



}


