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



#include "StdAfx.h"
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "Common/Memory.h"
#include "Common/GameConst.h"
#include "Net/NetDef.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServicebase.h"
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
		SetTickInterval(10000);
	}

	MonitoringServiceEntity::~MonitoringServiceEntity()
	{
	}

	HRESULT MonitoringServiceEntity::InitializeEntity( EntityID newEntityID )
	{
		HRESULT hr = S_OK;
		Svr::Config::NetSocket *netAddress = nullptr;

		svrChk( __super::InitializeEntity(newEntityID) );

		svrChkPtr(Svr::Config::GetConfig().MonitoringServer);
		svrChkPtr(Svr::Config::GetConfig().MonitoringServer->NetPrivate);
		
		netAddress = Svr::Config::GetConfig().MonitoringServer->NetPrivate;

		svrChk(PerformanceCounterServer::Initialize(NetAddress(netAddress->IP.c_str(), netAddress->Port)));

	Proc_End:

		return hr;
	}

	// clear transaction
	HRESULT MonitoringServiceEntity::ClearEntity()
	{
		HRESULT hr = S_OK;

		svrChk( __super::ClearEntity() );

		svrChk(PerformanceCounterServer::Terminate());

	Proc_End:

		return hr;
	}

	HRESULT MonitoringServiceEntity::RegisterServiceMessageHandler(ServerEntity *pServerEntity)
	{
		HRESULT hr = S_OK;

		__super::RegisterServiceMessageHandler(pServerEntity);

		pServerEntity->BR_ENTITY_MESSAGE(Message::Monitoring::GetInstanceListCmd)				{ svrMemReturn(pNewTrans = new MonitoringTransGetInstanceList(pMsgData)); return S_OK; } );
		pServerEntity->BR_ENTITY_MESSAGE(Message::Monitoring::RequestCounterValuesCmd)			{ svrMemReturn(pNewTrans = new MonitoringTransRequestCounterValues(pMsgData)); return S_OK; } );

		return S_OK;

	Proc_End:

		return hr;
	}

	HRESULT MonitoringServiceEntity::TickUpdate(Svr::TimerAction *pAction)
	{
		HRESULT hr = S_OK;

		svrChk( __super::TickUpdate(pAction) );

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



