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
#include "Common/PageQueue.h"
#include "Common/LocalUIDGenerator.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/MessageRoute.h"
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

	class RankingServiceEntity : public ReplicaClusterServiceEntity
	{
	public:


	private:



		// Currently processing number of members
		UINT m_CurrentProcessingNumberofMember;

		BRCLASS_ATTRIBUTE(bool,LastRankingFailed);
		Util::TimeStampTimer m_RankingCheckTimer;

	protected:


	public:

		RankingServiceEntity( ClusterID clusterID, ClusterMembership initialMembership = ClusterMembership::StatusWatcher );
		~RankingServiceEntity();

		// We are not going to use hashed key
		virtual UINT KeyHash( UINT64 key ) { return (UINT)key; }
		

		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity operations
		//

		Result InitializeEntity( EntityID newEntityID );

		// clear transaction
		virtual Result ClearEntity();

		// TickUpdate 
		virtual Result TickUpdate(Svr::TimerAction *pAction = nullptr) override;


		//////////////////////////////////////////////////////////////////////////
		//
		//	Ranking operations
		//

		
	};





	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	RankingWatcherServiceEntity class
	//

	class RankingWatcherServiceEntity : public ReplicaClusterServiceEntity, public IServerComponent
	{
	public:

	private:

	public:

		RankingWatcherServiceEntity( ClusterID clusterID, UINT componentID );
		~RankingWatcherServiceEntity();
	};



}; // namespace Svr
}; // namespace BR




