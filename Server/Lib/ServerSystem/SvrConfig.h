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
#include "Common/BrBaseTypes.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrXML.h"
#include "String/FixedString.h"



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


	// Public Net socket
	class PublicNetSocket : public XML::DOMElement
	{
	public:
		PublicNetSocket();

		// IP address
		std::string	IPV4;
		std::string	IPV6;

		std::string	ListenIP;

		// Port
		USHORT Port;

		// Max connection
		UINT MaxConnection;

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

		// Instance name
		std::string DBInstanceName;

		// DB Name
		std::string DBName;

		virtual void AddChild( DOMElement *pChild ) override;

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


	///////////////////////////////////////////////////////////////////////////////////////
	//
	// Modules
	//

	// ModuleBase
	class ModuleBase : public XML::DOMElement
	{
	private:
		ClusterID m_ModuleClusterID;

	protected:
		void SetModuleClusterID(ClusterID clusterID) { m_ModuleClusterID = clusterID; }

	public:
		ModuleBase();
		ModuleBase(const char* name, ClusterID clusterID);

		virtual void SetName(const char* Name) override;

		ClusterID GetModuleClusterID() { return m_ModuleClusterID; }

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};

	// ModuleSimple
	class ModuleSimple : public ModuleBase
	{
	public:
		ModuleSimple();


		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};

	// ModuleMatching
	class ModuleMatching : public ModuleBase
	{
	public:
		ModuleMatching();

		bool UseBot;

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};

	// ModuleLogin
	class ModuleLogin : public ModuleBase
	{
	public:
		ModuleLogin();

		PublicNetSocket* NetPublic;

		FixedString GameCluster;

		virtual void AddChild(DOMElement *pChild) override;

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};

	// ModuleRanking
	class ModuleRanking : public ModuleBase
	{
	public:
		ModuleRanking();

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};

	// ModuleGame
	class ModuleGame : public ModuleBase
	{
	private:

		GameID m_GameID;

	public:
		ModuleGame();

		GameID GetGameID() { return m_GameID; }

		PublicNetSocket* NetPublic;

		virtual void AddChild(DOMElement *pChild) override;

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};

	// ModuleGameInstance
	class ModuleGameInstance : public ModuleBase
	{
	public:
		ModuleGameInstance();

		virtual void AddChild(DOMElement *pChild) override;

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};

	// ModulePurchaseValidateGoogle
	class ModulePurchaseValidateGoogle : public ModuleBase
	{
	public:
		ModulePurchaseValidateGoogle();

		std::string      Account;
		std::string      P12KeyFile;
		std::string      AuthScopes;

		// for parsing
		virtual bool SetAttributeValue(const std::string& name, const std::string& value) override;
	};

	// ModulePurchaseValidateIOS
	class ModulePurchaseValidateIOS : public ModuleBase
	{
	public:
		ModulePurchaseValidateIOS();

		std::string      URL;
		std::string      AltURL;

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
		PublicNetSocket*		NetPublic;

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
	//

	class ModuleServer : public GenericServer
	{
	private:

		GameID m_GameID;

	public:
		ModuleServer();

		GameID GetGameID() const { return m_GameID; }

		std::vector<ModuleBase*> Modules;
		
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
	class GameClusterInfo : public XML::DOMElement
	{
	private:
		GameID              m_GameID;

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

		GameID GetGameID() const { return m_GameID; }
		
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
		Result ClearConfig();


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
		std::vector<ModuleServer*>			ModuleServers;

		// game cluster Server
		GameClusterInfo*					GameCluster;

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
	Result LoadConfig( const char* strConfigFileName );

	// Clear Config data
	Result ClearConfig();

	// Get Config data instance
	const ConfigData& GetConfig();

	

}; // namespace Config
}; // namespace Svr
}; // namespace BR

