////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "String/StrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"
#include "Memory/SFMemory.h"
#include "GameConst.h"
#include "Net/NetDef.h"
#include "Net/NetServerUDP.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "ServiceEntity/Game/GameServiceEntity.h"
#include "ServiceEntity/Game/GamePlayerEntity.h"
#include "SvrTrace.h"
#include "SvrConst.h"
#include "ServiceEntity/Game/GameClusterServiceEntity.h"
#include "Entity/EntityManager.h"




namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	GameServiceEntity::GameServiceEntity(GameID gameID, const ServerConfig::NetPublic *publicNetSocket, ClusterMembership initialMembership)
		: super(gameID, ClusterID::Game, initialMembership)
		, m_PublicNetSocket(publicNetSocket)
		, m_GameID(gameID)
		, m_NewConnectionQueue(GetMemoryManager())
	{
		AssertRel(publicNetSocket != nullptr);
	}

	GameServiceEntity::~GameServiceEntity()
	{
	}

	Result GameServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;
		GameClusterServiceEntity *pGameService = nullptr;
		ServerServiceInformation* pServiceInfo = nullptr;

		svrChk(super::InitializeEntity(newEntityID) );

		// public network
		svrChkPtr(m_PublicNetSocket);

		svrMem(m_pNetPublic = new(GetMemoryManager()) Net::ServerMUDP(BrServer::GetInstance()->GetServerUID(), NetClass::Game));
		m_pNetPublic->RegisterToEngineObjectManager();
		svrChk(m_pNetPublic->HostOpen(NetClass::Game, m_PublicNetSocket->IPV6, m_PublicNetSocket->Port));


		// Register game cluster as a slave
		svrMem(pGameService = new(GetMemoryManager()) Svr::GameClusterServiceEntity(m_GameID, m_PublicNetSocket, ClusterMembership::Slave));
		svrChk(Service::EntityManager->AddEntity(EntityFaculty::Service, pGameService));
		svrChk(Service::ClusterManager->AddClusterServiceEntity(pGameService, pServiceInfo));


	Proc_End:

		return hr;
	}

	// clear transaction
	Result GameServiceEntity::ClearEntity()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::ClearEntity() );

		svrChk(m_pNetPublic->HostClose());

	Proc_End:

		m_pNetPublic = nullptr;

		return hr;
	}

	Result GameServiceEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::TickUpdate(pAction) );

		// check below only if we are working
		if( GetEntityState() != EntityState::WORKING )
			goto Proc_End;

		if( BrServer::GetInstance()->GetServerState() != ServerState::RUNNING )
			goto Proc_End;

		svrChk(ProcessNewConnection());

	Proc_End:

		return hr;
	}




	// Process network event
	Result GameServiceEntity::ProcessNewConnection()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ServerEntity *pServerEntity = nullptr;
		Entity* pEntity = nullptr;
		GamePlayerEntity* pGamePlayerEntity = nullptr;
		SharedPointerT<Net::Connection> pConn;

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
				pConn->DisconnectNRelease("Disconnected before handed over to GameService");
				pConn = nullptr;
				break;
			}

			if (pConnAtomic == nullptr)
				continue;

			pConn = std::forward <SharedPointerAtomicT<Net::Connection>>(pConnAtomic);

			svrChkPtr(pConn);

			svrChk(Service::EntityManager->CreateEntity(GetClusterID(), EntityFaculty::User, pEntity));
			svrChkPtr(pGamePlayerEntity = dynamic_cast<GamePlayerEntity*>(pEntity));

			svrChk(Service::EntityManager->AddEntity(EntityFaculty::User, pGamePlayerEntity));

			if (!(pGamePlayerEntity->SetConnection(std::forward<Net::ConnectionPtr>(pConn))))
			{
				// NOTE: We need to mark to close this
				pGamePlayerEntity->ClearEntity();
			}

			pGamePlayerEntity = nullptr;
		}


	Proc_End:

		Util::SafeDelete(pGamePlayerEntity);

		return ResultCode::SUCCESS;
	}



}; // namespace Svr {
}; // namespace SF {



