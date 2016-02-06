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
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "Common/Memory.h"
#include "Common/GameConst.h"
#include "Net/NetDef.h"
#include "Net/NetServerUDP.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/Login/LoginServiceEntity.h"
#include "ServerSystem/ServiceEntity/Login/LoginPlayerEntity.h"
#include "ServerSystem/ServiceEntity/Login/LoginPlayerEntityTrans.h"
#include "ServerSystem/ServiceEntity/RankingServiceTrans.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrConfig.h"

#include "Protocol/Message/LoginServerMsgClass.h"

#include "DB/AccountDB.h"
#include "DB/LoginSessionDB.h"



namespace BR {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//


	LoginServiceEntity::LoginServiceEntity(Config::PublicNetSocket *publicNetSocket, ClusterMembership initialMembership)
		: ReplicaClusterServiceEntity(ClusterID::Login, initialMembership)
		, m_PublicNetSocket(publicNetSocket)
		, m_pNetPublic(nullptr)
	{
		AssertRel(publicNetSocket != nullptr);
	}

	LoginServiceEntity::~LoginServiceEntity()
	{
	}

	HRESULT LoginServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_SYSTEM_OK;
		ClusteredServiceEntity *pClusteredEntity = nullptr;
		auto pServerInst = BrServer::GetInstance();

		svrChkPtr(pServerInst);

		svrChk(ReplicaClusterServiceEntity::InitializeEntity(newEntityID) );

		LoginPlayerEntity::GetAuthTicketGenerator().SetServerID(pServerInst->GetServerUID());


		// public network
		svrChkPtr(m_PublicNetSocket);

		svrMem(m_pNetPublic = new Net::ServerMUDP(BrServer::GetInstance()->GetServerUID(), NetClass::Login));
		svrChk(m_pNetPublic->HostOpen(NetClass::Login, m_PublicNetSocket->IPV6.c_str(), m_PublicNetSocket->Port));

		// Account DB
		svrChk(pServerInst->AddDBCluster<DB::AccountDB>(Config::GetConfig().AccountDB));

		// Session DB initialize
		svrChk(pServerInst->AddDBCluster<DB::LoginSessionDB>(Config::GetConfig().LoginSessionDB));

		// Register game clusters, so that login server can monitor game servers status
		for (ClusterID gameCluster = ClusterID::Game; gameCluster < ClusterID::Game_Max; gameCluster++)
		{
			svrChk(pServerInst->GetComponent<ClusterManagerServiceEntity>()->CreateWatcherForCluster(gameCluster, ClusterType::Shard, pClusteredEntity));
		}

		m_pNetPublic->SetIsEnableAccept(true);

	Proc_End:

		return hr;
	}

	// clear transaction
	HRESULT LoginServiceEntity::ClearEntity()
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(ReplicaClusterServiceEntity::ClearEntity() );

		if(m_pNetPublic != nullptr)
			svrChk(m_pNetPublic->HostClose());

	Proc_End:

		Util::SafeDelete(m_pNetPublic);

		return hr;
	}

	HRESULT LoginServiceEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_SYSTEM_OK;

		svrChk(ReplicaClusterServiceEntity::TickUpdate(pAction) );

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
	HRESULT LoginServiceEntity::ProcessPublicNetworkEvent()
	{
		HRESULT hr = S_SYSTEM_OK;
		Net::INet::Event curEvent;
		LoginPlayerEntity *pLoginPlayerEntity = nullptr;
		Net::Connection *pConn = nullptr;

		if (m_pNetPublic == nullptr)
			return S_SYSTEM_OK;

		while (SUCCEEDED(m_pNetPublic->DequeueNetEvent(curEvent)))
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

				pConn = dynamic_cast<Net::Connection*>(curEvent.EventConnection);

				svrChkPtr(pConn);
				svrChkPtr(GetServerComponent<Svr::EntityManager>());

				svrMem(pLoginPlayerEntity = new LoginPlayerEntity);

				svrChk(GetServerComponent<EntityManager>()->AddEntity(EntityFaculty::User, pLoginPlayerEntity));

				if (FAILED(pLoginPlayerEntity->SetConnection(pConn)))
				{
					// NOTE: We need to mark to close this
					pLoginPlayerEntity->ClearEntity();
				}

				pLoginPlayerEntity = nullptr;

				break;
			case Net::INet::Event::EVT_CONNECTION_DISCONNECTED:
				break;
			default:
				break;
			};
		}


	Proc_End:

		if (pConn && m_pNetPublic)
			m_pNetPublic->GetConnectionManager().PendingReleaseConnection(pConn);

		Util::SafeDelete(pLoginPlayerEntity);

		return S_SYSTEM_OK;
	}



}; // namespace Svr {
}; // namespace BR {


