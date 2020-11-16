////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game player Entity
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "SvrConst.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"
#include "Task/ServerTaskEvent.h"
#include "Task/ServerTask.h"
#include "Task/ServerTaskManager.h"
#include "Net/SFNetServerUDP.h"
#include "Util/SFTimeUtil.h"
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
		Result LoginPlayerEntity::InitializeEntity(EntityID newEntityID)
		{
			ScopeContext hr = ResultCode::SUCCESS;

			svrCheck(super::InitializeEntity(newEntityID));

			m_IsTicketOwner = false;
			memset(m_GCMKeys, 0, sizeof(m_GCMKeys));

			SetEntityKillTimer(DurationMS(Const::LOGIN_TIME_WAIT_PLAYER_JOIN));

			return hr;
		}

		// Set connection for pilot
		Result LoginPlayerEntity::SetConnection(SharedPointerT<Net::Connection>&& pCon)
		{
			Result hr = ResultCode::SUCCESS;

			if (GetConnection() == (Net::Connection*)pCon)
				return hr;

			if (GetConnection() != nullptr)
			{
				ReleaseConnection("Replacing user connection");
			}

			svrChk(super::SetConnection(std::forward<SharedPointerT<Net::Connection>>(pCon)));


			pCon = nullptr;

		Proc_End:

			return hr;
		}




		void LoginPlayerEntity::SetUserName(const char* userName)
		{
			StrUtil::StringCopy(m_UserName, userName);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Entity process
		//

		// register message handlers
		Result LoginPlayerEntity::RegisterMessageHandlers()
		{
			RegisterMessageHandler<LoginPlayerTransLogin>();
			RegisterMessageHandler<LoginPlayerTransLoginByFacebook>();
			RegisterMessageHandler<LoginPlayerTransCreateRandomUser>();
			RegisterMessageHandler<LoginPlayerJoinedToGameServerTrans>();
			RegisterMessageHandler<LoginPlayerKickPlayerTrans>();

			RegisterMessageHandler<RankingUpdateScoreTrans>();
			RegisterMessageHandler<LoginUserDataTestTrans>();
			RegisterMessageHandler<LoginUserDebugPrintALLRankingTrans>();

			return ResultCode::SUCCESS;
		}

		// clear transaction
		Result LoginPlayerEntity::ClearEntity()
		{
			Result hr = ResultCode::SUCCESS;

			ReleaseConnection("Terminate login player entity");

			svrChk(super::ClearEntity());

		Proc_End:

			return hr;
		}

		// Run the task
		Result LoginPlayerEntity::TickUpdate(TimerAction* pAction)
		{
			Result hr = ResultCode::SUCCESS;
			auto curTime = Util::Time.GetTimeMs();

			svrChk(super::TickUpdate(pAction));

			svrChk(UpdateLoginPlayer(curTime));

		Proc_End:

			return hr;
		}


		// Update Game Player 
		Result LoginPlayerEntity::UpdateLoginPlayer(TimeStampMS ulCurTime)
		{
			Result hr = ResultCode::SUCCESS;

			auto& connection = GetConnection();

			if (connection == nullptr || connection->GetConnectionState() == Net::ConnectionState::DISCONNECTED)
			{
				PendingCloseTransaction("Player Disconnected");
			}

			return hr;
		}

		Result LoginPlayerEntity::PendingCloseTransaction(const char* reason)
		{
			Result hr = ResultCode::SUCCESS;
			TransactionPtr trans;

			if (m_ClosingPended)
				return hr;

			if (GetAccountID() > 0)
			{
				svrTrace(Debug1, "Closing silent login players AccID:{0}, reason:{1}", GetAccountID(), reason);
			}

			m_ClosingPended = true;
			svrMem(trans = new(GetHeap()) LoginPlayerTransCloseInstance(GetHeap()));
			svrChk(trans->InitializeTransaction(this));
			svrChk(PendingTransaction(GetTaskWorker()->GetThreadID(), trans));

		Proc_End:

			if (trans != nullptr)
				ReleaseTransaction(trans);

			return hr;
		}


	} // namespace Svr
} // namespace SF


