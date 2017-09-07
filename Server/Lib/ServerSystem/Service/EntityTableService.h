////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : KyungKun Ko
//
// Description : EngineObject manager
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
	//	EntityTable service
	//

	class EntityTableService
	{
	public:
		virtual EntityID GenEntityID(EntityFaculty faculty) { return EntityID(); }
		virtual Result ReserveEntityID(EntityID uiEntityID) { return ResultCode::SUCCESS; }
		virtual bool FreeEntityID(EntityID uiEntityID) { return ResultCode::SUCCESS; }


		virtual Result find(EntityID key, SharedPointerT<Svr::Entity>& pEntity) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result insert(Svr::Entity *pEntity) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result erase(Svr::Entity *pEntity) { return ResultCode::NOT_IMPLEMENTED; }
		virtual Result erase(EntityID key, SharedPointerT<Svr::Entity>& removed) { return ResultCode::NOT_IMPLEMENTED; }


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity routing
		//

		// Route Message Cmd/Evt
		virtual Result RouteTransaction(EntityID entityID, SharedPointerT<Svr::Transaction> &pTrans) { return ResultCode::FAIL; }

		// Route Transaction result
		virtual Result RouteTransactionResult(Svr::TransactionResult* &pRes) { return ResultCode::FAIL; }
	};

	

}; // namespace SF

