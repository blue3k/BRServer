////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) Blue3k
// 
// Author : KyungKun Ko
//
// Description : Server interface definition
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "String/SFStrUtil.h"
#include "Container/SFArray.h"
#include "Container/SFHashTable.h"
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
#include "ServerConfig/SFServerConfig.h"



namespace SF {
	namespace DB {
		class DBClusterManager;
	}
};


namespace SF {

	class ServiceEntity;


	// Server running state
	enum class ServerState {
		STOPED,
		STARTING,
		RUNNING,
		STOPING,
	};

	namespace Svr {

	class ServerEntityManager;
	class ServerEntity;
	class SimpleUserEntity;
	class Transaction;
	class EntityManager;


	//////////////////////////////////////////////////////////////////////////
	//
	//	Server definition
	//
	class BrServer : public MasterEntity
	{
	public:

		typedef Result (*ModuleFactoryFuncType)(BrServer* ThisServer, GameID gameID, ServerConfig::ServerModule* config);

	private:

		ServerComponentCarrier m_Components;

		// local service entities
		DynamicArray<ServiceEntity*> m_LocalServiceEntities;

		// Server state
		ServerState		m_State = ServerState::STOPED;

		// Server execution time stamp
		UTCTimeStampSec					m_ServerUpUTCTIme;

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


		Thread* m_MainServerThread = nullptr;

		// Module constructor map
		HashTable<StringCrc32, ModuleFactoryFuncType> m_ModuleFactories;

		// singleton instance
		static BrServer *stm_pServerInstance;


	protected:

		// Set Loopback entity
		inline void SetLoopbackServerEntity( ServerEntity* pLoopback );

		// Set main server instance
		static void SetInstance( BrServer *pServerInstance );

		// Create entity manager
		virtual ServerEntity* CreateLoopbackEntity();




		//////////////////////////////////////////////////////////////////////////
		//
		//	Thread override
		//

		void StartThread();
		void StopThread();

		virtual Result OnStart();
		void Run(Thread* pThread);
		bool OnEnd();


		///////////////////////////////////////////////////////////////////////////
		//
		//	Message Processing
		//

		//virtual Result ProcessPrivateMessage( pIMsg

	public:
		BrServer();
		virtual ~BrServer();

		// Set server state
		inline void SetServerState( ServerState state );

		inline SyncCounter& GetNumberServicesToWait()			{ return m_NumberServicesToWait; }


		// Get Server state
		inline ServerState GetServerState();

		// Get Loopback entity
		inline ServerEntity* GetLoopbackServerEntity();

		EntityUID GetServerEntityUID() { return EntityUID(Service::ServerConfig->UID, EntityID(EntityFaculty::Server, 0)); }
		
		// Get Server start up time, UTC
		inline UTCTimeStampSec GetServerUpTime();

		// Get main server instance
		static inline BrServer* GetInstance();

		ServerComponentCarrier& GetComponentCarrier() { return m_Components; }


		const Array<ServiceEntity*>& GetLocalServiceEntities() const { return m_LocalServiceEntities; }

		template<class DBManagerType>
		Result AddDBCluster(const ServerConfig::DBCluster *pDBClusterCfg);

		// Emplace module constructor
		Result EmplaceModuleFactory(StringCrc32 moduleName, ModuleFactoryFuncType constructor);


		//////////////////////////////////////////////////////////////////////////
		//
		//	virtual network process
		//

		virtual Result TerminateEntity() override;


		void AddModuleFactory(StringCrc32 NameCrc, ModuleFactoryFuncType&& Constructor);

		// InitializeModuleFactory
		virtual Result InitializeModuleFactory();


		//////////////////////////////////////////////////////////////////////////
		//
		//	virtual interface definition
		//


		virtual Result CreateServerInstanceZK(const char* nodeName);

		virtual Result SetupConfiguration();

		virtual Result InitializeMonitoring();

		// Initialize server basic entities
		virtual Result InitializeEntities();

		virtual Result CloseEntities();

		virtual Result InitializeComponents();

		// Initialize server resource
		virtual Result InitializeServerResource();

		// Close server and release resource
		virtual Result CloseServerResource();

		// create remote entity by class
		virtual Result CreateServerEntity(ServerEntity*& pServerEntity);

		virtual Result InitializingServices();

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

		Result AddServiceEntity(ServiceEntity* pServiceEntity);

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

		pServiceEntity = new(GetHeap()) ServiceEntityType(constructorArgs...);
		if(pServiceEntity == nullptr)
			return nullptr;

		if (!AddServiceEntity(pServiceEntity))
		{
			delete pServiceEntity;
			return nullptr;
		}

		return pServiceEntity;
	}



	///////////////////////////////////////////////////////////////////////////////
	//
	//	Static interfaces
	//

	// Get Loopback ServerEntity
	inline ServerEntity* GetLoopbackServerEntity();
	

	template< class ComponentType >
	ComponentType* GetServerComponent();


#include "BrServer.inl"

} // namespace Svr



	// Initialize engine
	void InitializeEngineForServer();

	void InitializeEngineForPlayFabServer(uint32_t workerThreadCount, uint32_t netIOThreadCount);

	void DeinitializeEngine();

} // namespace SF





