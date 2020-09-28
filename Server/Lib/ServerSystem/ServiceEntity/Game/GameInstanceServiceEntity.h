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

#include "Memory/SFMemory.h"
#include "Types/BrSvrTypes.h"
#include "Container/SFPageQueue.h"
#include "Util/LocalUIDGenerator.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Transaction/MessageRoute.h"
#include "Container/SFHashTable.h"
#include "Container/SFIndexing.h"

#include "Entity/EntityInformation.h"
#include "ServiceEntity/ClusteredServiceEntity.h"


namespace SF {
namespace Svr {

	class Entity;
	class ServerEntity;



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServerServiceComponentEntity class
	//

	class GameInstanceServiceEntity : public ShardedClusterServiceEntity
	{
	public:

		typedef ShardedClusterServiceEntity super;

	private:

		// Currently processing number of members
		uint m_CurrentProcessingNumberofMember = 0;

		bool m_LastRankingFailed = false;
		Util::TimeStampTimer m_RankingCheckTimer;

		// Component manager
		ComponentManager m_ComponentManger;


	public:

		GameInstanceServiceEntity(GameID gameID, ClusterMembership initialMembership = ClusterMembership::Slave);
		~GameInstanceServiceEntity();

		bool GetLastRankingFailed() { return m_LastRankingFailed; }

		// We are not going to use hashed key
		virtual uint KeyHash( uint64_t key ) { return (uint)key; }
		

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Component manager
		//

		ComponentManager& GetComponentManager() { return m_ComponentManger; }

		template< class ComponentType >
		ComponentType* GetComponent() { return m_ComponentManger.GetComponent<ComponentType>(); }

		// Get component with its type
		template< class ComponentType >
		const ComponentType* GetComponent() const { return m_ComponentManger.GetComponent<ComponentType>(); }


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




