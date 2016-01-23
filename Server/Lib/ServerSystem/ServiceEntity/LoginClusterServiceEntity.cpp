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
#include "Net/NetDef.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServiceEntity/LoginClusterServiceEntity.h"
#include "ServerSystem/SvrTrace.h"

#include "Protocol/Message/LoginServerMsgClass.h"
#include "ServerSystem/ServiceEntity/ClusterServiceTrans.h"


namespace BR {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	LoginClusterServiceEntity::LoginClusterServiceEntity(ClusterMembership initialMembership)
		: ReplicaClusterServiceEntity(ClusterID::LoginServers, initialMembership )
		, IServerComponent(ComponentID)
	{
	}
	
	LoginClusterServiceEntity::~LoginClusterServiceEntity()
	{
	}

	//HRESULT LoginClusterServiceEntity::InitializeEntity( EntityID newEntityID )
	//{
	//	HRESULT hr = S_SYSTEM_OK;

	//	svrChk( ReplicaClusterServiceEntity::InitializeEntity(newEntityID) );

	//Proc_End:

	//	return hr;
	//}


	//HRESULT LoginClusterServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	//{
	//	ReplicaClusterServiceEntity::RegisterServiceMessageHandler( pServerEntity );

	//	pServerEntity->BR_ENTITY_MESSAGE(Message::LoginServer::KickPlayerCmd)			{ svrMemReturn(pNewTrans = new LoginPlayerJoindToGameServerTrans(pMsgData)); return S_SYSTEM_OK; } );

	//	return S_SYSTEM_OK;
	//}


}; // namespace Svr {
}; // namespace BR {



