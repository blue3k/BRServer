////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game play system
//	
////////////////////////////////////////////////////////////////////////////////





#include "GameInstancePCH.h"

#include "Util/SFTimeUtil.h"
#include "Util/SFRandom.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "ResultCode/SFResultCodeGame.h"

#include "SvrConst.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"
#include "ConspiracyGameInstanceSvrConst.h"


#include "GameSystem/ChattingLogSystem.h"
#include "GameInstance/GameInstanceEntity.h"




namespace SF {
namespace ConspiracyGameInstanceServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlaySystem class
	//

	constexpr StringCrc32 ChattingLogSystem::ComponentID;

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







