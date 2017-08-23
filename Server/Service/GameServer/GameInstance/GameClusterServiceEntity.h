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
#include "Common/BrMemory.h"
#include "Common/BrGameTypes.h"
#include "Common/BrSvrTypes.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Common/HashTable.h"
#include "Common/Indexing.h"

#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceEntity.h"


namespace BR {
	namespace Svr {
		namespace Config {
			class PublicNetSocket;
		}
	}
}

namespace BR {
namespace GameServer {

	class Entity;
	class ServerEntity;



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GameClusterServiceEntity class
	//

	class GameClusterServiceEntity : public Svr::GameClusterServiceEntity
	{
	public:


	public:
		// Constructor/Destructor
		GameClusterServiceEntity(GameID gameID, Svr::Config::PublicNetSocket *publicNetSocket, ClusterMembership initialMembership = ClusterMembership::StatusWatcher);
		virtual ~GameClusterServiceEntity();

		


	};




}; // namespace Svr
}; // namespace BR




