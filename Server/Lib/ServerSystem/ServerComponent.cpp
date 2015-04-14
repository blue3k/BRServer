////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Server Component implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "StdAfx.h"
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "Net/NetDef.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServicebase.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/SvrTrace.h"
#include "Common/BrRandom.h"

namespace BR {
namespace Svr {

	int ServerComponentDummy;

	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	////
	////	ReplicationClusterServiceComponent base class
	////

	//ClusterServiceComponentBase::ClusterServiceComponentBase()
	//{
	//}

	//ClusterServiceComponentBase::~ClusterServiceComponentBase()
	//{
	//	std::for_each( m_Servers.begin(), m_Servers.end(), []( ServerServiceBase* pService )
	//	{
	//		Util::SafeDelete(pService);
	//	});
	//	m_Servers.clear();
	//}

	//// Get a service by random
	//HRESULT ClusterServiceComponentBase::GetRandomService( ServerServiceBase* &service )
	//{
	//	HRESULT hr = S_OK;

	//	if( m_Servers.size() == 0 )
	//	{
	//		service = nullptr;
	//		return E_FAIL;
	//	}
	//	else if( m_Servers.size() == 1 )
	//	{
	//		service = GetMaster();
	//		return hr;
	//	}

	//	INT selected = Util::Random.Rand(0, (INT)m_Servers.size());
	//	service = m_Servers[selected];

	//Proc_End:

	//	return hr;

	//}
	//
	//// Get service by random except master
	//HRESULT ClusterServiceComponentBase::GetRandomServiceExceptMaster( ServerServiceBase* &service )
	//{
	//	HRESULT hr = S_OK;

	//	if( m_Servers.size() == 0 )
	//	{
	//		service = nullptr;
	//		return E_FAIL;
	//	}
	//	else if( m_Servers.size() == 1 )
	//	{
	//		service = GetMaster();
	//		return hr;
	//	}

	//	service = nullptr;

	//	INT selected = Util::Random.Rand(0, (INT)m_Servers.size());
	//	for( int iTry = 0; iTry < (INT)m_Servers.size(); iTry++, selected++ )
	//	{
	//		if( selected >= m_Servers.size() ) selected = 0;

	//		if( m_Servers[selected] != m_Master 
	//			&& m_Servers[selected]->GetServerEntity()->GetConnection()->GetState() == Net::IConnection::STATE_CONNECTED )
	//		{
	//			service = m_Servers[selected];
	//		}
	//	}

	//	if( service == nullptr )
	//		return E_SVR_CLUSTER_NOTREADY;

	//	return hr;
	//}
	//
	//// Update Master and select new one if needed
	//HRESULT ClusterServiceComponentBase::CheckService()
	//{
	//	HRESULT hr = S_OK;

	//	if( m_Master == nullptr 
	//		&& m_Master->GetServerEntity()->GetConnection() == nullptr 
	//		&& m_Master->GetServerEntity()->GetConnection()->GetState() != Net::IConnection::STATE_CONNECTED )
	//		return E_SVR_CLUSTER_NOTREADY;

	//Proc_End:

	//	return hr;
	//}
	//
	//// Add service entity
	//HRESULT ClusterServiceComponentBase::AddService( ServerServiceBase* pServerService )
	//{
	//	HRESULT hr = S_OK;

	//	svrChkPtr(pServerService);

	//	m_Servers.push_back( pServerService );

	//Proc_End:

	//	return hr;
	//}

	//// Set master service entity
	//HRESULT ClusterServiceComponentBase::SetMasterService( EntityUID entityUID )
	//{
	//	HRESULT hr = S_OK;

	//	for( auto itSvr = m_Servers.begin(); itSvr != m_Servers.end(); ++itSvr )
	//	{
	//		if( (*itSvr)->GetServiceEntityUID() == entityUID )
	//		{
	//			m_Master = *itSvr;
	//			return S_OK;
	//		}
	//	}

	//	return E_SVR_INVALID_ENTITYUID;
	//}


	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	////
	////	ReplicationClusterServiceComponent base class
	////

	//// Get service, default implementation is return null
	//ServerServiceBase* ReplicationClusterServiceComponent::GetService( UINT64 selectorKey )
	//{
	//	ServerServiceBase *pService = nullptr;

	//	switch( selectorKey )
	//	{
	//	case AccessMode_Write:
	//		return GetMaster();
	//	case AccessMode_Read:
	//		GetRandomServiceExceptMaster(pService);
	//		return pService;
	//	case AccessMode_SynchronizedRead:
	//	default:
	//		return GetMaster();
	//	}
	//}

	//
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	////
	////	RingClusterServiceComponent base class
	////

	//// Get service, default implementation is return null
	//ServerServiceBase* RingClusterServiceComponent::GetService( UINT64 selectorKey )
	//{
	//	// Ring cluster don't need to send to the master
	//	ServerServiceBase *pService = nullptr;
	//	GetRandomService( pService );
	//	return pService;
	//}


}; // namespace Svr {
}; // namespace BR {



