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
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/SvrConfig.h"
#include "Util/TimeUtil.h"
#include "Net/NetServerUDP.h"

#include "GameSvrConst.h"
#include "GameServerClass.h"
#include "GameInstance/GamePlayerEntity.h"
#include "GameInstance/GameEntityManager.h"

//#include "ServerEntity/GameMasterServerEntity.h"

#include "Protocol/Policy/EntityServerIPolicy.h"
#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"



namespace BR {
namespace GameServer {



	//////////////////////////////////////////////////////////////////////////
	//
	//	virtual network process
	//
	
	

	// Process Private network event
	Result GameServer::ProcessPublicNetworkEvent()
	{
		Result hr = ResultCode::SUCCESS;
		Net::INet::Event curEvent;
		//GamePlayerEntity *pGamePlayerEntity = nullptr;
		//Net::Connection *pConn = nullptr;

		if( m_pNetPublic == nullptr )
			return ResultCode::SUCCESS;

		while( (m_pNetPublic->DequeueNetEvent( curEvent )) )
		{
			//pConn = nullptr;

			switch( curEvent.EventType )
			{
			case Net::INet::Event::EVT_NET_INITIALIZED:
				break;
			case Net::INet::Event::EVT_NET_CLOSED:
				break;
			case Net::INet::Event::EVT_NEW_CONNECTION:
				if( curEvent.EventConnection == nullptr )
					break;
				// Only registered connection can be made through login server
				//pConn = BR_DYNAMIC_CAST(Net::Connection*,curEvent.EventConnection);

				//svrChkPtr( pConn );
				//svrChkPtr( Svr::GetServerComponent<GameEntityManager>() );

				//svrChk( Svr::GetServerComponent<GameEntityManager>()->CreateGamePlayer(pGamePlayerEntity) );

				//if( !(pGamePlayerEntity->SetConnection( pConn )) )
				//{
				//	// NOTE: We need to mark to close this
				//	pGamePlayerEntity->ClearEntity();
				//}

				//pGamePlayerEntity = nullptr;

				break;
			case Net::INet::Event::EVT_CONNECTION_DISCONNECTED:
				break;
			default:
				break;
			};
		}


//Proc_End:

		//if( pConn && m_pNetPublic )
		//	m_pNetPublic->GetConnectionManager().PendingDisconnectNReleaseConnection( pConn );

		//Util::SafeDelete( pGamePlayerEntity );

		return hr;
	}

		
}; // namespace GameServer
}; // namespace BR







