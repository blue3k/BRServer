////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server configuration  definition
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BRBaseTypes.h"
#include "Common/BRSvrTypes.h"
#include "Common/BrXML.h"


namespace BR {
namespace Svr { 
namespace Config { 


	//////////////////////////////////////////////////////////////////////////////
	//
	//	Generic server config
	//

	// Net socket
	class NetSocket : public XML::DOMElement
	{
	public:
		NetSocket();

		// IP address
		std::string	IP;

		// Port
		USHORT Port;

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};
	

	//////////////////////////////////////////////////////////////
	// DB Instance 
	class DBInstance : public XML::DOMElement
	{
	public:
		DBInstance();

		// Instance name
		std::string InstanceName;

		// Server connection string
		std::string ConnectionString;

		// ID
		std::string UserID;

		// Password
		std::string Password;

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};
	

	//////////////////////////////////////////////////////////////
	// DB Cluster instance
	class DBClusterInstance : public XML::DOMElement
	{
	public:
		DBClusterInstance();

		// Instance name
		std::string DBInstanceName;

		// DB Name
		std::string DBName;

		// DB partitioning ID, used for sharding
		UINT PartitioningID;

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};




	//////////////////////////////////////////////////////////////
	// DBSource
	class DBSource : public XML::DOMElement
	{
	public:
		DBSource();

		// Instance name
		std::string DBInstanceName;

		// DB Name
		std::string DBName;

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};


	//////////////////////////////////////////////////////////////
	// DB Cluster
	class DBCluster : public XML::DOMElement
	{
	public:
		DBCluster();

		// Cluster type
		DBClusterType					ClusterType;

		// shard partitioning count if cluster type is sharding
		UINT							PartitioningCount;

		// Instance name
		std::vector<DBClusterInstance*> DBMembers;

		virtual void AddChild( DOMElement *pChild );

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};

	//////////////////////////////////////////////////////////////
	// DB Server 
	class DBServer : public XML::DOMElement
	{
	public:
		DBServer();

		// Server connection string
		std::string ConnectionString;

		// ID
		std::string UserID;

		// Password
		std::string Password;

		// DataBase Name to connect
		std::string DBName;

		// DB Worker count
		ULONG		WorkerCount;

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};


	class ServerComponent : public XML::DOMElement
	{
	public:
		ServerComponent(const char* typeName = "ServerComponent");

	};

	class ServerComponentGoogle : public ServerComponent
	{
	public:
		ServerComponentGoogle();
		
		std::string      Account;
		std::string      P12KeyFile;
		std::string      AuthScopes;

		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};

	class ServerComponentIOS : public ServerComponent
	{
	public:
		ServerComponentIOS();

		std::string      URL;

		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};


	//////////////////////////////////////////////////////////////
	// server generic
	class GenericServer : public XML::DOMElement
	{
	public:
		GenericServer();

		// Server Unique ID
		UINT			UID;

		// Name of server
		std::string		Name;

		// Private Entity control thread count
		UINT			PrivateEntityControlCount;

		// Entity control thread count
		UINT			EntityControlCount;

		// Network IO Thread Count
		UINT			NetIOCount;

		// Private network socket
		NetSocket*		NetPrivate;

		std::vector<ServerComponent*> ServerComponents;
		
		///////////////////////////////////////////////////////////////////////////
		//
		//	Overriding
		//

		virtual void AddChild(DOMElement *pChild) override;

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;

		// Called when the load done
		virtual bool OnLoadDone() override;
	};


	//////////////////////////////////////////////////////////////
	// Server setting with public
	class PublicServer : public GenericServer
	{
	public:
		PublicServer();

		// Public network connection limits
		ULONG			MaxPublicConnection;

		// Public network info
		NetSocket*		NetPublic;
		
		///////////////////////////////////////////////////////////////////////////
		//
		//	Overriding
		//

		virtual void AddChild(DOMElement *pChild) override;

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};
	

	//////////////////////////////////////////////////////////////
	// Server setting
	
	class Module : public XML::DOMElement
	{
	public:

		static std::unordered_map<std::string,ClusterID> stm_ModuleNameMap;

	public:
		Module();

		ClusterID ModuleID;
		std::string Parameters;

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};


	class SharedModuleServer : public GenericServer
	{
	public:
		SharedModuleServer();

		std::vector<Module*> Modules;
		
		///////////////////////////////////////////////////////////////////////////
		//
		//	Overriding
		//

		virtual void AddChild(DOMElement *pChild) override;

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};


	

	//////////////////////////////////////////////////////////////
	// Server cluster
	class GameCluster : public XML::DOMElement
	{
	public:
		// Generic server
		DBCluster*			GameDB;
		DBCluster*			GameTransactionLogDB;

		// Cluster ID
		UINT				ClusterID;

		// Name
		std::string			ClusterName;

		// Game Servers
		std::vector<PublicServer*>	Games;

		// Game instance Servers
		std::vector<GenericServer*>	GameInstancess;

		// Game instance Servers
		std::vector<GenericServer*>	GameMatchingQueues;

		// Game instance Servers
		std::vector<GenericServer*>	GameMatchings;
		
		///////////////////////////////////////////////////////////////////////////
		//
		//	Overriding
		//

		virtual void AddChild(DOMElement *pChild) override;

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};




	//////////////////////////////////////////////////////////////////////////////
	//
	//	Configuration data
	//

 	class ConfigData : public XML::DOMElement
	{
	public:

 	public:
		ConfigData();
		virtual ~ConfigData();

		// clear loaded config values
		HRESULT ClearConfig();


		//////////////////////////////////////////////////////////////////////////////////////
		//
		//	Config fields
		//

		// DB Instances
		std::unordered_map<std::string,DBInstance*> DBInstances;

		GenericServer*						MonitoringServer;

		// Table DB server
		DBSource*							TableDB;

		// Accound DB server
		DBCluster*							AccountDB;

		// Login Session DB server
		DBCluster*							LoginSessionDB;

		// Ranking DB server
		DBCluster*							RankingDB;

		// Entity Server
		std::vector<GenericServer*>			EntityServers;

		// Login servers
		std::vector<PublicServer*>			LoginServers;

		// Shared module servers
		std::vector<SharedModuleServer*>	SharedModuleServers;

		// game cluster Server
		GameCluster*						GameCluster;

		// Map servers by ID
		std::unordered_map<ServerID,GenericServer*> ServerByID;

		///////////////////////////////////////////////////////////////////////////
		//
		//	Overriding
		//

		virtual void AddChild(DOMElement *pChild) override;

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;

	}; // class ConfigData



	//////////////////////////////////////////////////////////////////////////////
	//
	//	Configuration data Access Functions
	//

	// Load Config file
	HRESULT LoadConfig( const WCHAR* strConfigFileName );

	// Clear Config data
	HRESULT ClearConfig();

	// Get Config data instance
	const ConfigData& GetConfig();

	

}; // namespace Config
}; // namespace Svr
}; // namespace BR

