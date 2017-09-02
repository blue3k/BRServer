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

#include "SvrTrace.h"
#include "Entity/Entity.h"
#include "Container/HashTable2.h"
#include "Container/Indexing.h"
#include "Util/UniqueEntityIDGenerator.h"
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

	class EntityTable : public HashTable2< uint, SharedPointerT<Entity> >
	{
	public:

		typedef HashTable2< uint, SharedPointerT<Entity> > super;

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
		Result RouteTransaction( EntityID entityID, TransactionPtr &pTrans );

		// Route Transaction result
		Result RouteTransactionResult( TransactionResult* &pRes );
	};



}; // namespace Svr
}; // namespace SF


