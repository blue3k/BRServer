////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Game master server implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "GameMasterServer.h"
#include "ServerSystem/SvrTrace.h"
#include "GameMasterServerClass.h"
#include "Common/MemoryPool.h"

#include "Protocol/Message/GameMasterServerMsgClass.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"


#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerEntity/GameServerEntity.h"


#include "Protocol/Policy/GameMasterServerIPolicy.h"
#include "Protocol/Policy/GameServerIPolicy.h"


#include "GameMasterTransServer.h"


BR_MEMORYPOOL_IMPLEMENT(GameMasterServer::TransServerStarted);


namespace BR {
namespace GameMasterServer {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	
	//


	// Start Transaction
	HRESULT TransServerStarted::StartTransaction()
	{
		HRESULT hr = S_OK;
		Svr::ServerEntity *pServerEntity = nullptr;

		svrChk( __super::StartTransaction() );

		GetMyOwner()->SetClusterID( GetClusterID() );

		// Update UID
		GetMyOwner()->SetEntityUID( EntityUID( GetSvrID(), GetMyOwner()->GetEntityID() ) );

		// check ab0ut already registered server
		if( SUCCEEDED( Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerEntity( GetSvrID(), pServerEntity )) )
		{
			if( pServerEntity != GetMyOwner() )
			{
				Net::IConnection *pAlreadyConn = pServerEntity->GetConnection();
				Net::IConnection *pMyConn = GetMyOwner()->GetConnection();
				NetAddress AlreadyAddr(0), MyAddr(0);
				if( pAlreadyConn )
					AlreadyAddr = pAlreadyConn->GetConnectionInfo().Remote;
				if( pMyConn )
					MyAddr = pMyConn->GetConnectionInfo().Remote;

				svrTrace( Trace::TRC_ASSERT, "Error, Duplicated ServerID %0% : Already from : %1%, New %2%", GetSvrID(), AlreadyAddr, MyAddr );
			}
			else
			{
				GetMyOwner()->SetPublicNetAddress( GetPublicAddress() );
			}
		}
		else
		{
			GetMyOwner()->SetPublicNetAddress( GetPublicAddress() );
		}

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


};// namespace GameMasterServer 
};// namespace BR 

