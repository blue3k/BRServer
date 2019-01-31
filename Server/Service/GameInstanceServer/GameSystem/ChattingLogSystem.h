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
#include "Types/BrGameTypes.h"
#include "Memory/SFMemoryPool.h"

#include "ServiceEntity/Game/GameSystem.h"
#include "ServerSystem/GameLog/ChattingHistory.h"
#include "Container/SFArray.h"
#include "GameConst.h"
#include "GameSystemComponentIDs.h"

namespace SF {
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
}; // namespace SF







