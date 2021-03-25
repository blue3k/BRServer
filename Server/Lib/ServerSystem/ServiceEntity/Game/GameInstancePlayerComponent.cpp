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
		//	Game instance player component - movement
		//

		constexpr StringCrc32 GameInstancePlayerComponentMovement::ComponentID;

		GameInstancePlayerComponentMovement::GameInstancePlayerComponentMovement(IHeap& heap, GameInstancePlayer* owner)
			: GameInstancePlayerComponent(ComponentID, owner)
			, m_Heap(heap)
		{
		}

		void GameInstancePlayerComponentMovement::ResetMovement(const ActorMovement& initialMovement)
		{
			m_ActorMovement.EnqueueMovement(initialMovement);
			m_ActorMovement.SimulateCurrentMove(GetOwner().GetMovementFrame(), m_LatestSimulatedMovement);
		}

		void GameInstancePlayerComponentMovement::TickUpdate()
		{
			Result hr;
			GameInstancePlayerComponent::TickUpdate();

			hr = m_ActorMovement.SimulateCurrentMove(GetOwner().GetMovementFrame(), m_LatestSimulatedMovement);
			if (!hr)
			{
				svrTrace(Error, "Failed to update movement hr:{0}", hr);
			}
			else
			{
				if (!GetOwner().GetLatestMovement().CanMerge(&m_LatestSimulatedMovement))
				{
					// forcing broad casted frame timeout
					GetOwner().SetBroadCastedMovementFrame(GetOwner().GetMovementFrame() - ActorMovement::MoveFrameTimeout);
					// change frame
					GetOwner().SetLatestMovement(m_LatestSimulatedMovement);
				}
			}
			 
		}


	} // Svr
} // namespace SF

