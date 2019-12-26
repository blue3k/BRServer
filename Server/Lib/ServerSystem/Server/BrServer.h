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

#include "String/SFStrUtil.h"
#include "Container/SFArray.h"
#include "ResultCode/SFResultCodeDB.h"
#include "Net/SFNetDef.h"
#include "DB/DBClusterManager.h"
#include "Types/BrSvrTypes.h"
#include "Entity/MasterEntity.h"
#include "ServerEntity/ServerEntity.h"
#include "Task/ServerTaskManager.h"
#include "Component/ServerComponentCarrier.h"
#include "ServerConfig/SFServerConfig.h"
#include "SvrTrace.h"
#include "Service/ServerService.h"


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
	class BrServer : public MasterEntity
	{
	public:


		// Entity by ID map 

	private:

		ServerComponentCarrier m_Components;

		// Server state
		ServerState		m_State = ServerState::STOPED;

		// UID of this server
		uint32_t			m_uiUID = 0;

		// Network address
		NetAddress					m_AddrPrivate;

		// ServerPeer host interfaces
		SharedPointerT<Net::ServerPeerTCP>			m_pNetPrivate;

		GameID m_GameID;

		// Server net class
		NetClass					m_NetClass;

		// Server config
		const ServerConfig::GenericServer	*m_pMyConfig = nullptr;

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
		bool m_bIsNetPublicEnabled = false;
		bool m_bIsKillSignaled = false;
		bool m_bStartTransaction = false;


		PageQueue<SharedPointerAtomicT<Net::Connection>> m_NewConnectionQueue;

		Thread* m_MainServerThread = nullptr;

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
		virtual ServerEntity* CreateLoopbackEntity();




		//////////////////////////////////////////////////////////////////////////
		//
		//	Thread override
		//

		void StartThread();
		void StopThread();

		virtual bool OnStart();
		void Run(Thread* pThread);
		bool OnEnd();


		///////////////////////////////////////////////////////////////////////////
		//
		//	Message Processing
		//

		//virtual Result ProcessPrivateMessage( pIMsg

	public:
		BrServer( NetClass svrNetClass );
		virtual ~BrServer();

		GameID GetGameID() { return m_GameID; }
		
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
		Result AddDBCluster(const ServerConfig::DBCluster *pDBClusterCfg);

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

		Result CreateServerInstanceZK(const char* nodeName);

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


		////////////////////////////////////////////////////////////////////
		//
		//
		//

		template< class ServiceEntityType, typename... ConstructorArgs >
		ServiceEntityType* AddServiceEntity(ConstructorArgs... constructorArgs);

		template< class ServiceEntityType, typename... ConstructorArgs >
		Result AddServiceEntityComponent(ConstructorArgs... constructorArgs);


		// Register server module
		Result RegisterModule(ServerConfig::ServerModule* module);
	};




	///////////////////////////////////////////////////////////////////////////////
	//
	//	Template implementation
	//


	template< class ServiceEntityType, typename... ConstructorArgs >
	ServiceEntityType* BrServer::AddServiceEntity(ConstructorArgs... constructorArgs)
	{
		Result hr = ResultCode::SUCCESS;
		ServiceEntityType* pServiceEntity = nullptr;

		svrMem(pServiceEntity = new(GetHeap()) ServiceEntityType(constructorArgs...));

		svrChk(Service::EntityManager->AddEntity(EntityFaculty::Service, pServiceEntity));

		svrChk(Service::ClusterManager->AddClusterServiceEntity(pServiceEntity));


	Proc_End:

		return pServiceEntity;
	}


	template< class ServiceEntityType, typename... ConstructorArgs >
	Result BrServer::AddServiceEntityComponent(ConstructorArgs... constructorArgs)
	{
		Result hr = ResultCode::SUCCESS;

		auto pServiceEntity = AddServiceEntity<ServiceEntityType>(constructorArgs...);
		if (pServiceEntity == nullptr)
			return ResultCode::FAIL;

		svrChk(GetComponentCarrier().AddComponentWithAdapter(pServiceEntity));

	Proc_End:

		return hr;
	}

	///////////////////////////////////////////////////////////////////////////////
	//
	//	Static interfaces
	//

	// Get Loopback ServerEntity
	inline ServerEntity* GetLoopbackServerEntity();
	

	template< class ComponentType >
	ComponentType* GetServerComponent();

	inline GameID GetServerGameID() { if (BrServer::GetInstance() == nullptr) return nullptr; return BrServer::GetInstance()->GetGameID(); }




	// Initialize engine
	void InitializeEngineForServer();

	void InitializeEngineForPlayFabServer(uint32_t workerThreadCount, uint32_t netIOThreadCount);

	void DeinitializeEngine();



#include "BrServer.inl"

}; // namespace SF
}; // namespace Svr





