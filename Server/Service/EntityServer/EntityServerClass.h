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

#include "SFTypedefs.h"
#include "Server/BrServer.h"
#include "Entity/Entity.h"
#include "Transaction/Transaction.h"



namespace SF {
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
		virtual Result ApplyConfiguration();


		// Initialize server resource
		virtual Result InitializeServerResource();

		// Close server and release resource
		virtual Result CloseServerResource();


		// Initialize private Network
		virtual Result InitializeNetPrivate();

		// Close Private Network
		virtual Result CloseNetPrivate();
		
		// create remote entity by class
		virtual Result CreateServerEntity( BR::NetClass netClass, Svr::ServerEntity* &pServerEntity );


	};


	// Query login server instance
	inline EntityServer* GetMyServer();



#include "EntityServerClass.inl"

} // namespace EntityServer
} // namespace SF







