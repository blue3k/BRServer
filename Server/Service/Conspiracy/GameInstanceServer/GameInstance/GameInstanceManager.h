////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game entity manager
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"


#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"



namespace SF {
namespace ConspiracyGameInstanceServer {

	class GameInstanceEntity;

	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GameUser Entity manager
	//

	class GameInstanceManagerServiceEntity : public Svr::GameInstanceManagerServiceEntity
	{
	public:

	private:
		typedef Svr::GameInstanceManagerServiceEntity super;

	public:
		// Constructor/Destructor
		GameInstanceManagerServiceEntity(GameID gameID, ServerConfig::ServerModuleGameInstanceManager* config, ClusterID clusterID, const EndpointAddress& endpoint);
		virtual ~GameInstanceManagerServiceEntity();


		//////////////////////////////////////////////////////////////////////////
		//
		//	Game Instance operations
		//


		virtual Result RegisterServiceMessageHandler() override;
	};



}; // namespace ConspiracyGameInstanceServer
}; // namespace SF







