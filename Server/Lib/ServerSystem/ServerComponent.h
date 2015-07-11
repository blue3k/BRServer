////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Component class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"
#include "Common/Memory.h"
#include "ServerSystem/Component.h"
#include "ServerSystem/ServerServiceBase.h"


namespace BR {
namespace Svr {

	class Entity;
	class ServerEntity;
	class ServerServiceBase;


	enum ServerComponentID
	{
		ServerComponentID_AccountDB,
		ServerComponentID_LoginSessionDB,
		ServerComponentID_GameDB,
		ServerComponentID_GameTransactionDB,
		ServerComponentID_RankingDB,
		ServerComponentID_ServerEntityManager,		               // Server
		ServerComponentID_EntityManager,
		ServerComponentID_ParallelTransactionManager,

		ServerComponentID_ClusterManagerService,
		ServerComponentID_LoginClusterService,
		ServerComponentID_GameClusterService,
		ServerComponentID_GameInstanceManagerService,
		ServerComponentID_GamePartyManagerService,
		ServerComponentID_PartyManagerService,

		ServerComponentID_MatchingWatcherService_4,
		ServerComponentID_MatchingWatcherService_8,


		ServerComponentID_MatchingQueueWatcherService_4x1,
		ServerComponentID_MatchingQueueWatcherService_4x2,
		ServerComponentID_MatchingQueueWatcherService_4x3,
		ServerComponentID_MatchingQueueWatcherService_4x1S,
		ServerComponentID_MatchingQueueWatcherService_4x1W,


		ServerComponentID_MatchingQueueWatcherService_8x1,
		ServerComponentID_MatchingQueueWatcherService_8x2,
		ServerComponentID_MatchingQueueWatcherService_8x3,
		ServerComponentID_MatchingQueueWatcherService_8x4,
		ServerComponentID_MatchingQueueWatcherService_8x5,
		ServerComponentID_MatchingQueueWatcherService_8x6,
		ServerComponentID_MatchingQueueWatcherService_8x7,
		ServerComponentID_MatchingQueueWatcherService_8x1S,
		ServerComponentID_MatchingQueueWatcherService_8x1W,


		ServerComponentID_Max
	};

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	IServerComponent base class
	//

	class IServerComponent : public Component
	{
	public:
		IServerComponent( UINT id ) : Component(id)
		{
		}

		virtual ~IServerComponent()
		{
		}
	};
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	IServerService Component base class
	//

	class IServerServiceComponent : public IServerComponent
	{
	public:
		IServerServiceComponent( UINT id ) : IServerComponent(id)
		{
		}

		virtual ~IServerServiceComponent()
		{
		}

		// Check server component
		virtual HRESULT CheckService() { return S_OK; }

		
		// Get entity, default implementation is return null
		virtual ServerServiceBase* GetService( UINT64 serverSelector ) = 0;

		template< class ServerServiceType >
		ServerServiceType* GetService( UINT64 serverSelector )
		{
			ServerServiceBase *pService = GetService(serverSelector);
			Assert(pService);
			Assert(pService->GetServerEntity());
			if( pService == nullptr || pService->GetServerEntity() == nullptr )
				return nullptr;

			return DEBUG_DYNAMIC_CAST(ServerServiceType*,pService);
		}

	};




	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	////
	////	ClusterServiceComponentBase class
	////

	//class ClusterServiceComponentBase
	//{
	//public:

	//	typedef std::vector<ServerServiceBase*> ServiceList;

	//private:

	//	// Current Master entity
	//	BRCLASS_ATTRIBUTE_PTR(ServerServiceBase,Master);

	//	// server list array in the cluster
	//	ServiceList m_Servers;

	//protected:
	//	
	//	// Get a service by random
	//	HRESULT GetRandomService( ServerServiceBase* &service );

	//	// Get a service by random except master
	//	HRESULT GetRandomServiceExceptMaster( ServerServiceBase* &service );


	//public:

	//	ClusterServiceComponentBase();
	//	virtual ~ClusterServiceComponentBase();
	//	
	//	// Check server component
	//	virtual HRESULT CheckService();

	//	// Add service entity
	//	HRESULT AddService( ServerServiceBase* pServerService );

	//	// Set master service entity
	//	HRESULT SetMasterService( EntityUID entityUID );

	//};



	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	////
	////	ReplicationClusterServiceComponent base class
	////

	//class ReplicationClusterServiceComponent : public ClusterServiceComponentBase, public IServerServiceComponent
	//{
	//public:

	//	// This will be the selector
	//	enum AccessMode {
	//		AccessMode_Write,
	//		AccessMode_Read,
	//		AccessMode_SynchronizedRead,
	//	};

	//private:

	//public:
	//	ReplicationClusterServiceComponent( UINT id )
	//		: IServerServiceComponent(id)
	//	{
	//	}

	//	virtual ~ReplicationClusterServiceComponent()
	//	{
	//	}

	//	// Get service, pass one of AccessMode
	//	virtual ServerServiceBase* GetService( UINT64 selectorKey );

	//};


	//


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	////
	////	RingClusterServiceComponent base class
	////

	//class RingClusterServiceComponent : public ClusterServiceComponentBase, public IServerServiceComponent
	//{
	//public:

	//	typedef std::vector<ServerServiceBase*> ServiceList;

	//private:
	//	
	//	// Current Master entity
	//	BRCLASS_ATTRIBUTE_PTR(ServerEntity,Master);

	//	// server list array in the cluster
	//	ServiceList m_Servers;

	//public:
	//	RingClusterServiceComponent( UINT id )
	//		: IServerServiceComponent(id)
	//	{
	//	}

	//	virtual ~RingClusterServiceComponent()
	//	{
	//	}

	//	// Get Service, selectorKey will be ignored
	//	virtual ServerServiceBase* GetService( UINT64 selectorKey );
	//};




	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	////
	////	ShardClusterServerComponent base class
	////

	//class ShardClusterServerComponent : public IServerServiceComponent
	//{
	//public:

	//	typedef std::vector<ServerServiceBase*> ServiceList;

	//private:
	//	
	//	// server list array in the cluster
	//	ServiceList m_Servers;

	//public:
	//	ShardClusterServerComponent( UINT id )
	//		: IServerServiceComponent(id)
	//	{
	//	}

	//	virtual ~ShardClusterServerComponent()
	//	{
	//	}

	//	// Get entity, default implementation is return null
	//	virtual ServerServiceBase* GetService( UINT64 selectorKey );
	//};


	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Component career base class
	//


	typedef ComponentCarrier<ServerComponentID_Max> ServerComponentCarrier;


#include "ServerComponent.inl"


}; // namespace Svr
}; // namespace BR




