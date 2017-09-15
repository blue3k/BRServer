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
#include "Object/LibraryComponent.h"
#include "Object/SharedObjectManager.h"
#include "Service/ServerService.h"
#include "PerformanceCounter/PerformanceCounter.h"
#include "PerformanceCounter/PerformanceCounterInstance.h"



namespace SF {
namespace Svr {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GameUser Entity manager
	//

	class EntityManager : public LibraryComponent, public TickTaskManager, public EntityManagerService
	{
	public:
		static constexpr FixedString TypeName = "EntityManager";

	private:

		MemoryManager m_MemoryManager;

		SharedObjectManager m_SharedObjectManager;

		uint m_NumTaskGroup = 0;
		PerformanceCounterRaw < uint64_t > m_NumberOfServices;
		PerformanceCounterRaw < uint64_t > m_NumberOfTotalEntities;

	public:
		// Constructor/Destructor
		EntityManager(uint numTaskGroup);
		virtual ~EntityManager();


		IMemoryManager& GetMemoryManager() { return m_MemoryManager; }
		IHeap& GetHeap() { return m_MemoryManager; }


		virtual const FixedString& GetTypeName() override { return TypeName; }

		////////////////////////////////////////////////////////////////////////////
		//
		// TickTask add/remove proving
		//

		virtual Result CreateEntity(ClusterID clusterID, EntityFaculty faculty, Entity* &pEntity) override;

		// add entity to table
		virtual Result AddEntity( EntityFaculty faculty, Svr::Entity* pEntity ) override;
		virtual Result AddEntity( EntityID entityID, Svr::Entity* pEntity ) override;

		virtual void OnEntityAdded(Entity* pEntity) override;

		virtual Result FindEntity(EntityID entityID, SharedPointerT<Entity> &pEntity) override;

		// add entity to table
		virtual Result RemoveEntity( EntityID entityID ) override;
		virtual Result RemoveEntity( Svr::Entity* pEntity ) override;

		virtual void OnEntityRemoved(Entity* pEntity) override;

		//virtual void RegisterCounter() override {}

		virtual void FlushDeletedEntity() override;

		// Clear and remove all registered objects
		virtual void Clear() override;

		virtual void Update() override;

		// Initialize component
		virtual Result InitializeComponent() override;

		// Terminate component
		virtual void DeinitializeComponent() override;


	};



}; // namespace GameServer
}; // namespace SF







