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
#include "Component/SFLibraryComponent.h"
#include "Object/SFSharedObjectManager.h"
#include "Service/ServerService.h"
#include "PerformanceCounter/PerformanceCounter.h"
#include "PerformanceCounter/PerformanceCounterInstance.h"



namespace SF {
namespace Svr {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GameUser Entity manager
	//

	class EntityManager : public TickTaskManager, public EntityManagerService
	{
	public:
		static constexpr StringCrc64 TypeName = "EntityManager";


	private:

		Heap m_Heap;

		SharedObjectManager m_SharedObjectManager;

		uint m_NumTaskGroup = 0;
		PerformanceCounterRaw < uint64_t > m_NumberOfServices;
		PerformanceCounterRaw < uint64_t > m_NumberOfTotalEntities;


		DynamicArray<EntityCreator> m_EntityCreators;

	public:
		// Constructor/Destructor
		EntityManager(uint numTaskGroup);
		virtual ~EntityManager();


		IHeap& GetHeap() { return *m_Heap.get(); }

		virtual void RegisterEntityCreator(const EntityCreator& creator) override;

		////////////////////////////////////////////////////////////////////////////
		//
		// TickTask add/remove proving
		//

		virtual Result RunOnTaskThread(uint32_t groupID, std::function<void()>&& pTaskFunction) override;


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
		virtual Result InitializeComponent();

		// Terminate component
		virtual void DeinitializeComponent();


	};



}; // namespace GameServer
}; // namespace SF







