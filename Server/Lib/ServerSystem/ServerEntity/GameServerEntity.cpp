////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/Transaction.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/ServerEntity/GameServerEntity.h"

#include "Protocol/Policy/GameServerIPolicy.h"



namespace BR {
namespace Svr {
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game server entity
	//


	GameServerEntity::GameServerEntity( UINT uiTransQueueSize, UINT TransResQueueSize )
		:ServerEntity( uiTransQueueSize, TransResQueueSize )
		,m_Allocator(STDAllocator::GetInstance())
	{
	}

	GameServerEntity::~GameServerEntity()
	{
	}


	// Initialize entity to proceed new connection
	HRESULT GameServerEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;

		svrChk( __super::InitializeEntity( newEntityID ) );

	Proc_End:

		return hr;
	}



}; // namespace Svr
}; // namespace BR



