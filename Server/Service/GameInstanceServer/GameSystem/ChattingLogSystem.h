////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Log system
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Common/BrGameTypes.h"
#include "Memory/MemoryPool.h"
#include "Common/ClassUtil.h"
#include "ServerSystem/GameSystem.h"
#include "ServerSystem/GameLog/ChattingHistory.h"
#include "Common/ArrayUtil.h"
#include "Common/GameConst.h"
#include "GameSystemComponentIDs.h"

namespace BR {
namespace ConspiracyGameInstanceServer {

	class GameInstanceEntity;
	class GamePlayer;


	//////////////////////////////////////////////////////////////////////////
	//
	//	ChattingLogSystem class
	//


	class ChattingLogSystem : public Svr::GameSystem<GameInstanceEntity,ChattingLogSystem>, public Svr::ChattingHistory
	{
	public:
		enum {
			ComponentID = GameSystemComponentID_ChattingLog,
		};

	private:
		
	public:
		
		// Constructor 
		ChattingLogSystem( GameInstanceEntity* pEntity );
		~ChattingLogSystem();

		// Initialize server component
		virtual Result InitializeComponent() override;

		virtual void TerminateComponent() override;


	};



}; // ConspiracyGameInstanceServer
}; // namespace BR







