////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game master server implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/HRESSvrSys.h"
#include "GameMasterServer.h"
#include "ServerSystem/SvrTrace.h"
#include "GameMasterServerClass.h"
#include "Common/MemoryPool.h"
#include "GameMasterSvrConst.h"

#include "ServerEntity/GameServerEntity.h"

#include "Protocol/Message/GameMasterServerMsgClass.h"
#include "Protocol/Policy/GameMasterServerIPolicy.h"

#include "GameMasterServerClass.h"
#include "GameMasterTransGame.h"
#include "PlayerInfoManager.h"



BR_MEMORYPOOL_IMPLEMENT(GameMasterServer::TransS2SEvtPlayerEntered);
BR_MEMORYPOOL_IMPLEMENT(GameMasterServer::TransS2SEvtPlayerLeaved);



namespace BR {
namespace GameMasterServer {


	// Start Transaction
	HRESULT TransS2SEvtPlayerEntered::StartTransaction()
	{
		HRESULT hr = S_OK;
		PlayerInfoManager *pPCMgr = nullptr;

		svrChk( __super::StartTransaction() );

		svrChkPtr( pPCMgr = GetPlayerInfoMgr() );
		svrChk( pPCMgr->CreatePlayerInfo( GetMyOwner(), GetPlayerID(), GetPlayerName() ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	HRESULT TransS2SEvtPlayerLeaved::StartTransaction()
	{
		HRESULT hr = S_OK;
		PlayerInfoManager *pPCMgr = nullptr;

		svrChk( __super::StartTransaction() );

		svrChkPtr( pPCMgr = GetPlayerInfoMgr() );
		svrChk( pPCMgr->DeletePlayerInfoByPlayerID( GetPlayerID() ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}





};// namespace GameMasterServer 
};// namespace BR 

