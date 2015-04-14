////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game manager server main
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/TypeDefs.h"
#include "Common/ClassUtil.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/Transaction.h"


namespace BR {

 	namespace Policy
	{
		interface IPolicyEntityServer;
		interface IPolicyGameMasterServer;

	};
}


namespace BR {
namespace Net {
	class Server;
}};


namespace BR {
namespace GameMasterServer {

	class StaticZoneManager;
	class EntityServerEntity;
	class PlayerInfoManager;


	//////////////////////////////////////////////////////////////////////////
	//
	//	Character server class
	//

	class GameMasterServer : public Svr::BrServer
	{
	public:

	private:
		// Player info manager
		BRCLASS_ATTRIBUTE_READONLY(PlayerInfoManager*,PlayerInfoMgr);


	public:
		GameMasterServer();
		~GameMasterServer();

		virtual MemoryAllocator& GetAllocator()			{ return STDAllocator::GetInstance(); }

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
		virtual HRESULT CreateServerEntity( BR::Net::NetClass netClass, Svr::ServerEntity* &pServerEntity );


		// Update process
		virtual HRESULT RunTask();

	};


	// Query login server instance
	inline GameMasterServer* GetMyServer();


	// Get Entity Server
	EntityServerEntity* GetEntityServer();


	// Player info manager
	inline PlayerInfoManager* GetPlayerInfoMgr();


#include "GameMasterServerClass.inl"


} // namespace EntityServer
} // namespace BR


