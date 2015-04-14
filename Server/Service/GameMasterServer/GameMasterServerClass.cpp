////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConfig.h"
#include "ServerSystem/ServerEntityManager.h"
#include "Common/TimeUtil.h"


#include "GameMasterSvrConst.h"
#include "GameMasterServerClass.h"


#include "ServerEntity/GameServerEntity.h"
#include "ServerEntity/EntityServerEntity.h"
#include "ServerEntity/GameMasterServerEntity.h"

#include "Protocol/Policy/EntityServerIPolicy.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"

#include "Net/NetServer.h"
#include "Net/NetServerPeer.h"

#include "PlayerInfoManager.h"

#include "GameMasterTableSystem.h"


namespace BR {
namespace GameMasterServer {


	// Get Entity Server
	EntityServerEntity* GetEntityServer()
	{
		EntityServerEntity *pEntityServer = nullptr;
		Svr::ServerEntity *pServerEntity = nullptr;
		Svr::ServerEntityManager *pRemoteMgr = Svr::BrServer::GetInstance()->GetComponent<Svr::ServerEntityManager>();

		if( pRemoteMgr 
			&& SUCCEEDED(pRemoteMgr->GetServerEntity( BR::Net::NetClass_Entity, pServerEntity ) ) )
		{
			pEntityServer = dynamic_cast<EntityServerEntity*>(pServerEntity);
		}

		return pEntityServer;
	}



	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GameMasterServer::GameMasterServer()
		:BrServer(BR::Net::NetClass_GameMaster)
		,m_PlayerInfoMgr(nullptr)
	{
	}


	GameMasterServer::~GameMasterServer()
	{
	}

	// Apply configuration
	HRESULT GameMasterServer::ApplyConfiguration()
	{
		HRESULT hr = S_OK;

		const Svr::Config::GenericServer *pMyConfig = nullptr;

		const Svr::Config::GameCluster* pCluster = Svr::Config::GetConfig().GameCluster;
		svrChkPtr(pCluster);
		svrChkPtr( pCluster->ClusterMaster );
		pMyConfig = pCluster->ClusterMaster;
		svrAssert( pMyConfig );

		SetMyConfig( pMyConfig );

		svrChk(__super::ApplyConfiguration() );

	Proc_End:

		return hr;
	}


	// Initialize server resource
	HRESULT GameMasterServer::InitializeServerResource()
	{
		HRESULT hr = S_OK;

		svrChk( __super::InitializeServerResource() );

		svrChk( InitializeTable() );

		svrChk( InitializeEntity( EntityID(EntityFaculty::Server,0)) );

	Proc_End:

		return hr;
	}

	// Close server and release resource
	HRESULT GameMasterServer::CloseServerResource()
	{
		HRESULT hr = S_OK;

		svrChk( __super::CloseServerResource() );

		svrChk( TerminateEntity() );

	Proc_End:

		return hr;
	}


	// Initialize private Network
	HRESULT GameMasterServer::InitializeNetPrivate()
	{
		HRESULT hr = S_OK;
		Net::IConnection *pConn = nullptr;

		//Util::Time.UpdateTimer();

		svrChk( __super::InitializeNetPrivate() );

		//Connect to Entity Server
		const Svr::Config::GenericServer *pCfgEntity = Svr::Config::GetConfig().Entity;
		svrChkPtr(pCfgEntity->NetPrivate);
		svrChk( GetNetPrivate()->RegisterServer( pCfgEntity->NetPrivate->IP.c_str(), pCfgEntity->NetPrivate->Port, pConn ) );

		// Initialize plugins
		svrMem( m_PlayerInfoMgr = new PlayerInfoManager );
		svrChk( m_PlayerInfoMgr->InitializeManager() );

	Proc_End:

		return hr;
	}

	// Close Private Network
	HRESULT GameMasterServer::CloseNetPrivate()
	{
		HRESULT hr = S_OK;

		hr = __super::CloseNetPrivate();

		// Add Plugin terminations
		if( m_PlayerInfoMgr )
		{
			m_PlayerInfoMgr->TerminateManager();
			Util::SafeDelete(m_PlayerInfoMgr);
		}

		return hr;
	}


	// create remote entity by class
	HRESULT GameMasterServer::CreateServerEntity( BR::Net::NetClass netClass, Svr::ServerEntity* &pServerEntity )
	{
		switch( netClass )
		{
		case BR::Net::NetClass_Game:
			pServerEntity = new GameServerEntity();
			break;
		case BR::Net::NetClass_Entity:
			pServerEntity = new EntityServerEntity();
			break;
		case BR::Net::NetClass_GameMaster:
			pServerEntity = new GameMasterServerEntity();
			break;
		default:
			break;
		};

		if( pServerEntity == nullptr )
			return E_OUTOFMEMORY;

		return S_OK;
	}





	//////////////////////////////////////////////////////////////////////////
	//
	//	virtual Entity interface
	//


	// Run Server task
	//  - Make new transaction from connection queue
	//  - status update for game
	//  - Process transaction
	HRESULT GameMasterServer::RunTask()
	{
		HRESULT hr = S_OK, hrTem = S_OK;

		svrChk( __super::RunTask() );

	Proc_End:


		return hr;
	}






}; // namespace GameMasterServer
}; // namespace BR







