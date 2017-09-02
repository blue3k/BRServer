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


#include "SFTypedefs.h"

#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/Entity.h"
#include "Common/HashTable2.h"
#include "Common/Indexing.h"
#include "Common/UniqueEntityIDGenerator.h"
#include "Object/SharedPointer.h"


namespace SF {
namespace Net {
	class MessageData;
}
}


namespace SF {
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
	public:

		typedef Hash::HashTable2< UINT, SharedPointerT<Entity> > super;

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
		Result ReserveEntityID( EntityID uiEntityID );
		bool FreeEntityID( EntityID uiEntityID );


		bool Insert(Entity *pEntity);

		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity routing
		//

		// Route Message Cmd/Evt
		Result RouteTransaction( EntityID entityID, Transaction* &pTrans );

		// Route Transaction result
		Result RouteTransactionResult( TransactionResult* &pRes );
	};


#include "EntityTable.inl"


}; // namespace Svr
}; // namespace SF


