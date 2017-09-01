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

#pragma once

#include "String/FixedString32.h"
#include "String/SFString.h"


#include "ZooKeeper/SFZooKeeper.h"
#include "SFEnum.h"


typedef struct _xmlNode xmlNode;


namespace SF
{

	class ServerConfig
	{
	public:


		struct NetPrivate
		{
			String IP;
			uint Port = 0;

			NetPrivate(IMemoryManager& memoryManager)
				: IP(memoryManager)
			{}
		};


		struct NetPublic
		{
			String IPV4;
			String IPV6;
			String ListenIP;
			uint Port = 0;
			uint MaxConnection = 1000000;

			NetPublic(IMemoryManager& memoryManager)
				: IPV4(memoryManager)
				, IPV6(memoryManager)
				, ListenIP(memoryManager)
			{}
		};


		struct DBInstance
		{
			String InstanceName;
			String ConnectionString;
			String UserID;
			String Password;

			DBInstance(IMemoryManager& memoryManager)
				: InstanceName(memoryManager)
				, ConnectionString(memoryManager)
				, UserID(memoryManager)
				, Password(memoryManager)
			{}
		};

		struct DBCluster
		{
			String ClusterName;
			DBClusterType ClusterType;
			String DBInstanceName;
			String DBName;

			DBCluster(IMemoryManager& memoryManager)
				: ClusterName(memoryManager)
				, ClusterType(DBClusterType::Normal)
				, DBInstanceName(memoryManager)
				, DBName(memoryManager)
			{}
		};

		struct GenericServer
		{
			uint32_t UID = 0;
			String Name;
			NetPrivate PrivateNet;

			GenericServer(IMemoryManager& memoryManager)
				: Name(memoryManager)
				, PrivateNet(memoryManager)
			{}
			virtual ~GenericServer() {}
		};

		struct ServerModule
		{
			String ModuleName;
			ServerModule(IMemoryManager& memoryManager)
				: ModuleName(memoryManager)
			{}
			virtual ~ServerModule() {}
		};


		struct ServerModuleMatching_8 : public ServerModule
		{
			bool UseBot = false;

			ServerModuleMatching_8(IMemoryManager& memoryManager)
				: ServerModule(memoryManager)
			{}
		};

		struct ServerModuleMatching_4 : public ServerModule
		{
			bool UseBot = false;

			ServerModuleMatching_4(IMemoryManager& memoryManager)
				: ServerModule(memoryManager)
			{}
		};

		struct ServerModuleGooglePurchaseValidate : public ServerModule
		{
			String Account;
			String P12KeyFile;
			String AuthScopes;

			ServerModuleGooglePurchaseValidate(IMemoryManager& memoryManager)
				: ServerModule(memoryManager)
				, Account(memoryManager)
				, P12KeyFile(memoryManager)
				, AuthScopes(memoryManager)
			{}
		};

		struct ServerModuleIOSPurchaseValidate : public ServerModule
		{
			String URL;
			String AltURL;

			ServerModuleIOSPurchaseValidate(IMemoryManager& memoryManager)
				: ServerModule(memoryManager)
				, URL(memoryManager)
				, AltURL(memoryManager)
			{}
		};

		struct ServerModulePublicService : public ServerModule
		{
			NetPublic PublicNet;

			ServerModulePublicService(IMemoryManager& memoryManager)
				: ServerModule(memoryManager)
				, PublicNet(memoryManager)
			{}
		};


		struct ModuleServer : public GenericServer
		{
			DynamicArray<ServerModule*> Modules;

			ModuleServer(IMemoryManager& memoryManager) : GenericServer(memoryManager), Modules(memoryManager) {}
			virtual ~ModuleServer()
			{
				for (auto itServer : Modules)
				{
					IMemoryManager::Delete(itServer);
				}
				Modules.Clear();
			}
		};


		struct ServerComponent
		{
			String ComponentName;

			ServerComponent(IMemoryManager& memoryManager)
				: ComponentName(memoryManager)
			{}
			virtual ~ServerComponent() {}
		};

		struct ServerComponentGoogle : public ServerComponent
		{
			String Account;
			String P12KeyFile;
			String AuthScopes;

			ServerComponentGoogle(IMemoryManager& memoryManager)
				: ServerComponent(memoryManager)
				, Account(memoryManager)
				, P12KeyFile(memoryManager)
				, AuthScopes(memoryManager)
			{}
			virtual ~ServerComponentGoogle() {}
		};

		struct ServerComponentIOS : public ServerComponent
		{
			String URL;
			String AltURL;

			ServerComponentIOS(IMemoryManager& memoryManager)
				: ServerComponent(memoryManager)
				, URL(memoryManager)
				, AltURL(memoryManager)
			{}
			virtual ~ServerComponentIOS() {}
		};

		struct GameServer : public GenericServer
		{
			DynamicArray<ServerComponent*> Components;
			NetPublic PublicNet;

			GameServer(IMemoryManager& memoryManager)
				: GenericServer(memoryManager)
				, Components(memoryManager)
				, PublicNet(memoryManager)
			{}
			virtual ~GameServer()
			{
				for (auto& itComponent : Components)
				{
					IMemoryManager::Delete(itComponent);
				}
				Components.Clear();
			}
		};


		struct GameInstanceServer : public GenericServer
		{
			GameInstanceServer(IMemoryManager& memoryManager) : GenericServer(memoryManager) {}
		};

		struct GameCluster
		{
			GameID GameClusterID;

			DynamicArray<DBCluster*> DBClusters;
			DynamicArray<ModuleServer*> ModuleServers;
			DynamicArray<GameServer*> GameServers;
			DynamicArray<GameInstanceServer*> GameInstanceServers;

			GameCluster(IMemoryManager& memoryManager)
				: DBClusters(memoryManager)
				, ModuleServers(memoryManager)
				, GameServers(memoryManager)
				, GameInstanceServers(memoryManager)
			{}

			virtual ~GameCluster()
			{
				for (auto& itServer : DBClusters)
				{
					IMemoryManager::Delete(itServer);
				}
				DBClusters.Clear();

				for (auto& itServer : ModuleServers)
				{
					IMemoryManager::Delete(itServer);
				}
				ModuleServers.Clear();

				for (auto& itServer : GameServers)
				{
					IMemoryManager::Delete(itServer);
				}
				GameServers.Clear();

				for (auto& itServer : GameInstanceServers)
				{
					IMemoryManager::Delete(itServer);
				}
				GameInstanceServers.Clear();
			}
		};



	private:

		// MemoryManager for this instance
		IMemoryManager& m_MemoryManager;


		DynamicArray<DBInstance*> m_DBInstances;
		DynamicArray<DBCluster*> m_DBClusters;

		DynamicArray<ModuleServer*> m_Servers;
		DynamicArray<GameCluster*> m_GameClusters;

	private:

	public:

		// Constructor
		ServerConfig(IMemoryManager& memoryManager);
		~ServerConfig();

		// MemoryManager
		IMemoryManager& GetMemoryManager() { return m_MemoryManager; }

		const Array<DBInstance*>& GetDBInstances() const { return m_DBInstances; }
		Array<DBInstance*>& GetDBInstances() { return m_DBInstances; }

		const Array<DBCluster*>& GetDBClusters() const { return m_DBClusters; }
		Array<DBCluster*>& GetDBClusters() { return m_DBClusters; }

		const DBCluster* GetDBCluster(FixedString clusterName) const;
		DBCluster* GetDBCluster(FixedString clusterName);

		const Array<ModuleServer*>& GetServers() const { return m_Servers; }
		Array<ModuleServer*>& GetServers() { return m_Servers; }

		const Array<GameCluster*>& GetGameClusters() const { return m_GameClusters; }
		Array<GameCluster*>& GetGameClusters() { return m_GameClusters; }

		bool operator == (const ServerConfig& src) const;
	};
}

