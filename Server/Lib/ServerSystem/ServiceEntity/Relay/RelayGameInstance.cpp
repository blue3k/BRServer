////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2019 The Braves
// 
// Author : KyungKun Ko
//
// Description : Relay Game Instance
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

#include "RelayGameInstance.h"



SF_MEMORYPOOL_IMPLEMENT(SF::Svr::RelayGameInstance);




namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	RelayGameInstance::RelayGameInstance(IHeap& heap)
		: m_Users(heap)
	{
	}

	RelayGameInstance::~RelayGameInstance()
	{
		Clear();
	}

	// Initialize entity to proceed new connection
	Result RelayGameInstance::InitializeGameInstance(GameID gameId, uint32_t maxUser)
	{
		FunctionContext hr;

		m_MaxUser = maxUser;
		m_Users.resize(maxUser);

		m_TimeToKill.SetTimer(DurationMS(Const::RELAY_GAMEINSTANCE_TIMEOUT));

		return hr;
	}

	void RelayGameInstance::SetGameInstanceState(GameInstanceState newState)
	{
		m_GameInstanceState = newState;
	}

	void RelayGameInstance::HeartBit()
	{
		m_TimeToKill.SetTimer(DurationMS(Const::RELAY_GAMEINSTANCE_TIMEOUT));
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity process
	//

	// register message handlers
	Result RelayGameInstance::RegisterMessageHandlers()
	{
		return ResultCode::SUCCESS;
	}

	// clear transaction
	Result RelayGameInstance::Clear()
	{
		FunctionContext hr;

		return hr;
	}

	// Run the task
	Result RelayGameInstance::TickUpdate()
	{
		FunctionContext hr = ResultCode::SUCCESS;
		auto curTime = Util::Time.GetTimeMs();


		svrCheck( UpdateRelayGameInstances(curTime) );

		return hr;
	}


	// Update Game Player 
	Result RelayGameInstance::UpdateRelayGameInstances( TimeStampMS ulCurTime )
	{
		FunctionContext hr = ResultCode::SUCCESS;

		if( m_TimeToKill.CheckTimer())
		{
			// Close the instance, even though we might have user, no one has activity. just kill them
			SetGameInstanceState(GameInstanceState::Closing);
		}

		return hr;
	}
	


}; // namespace Svr
}; // namespace SF


