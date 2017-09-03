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

#include "Entity/Entity.h"
#include "Task/ServerTaskManager.h"
#include "Component/ServerComponent.h"
#include "PerformanceCounter/PerformanceCounter.h"
#include "PerformanceCounter/PerformanceCounterInstance.h"



namespace SF {
namespace Svr {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GameUser Entity manager
	//

	class EntityManager : public TickTaskManager, public IServerComponent
	{
	public:

		enum { ComponentID = Svr::ServerComponentID_EntityManager };

	private:

		MemoryManager m_MemoryManager;

		PerformanceCounterRaw < uint64_t > m_NumberOfServices;
		PerformanceCounterRaw < uint64_t > m_NumberOfTotalEntities;

	public:
		// Constructor/Destructor
		EntityManager();
		virtual ~EntityManager();

		IMemoryManager& GetMemoryManager() { return m_MemoryManager; }

		////////////////////////////////////////////////////////////////////////////
		//
		// TickTask add/remove proving
		//

		virtual Result CreateEntity(ClusterID clusterID, EntityFaculty faculty, Entity* &pEntity);

		// add entity to table
		Result AddEntity( EntityFaculty faculty, Svr::Entity* pEntity );
		Result AddEntity( EntityID entityID, Svr::Entity* pEntity );

		virtual void OnEntityAdded(Entity* pEntity);

		Result FindEntity(EntityID entityID, SharedPointerT<Entity> &pEntity);

		// add entity to table
		Result RemoveEntity( EntityID entityID );
		Result RemoveEntity( Svr::Entity* pEntity );

		virtual void OnEntityRemoved(Entity* pEntity);

		virtual void RegisterCounter() {}

		// Initialize TickTaskManager
		virtual Result InitializeManager(uint uiNumGroup = 2) override;

		// Terminate TickTaskManager
		virtual Result TerminateManager() override;


		// Use different initialization but same terminate to component
		virtual void TerminateComponent() override;
	};



}; // namespace GameServer
}; // namespace SF







