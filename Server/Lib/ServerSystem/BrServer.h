////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server interface definition
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/StrUtil.h"
#include "Common/ArrayUtil.h"
#include "Net/Netdef.h"
//#include "ServerSystem/PlugIn.h"
#include "ServerSystem/SvrTypes.h"
#include "ServerSystem/TaskManager.h"
#include "ServerSystem/MasterEntity.h"
#include "ServerSystem/EntityTable.h"
#include "ServerSystem/SvrConfig.h"
#include "ServerSystem/ServerComponent.h"



namespace BR {
namespace Svr { 
namespace Config { 

	class ConfigData;

	typedef ::std::auto_ptr< ConfigData > ServerConfigurationsPtr;
}}}


namespace BR {
namespace Net {
	class ServerPeer;
	class ServerPeerTCP;
}
};


namespace BR {
	namespace DB {
		class QueryManager;
	}
};


namespace BR {
namespace Svr {

	class ServerEntityManager;
	class ServerEntity;
	class SimpleUserEntity;
	class Transaction;
	class EntityManager;


	// Server running state
	enum class ServerState {
		STOPED,
		STARTING,
		RUNNING,
		STOPING,
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//	Server definition
	//
	class BrServer : public BR::Thread, public MasterEntity, public Svr::ServerComponentCarrier
	{
	public:


		// Entity by ID map 

	private:
		// Server state
		ServerState		m_State;

		// UID of this server
		UINT32			m_uiUID;

		// Cluster ID
		//UINT			m_uiClusterID;

		// Startup sequence done
		bool			m_bIsStartProcessDone;


		// Network address
		NetAddress					m_AddrPrivate;

		// ServerPeer host interfaces
		Net::ServerPeerTCP*			m_pNetPrivate;
		
		// Server net class
		NetClass					m_NetClass;

		// Server config
		const Config::GenericServer	*m_pMyConfig;

		// Entity Table
		EntityTable					m_EntityTable;

		// Server execution time stamp
		ULONGLONG					m_ServerUpUTCTIme;

		// Loopback ServerEntity
		ServerEntity*				m_pLoopbackServerEntity;

		// Sync counter for service initialized
		SyncCounter					m_NumberServicesToWait;

		// DB manager list
		StaticArray<DB::QueryManager*, 10>	m_DBManagers;

		// Server status
		bool m_bIsNetPublicEnabled;
		bool m_bIsKillSignaled;
		bool m_bStartTransaction;

		// singletone instance
		static BrServer *stm_pServerInstance;


	protected:

		// Set Loopback entity
		inline void SetLoopbackServerEntity( ServerEntity* pLoopback );

		// Set server UID
		inline void SetServerUID( UINT uiUID );
		//inline void SetClusterID( UINT uiUID );

		// Set main server instance
		static void SetInstance( BrServer *pServerInstance );

		// Set my config 
		inline void SetMyConfig( const Config::GenericServer* pMyConfig );

		// Create entity manager
		virtual EntityManager* CreateEntityManager();
		virtual ServerEntity* CreateLoopbackEntity();

		//////////////////////////////////////////////////////////////////////////
		//
		//	Thread override
		//

		virtual bool OnStart();
		virtual bool Run();
		virtual bool OnEnd();


		///////////////////////////////////////////////////////////////////////////
		//
		//	Message Processing
		//

		//virtual HRESULT ProcessPrivateMessage( pIMsg

	public:
		BrServer( NetClass svrNetClass );
		virtual ~BrServer();

		
		// Get net class
		inline NetClass GetNetClass();


		// Set server state
		inline void SetServerState( ServerState state );

		inline SyncCounter& GetNumberServicesToWait()			{ return m_NumberServicesToWait; }


		// Get Server state
		inline ServerState GetServerState();
		
		// Get Loopback entity
		inline ServerEntity* GetLoopbackServerEntity();

		// Get Server UID
		inline UINT GetServerUID();
		//inline UINT GetClusterID();
		
		// Get Server start up time, UTC
		inline ULONGLONG GetServerUpTime();

		// Get main server instance
		static inline BrServer* GetInstance();

		// Get config
		inline const Config::GenericServer* GetMyConfig();

		// Get/Set Public network enable status
		inline bool IsNetPublicEnabled();
		inline virtual bool EnableNetPublic( bool bIsEnable );


		// Get net private
		inline BR::Net::ServerPeerTCP* GetNetPrivate()								{ return m_pNetPrivate; }

		// Get entity table
		inline EntityTable& GetEntityTable();

		template<class DBManagerType>
		HRESULT InitializeDBCluster(Svr::Config::DBCluster *pDBClusterCfg);

		//////////////////////////////////////////////////////////////////////////
		//
		//	virtual network process
		//

		// Process Private network event
		virtual HRESULT ProcessPrivateNetworkEvent();

		// Process Public network event
		virtual HRESULT ProcessPublicNetworkEvent();


		virtual HRESULT TerminateEntity() override;


		//////////////////////////////////////////////////////////////////////////
		//
		//	virtual interface definition
		//

		// initialize memory pool
		virtual HRESULT InitializeMemoryPool();

		// Terminate memory pool
		virtual HRESULT TerminateMemoryPool();


		// Apply configuration
		virtual HRESULT ApplyConfiguration();

		virtual HRESULT InitializeMonitoring();

		// Initialize server basic entities
		virtual HRESULT InitializeEntities();

		// Initialize server resource
		virtual HRESULT InitializeServerResource();

		// Close server and release resource
		virtual HRESULT CloseServerResource();


		// Initialize private Network
		virtual HRESULT InitializeNetPrivate();

		// Close Private Network
		virtual HRESULT CloseNetPrivate();

		// create remote entity by class
		virtual HRESULT CreateServerEntity( BR::NetClass netClass, ServerEntity* &pServerEntity ) = 0;

		// Initialize private Network
		virtual HRESULT InitializeNetPublic();

		// Close Public Network
		virtual HRESULT CloseNetPublic();


		// Run the task
		virtual HRESULT TickUpdate(Svr::TimerAction *pAction = nullptr);



		// Start server thread
		virtual HRESULT StartServer();

		// Stop server thread
		virtual HRESULT StopServer();

	};


	///////////////////////////////////////////////////////////////////////////////
	//
	//	Static interfaces
	//

	// Get entity table
	inline EntityTable& GetEntityTable();

	// Get Loopback ServerEntity
	inline ServerEntity* GetLoopbackServerEntity();
	
	// Get server ID
	inline ServerID GetMyServerID();

	template< class ComponentType >
	ComponentType* GetServerComponent();




#include "BrServer.inl"

}; // namespace BR
}; // namespace Svr





