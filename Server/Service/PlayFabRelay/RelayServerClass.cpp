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
#include "ServiceEntity/Party/GamePartyManagerServiceEntity.h"

#include "ServerEntity/GenericServerEntity.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"

#include "Net/SFNetServerPeerTCP.h"
#include "Net/SFNetSvrDef.h"
#include "Net/SFNetServerUDP.h"
#include "Entity/EntityManager.h"

#include "RelaySvrConst.h"
#include "RelayServerClass.h"

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
	}


	RelayServer::~RelayServer()
	{
	}



	Result RelayServer::SetupConfiguration()
	{
		// put default
		m_MyConfig.Name = Util::GetServiceName();
		m_MyConfig.UID = 1;

		SetMyConfig(&m_MyConfig);

		// handle gsdk config
		const std::unordered_map<std::string, std::string>& config = Microsoft::Azure::Gaming::GSDK::getConfigSettings();

		return ResultCode::SUCCESS;
	}

	// Apply configuration
	Result RelayServer::ApplyConfiguration()
	{
		FunctionContext hr;

		svrCheck(Svr::BrServer::ApplyConfiguration() );

		// Initializing PlayFab stuffs
		Microsoft::Azure::Gaming::GSDK::start();

		Microsoft::Azure::Gaming::GSDK::registerShutdownCallback([this]() { OnShutdownRequested(); });
		Microsoft::Azure::Gaming::GSDK::registerHealthCallback([this]() { return IsAlive(); });
		Microsoft::Azure::Gaming::GSDK::registerMaintenanceCallback([this](tm t) { OnMaintenanceScheduled(t); });

		return hr;
	}

	Result RelayServer::InitializeMonitoring()
	{
		// Skip monitoring setup
		return ResultCode::SUCCESS;
	}

	void RelayServer::OnShutdownRequested()
	{
		StopServer();
	}

	bool RelayServer::IsAlive()
	{
		svrTrace(Info, "IsAlive")
		return GetServerState() != Svr::ServerState::STOPED;
	}

	void RelayServer::OnMaintenanceScheduled(tm t)
	{
		// TODO:
		//nextMaintenance = mktime(&t);
		//isMaintenancedScheduled = true;
	}

	Result RelayServer::CreateEntityManager()
	{
		return ResultCode::SUCCESS;
	}

	Result RelayServer::CreateServerInstanceZK(const char* nodeName)
	{
		return ResultCode::SUCCESS;
	}

	// Initialize server resource
	Result RelayServer::InitializeServerResource()
	{
		FunctionContext hr;

		svrCheck(Svr::BrServer::InitializeServerResource() );

		svrCheck( InitializeEntity( EntityID(EntityFaculty::Server,0) ) );

		return hr;
	}

	// Close server and release resource
	Result RelayServer::CloseServerResource()
	{
		FunctionContext hr;

		svrCheck(Svr::BrServer::CloseServerResource() );

		svrCheck( TerminateEntity() );

		return hr;
	}

	// Initialize private Network
	Result RelayServer::InitializeNetPrivate()
	{
		FunctionContext hr;

		svrCheck(Svr::BrServer::InitializeNetPrivate() );

		svrTrace(Info, "readyForPlayers ");
		Microsoft::Azure::Gaming::GSDK::readyForPlayers();
		svrTrace(Info, "readyForPlayers end");

		return hr;
	}


	// Close Private Network
	Result RelayServer::CloseNetPrivate()
	{
		FunctionContext hr;

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







