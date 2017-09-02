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
#include "SFAssert.h"
#include "Types/BrSvrTypes.h"
#include "ServerSystem/SvrConfig.h"
#include "ServerSystem/ServerEntity.h"
#include "Common/SynchronizationTrait.h"


namespace SF {

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
	//Result RouteMessage( Message::MessageData* &pMsg )
	//{
	//	Result hr = ResultCode::SUCCESS;
	//	ServerEntity* pServerEntity = nullptr;
	//	MsgClass msg;

	//	svrChk( msg.ParseMessage(pMsg) );

	//	svrChkErr( ResultCode::E_ROUTEMSG_FAILED, BrServer::GetInstance()->GetServerEntityManager()->GetServerEntity( msg.GetRouteContext().To, pServerEntity ) );

	//	svrChkPtr( pServerEntity->GetConnection() );
	//	svrChk( pServerEntity->GetConnection()->Send( pMsg ) );

	//Proc_End:

	//	Util::SafeRelease( pMsg );

	//	return hr;
	//}
	//
	




}; // namespace Svr
}; // namespace SF





