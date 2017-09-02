////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Instance entity
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "Util/TimeUtil.h"
#include "Types/BrBaseTypes.h"
#include "ResultCode/SFResultCodeCommon.h"
#include "ResultCode/SFResultCodeGame.h"
#include "Net/Message.h"
#include "Common/BrRandom.h"

#include "ServerSystem/BrServer.h"
#include "ServerSystem/EntityManager.h"

//#include "ConspiracyGameInstanceSvrConst.h"
//#include "ConspiracyGameInstanceServerClass.h"

//#include "Protocol/Policy/GameIPolicy.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"

//#include "GameInstanceEntity.h"

#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServiceEntity/Game/GameInstancePlayer.h"
#include "ServerSystem/ServerService/GameInstanceManagerService.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceManagerServiceEntity.h"






namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GameInstanceEntity::GameInstanceEntity()
		: MasterEntity(64,64)
		, m_NumBot(0)
		, m_Allocator(STDAllocator::GetInstance())
	{
		SetTickInterval(DurationMS(Const::GAMEINSTANCE_TICK_TIME));
		m_EmptyInstanceKillTimeOut = DurationMS(Const::GAMEINSTANCE_EMPTYINSTANCE_KILL_TIMEOUT);
	}

	GameInstanceEntity::~GameInstanceEntity()
	{
	}


	// Initialize entity to proceed new connection
	Result GameInstanceEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::InitializeEntity( newEntityID ) );

		//svrChk( InitializeSystem() );

		m_AcceptJoin = true;

	Proc_End:	

		return hr;
	}

	// Close entity and clear transaction
	Result GameInstanceEntity::TerminateEntity()
	{
		Result hr = ResultCode::SUCCESS;

		if( GetEntityState() == EntityState::FREE )
			return ResultCode::SUCCESS;

		CloseGameInstance();

		 super::TerminateEntity();

		m_AcceptJoin = false;

		GetServerComponent<Svr::EntityManager>()->RemoveEntity(this);

	//Proc_End:


		return hr;
	}

	// Run entity
	Result GameInstanceEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;
		auto CurTime = Util::Time.GetTimeMs();
		StaticArray<PlayerID,64> LeaverList;
		GameInstancePlayer *pGamePlayer = nullptr;
		PlayerID pltID;
		INT playerCount = 0;

		hr = super::TickUpdate(pAction);
		if (!(hr) || hr == Result(ResultCode::SUCCESS_FALSE))
			return hr;

		// Update Players
		m_GamePlayerByUID.ForeachOrder(0, GameConst::MAX_GAMEPLAYER, [&CurTime, &playerCount](const PlayerID& playerID, GameInstancePlayer* pPlayer)-> bool
		{
			if (pPlayer->GetServerEntity() != nullptr)
				playerCount++;

			pPlayer->UpdateGamePlayer(CurTime);
			return true;
		});

		UpdateGameStatus( CurTime );

		// Leave player
		while( (m_PendingReleasePlayer.Dequeue( pltID )) )
		{
			if ((m_GamePlayerByUID.Find(pltID, pGamePlayer)))
			{
				playerCount--;
				LeavePlayer( pGamePlayer );

				if (playerCount <= 0)
				{
					CloseGameInstance();
				}

			}
		}

		if (GetEntityState() == EntityState::FREE)
			return ResultCode::SUCCESS_FALSE;

	//Proc_End:

		return hr;
	}


	////////////////////////////////////////////////////////////
	//
	//	Update processes
	//



	// Update Game status
	Result GameInstanceEntity::UpdateGameStatus( TimeStampMS ulCurTime )
	{
		Result hr = ResultCode::SUCCESS;

		// Call check timer to update
		m_TimeToKill.CheckTimer();

	//Proc_End:

		return hr;
	}



	////////////////////////////////////////////////////////////
	//
	//	Game timers
	//


	// set game instance kill timer
	Result GameInstanceEntity::SetGameKillTimer(DurationMS ulWaitTime )
	{
		// Server Notice ?

		return m_TimeToKill.SetTimer( ulWaitTime );
	}


	// On Game Kill timer
	void GameInstanceEntity::OnGameKillTimer()
	{
		CloseGameInstance();
	}

	// Close Game Instance
	void GameInstanceEntity::CloseGameInstance()
	{
		svrTrace(TRC_INFO, "CloseGameInstance:{0}", GetEntityUID());

		LeaveAllPlayerForGameDelete();

		m_AcceptJoin = false;

		if (GetEntityState() == EntityState::WORKING)
			SetEntityState(EntityState::CLOSING);


		// 2. Get service entity list in the cluster
		GetServerComponent<GameInstanceManagerServiceEntity>()->FreeGameInstance(GetEntityUID());

	}



	////////////////////////////////////////////////////////////
	//
	//	Game entity
	//

	// Initialize entity to proceed new connection
	Result GameInstanceEntity::InitializeGameEntity(UINT numBot, UINT maxPlayer)
	{
		Result hr = ResultCode::SUCCESS;
		GameInstancePlayer *pPlayer = nullptr;

		if (maxPlayer > GameConst::MAX_GAMEPLAYER)
		{
			svrErr(ResultCode::E_GAME_INVALID_PLAYER_COUNT);
		}

		if (numBot > maxPlayer)
		{
			// Too many boot number
			svrTrace(Trace::TRC_ERROR, "Too many bot number numBot:{0} -> {1}, maxPlayer:{2}", numBot, maxPlayer-1, maxPlayer);
			numBot = maxPlayer - 1;
		}

		m_TotalJoinedPlayer = 0;
		m_NumBot = numBot;
		m_MaxPlayer = maxPlayer;

		// set kill timer
		Assert(m_EmptyInstanceKillTimeOut.count() != 0);
		SetGameKillTimer(m_EmptyInstanceKillTimeOut);
		m_TimeToKill.SetTimerFunc([&]() { OnGameKillTimer(); });


		// add fake bot player
		for (UINT iBot = 0; iBot < m_NumBot; iBot++)
		{
			PlayerInformation playerInfo;
			playerInfo.PlayerID = iBot + 1;
			playerInfo.Level = 1;
			StrUtil::Format(playerInfo.NickName, "Bot{0}", iBot);
			svrChk(CreatePlayerInstance(playerInfo, pPlayer));
			pPlayer->SetIsBot(true);
			svrChk(AddPlayerToJoin(pPlayer));
		}


	Proc_End:

		Util::SafeDelete(pPlayer);

		return hr;
	}




	//////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game Player
	//


	Result GameInstanceEntity::CreatePlayerInstance(const PlayerInformation& playerInfo, GameInstancePlayer* &pPlayer)
	{
		pPlayer = new GameInstancePlayer(this, playerInfo);

		return pPlayer != nullptr ? ResultCode::SUCCESS : ResultCode::OUT_OF_MEMORY;
	}

	// Register new player to join
	Result GameInstanceEntity::AddPlayerToJoin(GameInstancePlayer* &pPlayer)
	{
		Result hr = ResultCode::SUCCESS;
		GameInstancePlayer* pFound = nullptr;
//		UINT playerIndex;

		svrChkPtr( pPlayer );
		if ((m_GamePlayerByUID.Find(pPlayer->GetPlayerID(), pFound)))
		{
			svrErr(ResultCode::E_GAME_ALREADY_IN_GAME);
		}

		m_TotalJoinedPlayer++;

		svrChk(m_GamePlayerByUID.Insert(pPlayer->GetPlayerID(), pPlayer));

		// clear game killing timer
		m_TimeToKill.ClearTimer();

		pPlayer = nullptr;

	Proc_End:


		return hr;
	}


	// Player leave
	Result GameInstanceEntity::LeavePlayer( GameInstancePlayer* &pPlayer )
	{
		Result hr = ResultCode::SUCCESS;

		pPlayer->SetServerEntity(nullptr,0);

		// We will leave him as an inactive player so the clean-up and any notify aren't needed


	//Proc_End:

		svrTrace(TRC_INFO, "LeavePlayer, remain:{0}", m_GamePlayerByUID.GetItemCount());

		if (m_GamePlayerByUID.GetItemCount() == 0) // if no player remain
		{
			SetGameKillTimer(m_EmptyInstanceKillTimeOut);
		}


		return hr;
	}

	Result GameInstanceEntity::LeavePlayer( PlayerID pltID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( m_PendingReleasePlayer.Enqueue( pltID ) );

	Proc_End:

		return hr;
	}

	// Leave all player
	Result GameInstanceEntity::LeaveAllPlayerForGameDelete()
	{
		m_GamePlayerByUID.ForeachOrder(0, GameConst::MAX_GAMEPLAYER, [&](const PlayerID& playerID, GameInstancePlayer* pPlayer)-> bool
		{
			auto pPolicy = pPlayer->GetPolicy<Policy::ISvrPolicyGameInstance>();
			if (pPolicy != nullptr && pPlayer->GetPlayerEntityUID() != 0)
				pPolicy->PlayerKickedS2CEvt(RouteContext(GetEntityUID(), pPlayer->GetPlayerEntityUID()), pPlayer->GetPlayerID());

			LeavePlayer( pPlayer );

			Util::SafeDelete(pPlayer);

			return true;
		});

		m_GamePlayerByUID.ClearMap();

		return ResultCode::SUCCESS;
	}

	// Find Player pilotid
	Result GameInstanceEntity::FindPlayer( PlayerID pltID, GameInstancePlayer* &pGamePlayer )
	{
		Result hr = ResultCode::SUCCESS;

		if (!(m_GamePlayerByUID.Find(pltID, pGamePlayer)))
		{
			return ResultCode::E_SVR_PLAYER_NOT_FOUND;
		}

	//Proc_End:

		return hr;
	}


	// Called when a player get out of game
	Result GameInstanceEntity::OnPlayerGetOutOfGame( GameInstancePlayer* pPlayer )
	{
		Result hr = ResultCode::SUCCESS;


	//Proc_End:

		return hr;
	}

}; // Svr
}; // namespace SF


