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


#include "SFTypedefs.h"
#include "Common/ClassUtil.h"
#include "Memory/SFMemory.h"
#include "Types/BrGameTypes.h"
#include "Types/BrSvrTypes.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Common/HashTable.h"
#include "Container/Indexing.h"

#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceEntity.h"


namespace SF {
	namespace Svr {
		namespace Config {
			class PublicNetSocket;
		}
	}
}

namespace SF {
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
}; // namespace SF




