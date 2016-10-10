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
	Result GameInstancePlayer::InitializePlayer( GameInstanceEntity* pGameOwner, bool isBot )
	{
		Result hr = ResultCode::SUCCESS;

		// disable, not implemented yet
		svrChkPtr( m_GameOwner = pGameOwner );

		m_IsBot = isBot;

	Proc_End:

		return hr;
	}



	// Update Game Player 
	Result GameInstancePlayer::UpdateGamePlayer( TimeStampMS ulCurTime )
	{
		Result hr = ResultCode::SUCCESS;

	//Proc_End:

		return hr;
	}



}; // ConspiracyGameInstanceServer
}; // namespace BR


