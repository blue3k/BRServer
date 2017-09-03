////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : KyungKun Ko
//
// Description : Game player Entity
//	
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "SvrConst.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"
#include "Task/ServerTaskEvent.h"
#include "Transaction/ExternalTransactionManager.h"
#include "Net/NetServerUDP.h"
#include "Util/TimeUtil.h"
#include "Types/SFEngineTypedefs.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "Protocol/Message/GameMsgClass.h"

#include "ServiceEntity/Game/GamePlayerEntity.h"

#include "DB/GameConspiracyDB.h"
#include "DB/GameTransactionDB.h"


SF_MEMORYPOOL_IMPLEMENT(SF::Svr::GamePlayerEntity);




namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GamePlayerEntity::GamePlayerEntity()
		: m_ISvrGamePolicy(nullptr)
		, m_PlayerState(PlayerState_None)
		, m_GameInsUID(0)
		, m_LatestUpdateTime(TimeStampSec(DurationSec(0)))
		, m_LatestActiveTime(TimeStampSec(DurationSec(0)))
		, m_LatestDBSyncTime(TimeStampSec(DurationSec(0)))
	{
	}

	GamePlayerEntity::~GamePlayerEntity()
	{
	}

	// Initialize entity to proceed new connection
	Result GamePlayerEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::SimpleUserEntity::InitializeEntity( newEntityID ) );

		m_GameInsUID = 0;
		m_ShardID = 0;

		memset( &m_PlayerInformation, 0, sizeof(m_PlayerInformation) );

		
		SetLatestActiveTime(Util::Time.GetTimeUTCSec());
		m_LatestUpdateTime = TimeStampSec(DurationSec(0));

		SetPlayerAutoLogout(DurationMS(15*60*1000));

		//m_TimeToKill.SetTimer(DurationMS(PlayerAutoLogout * 1000));

		//svrChk( AddComponent<UserFriendSystem>(this) );
		//svrChk( AddComponent<UserGamePlayerInfoSystem>(this) );
		//svrChk( AddComponent<UserNotifySystem>(this) );

		//svrChk( InitializeComponents() );

	Proc_End:


		return hr;
	}


	// Set connection for pilot
	Result GamePlayerEntity::SetConnection(SharedPointerT<Net::Connection>&& pCon)
	{
		Result hr = ResultCode::SUCCESS;

		if (pCon == GetConnection())
			return hr;

		if( GetConnection() != nullptr )
		{
			ReleaseConnection();
		}

		svrChk(Svr::SimpleUserEntity::SetConnection(std::forward<SharedPointerT<Net::Connection>>(pCon)));

		if (GetConnection() != nullptr)
		{
			svrChkPtr(m_ISvrGamePolicy = GetConnection()->GetInterface<Policy::NetSvrPolicyGame>());
		}

	Proc_End:

		return hr;
	}

	// Release connection if has
	void GamePlayerEntity::ReleaseConnection()
	{
		m_ISvrGamePolicy = nullptr;

		Svr::SimpleUserEntity::ReleaseConnection();
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity process
	//

	void GamePlayerEntity::SetLatestActiveTime(TimeStampSec latestActiveTime)
	{
		m_TimeToKill.SetTimer(GetPlayerAutoLogout());

		m_LatestActiveTime = latestActiveTime;
	}

	Result GamePlayerEntity::OnNewUserTranscation()
	{
		// m_LatestUpdateTime is used as a valid character data signal
		if (m_LatestUpdateTime == TimeStampSec::min())
			return ResultCode::SUCCESS_FALSE;

		SetLatestActiveTime(Util::Time.GetTimeUTCSec());

		if (m_LatestDBSyncTime == TimeStampSec::min() || Util::TimeSinceUTC(m_LatestDBSyncTime) > DurationSec(GameConst::PLAYER_UPDATE_STATUS_TIME))
		{
			UpdateDBSync();
		}

		return ResultCode::SUCCESS;
	}

	Result GamePlayerEntity::UpdateDBSync(TransactionID transID)
	{
		Result hr = ResultCode::SUCCESS;

		if (!(UpdateGamePlayer()))
			return ResultCode::SUCCESS_FALSE;

		m_LatestDBSyncTime = Util::Time.GetTimeUTCSec();
		//auto pPlayerInfoSystem = GetComponent<UserGamePlayerInfoSystem>();

		//svrChk(Svr::GetServerComponent<DB::GameConspiracyDB>()->UpdateTickStatusCmd(transID, GetShardID(), GetPlayerID(),
		//	pPlayerInfoSystem->GetGem(), pPlayerInfoSystem->GetStamina(), GetIsInGame() ? 1 : 0,
		//	GetLatestActiveTime(),
		//	GetLatestUpdateTime()));

	//Proc_End:

		return hr;
	}

	// register message handlers
	Result GamePlayerEntity::RegisterMessageHandlers()
	{
		
		BR_ENTITY_MESSAGE(Message::Game::HeartBitC2SEvt)				{ pNewTrans = nullptr; return OnNewUserTranscation(); } );

		return ResultCode::SUCCESS;
	}

	// clear transaction
	Result GamePlayerEntity::ClearEntity()
	{
		Result hr = ResultCode::SUCCESS;

		ReleaseConnection();

		svrChk(Svr::SimpleUserEntity::ClearEntity() );

	Proc_End:

		return hr;
	}

	// Run the task
	Result GamePlayerEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;
		Transaction *trans = nullptr;

		hr = Svr::SimpleUserEntity::TickUpdate(pAction);
		if (hr == Result(ResultCode::SUCCESS_FALSE))
			return hr;


		if (m_TimeToKill.CheckTimer() 
			&& GetEntityState() == EntityState::WORKING)
		{
			svrMem(trans = CreateCloseTransaction());
			svrChk(trans->InitializeTransaction(this));
			PendingTransaction(GetTaskWorker()->GetThreadID(), trans);
		}

	Proc_End:

		if (trans != nullptr)
		{
			SharedPointerT<Svr::Transaction>(trans);
		}

		return hr;
	}



	// Update Game Player 
	Result GamePlayerEntity::UpdateGamePlayer()
	{
		Result hr = ResultCode::SUCCESS;

		// m_LatestUpdateTime will be initialized when character data is loaded
		if (m_LatestUpdateTime == TimeStampSec::min())
		{
			return ResultCode::SUCCESS_FALSE;
		}


	//Proc_End:

		return hr;
	}
	
	

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	
	//

	bool GamePlayerEntity::GetIsInGame() const
	{
		return GetGameInsUID() != 0 || GetPartyUID() != 0;
	}

	void GamePlayerEntity::SetGameInsUID( const GameInsUID& gameInsID )
	{
		m_GameInsUID = gameInsID;

		m_PlayerInformation.IsPlayingGame = m_GameInsUID != 0 ? TRUE : FALSE;
	}


	const PlayerInformation& GamePlayerEntity::GetPlayerInformation() const
	{
		//const_cast<GamePlayerEntity*>(this)->m_PlayerInformation.Level = GetComponent<UserGamePlayerInfoSystem>()->GetLevel();
		//const_cast<GamePlayerEntity*>(this)->m_PlayerInformation.WeeklyWin = GetComponent<UserGamePlayerInfoSystem>()->GetWeeklyWin();
		//const_cast<GamePlayerEntity*>(this)->m_PlayerInformation.WeeklyLose = GetComponent<UserGamePlayerInfoSystem>()->GetWeeklyLose();

		return m_PlayerInformation;
	}


	//const ServerFriendInformation& GamePlayerEntity::GetFriendInformation() const
	//{
	//	m_PlayerInformation.ShardID = GetShardID();
	//	m_PlayerInformation.Level = GetComponent<UserGamePlayerInfoSystem>()->GetLevel();
	//	m_PlayerInformation.WeeklyWin = GetComponent<UserGamePlayerInfoSystem>()->GetWeeklyWin();
	//	m_PlayerInformation.WeeklyLose = GetComponent<UserGamePlayerInfoSystem>()->GetWeeklyLose();
	//	m_PlayerInformation.LastActiveTime = GetLatestActiveTime().time_since_epoch().count();
	//	m_PlayerInformation.IsPlayingGame = GetIsInGame();

	//	return m_PlayerInformation;
	//}

	void GamePlayerEntity::AddGameTransactionLog(TransLogCategory LogCategory, INT consume, INT gain, uint64_t totalValue, const char* logMessage)
	{
		Svr::GetServerComponent<DB::GameTransactionDB>()->AddGameLog(GetShardID(), GetPlayerID(), Util::Time.GetTimeUTCSec(), LogCategory, consume, gain, totalValue, logMessage);
	}

	void GamePlayerEntity::AddGameTransactionLog(TransLogCategory LogCategory, INT consume, INT gain, uint64_t totalValue)
	{
		AddGameTransactionLog(LogCategory, consume, gain, totalValue, "");
	}

	//// Send push notify
	//Result GamePlayerEntity::SendPushNotify( const char* strMessage, uint64_t param )
	//{
	//	Result hr = ResultCode::SUCCESS;

	//	// Send GCM push notify
	//	if( GetGCMKeys() != nullptr && GetGCMKeys()[0] != '\0' )
	//	{
	//		Svr::ExternalTransactionManager *pExternal = Svr::GetServerComponent<Svr::ExternalTransactionManager>();
	//		svrChkPtr(pExternal);
	//		svrChk( pExternal->SendGCMNotify( 0, GetGCMKeys(), strMessage, param ) );
	//	}
	//Proc_End:

	//	return hr;
	//}


}; // namespace GameServer
}; // namespace SF


