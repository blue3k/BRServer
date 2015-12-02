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
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BRServerUtil.h"
#include "ServerSystem/EventTask.h"
#include "Net/NetServerUDP.h"
#include "Common/TimeUtil.h"
#include "Common/BRBaseTypes.h"
#include "Common/HRESCommon.h"
#include "Common/HRESGame.h"


#include "LoginSvrConst.h"
#include "LoginServerClass.h"



#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GameIPolicy.h"
#include "Protocol/Message/GameMsgClass.h"

#include "Protocol/Policy/LoginIPolicy.h"
#include "Protocol/Message/LoginMsgClass.h"
#include "Protocol/Policy/LoginServerIPolicy.h"
#include "Protocol/Message/LoginServerMsgClass.h"


#include "Entity/LoginPlayerEntityTrans.h"
#include "LoginPlayerEntity.h"
#include "LoginSvrConst.h"


BR_MEMORYPOOL_IMPLEMENT(BR::LoginServer::LoginPlayerEntity);




namespace BR {
namespace LoginServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	LoginPlayerEntity::LoginPlayerEntity()
		: m_Allocator(STDAllocator::GetInstance())
		, m_FacebookUID(0)
		, m_AuthTicket(0)
		, m_ShardID(0)
		, m_IsTicketOwner(false)
	{
		SetTickInterval(DurationMS(1000));
	}

	LoginPlayerEntity::~LoginPlayerEntity()
	{
	}

	// Initialize entity to proceed new connection
	HRESULT LoginPlayerEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;

		svrChk( super::InitializeEntity( newEntityID ) );

		m_IsTicketOwner = false;
		memset(m_GCMKeys, 0, sizeof(m_GCMKeys) );

		m_TimeToKill.SetTimer(DurationMS(Const::TIME_WAIT_PLAYER_JOIN));

	Proc_End:


		return hr;
	}

	// Set connection for pilot
	HRESULT LoginPlayerEntity::SetConnection( Net::Connection* &pCon )
	{
		HRESULT hr = S_OK;

		if( GetConnection() == pCon )
			return hr;

		if( GetConnection() != nullptr )
		{
			ReleaseConnection();
		}

		svrChk( super::SetConnection(pCon) );

		svrChk( GetConnection()->CreatePolicy( Policy::ISvrPolicyGame::ID_POLICY ) );

		pCon = nullptr;

	Proc_End:

		return hr;
	}


	void LoginPlayerEntity::HeartBit()
	{
		m_TimeToKill.SetTimer(DurationMS(Const::TIME_WAIT_PLAYER_JOIN));
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity process
	//

	// register message handlers
	HRESULT LoginPlayerEntity::RegisterMessageHandlers()
	{
		BR_ENTITY_MESSAGE(Message::Login::LoginCmd)							{ pNewTrans = new LoginPlayerTransLogin(pMsgData); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::Login::LoginByFacebookCmd)				{ pNewTrans = new LoginPlayerTransLoginByFacebook(pMsgData); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::Login::CreateRandomUserCmd)				{ pNewTrans = new LoginPlayerTransCreateRandomUser(pMsgData); return S_OK; } );
		BR_ENTITY_MESSAGE(Message::LoginServer::PlayerJoinedToGameServerCmd){ pNewTrans = new LoginPlayerJoinedToGameServerTrans(pMsgData); return S_OK; } );

		return S_OK;
	}

	// clear transaction
	HRESULT LoginPlayerEntity::ClearEntity()
	{
		HRESULT hr = S_OK;

		ReleaseConnection();

		svrChk( super::ClearEntity() );

	Proc_End:

		return hr;
	}

	// Run the task
	HRESULT LoginPlayerEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_OK;
		auto curTime = Util::Time.GetTimeMs();

		svrChk( super::TickUpdate(pAction) );

		svrChk( UpdateLoginPlayer(curTime) );

	Proc_End:

		return hr;
	}


	// Update Game Player 
	HRESULT LoginPlayerEntity::UpdateLoginPlayer( TimeStampMS ulCurTime )
	{
		HRESULT hr = S_OK;

		if( m_TimeToKill.CheckTimer() )
		{
			PendingCloseTransaction();
		}

	Proc_End:

		return hr;
	}
	
	HRESULT LoginPlayerEntity::PendingCloseTransaction()
	{
		HRESULT hr = S_OK;
		Svr::Transaction *trans = nullptr;

		svrMem( trans = new LoginPlayerTransCloseInstance );
		svrChk( trans->InitializeTransaction(this) );
		svrChk(this->PendingTransaction(GetTaskWorker()->GetThreadID(), trans));

	Proc_End:

		return hr;
	}


}; // namespace LoginServer
}; // namespace BR


