////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Monitoring entity implementation
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
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServiceEntity/MonitoringServiceEntity.h"
#include "ServerSystem/ServiceEntity/MonitoringServiceTrans.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/PerformanceCounter/PerformanceCounterServer.h"


namespace BR {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	MonitoringServiceEntity::MonitoringServiceEntity()
		: ServiceEntity()
	{
		SetTickInterval(DurationMS(10000));

		BR_ENTITY_MESSAGE(Message::Monitoring::GetInstanceListCmd) { svrMemReturn(pNewTrans = new MonitoringTransGetInstanceList(pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::Monitoring::RequestCounterValuesCmd) { svrMemReturn(pNewTrans = new MonitoringTransRequestCounterValues(pMsgData)); return ResultCode::SUCCESS; } );
	}

	MonitoringServiceEntity::~MonitoringServiceEntity()
	{
	}

	Result MonitoringServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		Result hr = ResultCode::SUCCESS;
		Svr::Config::NetSocket *netAddress = nullptr;
		NetAddress localAddr;

		svrChk(ServiceEntity::InitializeEntity(newEntityID) );

		svrChkPtr(Svr::Config::GetConfig().MonitoringServer);
		svrChkPtr(Svr::Config::GetConfig().MonitoringServer->NetPrivate);
		
		netAddress = Svr::Config::GetConfig().MonitoringServer->NetPrivate;

		svrChk(Net::SetLocalNetAddress(localAddr, netAddress->IP.c_str(), netAddress->Port));

		svrChk(PerformanceCounterServer::Initialize(localAddr));

	Proc_End:

		return hr;
	}

	// clear transaction
	Result MonitoringServiceEntity::ClearEntity()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(ServiceEntity::ClearEntity() );

		svrChk(PerformanceCounterServer::Terminate());

	Proc_End:

		return hr;
	}

	Result MonitoringServiceEntity::RegisterServiceMessageHandler(ServerEntity *pServerEntity)
	{
		//Result hr = ResultCode::SUCCESS;

		ServiceEntity::RegisterServiceMessageHandler(pServerEntity);

		pServerEntity->BR_ENTITY_MESSAGE(Message::Monitoring::GetInstanceListCmd)				{ svrMemReturn(pNewTrans = new MonitoringTransGetInstanceList(pMsgData)); return ResultCode::SUCCESS; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::Monitoring::RequestCounterValuesCmd)			{ svrMemReturn(pNewTrans = new MonitoringTransRequestCounterValues(pMsgData)); return ResultCode::SUCCESS; } );

		return ResultCode::SUCCESS;
	}

	Result MonitoringServiceEntity::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(ServiceEntity::TickUpdate(pAction) );

		// check below only if we are working
		if( GetEntityState() != EntityState::WORKING )
			goto Proc_End;

		if( BrServer::GetInstance()->GetServerState() != ServerState::RUNNING )
			goto Proc_End;

	Proc_End:

		return hr;
	}



}; // namespace Svr {
}; // namespace BR {



