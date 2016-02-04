////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game player
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Net/NetServerUDP.h"
#include "Common/TimeUtil.h"
#include "Common/BrBaseTypes.h"
#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeGame.h"


#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GameIPolicy.h"


#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/ServiceEntity/Game/GameInstancePlayer.h"
#include "ServerSystem/ServiceEntity/Game/GameInstanceEntity.h"







namespace BR {
namespace Svr {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GameInstancePlayer::GameInstancePlayer(GameInstanceEntity* pGameOwner, const PlayerInformation& player)
		: PlayerEntityInformation(player)
		, m_GameOwner(pGameOwner)
		, m_IsBot(false)
	{
	}

	GameInstancePlayer::~GameInstancePlayer()
	{
	}


	// Initialize player
	HRESULT GameInstancePlayer::InitializePlayer( GameInstanceEntity* pGameOwner, bool isBot )
	{
		HRESULT hr = S_SYSTEM_OK;

		// disable, not implemented yet
		svrChkPtr( m_GameOwner = pGameOwner );

		m_IsBot = isBot;

	Proc_End:

		return hr;
	}



	// Update Game Player 
	HRESULT GameInstancePlayer::UpdateGamePlayer( TimeStampMS ulCurTime )
	{
		HRESULT hr = S_SYSTEM_OK;

	Proc_End:

		return hr;
	}



}; // ConspiracyGameInstanceServer
}; // namespace BR


