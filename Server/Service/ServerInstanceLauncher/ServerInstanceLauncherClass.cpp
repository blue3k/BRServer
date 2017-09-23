////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "SvrConst.h"
#include "Server/BrServer.h"
#include "SvrTrace.h"
#include "ServerConfig/SFServerConfig.h"
#include "Entity/EntityManager.h"
#include "ServerEntity/ServerEntityManager.h"
//#include "ServiceEntity/EntityManagerServiceEntity.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"
#include "Util/TimeUtil.h"

#include "ServerInstanceLauncherClass.h"



#define PATH_DATABASE "../../../Data"


namespace SF {
namespace ServerInstanceLauncher {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	ServerInstanceLauncher::ServerInstanceLauncher()
		: BrServer(NetClass::Server)
	{
	}


	ServerInstanceLauncher::~ServerInstanceLauncher()
	{
	}
	
	
	Svr::ServerEntity* ServerInstanceLauncher::CreateLoopbackEntity()
	{
		return new(GetMemoryManager()) Svr::ServerEntity;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Public Network event handling
	//

	bool ServerInstanceLauncher::OnStart()
	{
		Result hr = ResultCode::SUCCESS;

		SetServerState(Svr::ServerState::STARTING);

		svrTrace(Info, "Starting Server");


		svrChk(InitializeServerResource());

	Proc_End:


		if (!(hr))
		{
			// TODO
			Service::EntityManager->Clear();

			GetComponentCarrier().TerminateComponents();
			GetComponentCarrier().ClearComponents();

			Service::EntityManager->FlushDeletedEntity();

			SetServerState(Svr::ServerState::STOPED);
			svrTrace(Info, "Start failed hr:{0:X8}", hr);
		}
		else
		{
			svrTrace(Info, "Start process done");
		}

		return (hr);
	}

	// Apply configuration
	Result ServerInstanceLauncher::ApplyConfiguration()
	{
		Result hr = ResultCode::SUCCESS;

		//svrChk(Svr::BrServer::ApplyConfiguration() );

	Proc_End:

		return hr;
	}


	// Initialize server resource
	Result ServerInstanceLauncher::InitializeServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::InitializeServerResource() );

		svrChk( InitializeEntity( EntityID(EntityFaculty::Server,0)) );

	Proc_End:

		return hr;
	}

	// Close server and release resource
	Result ServerInstanceLauncher::CloseServerResource()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Svr::BrServer::CloseServerResource() );

		svrChk( TerminateEntity() );

	Proc_End:

		return hr;
	}



	Result ServerInstanceLauncher::TickUpdate(TimerAction *pAction)
	{
		

		return BrServer::TickUpdate(pAction);
	}

}; // namespace ServerInstanceLauncher
}; // namespace SF







