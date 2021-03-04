////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Game player
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "Net/SFNetServerUDP.h"
#include "Util/SFTimeUtil.h"
#include "Types/SFEngineTypedefs.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameNetPolicy.h"

#include "SvrConst.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"
#include "ServiceEntity/Game/GameInstancePlayer.h"
#include "ServiceEntity/Game/GameInstanceEntity.h"
#include "ServiceEntity/Game/GameInstancePlayerComponent.h"



namespace SF {
	namespace Svr {


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity server class
		//

		constexpr StringCrc32 GameInstancePlayerComponentMovement::ComponentID;

		GameInstancePlayerComponentMovement::GameInstancePlayerComponentMovement(IHeap& heap, GameInstancePlayer* owner)
			: GameInstancePlayerComponent(ComponentID, owner)
			, m_Heap(heap)
		{
		}

		void GameInstancePlayerComponentMovement::TickUpdate()
		{
			GameInstancePlayerComponent::TickUpdate();
		}


	} // Svr
} // namespace SF

