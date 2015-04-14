////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Entity table definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"

#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/Entity.h"
#include "Common/HashTable2.h"
#include "Common/Indexing.h"
#include "Common/UniqueEntityIDGenerator.h"
#include "Common/SharedPointer.h"


namespace BR {
namespace Net {
	class MessageData;
}
}


namespace BR {
namespace Svr {

	class Entity;
	class Transaction;
	class TransactionResult;

	//////////////////////////////////////////////////////////////////////////////
	//
	// Entity manager class
	//

	class EntityTable : public Hash::HashTable2< UINT, SharedPointerT<Entity> >
	{
	private:
		// ID generator
		UniqueEntityIDGenerator		m_UIDGenerator;
		UniqueEntityIDGenerator		m_UIDGeneratorForService;

	public:

		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity ID gen
		//

		EntityID GenEntityID(EntityFaculty faculty);
		HRESULT ReserveEntityID( EntityID uiEntityID );
		bool FreeEntityID( EntityID uiEntityID );

		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity routing
		//

		// Route Message Cmd/Evt
		HRESULT RouteTransaction( EntityID entityID, Transaction* &pTrans );

		// Route Transaction result
		HRESULT RouteTransactionResult( BR::Svr::TransactionResult* &pRes );
	};


#include "EntityTable.inl"


}; // namespace Svr
}; // namespace BR


