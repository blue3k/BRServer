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

#include "SFTypedefs.h"
#include "Server/BrServer.h"
#include "Entity/Entity.h"
#include "Transaction/Transaction.h"





namespace SF {
namespace Net {
	class ServerMUDP;
}};


namespace SF {
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


	public:
		SharedModuleServer();
		~SharedModuleServer();



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
		virtual Result ApplyConfiguration() override;


		// Initialize server resource
		virtual Result InitializeServerResource() override;

		// Close server and release resource
		virtual Result CloseServerResource() override;


		// Initialize private Network
		virtual Result InitializeEntities() override;

		// Close Private Network
		virtual Result CloseEntities() override;

	};


	// Query Game server instance
	inline SharedModuleServer* GetMyServer();

	// Query Loopback entity
	inline SharedModuleServerEntity* GetLoopbackGameServerEntity();

	// Get Entity Server
	//EntityServerEntity* GetEntityServer();


#include "SharedModuleServerClass.inl"


}; // namespace SharedModuleServer
}; // namespace SF







