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
#include "Common/GameConst.h"
#include "Net/NetDef.h"
#include "Net/NetServerUDP.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServiceEntity/Game/GameServiceEntity.h"
#include "ServerSystem/ServiceEntity/Game/GamePlayerEntity.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceEntity.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"



namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	GameServiceEntity::GameServiceEntity(GameID gameID, Config::PublicNetSocket *publicNetSocket, ClusterMembership initialMembership)
		: FreeReplicaClusterServiceEntity(ClusterID::Game, initialMembership)
		, m_PublicNetSocket(publicNetSocket)
		, m_GameID(gameID)
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

		svrChk(FreeReplicaClusterServiceEntity::InitializeEntity(newEntityID) );

		// public network
		svrChkPtr(m_PublicNetSocket);

		svrMem(m_pNetPublic = new Net::ServerMUDP(BrServer::GetInstance()->GetServerUID(), NetClass::Game));
		svrChk(m_pNetPublic->HostOpen(NetClass::Game, m_PublicNetSocket->IPV6.c_str(), m_PublicNetSocket->Port));


		// Register game cluster as a slave
		svrMem(pGameService = new Svr::GameClusterServiceEntity(m_GameID, m_PublicNetSocket, ClusterMembership::Slave));
		svrChk(GetServerComponent<Svr::EntityManager>()->AddEntity(EntityFaculty::Service, pGameService));
		svrChk(GetServerComponent<Svr::ClusterManagerServiceEntity>()->AddClusterServiceEntity(pGameService));
		svrChk(AddServerComponent(pGameService));


	Proc_End:

		return hr;
	}

	// clear transaction
	Result GameServiceEntity::ClearEntity()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(FreeReplicaClusterServiceEntity::ClearEntity() );

		svrChk(m_pNetPublic->HostClose());

	Proc_End:

		Util::SafeDelete(m_pNetPublic);

		return hr;
	}

	Result GameServiceEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(FreeReplicaClusterServiceEntity::TickUpdate(pAction) );

		// check below only if we are working
		if( GetEntityState() != EntityState::WORKING )
			goto Proc_End;

		if( BrServer::GetInstance()->GetServerState() != ServerState::RUNNING )
			goto Proc_End;

		svrChk(ProcessPublicNetworkEvent());

	Proc_End:

		return hr;
	}




	// Process network event
	Result GameServiceEntity::ProcessPublicNetworkEvent()
	{
		Result hr = ResultCode::SUCCESS;
		Net::INet::Event curEvent;
		GamePlayerEntity *pGamePlayerEntity = nullptr;
		Net::ConnectionPtr pConn = nullptr;
		EntityManager *pEntityManager = nullptr;
		Entity* pEntity = nullptr;

		if (m_pNetPublic == nullptr)
			return ResultCode::SUCCESS;

		while ((m_pNetPublic->DequeueNetEvent(curEvent)))
		{
			pConn = nullptr;

			switch (curEvent.EventType)
			{
			case Net::INet::Event::EVT_NET_INITIALIZED:
				break;
			case Net::INet::Event::EVT_NET_CLOSED:
				break;
			case Net::INet::Event::EVT_NEW_CONNECTION:
				if (curEvent.EventConnection == nullptr)
					break;

				pConn = std::forward<Net::ConnectionPtr>(curEvent.EventConnection);

				pEntityManager = GetServerComponent<EntityManager>();

				svrChkPtr(pConn);
				svrChkPtr(pEntityManager);

				svrChk(pEntityManager->CreateEntity(GetClusterID(), EntityFaculty::User, pEntity));
				svrChkPtr(pGamePlayerEntity = dynamic_cast<GamePlayerEntity*>(pEntity));
				//svrChk(pEntityManager->CreateGamePlayerEntity(pGamePlayerEntity));

				svrChk(pEntityManager->AddEntity(EntityFaculty::User, pGamePlayerEntity));

				if (!(pGamePlayerEntity->SetConnection(std::forward<Net::ConnectionPtr>(pConn))))
				{
					// NOTE: We need to mark to close this
					pGamePlayerEntity->ClearEntity();
				}

				pGamePlayerEntity = nullptr;

				break;
			case Net::INet::Event::EVT_CONNECTION_DISCONNECTED:
				break;
			default:
				break;
			};
		}


	Proc_End:

		if (pConn != nullptr && m_pNetPublic)
			m_pNetPublic->GetConnectionManager().PendingReleaseConnection(pConn);

		Util::SafeDelete(pGamePlayerEntity);

		return ResultCode::SUCCESS;
	}



}; // namespace Svr {
}; // namespace SF {



