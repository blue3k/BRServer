////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game master 
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
//#include "ServerSystem/PlugIn.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/ServerEntity/GameMasterServerEntity.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"



namespace BR {
namespace Svr {
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game master server entity
	//


	GameMasterServerEntity::GameMasterServerEntity( UINT uiTransQueueSize, UINT TransResQueueSize )
		:ServerEntity( uiTransQueueSize, TransResQueueSize )
		,m_Allocator(STDAllocator::GetInstance())
	{
	}

	GameMasterServerEntity::~GameMasterServerEntity()
	{
	}



	// Get Server IPolicy
	Policy::IPolicyGameMasterServer* GameMasterServerEntity::GetPolicy()
	{
		if( GetConnection() ) return GetConnection()->GetPolicy<Policy::IPolicyGameMasterServer>();
		return nullptr;
	}

	// Initialize entity to proceed new connection
	HRESULT GameMasterServerEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;

		svrChk( __super::InitializeEntity( newEntityID ) );

	Proc_End:

		return hr;
	}




}; // namespace Svr
}; // namespace BR



