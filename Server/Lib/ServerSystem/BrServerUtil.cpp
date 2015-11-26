////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server Utility definition
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerSystem/Transaction.h"
#include "ServerSystem/SvrTypes.h"
#include "ServerSystem/TaskManager.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/EntityTable.h"
#include "ServerSystem/SvrConfig.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"


namespace BR {
namespace Svr {



	// Get entity table
	EntityTable& GetEntityTable()
	{
		Assert(BrServer::GetInstance());
		return BrServer::GetInstance()->GetEntityTable();
	}

	HRESULT FindEntity(EntityID entityID, SharedPointerT<Entity>& entity)
	{
		Assert(BrServer::GetInstance());
		return BrServer::GetInstance()->GetEntityTable().Find(entityID, entity);
	}

	UINT GetServerUID()
	{
		return Svr::BrServer::GetInstance()->GetServerUID();
	}


	// Get server ID
	ServerID GetMyServerID()
	{
		Assert(BrServer::GetInstance());
		return BrServer::GetInstance()->GetServerUID();
	}

}
}

