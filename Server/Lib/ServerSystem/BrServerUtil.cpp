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
#include "Types/BrSvrTypes.h"
#include "Task/TaskManager.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/EntityTable.h"
#include "ServerSystem/SvrConfig.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"


namespace SF {
namespace Svr {


	// Entity Table
	EntityTable					g_EntityTable;


	// Get entity table
	EntityTable& GetEntityTable()
	{
		return g_EntityTable;
	}

	Result FindEntity(EntityID entityID, SharedPointerT<Entity>& entity)
	{
		return g_EntityTable.Find(entityID, entity);
	}

	uint GetServerUID()
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

