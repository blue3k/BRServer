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
#include "Common/BrSvrTypes.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "Common/HashTable.h"
#include "Common/Indexing.h"
#include "Common/StaticHashTable.h"

#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServiceEntity/ClusteredServiceEntity.h"


namespace BR {
namespace Svr {

	class Entity;
	class ServerEntity;



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServerServiceComponentEntity class
	//

	class LoginClusterServiceEntity : public ReplicaClusterServiceEntity, public IServerComponent
	{
	public:

		enum { ComponentID = ServerComponentID_LoginClusterService };


		MemoryAllocator& GetAllocator() { return STDAllocator::GetInstance(); }

	public:
		// Constructor/Destructor
		LoginClusterServiceEntity( ClusterMembership initialMembership = ClusterMembership::StatusWatcher);
		virtual ~LoginClusterServiceEntity();

		//virtual HRESULT InitializeEntity( EntityID newEntityID ) override;

		//virtual HRESULT RegisterServiceMessageHandler( ServerEntity *pServerEntity );
	};




}; // namespace Svr
}; // namespace BR




