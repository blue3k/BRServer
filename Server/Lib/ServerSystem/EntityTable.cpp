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



#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "ServerSystem/EntityTable.h"
#include "ServerSystem/SvrTrace.h"
#include "Net/NetDef.h"
#include "ServerSystem/Transaction.h"


namespace BR {
namespace Svr {


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
			if (uiEntityID.GetFacultyID() == (UINT)EntityFaculty::Service)
				EntityID((EntityFaculty)uiEntityID.GetFacultyID(), m_UIDGenerator.ReserveID(uiEntityID.GetEntityLID()));
			else
				EntityID((EntityFaculty)uiEntityID.GetFacultyID(), m_UIDGeneratorForService.ReserveID(uiEntityID.GetEntityLID()));

			return ResultCode::SUCCESS;
		}

		bool EntityTable::FreeEntityID( EntityID uiEntityID )
		{
			if (uiEntityID.GetFacultyID() == (UINT)EntityFaculty::Service)
				return m_UIDGenerator.FreeID( uiEntityID.GetEntityLID());
			else
				return m_UIDGeneratorForService.FreeID( uiEntityID.GetEntityLID());
		}


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity routing
		//


		// Route Message Cmd/Evt
		Result EntityTable::RouteTransaction( EntityID entityID, Transaction* &pTrans )
		{
			Result hr = ResultCode::SUCCESS;
			SharedPointerT<Entity> pEntity;

			svrChk(Find(entityID.ID, pEntity));

			svrChk(pEntity->PendingTransaction(ThisThread::GetThreadID(), pTrans));

		Proc_End:

			//ReleaseTransaction(pTrans);
			//Util::SafeDelete(pTrans);

			return hr;
		}

		// Route Transaction result
		Result EntityTable::RouteTransactionResult( TransactionResult* &pRes )
		{
			Result hr = ResultCode::SUCCESS;
			SharedPointerT<Entity> pEntity;

			svrChkPtr( pRes );

			svrChk(Find(pRes->GetTransID().GetEntityID(), pEntity));

			svrChk(pEntity->PendingTransactionResult(pRes));

		Proc_End:

			Util::SafeRelease(pRes);

			return hr;
		}


}; // namespace BR {
}; // namespace Svr {



