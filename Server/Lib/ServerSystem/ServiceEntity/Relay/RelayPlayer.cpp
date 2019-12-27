////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2019 The Braves
// 
// Author : KyungKun Ko
//
// Description : Relay player
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

#include "RelayPlayer.h"



namespace SF {



	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//

	RelayPlayer::~RelayPlayer()
	{
		Clear();
	}

	// clear 
	void RelayPlayer::Clear()
	{
	}

	// Initialize entity to proceed new connection
	Result RelayPlayer::InitializePlayer(IHeap& heap, RelayInstance* relayInstance, const sockaddr_storage& remoteAddr, const RelayPlayerID& relayPlayerID, PlayerID playerID, const String& playerIdentifier, const char* PlayerDisplayName)
	{
		FunctionContext hr;

		m_Address = remoteAddr;
		m_PlayerIdentifier.SetHeap(heap);
		m_RelayInstance = relayInstance;
		m_PlayerID = playerID;
		m_PlayerIdentifier = playerIdentifier;
		m_RelayPlayerID = relayPlayerID;
		SetPlayerDisplayName(PlayerDisplayName);

		m_TimeToKill.SetTimer(DurationMS(Svr::Const::RELAY_PLAYER_JOIN_TIMEOUT));

		return hr;
	}

	void RelayPlayer::SetPlayerState(PlayerState newState)
	{
		m_PlayerState = newState;
	}

	void RelayPlayer::HeartBit()
	{
		m_TimeToKill.SetTimer(DurationMS(Svr::Const::RELAY_PLAYER_TIMEOUT));
	}



	Result RelayPlayer::SetPlayerDisplayName(const char* playerName)
	{
		return StrUtil::StringCopy(m_PlayerDisplayName, playerName);
	}



	// Run the task
	Result RelayPlayer::TickUpdate()
	{
		FunctionContext hr;
		auto curTime = Util::Time.GetTimeMs();


		svrCheck(UpdateTimeout(curTime) );

		return hr;
	}


	// Update Game Player 
	Result RelayPlayer::UpdateTimeout( TimeStampMS ulCurTime )
	{
		FunctionContext hr;

		if( m_TimeToKill.CheckTimer())
		{
			SetPlayerState(PlayerState::Closing);
		}

		return hr;
	}
	


}; // namespace SF


