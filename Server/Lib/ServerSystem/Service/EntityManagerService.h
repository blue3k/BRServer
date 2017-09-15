////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : KyungKun Ko
//
// Description : Entity manager
//
////////////////////////////////////////////////////////////////////////////////


#pragma once 

#include "SFTypedefs.h"
#include "Object/SharedPointer.h"
#include "String/FixedStringDB.h"



namespace SF {

	namespace Svr
	{
		class Transaction;
		class Entity;
		class TransactionResult;
	}



	
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity Manager service
	//

	class EntityManagerService
	{
	public:

		////////////////////////////////////////////////////////////////////////////
		//
		// TickTask add/remove proving
		//

		virtual Result CreateEntity(ClusterID clusterID, EntityFaculty faculty, Svr::Entity* &pEntity) { return ResultCode::NOT_IMPLEMENTED; }

		// add entity to table
		virtual Result AddEntity(EntityFaculty faculty, Svr::Entity* pEntity) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result AddEntity(EntityID entityID, Svr::Entity* pEntity) { return ResultCode::NOT_IMPLEMENTED; }

		virtual void OnEntityAdded(Svr::Entity* pEntity) {}

		virtual Result FindEntity(EntityID entityID, SharedPointerT<Svr::Entity> &pEntity) { return ResultCode::NOT_IMPLEMENTED; }

		// add entity to table
		virtual Result RemoveEntity(EntityID entityID) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result RemoveEntity(Svr::Entity* pEntity) { return ResultCode::NOT_IMPLEMENTED; }

		virtual void OnEntityRemoved(Svr::Entity* pEntity) {  }

		virtual void RegisterCounter() {}

		virtual void FlushDeletedEntity() {}
		virtual void Clear() {}

		virtual void Update() {}

	};

	

}; // namespace SF

