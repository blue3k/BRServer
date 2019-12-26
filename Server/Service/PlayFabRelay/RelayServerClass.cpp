////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2019 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : RelayServerClass
//
////////////////////////////////////////////////////////////////////////////////

#include "RelayServerPCH.h"
#include "Util/SFTimeUtil.h"
#include "SvrConst.h"
#include "Server/BrServer.h"
#include "SvrTrace.h"
#include "Service/ServerService.h"
#include "ServerEntity/ServerEntityManager.h"


#include "ServiceEntity/Game/PlayerManagerServiceEntity.h"

#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/MatchingServiceEntity.h"
#include "ServiceEntity/GamePartyManagerServiceEntity.h"

#include "Table/TableSystem.h"

#include "ServerEntity/GenericServerEntity.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "Net/SFNetServerPeerTCP.h"
#include "Net/SFNetSvrDef.h"
#include "Net/SFNetServerUDP.h"
#include "Entity/EntityManager.h"

#include "RelaySvrConst.h"
#include "RelayServerClass.h"

#include "DB/GameConspiracyDB.h"
#include "DB/AccountDB.h"

#include "gsdk.h"




namespace SF {

	
	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	RelayServer::RelayServer()
		: BrServer(NetClass::Game)
		, m_MyConfig(GetEngineHeap())
	{
		// put default
		m_MyConfig.Name = Util::GetServiceName();
		m_MyConfig.UID = 1;

		SetMyConfig(&m_MyConfig);

		// handle gsdk config
		const std::unordered_map<std::string, std::string>& config = Microsoft::Azure::Gaming::GSDK::getConfigSettings();

	}


	RelayServer::~RelayServer()
	{
	}



	// Apply configuration
	Result RelayServer::ApplyConfiguration()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::ApplyConfiguration() );

		// Initializing PlayFab stuffs
		Microsoft::Azure::Gaming::GSDK::start();
		Microsoft::Azure::Gaming::GSDK::registerShutdownCallback([this]() { OnShutdownRequested(); });
		Microsoft::Azure::Gaming::GSDK::registerHealthCallback([this]() { return IsAlive(); });
		Microsoft::Azure::Gaming::GSDK::registerMaintenanceCallback([this](tm t) { OnMaintenanceScheduled(t); });

	Proc_End:

		return hr;
	}

	void RelayServer::OnShutdownRequested()
	{
		StopServer();
	}

	bool RelayServer::IsAlive()
	{
		return GetServerState() != Svr::ServerState::STOPED;
	}

	void RelayServer::OnMaintenanceScheduled(tm t)
	{
		// TODO:
		//nextMaintenance = mktime(&t);
		//isMaintenancedScheduled = true;
	}


	// Initialize server resource
	Result RelayServer::InitializeServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::InitializeServerResource() );

		svrChk( InitializeEntity( EntityID(EntityFaculty::Server,0) ) );


	Proc_End:

		return hr;
	}

	// Close server and release resource
	Result RelayServer::CloseServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::CloseServerResource() );

		svrChk( TerminateEntity() );

	Proc_End:

		return hr;
	}

	// Initialize private Network
	Result RelayServer::InitializeNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::InitializeNetPrivate() );


	Proc_End:

		return hr;
	}


	// Close Private Network
	Result RelayServer::CloseNetPrivate()
	{
		Result hr = ResultCode::SUCCESS;

		hr = Svr::BrServer::CloseNetPrivate();

		return hr;
	}


	// create remote entity by class
	Result RelayServer::CreateServerEntity( NetClass netClass, Svr::ServerEntity* &pServerEntity )
	{
		pServerEntity = new(GetHeap()) Svr::GenericServerEntity();

		if( pServerEntity == nullptr )
			return ResultCode::OUT_OF_MEMORY;

		return ResultCode::SUCCESS;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	virtual Entity interface
	//


}; // namespace SF







