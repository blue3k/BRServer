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
#include "Task/ServerTask.h"
#include "Task/ServerTaskManager.h"
#include "Net/SFNetRawUDP.h"
#include "Util/SFTimeUtil.h"
#include "Types/SFEngineTypedefs.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"

#include "Protocol/GameServerNetPolicy.h"
#include "Protocol/GameNetPolicy.h"
#include "Protocol/GameMsgClass.h"

#include "RelayInstance.h"






namespace SF {

	const int32_t RelayInstance::RELAY_INSTANCE_TIMEOUT = 2 * 60 * 1000;

	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	RelayInstance::RelayInstance(IHeap& heap, Net::RawUDP* pNet, uint32_t instanceID)
		: m_Heap("RelayInstance", heap)
		, m_pNet(pNet)
		, m_Users(heap)
		, m_InstanceID(instanceID)
	{
	}

	RelayInstance::~RelayInstance()
	{
		Clear();
	}

	// Initialize entity to proceed new connection
	Result RelayInstance::InitializeGameInstance(GameID gameId, uint32_t maxUser)
	{
		ScopeContext hr;

		m_MaxUser = maxUser;
		m_Users.resize(maxUser);

		m_TimeToKill.SetTimer(DurationMS(RELAY_INSTANCE_TIMEOUT));

		return hr;
	}

	void RelayInstance::SetGameInstanceState(GameInstanceState newState)
	{
		m_GameInstanceState = newState;
	}

	void RelayInstance::Heartbeat()
	{
		m_TimeToKill.SetTimer(DurationMS(RELAY_INSTANCE_TIMEOUT));
	}


	Result RelayInstance::AddPlayer(const sockaddr_storage& remoteAddr, PlayerID playerID, const String playerIdentification, RelayPlayerID& relayPlayerID)
	{
		ScopeContext hr;
		RelayPlayer *pPlayer = nullptr;

		for (uint32_t iUser = 0; iUser < m_Users.size(); iUser++)
		{
			if (m_Users[iUser].GetPlayerID() == playerID)
			{
				pPlayer = &m_Users[iUser];
				relayPlayerID.SetPlayerIndex(iUser);
				break;
			}
		}

		if (pPlayer == nullptr)
		{
			m_Users.push_back({});
			relayPlayerID.SetPlayerIndex(static_cast<uint32_t>(m_Users.size() - 1));
			pPlayer = &m_Users[relayPlayerID];
		}

		svrCheckPtr(pPlayer);

		svrCheck(pPlayer->InitializePlayer(GetHeap(), this, remoteAddr, relayPlayerID, playerID, playerIdentification, "TestTemp"));

		return hr;
	}

	Result RelayInstance::RemovePlayer(PlayerID playerID)
	{
		ScopeContext hr;
		RelayPlayer* pPlayer = nullptr;

		for (uint32_t iUser = 0; iUser < m_Users.size(); iUser++)
		{
			if (m_Users[iUser].GetPlayerID() == playerID)
			{
				pPlayer = &m_Users[iUser];
				break;
			}
		}

		if (pPlayer == nullptr)
			return hr = ResultCode::INVALID_PLAYERID;

		pPlayer->Clear();

		return hr;
	}

	// Handle relay packet
	void RelayInstance::OnRelayPacket(const sockaddr_storage& remoteAddr, const Message::PlayInstance::PlayPacketC2SEvt& message)
	{
		RelayPlayerID senderRelayID = message.GetSenderEndpointID();

		uint32_t targetMask = message.GetTargetEndpointMask();
		MessageDataPtr messageData = message.GetMessagePtr();
		for (uint32_t iUser = 0; iUser < m_Users.size(); iUser++)
		{
			if (m_Users[iUser].GetPlayerID() == 0)
				continue;

			auto relayID = m_Users[iUser].GetRelayPlayerID();

#if defined(DEBUG)
			RelayPlayerID relayIDTest;
			relayIDTest.SetPlayerIndex(iUser);
			assert(relayIDTest == relayID);
#endif
			// Skip sender
			if (senderRelayID == relayID)
				continue;

			// it is not targeted
			if ((targetMask & relayID.GetRelayPlayerID()) == 0)
				continue;

			m_pNet->SendMsg(remoteAddr, messageData);
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity process
	//

	// register message handlers
	Result RelayInstance::RegisterMessageHandlers()
	{
		return ResultCode::SUCCESS;
	}

	// clear transaction
	Result RelayInstance::Clear()
	{
		ScopeContext hr;

		return hr;
	}

	// Run the task
	Result RelayInstance::TickUpdate()
	{
		ScopeContext hr = ResultCode::SUCCESS;
		auto curTime = Util::Time.GetTimeMs();


		svrCheck( UpdateRelayInstances(curTime) );

		return hr;
	}


	// Update Game Player 
	Result RelayInstance::UpdateRelayInstances( TimeStampMS ulCurTime )
	{
		ScopeContext hr = ResultCode::SUCCESS;

		if( m_TimeToKill.CheckTimer())
		{
			// Close the instance, even though we might have user, no one has activity. just kill them
			SetGameInstanceState(GameInstanceState::Closing);
		}

		return hr;
	}
	


}; // namespace SF


