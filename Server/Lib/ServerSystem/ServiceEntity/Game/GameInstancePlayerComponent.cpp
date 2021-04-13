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

#include "Protocol/GameServerNetPolicy.h"
#include "Protocol/GameNetPolicy.h"

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
			GetOwner().SetMovementFrame(initialMovement.MoveFrame);
			m_ReceivedActorMovement = initialMovement;
			m_ReceivedActorMovement.MoveFrame = GetOwner().GetMovementFrame();
			m_LatestReceivedFrame = m_ReceivedActorMovement.MoveFrame;
			m_LatestSimulatedMovement = m_ReceivedActorMovement;

			GetOwner().SetLatestMovement(m_LatestSimulatedMovement);

			svrTrace(Debug2, "ResetMovement, PlayerId:{0}, init:{1}, simulated:{2}", GetOwner().GetPlayerID(), initialMovement, m_LatestSimulatedMovement);
		}


		Result GameInstancePlayerComponentMovement::NewMovement(const ActorMovement& newMovement)
		{
			int32_t recvDeltaFrames(newMovement.MoveFrame - m_LatestReceivedFrame);
			if (recvDeltaFrames < 0) // It is too old drop it
			{
				return ResultCode::INVALID_FRAME;
			}

			m_ReceivedActorMovement = newMovement;

			int32_t deltaFrames((m_ReceivedActorMovement.MoveFrame - GetOwner().GetMovementFrame()));
			if (deltaFrames < 0) // old packet arrived late
			{
				// Server was sending made up packets, so we can't go back
				// replace move frame, and pretend it is current frame
				m_ReceivedActorMovement.MoveFrame = GetOwner().GetMovementFrame();
			}
			
			return ResultCode::SUCCESS;
		}

		void GameInstancePlayerComponentMovement::TickUpdate()
		{
			Result hr;
			GameInstancePlayerComponent::TickUpdate();

			if (m_LatestSimulatedMovement.MoveFrame != GetOwner().GetMovementFrame()) // Skip if time tick hasn't updated
			{
				hr = m_ReceivedActorMovement.SimulateCurrentMove(GetOwner().GetMovementFrame(), m_LatestSimulatedMovement);
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
		}


	} // Svr
} // namespace SF

