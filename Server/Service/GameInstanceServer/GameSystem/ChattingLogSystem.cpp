////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game play system
//	
////////////////////////////////////////////////////////////////////////////////





#include "stdafx.h"

#include "Common/TimeUtil.h"
#include "Common/BrRandom.h"
#include "Common/ResultCode/BRResultCodeSvr.h"
#include "Common/ResultCode/BRResultCodeGame.h"

#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServerUtil.h"
#include "ConspiracyGameInstanceSvrConst.h"


#include "GameSystem/ChattingLogSystem.h"
#include "GameInstance/GameInstanceEntity.h"


BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::ChattingLogSystem);


namespace BR {
namespace ConspiracyGameInstanceServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlaySystem class
	//

	// Constructor 
	ChattingLogSystem::ChattingLogSystem( GameInstanceEntity* pEntity )
		:GameSystem(pEntity)
	{
	}

	ChattingLogSystem::~ChattingLogSystem()
	{
	}
	

	// Initialzie system
	Result ChattingLogSystem::InitializeComponent()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( InitializeLog() );

		svrChk( GameSystem::InitializeComponent() );

	Proc_End:

		return hr;
	}

	void ChattingLogSystem::TerminateComponent()
	{
		TerminateLog();

		GameSystem::TerminateComponent();
	}

	
	
}; // ConspiracyGameInstanceServer
}; // namespace BR







