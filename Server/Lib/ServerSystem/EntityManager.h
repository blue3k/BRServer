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

#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"

#include "ServerSystem/Entity.h"
#include "Common/Task/TaskManager.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounter.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterInstance.h"



namespace BR {
namespace Svr {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GameUser Entity manager
	//

	class EntityManager : public TaskManager, public IServerComponent
	{
	public:

		enum { ComponentID = Svr::ServerComponentID_EntityManager };

	private:

		//SharedPointerT < PerformanceCounterInstance > m_PerformanceCounterInstance;
		PerformanceCounterRaw < UINT64 > m_NumberOfServices;
		PerformanceCounterRaw < UINT64 > m_NumberOfTotalEntities;

	public:
		// Constructor/Destructor
		EntityManager();
		virtual ~EntityManager();


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

		// Initialize TaskManager
		virtual Result InitializeManager(UINT uiNumGroup = 2) override;

		// Terminate TaskManager
		virtual Result TerminateManager() override;


		// Use different initialization but same terminate to component
		virtual void TerminateComponent() override;
	};



}; // namespace GameServer
}; // namespace BR







