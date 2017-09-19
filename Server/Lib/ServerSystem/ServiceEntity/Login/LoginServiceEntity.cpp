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
#include "Entity/EntityManager.h"
#include "ServiceEntity/Login/LoginServiceEntity.h"
#include "ServiceEntity/Login/LoginPlayerEntity.h"
#include "ServiceEntity/Login/LoginPlayerEntityTrans.h"
#include "ServiceEntity/RankingServiceTrans.h"

#include "SvrTrace.h"
#include "SvrConst.h"
#include "ServerConfig/SFServerConfig.h"
#include "Service/ServerService.h"

#include "Protocol/Message/LoginServerMsgClass.h"

#include "DB/AccountDB.h"
#include "DB/LoginSessionDB.h"



namespace SF {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//


	LoginServiceEntity::LoginServiceEntity(const ServerConfig::NetPublic *publicNetSocket, ClusterMembership initialMembership)
		: super(GameID::Invalid, ClusterID::Login, initialMembership)
		, m_PublicNetSocket(publicNetSocket)
		, m_pNetPublic(nullptr)
		, m_NewConnectionQueue(GetMemoryManager())
	{
		AssertRel(publicNetSocket != nullptr);
	}

	LoginServiceEntity::~LoginServiceEntity()
	{
	}

	Result LoginServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;
		ClusteredServiceEntity *pClusteredEntity = nullptr;
		auto pServerInst = BrServer::GetInstance();

		svrChkPtr(pServerInst);

		svrChk(super::InitializeEntity(newEntityID) );

		LoginPlayerEntity::GetAuthTicketGenerator().SetServerID(pServerInst->GetServerUID());


		// public network
		svrChkPtr(m_PublicNetSocket);

		svrMem(m_pNetPublic = new(GetMemoryManager()) Net::ServerMUDP(BrServer::GetInstance()->GetServerUID(), NetClass::Login));
		m_pNetPublic->RegisterToEngineObjectManager();
		m_pNetPublic->SetNewConnectionhandler([this](SharedPointerT<Net::Connection>& conn)
		{
			SharedPointerAtomicT<Net::Connection> pConTem;
			pConTem = std::forward<SharedPointerT<Net::Connection>>(conn);
			m_NewConnectionQueue.Enqueue(pConTem);
		});

		svrChk(m_pNetPublic->HostOpen(NetClass::Login, m_PublicNetSocket->ListenIP, m_PublicNetSocket->Port));

		// Account DB
		svrChk(pServerInst->AddDBCluster<DB::AccountDB>(Service::ServerConfig->FindDBCluster("AccountDB")));

		// Session DB initialize
		svrChk(pServerInst->AddDBCluster<DB::LoginSessionDB>(Service::ServerConfig->FindDBCluster("LoginSessionDB")));

		// TODO: need to do this more better way
		// Register game clusters, so that login server can monitor game servers status
		svrChk(Service::ClusterManager->SetWatchForCluster(GameID::Conspiracy, ClusterID::Game, true));
		svrChk(Service::ClusterManager->SetWatchForCluster(GameID::MyTownHero, ClusterID::Game, true));

		svrChk(Service::ClusterManager->SetWatchForCluster(GameID::MyTownHero, ClusterID::Ranking, true));


		m_pNetPublic->SetIsEnableAccept(true);

	Proc_End:

		return hr;
	}

	// clear transaction
	Result LoginServiceEntity::ClearEntity()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::ClearEntity() );

		if(m_pNetPublic != nullptr)
			svrChk(m_pNetPublic->HostClose());

	Proc_End:

		m_pNetPublic = nullptr;

		return hr;
	}

	Result LoginServiceEntity::TickUpdate(TimerAction *pAction)
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
	Result LoginServiceEntity::ProcessNewConnection()
	{
		Result hr = ResultCode::SUCCESS;
		SharedPointerT<LoginPlayerEntity> pLoginPlayerEntity;
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
				pConn->DisconnectNRelease("Disconnected? before handed over to LoginService");
				pConn = nullptr;
				break;
			}

			if (pConnAtomic == nullptr)
				continue;

			pConn = std::forward <SharedPointerAtomicT<Net::Connection>>(pConnAtomic);

			svrMem(pLoginPlayerEntity = new(GetMemoryManager()) LoginPlayerEntity);

			svrChk(Service::EntityManager->AddEntity(EntityFaculty::User, *pLoginPlayerEntity));

			if (!(pLoginPlayerEntity->SetConnection(std::forward<Net::ConnectionPtr>(pConn))))
			{
				// NOTE: We need to mark to close this
				pLoginPlayerEntity->ClearEntity();
			}
			else
			{
				pConn = nullptr;
			}

			pLoginPlayerEntity = nullptr;
		}


	Proc_End:


		return ResultCode::SUCCESS;
	}



}; // namespace Svr {
}; // namespace SF {



