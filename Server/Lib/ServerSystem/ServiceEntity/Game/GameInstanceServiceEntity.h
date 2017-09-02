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
#include "Types/BrSvrTypes.h"
#include "Container/PageQueue.h"
#include "Common/LocalUIDGenerator.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Transaction/MessageRoute.h"
#include "Common/HashTable.h"
#include "Container/Indexing.h"

#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServiceEntity/ClusteredServiceEntity.h"


namespace SF {
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
		uint m_CurrentProcessingNumberofMember;

		BRCLASS_ATTRIBUTE(bool,LastRankingFailed);
		Util::TimeStampTimer m_RankingCheckTimer;

	protected:


	public:

		GameInstanceServiceEntity( ClusterMembership initialMembership = ClusterMembership::StatusWatcher );
		~GameInstanceServiceEntity();

		// We are not going to use hashed key
		virtual uint KeyHash( uint64_t key ) { return (uint)key; }
		

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
}; // namespace SF




