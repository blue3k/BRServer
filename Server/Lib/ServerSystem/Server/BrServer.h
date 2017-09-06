////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Blue3k
// 
// Author : KyungKun Ko
//
// Description : Server interface definition
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "String/StrUtil.h"
#include "Container/SFArray.h"
#include "ResultCode/SFResultCodeDB.h"
#include "Net/NetDef.h"
#include "DB/DBClusterManager.h"
#include "Types/BrSvrTypes.h"
#include "Entity/MasterEntity.h"
#include "ServerEntity/ServerEntity.h"
#include "Task/ServerTaskManager.h"
#include "Component/ServerComponentCarrier.h"
#include "ServerConfig/SFServerConfig.h"


namespace SF {
namespace Svr { 
namespace Config { 

	class ConfigData;
}}}


namespace SF {
namespace Net {
	class ServerPeer;
	class ServerPeerTCP;
}
};


namespace SF {
	namespace DB {
		class DBClusterManager;
	}
};


namespace SF {
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
	class BrServer : public Thread, public MasterEntity
	{
	public:


		// Entity by ID map 

	private:

		ServerComponentCarrier m_Components;

		// Server state
		ServerState		m_State;

		// UID of this server
		uint32_t			m_uiUID;

		// Network address
		NetAddress					m_AddrPrivate;

		// ServerPeer host interfaces
		SharedPointerT<Net::ServerPeerTCP>			m_pNetPrivate;
		
		// Server net class
		NetClass					m_NetClass;

		// Server config
		const ServerConfig::GenericServer	*m_pMyConfig;

		// Server execution time stamp
		TimeStampSec					m_ServerUpUTCTIme;

		// Loopback ServerEntity
		SharedPointerT<ServerEntity>	m_pLoopbackServerEntity;

		// Sync counter for service initialized
		SyncCounter					m_NumberServicesToWait;

		// DB manager list
		StaticArray<DB::DBClusterManager*, 10>	m_DBManagers;

		// Startup sequence done
		//bool			m_bIsStartProcessDone;

		// Server status
		bool m_bIsNetPublicEnabled;
		bool m_bIsKillSignaled;
		bool m_bStartTransaction;


		PageQueue<SharedPointerAtomicT<Net::Connection>> m_NewConnectionQueue;


		// singleton instance
		static BrServer *stm_pServerInstance;


	protected:

		// Set Loopback entity
		inline void SetLoopbackServerEntity( ServerEntity* pLoopback );

		// Set server UID
		inline void SetServerUID( uint uiUID );
		//inline void SetClusterID( uint uiUID );

		// Set main server instance
		static void SetInstance( BrServer *pServerInstance );

		// Set my config 
		inline void SetMyConfig( const ServerConfig::GenericServer* pMyConfig );

		// Create entity manager
		virtual EntityManager* CreateEntityManager();
		virtual ServerEntity* CreateLoopbackEntity();




		//////////////////////////////////////////////////////////////////////////
		//
		//	Thread override
		//

		bool OnStart();
		virtual void Run() override;
		bool OnEnd();


		///////////////////////////////////////////////////////////////////////////
		//
		//	Message Processing
		//

		//virtual Result ProcessPrivateMessage( pIMsg

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
		inline uint GetServerUID();
		//inline uint GetClusterID();
		
		// Get Server start up time, UTC
		inline TimeStampSec GetServerUpTime();

		// Get main server instance
		static inline BrServer* GetInstance();

		// Get config
		inline const ServerConfig::GenericServer* GetMyConfig();

		// Get/Set Public network enable status
		inline bool IsNetPublicEnabled();
		inline virtual bool EnableNetPublic( bool bIsEnable );


		// Get net private
		inline Net::ServerPeerTCP* GetNetPrivate()								{ return *m_pNetPrivate; }

		ServerComponentCarrier& GetComponentCarrier() { return m_Components; }


		template<class DBManagerType>
		Result AddDBCluster(ServerConfig::DBCluster *pDBClusterCfg);

		//////////////////////////////////////////////////////////////////////////
		//
		//	virtual network process
		//

		// Process Private network event
		virtual Result ProcessNewConnection();

		// Process Public network event
		virtual Result ProcessPublicNetworkEvent();


		virtual Result TerminateEntity() override;


		//////////////////////////////////////////////////////////////////////////
		//
		//	virtual interface definition
		//


		// Apply configuration
		virtual Result ApplyConfiguration();

		virtual Result InitializeMonitoring();

		// Initialize server basic entities
		virtual Result InitializeEntities();

		virtual Result InitializeComponents();

		// Initialize server resource
		virtual Result InitializeServerResource();

		// Close server and release resource
		virtual Result CloseServerResource();


		// Initialize private Network
		virtual Result InitializeNetPrivate();

		// Close Private Network
		virtual Result CloseNetPrivate();

		// create remote entity by class
		virtual Result CreateServerEntity( NetClass netClass, ServerEntity* &pServerEntity ) = 0;

		// Initialize private Network
		virtual Result InitializeNetPublic();

		// Close Public Network
		virtual Result CloseNetPublic();


		// Run the task
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;



		// Start server thread
		virtual Result StartServer();

		// Stop server thread
		virtual Result StopServer();
	};


	///////////////////////////////////////////////////////////////////////////////
	//
	//	Static interfaces
	//

	// Get Loopback ServerEntity
	inline ServerEntity* GetLoopbackServerEntity();
	

	template< class ComponentType >
	ComponentType* GetServerComponent();

	template< class ComponentType >
	Result AddServerComponent(ComponentType* &newComponent);



	// Initialize and deinitialization
	void InitializeEngineForServer();

	void DeinitializeEngine();



#include "BrServer.inl"

}; // namespace SF
}; // namespace Svr





