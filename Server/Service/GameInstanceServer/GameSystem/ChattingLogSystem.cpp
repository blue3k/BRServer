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

#include "Util/TimeUtil.h"
#include "Util/SFRandom.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "ResultCode/SFResultCodeGame.h"

#include "SvrConst.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"
#include "ConspiracyGameInstanceSvrConst.h"


#include "GameSystem/ChattingLogSystem.h"
#include "GameInstance/GameInstanceEntity.h"


SF_MEMORYPOOL_IMPLEMENT(SF::ConspiracyGameInstanceServer::ChattingLogSystem);


namespace SF {
namespace ConspiracyGameInstanceServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlaySystem class
	//

	// Constructor 
	ChattingLogSystem::ChattingLogSystem( GameInstanceEntity* pEntity )
		: GameSystem(pEntity)
		, ChattingHistory(GetHeap())
	{
	}

	ChattingLogSystem::~ChattingLogSystem()
	{
	}
	

	// Initialize system
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
}; // namespace SF







