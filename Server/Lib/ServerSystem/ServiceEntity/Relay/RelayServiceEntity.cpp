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
#include "ServiceEntity/Relay/RelayServiceEntity.h"
#include "ServiceEntity/Relay/RelayPlayer.h"
#include "ServiceEntity/RankingServiceTrans.h"
#include "Server/BrServer.h"

#include "SvrTrace.h"
#include "SvrConst.h"
#include "ServerConfig/SFServerConfig.h"
#include "Service/ServerService.h"




namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//


	RelayServiceEntity::RelayServiceEntity(GameID gameID, const ServerConfig::NetPublic& publicNetSocket, uint32_t maximumRelayInstances, ClusterMembership initialMembership)
		: super(nullptr, ClusterID::Relay, initialMembership)
		, m_GameID(gameID)
		, m_MaxInstances(maximumRelayInstances)
		, m_PublicNetSocket(&publicNetSocket)
		, m_pNetPublic(nullptr)
		, m_NewConnectionQueue(GetHeap())
	{
	}

	RelayServiceEntity::~RelayServiceEntity()
	{
	}

	Result RelayServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;
		ClusteredServiceEntity *pClusteredEntity = nullptr;
		auto pServerInst = BrServer::GetInstance();

		svrChkPtr(pServerInst);

		svrChk(super::InitializeEntity(newEntityID) );

		// public network
		svrChkPtr(m_PublicNetSocket);

		svrMem(m_pNetPublic = new(GetHeap()) Net::RawUDP);

		svrChk(m_pNetPublic->InitializeNet(NetAddress(m_PublicNetSocket->ListenIP, m_PublicNetSocket->Port), this));

	Proc_End:

		return hr;
	}

	// clear transaction
	Result RelayServiceEntity::ClearEntity()
	{
		FunctionContext hr;

		svrCheck(super::ClearEntity());

		if (m_pNetPublic != nullptr)
		{
			m_pNetPublic->TerminateNet();
			delete m_pNetPublic;
			m_pNetPublic = nullptr;
		}

		return hr;
	}

	Result RelayServiceEntity::TickUpdate(TimerAction *pAction)
	{
		FunctionContext hr;

		svrCheck(super::TickUpdate(pAction) );

		// check below only if we are working
		if( GetEntityState() != EntityState::WORKING )
			return hr;

		if( BrServer::GetInstance()->GetServerState() != ServerState::RUNNING )
			return hr;

		svrCheck(ProcessNewConnection());

		return hr;
	}

	Result RelayServiceEntity::OnRecv(const sockaddr_storage& remoteAddr, SharedPointerT<Message::MessageData>& pMsg)
	{
		FunctionContext hr;

		return hr;
	}


	// Process network event
	Result RelayServiceEntity::ProcessNewConnection()
	{
		FunctionContext hr;
		//FunctionContext hr([]() {});
		SharedPointerT<Net::Connection> pConn;

		if (m_pNetPublic == nullptr)
			return ResultCode::SUCCESS;


		auto numQueued = m_NewConnectionQueue.GetEnqueCount();
		for (uint iQueue = 0; iQueue < numQueued; iQueue++)
		{
			SharedPointerAtomicT<Net::Connection> pConnAtomic;

			if (!m_NewConnectionQueue.Dequeue(pConnAtomic))
				break;

			auto connectionState = pConnAtomic->GetConnectionState();
			switch (connectionState)
			{
			case Net::ConnectionState::CONNECTING:
				m_NewConnectionQueue.Enqueue(std::forward<SharedPointerAtomicT<Net::Connection>>(pConnAtomic));
				break;
			case Net::ConnectionState::CONNECTED:
				break;
			default:
				assert(connectionState == Net::ConnectionState::DISCONNECTED); // I want to see when this happens
				pConn = std::forward <SharedPointerAtomicT<Net::Connection>>(pConnAtomic);
				Service::ConnectionManager->RemoveConnection(pConn);
				pConn->DisconnectNRelease("Disconnected? before handed over to RelayService");
				pConn = nullptr;
				break;
			}

			if (pConnAtomic == nullptr)
				continue;

			pConn = std::forward <SharedPointerAtomicT<Net::Connection>>(pConnAtomic);

			RelayPlayer *pRelayPlayer;
			svrCheckMem(pRelayPlayer = new(GetHeap()) RelayPlayer(nullptr));

			//if (!(pRelayPlayer->SetConnection(std::forward<Net::ConnectionPtr>(pConn))))
			//{
			//	// NOTE: We need to mark to close this
			//	pRelayUser->ClearEntity();
			//}
			//else
			//{
			//	pConn = nullptr;
			//}

			//pRelayUser = nullptr;
		}

		return hr;
	}



}; // namespace Svr {
}; // namespace SF {



