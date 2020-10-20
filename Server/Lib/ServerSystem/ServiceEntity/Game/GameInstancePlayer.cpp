////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game player
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "Net/SFNetServerUDP.h"
#include "Util/SFTimeUtil.h"
#include "Types/SFEngineTypedefs.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameNetPolicy.h"

#include "SvrConst.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"
#include "ServiceEntity/Game/GameInstancePlayer.h"
#include "ServiceEntity/Game/GameInstanceEntity.h"



namespace SF {
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
		Result GameInstancePlayer::InitializePlayer(GameInstanceEntity* pGameOwner, bool isBot)
		{
			Result hr = ResultCode::SUCCESS;

			// disable, not implemented yet
			svrCheckPtr(m_GameOwner = pGameOwner);

			m_IsBot = isBot;

			return hr;
		}



		// Update Game Player 
		Result GameInstancePlayer::UpdateGamePlayer(TimeStampMS ulCurTime)
		{
			Result hr = ResultCode::SUCCESS;

			return hr;
		}

	} // Svr
} // namespace SF

