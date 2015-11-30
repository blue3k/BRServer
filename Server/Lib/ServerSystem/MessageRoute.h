////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server Utility definition
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Net/NetDef.h"
#include "Common/BrAssert.h"
#include "Common/StaticHashTable.h"
#include "ServerSystem/SvrTypes.h"
#include "ServerSystem/SvrConfig.h"
#include "ServerSystem/ServerEntity.h"
#include "Common/SynchronizationTrait.h"


namespace BR {

	namespace Net {
		class IConnection;
	};

namespace Svr {

	class SimpleUserEntity;
	class Transaction;
	class TransactionResult;

	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Route message
	//

	//// Route Message utility func
	//template< class MsgClass >
	//HRESULT RouteMessage( Message::MessageData* &pMsg )
	//{
	//	HRESULT hr = S_OK;
	//	ServerEntity* pServerEntity = nullptr;
	//	MsgClass msg;

	//	svrChk( msg.ParseIMsg(pMsg) );

	//	svrChkErr( E_SVR_ROUTEMSG_FAILED, BrServer::GetInstance()->GetServerEntityManager()->GetServerEntity( msg.GetRouteContext().To, pServerEntity ) );

	//	svrChkPtr( pServerEntity->GetConnection() );
	//	svrChk( pServerEntity->GetConnection()->Send( pMsg ) );

	//Proc_End:

	//	Util::SafeRelease( pMsg );

	//	return hr;
	//}
	//
	




}; // namespace Svr
}; // namespace BR





