////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Entity server main
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/Transaction.h"



namespace BR {
namespace EntityServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//

	class EntityServer : public Svr::BrServer
	{
	public:

	private:

	public:
		EntityServer();
		~EntityServer();


		///////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//
		
	protected:

		virtual MemoryAllocator& GetAllocator()			{ return STDAllocator::GetInstance(); }

		virtual Svr::EntityManager* CreateEntityManager() override;
		virtual Svr::ServerEntity* CreateLoopbackEntity() override;


		//////////////////////////////////////////////////////////////////////////
		//
		//	Public Network event handling
		//

		// Apply configuration
		virtual HRESULT ApplyConfiguration();


		// Initialize server resource
		virtual HRESULT InitializeServerResource();

		// Close server and release resource
		virtual HRESULT CloseServerResource();


		// Initialize private Network
		virtual HRESULT InitializeNetPrivate();

		// Close Private Network
		virtual HRESULT CloseNetPrivate();
		
		// create remote entity by class
		virtual HRESULT CreateServerEntity( BR::NetClass netClass, Svr::ServerEntity* &pServerEntity );


	};


	// Query login server instance
	inline EntityServer* GetMyServer();



#include "EntityServerClass.inl"

} // namespace EntityServer
} // namespace BR







