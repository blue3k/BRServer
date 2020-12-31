////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Server service Component entity implementation
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
#include "ServiceEntity/Login/LoginServiceEntity.h"
#include "ServiceEntity/Login/LoginPlayerEntity.h"
#include "ServiceEntity/Login/LoginPlayerEntityTrans.h"
#include "ServiceEntity/RankingServiceTrans.h"
#include "Server/BrServer.h"

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


		LoginServiceEntity::LoginServiceEntity(const ServerConfig::NetPublic* publicNetSocket, ClusterMembership initialMembership)
			: super(nullptr, ClusterID::Login, initialMembership)
			, m_PublicNetSocket(publicNetSocket)
			, m_pNetPublic(nullptr)
			, m_NewConnectionQueue(GetHeap())
		{
			AssertRel(publicNetSocket != nullptr);
		}

		LoginServiceEntity::~LoginServiceEntity()
		{
		}

		Result LoginServiceEntity::InitializeEntity(EntityID newEntityID)
		{
			ScopeContext hr;
			ClusteredServiceEntity* pClusteredEntity = nullptr;
			auto pServerInst = BrServer::GetInstance();

			svrCheckPtr(pServerInst);

			svrCheck(super::InitializeEntity(newEntityID));

			LoginPlayerEntity::GetAuthTicketGenerator().SetServerID(pServerInst->GetServerUID());


			// public network
			svrCheckPtr(m_PublicNetSocket);

			svrCheckMem(m_pNetPublic = NewObject<Net::ServerMUDP>(GetHeap(), BrServer::GetInstance()->GetServerUID(), NetClass::Login));

			m_pNetPublic->SetNewConnectionhandler([this](SharedPointerT<Net::Connection>& conn)
				{
					SharedPointerAtomicT<Net::Connection> pConTem;
					pConTem = std::forward<SharedPointerT<Net::Connection>>(conn);
					m_NewConnectionQueue.Enqueue(pConTem);
				});

			svrTrace(Info, "Starting Login network service");
			svrCheck(m_pNetPublic->HostOpen(NetClass::Login, m_PublicNetSocket->ListenIP, m_PublicNetSocket->Port));

			// Account DB
			svrCheck(pServerInst->AddDBCluster<DB::AccountDB>(Service::ServerConfig->FindDBCluster("AccountDB")));

			// Session DB initialize
			svrCheck(pServerInst->AddDBCluster<DB::LoginSessionDB>(Service::ServerConfig->FindDBCluster("LoginSessionDB")));


			for (auto& itCluster : Service::ServerConfig->GetGameClusters())
			{
				svrCheck(Service::ClusterManager->SetWatchForCluster(itCluster->GameClusterID, ClusterID::Game, true));
				svrCheck(Service::ClusterManager->SetWatchForCluster(itCluster->GameClusterID, ClusterID::Ranking, true));
			}


			m_pNetPublic->SetIsEnableAccept(true);

			return hr;
		}

		// clear transaction
		Result LoginServiceEntity::ClearEntity()
		{
			ScopeContext hr = ResultCode::SUCCESS;

			svrCheck(super::ClearEntity());

			if (m_pNetPublic != nullptr)
				m_pNetPublic->HostClose();
			m_pNetPublic = nullptr;

			return hr;
		}

		Result LoginServiceEntity::TickUpdate(TimerAction* pAction)
		{
			Result hr = ResultCode::SUCCESS;

			svrChk(super::TickUpdate(pAction));

			// check below only if we are working
			if (GetEntityState() != EntityState::WORKING)
				goto Proc_End;

			if (BrServer::GetInstance()->GetServerState() != ServerState::RUNNING)
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
					pConn = std::forward <SharedPointerAtomicT<Net::Connection>>(pConnAtomic);
					Service::ConnectionManager->RemoveConnection(pConn);
					pConn->DisconnectNRelease("Disconnected? before handed over to LoginService");
					pConn = nullptr;
					break;
				}

				if (pConnAtomic == nullptr)
					continue;

				pConn = std::forward <SharedPointerAtomicT<Net::Connection>>(pConnAtomic);

				svrMem(pLoginPlayerEntity = new(GetHeap()) LoginPlayerEntity);

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



	} // namespace Svr {
} // namespace SF {



