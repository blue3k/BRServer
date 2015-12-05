////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game server class
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/Transaction.h"
#include "Common/ClassUtil.h"




namespace BR {
namespace Net {
	class ServerMUDP;
}};


namespace BR {
namespace SharedModuleServer
{

	class EntityServerEntity;
	class GameServerEntity;
	class SharedModuleServerEntity;



	//////////////////////////////////////////////////////////////////////////
	//
	//	Game server class
	//

	class SharedModuleServer : public Svr::BrServer
	{
	public:

	private:

	protected:

		// Create entity manager
		virtual Svr::ServerEntity* CreateLoopbackEntity() override;

	public:
		SharedModuleServer();
		~SharedModuleServer();

		virtual MemoryAllocator& GetAllocator() override			{ return STDAllocator::GetInstance(); }

		///////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		// Query Loopback entity
		inline SharedModuleServerEntity* GetLoopbackGameServerEntity();
		
		//////////////////////////////////////////////////////////////////////////
		//
		//	virtual network process
		//


		//////////////////////////////////////////////////////////////////////////
		//
		//	Public Network event handling
		//


		// Apply configuration
		virtual HRESULT ApplyConfiguration() override;


		// Initialize server resource
		virtual HRESULT InitializeServerResource() override;

		// Close server and release resource
		virtual HRESULT CloseServerResource() override;


		// Initialize private Network
		virtual HRESULT InitializeNetPrivate() override;

		// Close Private Network
		virtual HRESULT CloseNetPrivate() override;


		// create remote entity by class
		virtual HRESULT CreateServerEntity( NetClass netClass, Svr::ServerEntity* &pServerEntity ) override;



		////////////////////////////////////////////////////////////////////
		//
		//
		//

		template< class ServiceEntityType >
		HRESULT AddServiceEntityComponent( ServiceEntityType* pServiceEntity );
		
		// Register clustered service
		HRESULT RegisterClusteredService( ClusterID clusterID, const char* params );
		HRESULT RegisterClustereWatcherComponents(ClusterID clusterID, Svr::ServerComponentID componentIDStart, Svr::ServerComponentID componentIDEnd);

	};


	// Query Game server instance
	inline SharedModuleServer* GetMyServer();

	// Query Loopback entity
	inline SharedModuleServerEntity* GetLoopbackGameServerEntity();

	// Get Entity Server
	//EntityServerEntity* GetEntityServer();


#include "SharedModuleServerClass.inl"


}; // namespace SharedModuleServer
}; // namespace BR







