////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2019 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Relay Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "String/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFThread.h"
#include "Memory/SFMemory.h"
#include "GameConst.h"
#include "Net/SFNetDef.h"
#include "Net/SFNetServerUDP.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "Entity/EntityManager.h"
#include "NetRelayServer.h"
#include "ServiceEntity/Relay/RelayPlayer.h"
#include "ServiceEntity/RankingServiceTrans.h"
#include "Server/BrServer.h"

#include "SvrTrace.h"
#include "SvrConst.h"
#include "ServerConfig/SFServerConfig.h"
#include "Service/ServerService.h"
#include "ServiceEntity/Relay/RelayInstance.h"
#include "ServiceEntity/Relay/RelayPlayer.h"

#include "Protocol/Message/RelayMsgClass.h"
#include "ServiceEntity/Relay/RelayInstance.h"


namespace SF {
namespace Net {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//


	RelayServer::RelayServer(GameID gameID, uint32_t maximumRelayInstances)
		: m_GameID(gameID)
		, m_MaxInstances(maximumRelayInstances)
		, m_RelayInstanceByID(m_NetRawUDP.GetHeap())
		, m_HandlerTable(m_NetRawUDP.GetHeap())
	{
	}

	RelayServer::~RelayServer()
	{
		TerminateNet();
	}

	Result RelayServer::InitializeNet(const NetAddress& listenAddr)
	{
		FunctionContext hr;

		svrCheck(m_NetRawUDP.InitializeNet(listenAddr, [this](const sockaddr_storage& remoteAddr, SharedPointerT<Message::MessageData>& pMsg) -> Result
		{
			return OnRecv(remoteAddr, pMsg);
		}));

		return hr;
	}

	// clear 
	void RelayServer::TerminateNet()
	{
		m_NetRawUDP.TerminateNet();
	}


	Result RelayServer::TickUpdate()
	{
		FunctionContext hr;


		return hr;
	}

	Result RelayServer::RegisterMessageHandlers()
	{
		m_HandlerTable.Register<Message::Relay::JoinRelayInstanceC2SEvt>(__FILE__, __LINE__, &RelayServer::OnJoinRelayInstanceC2SEvt);
		m_HandlerTable.Register<Message::Relay::LeaveRelayInstanceC2SEvt>(__FILE__, __LINE__, &RelayServer::OnLeaveRelayInstanceC2SEvt);
		m_HandlerTable.Register<Message::Relay::RelayPacketC2SEvt>(__FILE__, __LINE__, &RelayServer::OnRelayPacketC2SEvt);

		return ResultCode::SUCCESS;
	}


	Result RelayServer::OnJoinRelayInstanceC2SEvt(const sockaddr_storage& remoteAddr, MessageDataPtr&& pMsgData)
	{
		RelayInstance* pRelayInstance = nullptr;
		StaticArray<RelayPlayerInfo, 32> userInfos(GetHeap());


		FunctionContext hr([&pRelayInstance](Result result) 
		{
			if (!result)
			{
				if (pRelayInstance != nullptr)
					delete pRelayInstance;
			}
		});

		Message::Relay::JoinRelayInstanceC2SEvt message(std::forward<MessageDataPtr>(pMsgData));
		svrCheck(message.ParseMsg());

		auto instanceID = message.GetRelayInstanceID();
		auto myPlayerID = message.GetPlayerID();

		auto itFound = m_RelayInstanceByID.find(instanceID);
		if (itFound == m_RelayInstanceByID.end())
		{
			pRelayInstance = new(GetHeap()) RelayInstance(GetHeap(), &m_NetRawUDP, message.GetRelayInstanceID());
			svrCheckMem(pRelayInstance);
			// TODO: change max user
			pRelayInstance->InitializeGameInstance(GetGameID(), 32);

			svrCheck(m_RelayInstanceByID.insert(pRelayInstance->GetInstanceID(), pRelayInstance));
		}
		else
		{
			pRelayInstance = *itFound;
		}

		RelayPlayerID relayPlayerID;
		svrCheck(pRelayInstance->AddPlayer(remoteAddr, myPlayerID, message.GetPlayerIdentifier(), relayPlayerID));

		// TODO: max constant
		pRelayInstance->ForeachPlayer([&userInfos, myPlayerID](uint32_t relayPlayerID, const RelayPlayer& relayPlayer)
		{
			// relayPlayer.GetPlayerID() == 0 is for left player
			if (relayPlayer.GetPlayerID() == 0 || relayPlayer.GetPlayerID() == myPlayerID)
				return;

			RelayPlayerInfo playerInfo;
			playerInfo.RelayPlayerID = relayPlayerID;
			playerInfo.EndpointID = relayPlayer.GetPlayerID();
			userInfos.push_back(playerInfo);
		});


		MessageDataPtr pMessage = Message::Relay::JoinRelayInstanceResS2CEvt::Create(GetHeap(), hr, pRelayInstance->GetInstanceID(), relayPlayerID, userInfos);
		svrCheckMem(pMessage);

		svrCheck(m_NetRawUDP.SendMsg(remoteAddr, pMessage));

		return hr;
	}

	Result RelayServer::OnLeaveRelayInstanceC2SEvt(const sockaddr_storage& remoteAddr, MessageDataPtr&& pMsgData)
	{
		FunctionContext hr;

		Message::Relay::LeaveRelayInstanceC2SEvt message(std::forward<MessageDataPtr>(pMsgData));
		svrCheck(message.ParseMsg());


		auto instanceID = message.GetRelayInstanceID();

		auto itFound = m_RelayInstanceByID.find(instanceID);
		if (itFound == m_RelayInstanceByID.end())
		{
			return hr = ResultCode::INVALID_INSTANCEID;
		}

		RelayInstance* pRelayInstance = *itFound;
		svrCheckPtr(pRelayInstance);

		svrCheck(pRelayInstance->RemovePlayer(message.GetPlayerID()));

		return hr;
	}

	Result RelayServer::OnRelayPacketC2SEvt(const sockaddr_storage& remoteAddr, MessageDataPtr&& pMsgData)
	{
		FunctionContext hr;

		Message::Relay::RelayPacketC2SEvt message(std::forward<MessageDataPtr>(pMsgData));
		svrCheck(message.ParseMsg());

		auto relayInstanceID = message.GetRelayInstanceID();

		auto itFound = m_RelayInstanceByID.find(relayInstanceID);
		if (itFound == m_RelayInstanceByID.end())
			return hr = ResultCode::INVALID_INSTANCEID;

		auto relayInstance = *itFound;
		relayInstance->OnRelayPacket(remoteAddr, message);


		return hr;
	}

	Result RelayServer::SendMsg(const sockaddr_storage& dest, SharedPointerT<Message::MessageData>& pMsg)
	{
		return m_NetRawUDP.SendMsg(dest, pMsg);
	}

	Result RelayServer::OnRecv(const sockaddr_storage& remoteAddr, SharedPointerT<Message::MessageData>& pMsg)
	{
		FunctionContext hr;

		MessageHandlerType handler;

		if (m_HandlerTable.GetHandler(pMsg->GetMessageHeader()->msgID, handler))
		{
			(this->*handler)(remoteAddr, std::forward<SharedPointerT<Message::MessageData>>(pMsg));
		}
		else
		{
			svrTrace(Error, "Unhandled Relaymessage {0}", pMsg->GetMessageHeader()->msgID);
		}

		return hr;
	}



}; // namespace Net {
}; // namespace SF {



