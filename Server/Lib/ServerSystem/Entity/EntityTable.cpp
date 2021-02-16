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



#include "ServerSystemPCH.h"
#include "Util/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Multithread/SFThread.h"
#include "Entity/EntityTable.h"
#include "SvrTrace.h"
#include "Net/SFNetDef.h"
#include "Transaction/Transaction.h"
#include "Service/ServerService.h"


namespace SF {
namespace Svr {


	constexpr StringCrc64 EntityTable::TypeName;

	EntityTable::EntityTable()
		: LibraryComponent("EntityTable")
		, m_EntityMap(GetSystemHeap())
	{

	}




	//////////////////////////////////////////////////////////////////////////
	//
	//	Component override
	//


	// Initialize component
	Result EntityTable::InitializeComponent()
	{
		Result result = LibraryComponent::InitializeComponent();

		Service::EntityTable = this;

		return result;
	}

	// Terminate component
	void EntityTable::DeinitializeComponent()
	{
		Service::EntityTable = nullptr;
		LibraryComponent::DeinitializeComponent();
	}



	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity ID gen
	//

	EntityID EntityTable::GenEntityID(EntityFaculty faculty)
	{
		if( faculty == EntityFaculty::Service )
			return EntityID(faculty, m_UIDGenerator.NewID() );
		else
			return EntityID(faculty, m_UIDGeneratorForService.NewID() );
	}

	Result EntityTable::ReserveEntityID( EntityID uiEntityID )
	{
		if (uiEntityID.GetFacultyID() == (uint)EntityFaculty::Service)
			EntityID((EntityFaculty)uiEntityID.GetFacultyID(), m_UIDGenerator.ReserveID(uiEntityID.GetEntityLID()));
		else
			EntityID((EntityFaculty)uiEntityID.GetFacultyID(), m_UIDGeneratorForService.ReserveID(uiEntityID.GetEntityLID()));

		return ResultCode::SUCCESS;
	}

	bool EntityTable::FreeEntityID( EntityID uiEntityID )
	{
		if (uiEntityID.GetFacultyID() == (uint)EntityFaculty::Service)
			return m_UIDGenerator.FreeID( uiEntityID.GetEntityLID());
		else
			return m_UIDGeneratorForService.FreeID( uiEntityID.GetEntityLID());
	}

	Result EntityTable::find(EntityID key, SharedPointerT<Svr::Entity>& pEntity)
	{
		return m_EntityMap.find(key, pEntity);
	}

	Result EntityTable::insert(Entity *pEntity)
	{
		return m_EntityMap.insert(pEntity->GetEntityID(), pEntity);
	}

	Result EntityTable::erase(Entity *pEntity)
	{
		return m_EntityMap.erase(pEntity->GetEntityID());
	}

	Result EntityTable::erase(EntityID key, SharedPointerT<Svr::Entity>& removed)
	{
		return m_EntityMap.erase(key, removed);
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity routing
	//


	// Route Message Cmd/Evt
	Result EntityTable::RouteTransaction( EntityID entityID, TransactionPtr &pTrans )
	{
		Result hr = ResultCode::SUCCESS;
		SharedPointerT<Entity> pEntity;

		svrCheck(m_EntityMap.find(entityID.ID, pEntity));

		svrCheck(pEntity->PendingTransaction(ThisThread::GetThreadID(), pTrans));

		return hr;
	}

	// Route Transaction result
	Result EntityTable::RouteTransactionResult(SFUniquePtr<TransactionResult>& pRes)
	{
		Result hr = ResultCode::SUCCESS;
		SharedPointerT<Entity> pEntity;

		svrCheckPtr( pRes );

		if (!m_EntityMap.find(pRes->GetTransID().GetEntityID(), pEntity))
		{
			return hr = ResultCode::INVALID_ENTITY;
		}

		svrCheck(pEntity->PendingTransactionResult(pRes));

		return hr;
	}

	Result EntityTable::for_each(std::function<bool(const SharedPointerT<Svr::Entity>&)> func)
	{
		m_EntityMap.for_each([&](const uint& key, const SharedPointerT<Entity>& item)-> bool
		{
			return func(item);
		});

		return ResultCode::SUCCESS;
	}

} // namespace SF {
} // namespace Svr {

