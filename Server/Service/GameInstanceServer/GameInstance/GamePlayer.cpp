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
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServerUtil.h"
#include "Net/NetServerUDP.h"
#include "Common/TimeUtil.h"
#include "Common/BrBaseTypes.h"
#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeGame.h"


#include "ConspiracyGameInstanceSvrConst.h"
#include "ConspiracyGameInstanceServerClass.h"

#include "Protocol/Policy/GameServerIPolicy.h"
#include "Protocol/Policy/GameIPolicy.h"


#include "GamePlayer.h"
#include "GameInstanceEntity.h"
#include "GameSystem/GamePlaySystem.h"
#include "GameSystem/GameStateSystem.h"



BR_MEMORYPOOL_IMPLEMENT(ConspiracyGameInstanceServer::GamePlayer);




namespace BR {
namespace ConspiracyGameInstanceServer {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity server class
	//


	GamePlayer::GamePlayer(GameInstanceEntity* pGameOwner, const PlayerInformation& player)
		: super(pGameOwner, player)
		, m_Index(0)
		, m_Character(0)
		, m_PlayerState(PlayerState::None)
		, m_RevealedBySeer(false)
		, m_RequestedRole(PlayerRole::None)
		, m_Voted(0)
		, m_Vote(0)
	{
	}

	GamePlayer::~GamePlayer()
	{
	}


	// Initialize player
	HRESULT GamePlayer::InitializePlayer( GameInstanceEntity* pGameOwner, bool isBot )
	{
		HRESULT hr = S_SYSTEM_OK;

		// disable, not implemented yet
		svrChk(super::InitializePlayer(pGameOwner, isBot));

		SetRole( PlayerRole::None );

		m_RevealedBySeer = false;
		m_ReviveCount = 0;


	Proc_End:

		return hr;
	}



	// Update Game Player 
	HRESULT GamePlayer::UpdateGamePlayer( TimeStampMS ulCurTime )
	{
		HRESULT hr = S_SYSTEM_OK;
		auto gameStateSystem = GetGameOwner()->GetComponent<GameStateSystem>();

		svrChk(super::UpdateGamePlayer(ulCurTime));

		svrChkPtr(gameStateSystem);

		if (GetIsBot())
		{
			switch (gameStateSystem->GetCurrentGameState())
			{
			case GameStateID::FreeDebate:
			case GameStateID::MorningDebate:
			case GameStateID::DefenceOfSuspects:
				// random talk?
				break;
				// random vote
			case GameStateID::NightVote:
			case GameStateID::VoteForSuspects:
			case GameStateID::VoteForHanging:
				//gameStateSystem->Vote()
				break;
			default:
				break;
			}
		}


	Proc_End:

		return hr;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	
	//

	bool GamePlayer::IsWinnerSide(GameWinner winner)
	{
		switch( GetRole() )
		{
		default:
		case PlayerRole::None:
			return false;
		case PlayerRole::Villager:
		case PlayerRole::Seer:
		case PlayerRole::Medium:
		//case PlayerRole::Bodyguard:
		//case PlayerRole::Owlman:
			return winner == GameWinner::Villager;
		case PlayerRole::Werewolf:
		//case PlayerRole::Possessed:
			return winner == GameWinner::Werewolf;
		}
	}



}; // ConspiracyGameInstanceServer
}; // namespace BR


