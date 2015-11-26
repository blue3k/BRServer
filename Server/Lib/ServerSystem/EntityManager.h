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
#include "ServerSystem/TaskManager.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounter.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterInstance.h"



namespace BR {
namespace Svr {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GameUser Entity manager
	//

	class EntityManager : public Svr::TaskManager, public Svr::IServerComponent
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


		//PerformanceCounterInstance* GetPerformanceCounterInstance() { return (PerformanceCounterInstance*)m_PerformanceCounterInstance; }

		////////////////////////////////////////////////////////////////////////////
		//
		// TickTask add/remove proving
		//

		// add entity to table
		HRESULT AddEntity( EntityFaculty faculty, Svr::Entity* pEntity );
		HRESULT AddEntity( EntityID entityID, Svr::Entity* pEntity );

		virtual void OnEntityAdded(Entity* pEntity);

		HRESULT FindEntity(EntityID entityID, SharedPointerT<Entity> &pEntity);

		// add entity to table
		HRESULT RemoveEntity( EntityID entityID );
		HRESULT RemoveEntity( Svr::Entity* pEntity );

		virtual void OnEntityRemoved(Entity* pEntity);

		virtual void RegisterCounter() {}

		// Initialize TaskManager
		virtual HRESULT InitializeManager(UINT uiNumGroup = 2) override;

		// Terminate TaskManager
		virtual HRESULT TerminateManager() override;


		// Use different initialization but same terminate to component
		virtual void TerminateComponent();
	};



}; // namespace GameServer
}; // namespace BR







