////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Entity server entity
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/Transaction.h"
#include "ServerSystem/SvrTrace.h"
#include "EntityServerEntity.h"
#include "GameSvrConst.h"

#include "Protocol/Policy/EntityServerIPolicy.h"



namespace BR {
namespace GameServer {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity server entity
	//


	EntityServerEntity::EntityServerEntity()
		:Svr::EntityServerEntity( Const::ENTITY_ENTITY_TRANS_QUEUE, Const::ENTITY_ENTITY_TRANSRES_QUEUE )
		,m_Allocator(STDAllocator::GetInstance())
	{
	}

	EntityServerEntity::~EntityServerEntity()
	{
	}



	// Initialize entity to proceed new connection
	HRESULT EntityServerEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(Svr::EntityServerEntity::InitializeEntity( newEntityID ) );


	Proc_End:

		return hr;
	}







}; // namespace GameServer
}; // namespace BR



