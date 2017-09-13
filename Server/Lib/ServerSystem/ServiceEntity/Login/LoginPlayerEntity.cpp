////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
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
#include "Task/ServerTask.h"
#include "Task/ServerTaskManager.h"
#include "Net/NetServerUDP.h"
#include "Util/TimeUtil.h"
#include "Types/SFEngineTypedefs.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "Protocol/Message/GameMsgClass.h"

#include "Protocol/Policy/LoginNetPolicy.h"
#include "Protocol/Message/LoginMsgClass.h"
#include "Protocol/Policy/LoginServerNetPolicy.h"
#include "Protocol/Message/LoginServerMsgClass.h"


#include "LoginPlayerEntityTrans.h"
#include "LoginPlayerEntity.h"



SF_MEMORYPOOL_IMPLEMENT(SF::Svr::LoginPlayerEntity);




namespace SF {
namespace Svr {

	GlobalUIDGenerator LoginPlayerEntity::stm_AuthTicketGenerator;


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	LoginPlayerEntity::LoginPlayerEntity()
		: m_FacebookUID(0)
		, m_AuthTicket(0)
		, m_ShardID(0)
		, m_IsTicketOwner(false)
	{
		m_UserName[0] = '\0';
		m_GCMKeys[0] = '\0';
		SetTickInterval(DurationMS(1000));
	}

	LoginPlayerEntity::~LoginPlayerEntity()
	{
	}

	// Initialize entity to proceed new connection
	Result LoginPlayerEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::InitializeEntity( newEntityID ) );

		m_IsTicketOwner = false;
		memset(m_GCMKeys, 0, sizeof(m_GCMKeys) );

		m_TimeToKill.SetTimer(DurationMS(Const::LOGIN_TIME_WAIT_PLAYER_JOIN));

	Proc_End:


		return hr;
	}

	// Set connection for pilot
	Result LoginPlayerEntity::SetConnection(SharedPointerT<Net::Connection>&& pCon )
	{
		Result hr = ResultCode::SUCCESS;

		if( GetConnection() == (Net::Connection*)pCon )
			return hr;

		if( GetConnection() != nullptr )
		{
			ReleaseConnection();
		}

		svrChk( super::SetConnection(std::forward<SharedPointerT<Net::Connection>>(pCon)) );


		pCon = nullptr;

	Proc_End:

		return hr;
	}


	void LoginPlayerEntity::HeartBit()
	{
		m_TimeToKill.SetTimer(DurationMS(Const::LOGIN_TIME_WAIT_PLAYER_JOIN));
	}



	void LoginPlayerEntity::SetUserName(const char* userName)
	{
		StrUtil::StringCpy(m_UserName, userName);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity process
	//

	// register message handlers
	Result LoginPlayerEntity::RegisterMessageHandlers()
	{
		BR_ENTITY_MESSAGE(Message::Login::HeartBitC2SEvt)					{ pNewTrans = nullptr; HeartBit(); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::Login::LoginCmd)							{ pNewTrans = new(GetMemoryManager()) LoginPlayerTransLogin(GetMemoryManager(), pMsgData); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::Login::LoginByFacebookCmd)				{ pNewTrans = new(GetMemoryManager()) LoginPlayerTransLoginByFacebook(GetMemoryManager(), pMsgData); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::Login::CreateRandomUserCmd)				{ pNewTrans = new(GetMemoryManager()) LoginPlayerTransCreateRandomUser(GetMemoryManager(), pMsgData); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::LoginServer::PlayerJoinedToGameServerCmd){ pNewTrans = new(GetMemoryManager()) LoginPlayerJoinedToGameServerTrans(GetMemoryManager(), pMsgData); return ResultCode::SUCCESS; } );

		BR_ENTITY_MESSAGE(Message::Login::UpdateMyScoreCmd)					{ pNewTrans = new(GetMemoryManager()) RankingUpdateScoreTrans(GetMemoryManager(), pMsgData); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::Login::DataTestCmd)						{ pNewTrans = new(GetMemoryManager()) LoginUserDataTestTrans(GetMemoryManager(), pMsgData); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::Login::DebugPrintALLRankingCmd)			{ pNewTrans = new(GetMemoryManager()) LoginUserDebugPrintALLRankingTrans(GetMemoryManager(), pMsgData); return ResultCode::SUCCESS; } );
		return ResultCode::SUCCESS;
	}

	// clear transaction
	Result LoginPlayerEntity::ClearEntity()
	{
		Result hr = ResultCode::SUCCESS;

		ReleaseConnection();

		svrChk( super::ClearEntity() );

	Proc_End:

		return hr;
	}

	// Run the task
	Result LoginPlayerEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;
		auto curTime = Util::Time.GetTimeMs();

		svrChk( super::TickUpdate(pAction) );

		svrChk( UpdateLoginPlayer(curTime) );

	Proc_End:

		return hr;
	}


	// Update Game Player 
	Result LoginPlayerEntity::UpdateLoginPlayer( TimeStampMS ulCurTime )
	{
		Result hr = ResultCode::SUCCESS;

		if( m_TimeToKill.CheckTimer() )
		{
			if (GetAccountID() > 0)
			{
				svrTrace(Debug1, "Closing silent login players AccID:{0}", GetAccountID());
			}
			PendingCloseTransaction();
		}

	//Proc_End:

		return hr;
	}
	
	Result LoginPlayerEntity::PendingCloseTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		TransactionPtr trans;

		svrMem( trans = new(GetMemoryManager()) LoginPlayerTransCloseInstance(GetMemoryManager()) );
		svrChk( trans->InitializeTransaction(this) );
		svrChk(PendingTransaction(GetTaskWorker()->GetThreadID(), trans));

	Proc_End:

		if(trans != nullptr)
			ReleaseTransaction(trans);

		return hr;
	}


}; // namespace Svr
}; // namespace SF


