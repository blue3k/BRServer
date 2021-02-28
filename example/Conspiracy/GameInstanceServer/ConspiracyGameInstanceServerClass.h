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



//namespace SF {
//namespace Policy {
//		interface IPolicyEntityServer;
//		interface IPolicyGameServer;
//		interface IPolicyGameInstance;
//}};
//

namespace SF {
namespace Net {
	class ServerMUDP;
}};


namespace SF {
namespace ConspiracyGameInstanceServer
{

	class EntityServerEntity;
	class GameServerEntity;
	class ConspiracyGameInstanceServerEntity;



	//////////////////////////////////////////////////////////////////////////
	//
	//	Game server class
	//

	class GameInstanceServer : public Svr::BrServer
	{
	public:
		GameInstanceServer();
		~GameInstanceServer();


		///////////////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		// Query Loopback entity
		inline ConspiracyGameInstanceServerEntity* GetLoopbackGameServerEntity();
		

		Svr::Entity* CreateEntity(ClusterID clusterID, EntityFaculty faculty);

		//////////////////////////////////////////////////////////////////////////
		//
		//	overrided
		//

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
	inline GameInstanceServer* GetMyServer();

	// Query Loopback entity
	inline ConspiracyGameInstanceServerEntity* GetLoopbackGameServerEntity();

	// Get Entity Server
	//EntityServerEntity* GetEntityServer();


#include "ConspiracyGameInstanceServerClass.inl"


}; // namespace ConspiracyGameInstanceServer
}; // namespace SF







