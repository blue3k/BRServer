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
#include "Common/BrSvrTypes.h"
#include "Container/PageQueue.h"
#include "Common/LocalUIDGenerator.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/MessageRoute.h"
#include "Common/HashTable.h"
#include "Common/Indexing.h"

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

	class GameInstanceServiceEntity : public ReplicaClusterServiceEntity
	{
	public:


	private:

		// Currently processing number of members
		UINT m_CurrentProcessingNumberofMember;

		BRCLASS_ATTRIBUTE(bool,LastRankingFailed);
		Util::TimeStampTimer m_RankingCheckTimer;

	protected:


	public:

		GameInstanceServiceEntity( ClusterMembership initialMembership = ClusterMembership::StatusWatcher );
		~GameInstanceServiceEntity();

		// We are not going to use hashed key
		virtual UINT KeyHash( uint64_t key ) { return (UINT)key; }
		

		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity operations
		//

		Result InitializeEntity( EntityID newEntityID );

		// clear transaction
		virtual Result ClearEntity();

		// TickUpdate 
		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////
		//
		//	
		//

		
	};







}; // namespace Svr
}; // namespace BR




