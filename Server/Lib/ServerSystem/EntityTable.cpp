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



#include "StdAfx.h"
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

		HRESULT EntityTable::ReserveEntityID( EntityID uiEntityID )
		{
			if (uiEntityID.FacultyID == (UINT)EntityFaculty::Service)
				return EntityID((EntityFaculty)uiEntityID.FacultyID, m_UIDGenerator.ReserveID(uiEntityID.EntityLID));
			else
				return EntityID((EntityFaculty)uiEntityID.FacultyID, m_UIDGeneratorForService.ReserveID(uiEntityID.EntityLID));
		}

		bool EntityTable::FreeEntityID( EntityID uiEntityID )
		{
			if (uiEntityID.FacultyID == (UINT)EntityFaculty::Service)
				return m_UIDGenerator.FreeID( uiEntityID.EntityLID );
			else
				return m_UIDGeneratorForService.FreeID( uiEntityID.EntityLID );
		}


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity routing
		//


		// Route Message Cmd/Evt
		HRESULT EntityTable::RouteTransaction( EntityID entityID, Transaction* &pTrans )
		{
			HRESULT hr = S_OK;
			SharedPointerT<Entity> pEntity;

			svrChk(Find(entityID.ID, pEntity));

			svrChk(pEntity->PendingTransaction(ThisThread::GetThreadID(), pTrans));

		Proc_End:

			//ReleaseTransaction(pTrans);
			//Util::SafeDelete(pTrans);

			return hr;
		}

		// Route Transaction result
		HRESULT EntityTable::RouteTransactionResult( BR::Svr::TransactionResult* &pRes )
		{
			HRESULT hr = S_OK;
			SharedPointerT<Entity> pEntity;

			svrChkPtr( pRes );

			svrChk(Find(pRes->GetTransID().EntityID, pEntity));

			svrChk(pEntity->PendingTransactionResult(pRes));

		Proc_End:

			Util::SafeRelease(pRes);

			return hr;
		}


}; // namespace BR {
}; // namespace Svr {



