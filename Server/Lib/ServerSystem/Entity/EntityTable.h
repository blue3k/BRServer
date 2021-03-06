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
#include "Container/SFHashTable2.h"
#include "Container/SFIndexing.h"
#include "Util/UniqueEntityIDGenerator.h"
#include "Object/SFSharedPointer.h"
#include "Service/EntityTableService.h"


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

	class EntityTable : public LibraryComponent, public EntityTableService
	{
	public:

		typedef HashTable2< uint, SharedPointerT<Entity> > EntityMap;

		static constexpr StringCrc64 TypeName = "EntityTable";

	private:

		EntityMap					m_EntityMap;

		// ID generator
		UniqueEntityIDGenerator		m_UIDGenerator;
		UniqueEntityIDGenerator		m_UIDGeneratorForService;

	public:

		EntityTable();

		virtual const StringCrc64& GetTypeName() override { return TypeName; }


		//////////////////////////////////////////////////////////////////////////
		//
		//	Component override
		//


		// Initialize component
		virtual Result InitializeComponent() override;

		// Terminate component
		virtual void DeinitializeComponent() override;


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity ID gen
		//

		virtual EntityID GenEntityID(EntityFaculty faculty) override;
		virtual Result ReserveEntityID( EntityID uiEntityID ) override;
		virtual bool FreeEntityID( EntityID uiEntityID ) override;

		virtual Result find(EntityID key, SharedPointerT<Svr::Entity>& pEntity) override;
		virtual Result insert(Entity *pEntity) override;
		virtual Result erase(Entity *pEntity) override;
		virtual Result erase(EntityID key, SharedPointerT<Svr::Entity>& removed) override;

		// Only for special purpose. bad performance
		virtual Result for_each(std::function<bool(const SharedPointerT<Svr::Entity>&)> func) override;

		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity routing
		//

		// Route Message Cmd/Evt
		virtual Result RouteTransaction( EntityID entityID, TransactionPtr &pTrans ) override;

		// Route Transaction result
		virtual Result RouteTransactionResult(SFUniquePtr<TransactionResult>& pRes) override;
	};

} // namespace Svr
} // namespace SF
