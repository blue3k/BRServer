////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
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
#include "ServiceEntity/Game/GameInstancePlayerComponent.h"



namespace SF {
	namespace Svr {


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity server class
		//


		GameInstancePlayer::GameInstancePlayer(GameInstanceEntity* pGameOwner, EntityUID playerEntityUID, const PlayerInformation& player)
			: PlayerEntityInformation(pGameOwner->GetHeap(), playerEntityUID, player)
			, m_GameOwner(pGameOwner)
			, m_CharacterVisual(GetHeap())
			, m_CharacterAttribute(GetHeap())
			, m_ComponentManager(GetHeap())
			, m_IsBot(false)
		{
		}

		GameInstancePlayer::~GameInstancePlayer()
		{
			m_ComponentManager.TerminateComponents();
		}


		// Initialize player
		Result GameInstancePlayer::InitializePlayer(GameInstanceEntity* pGameOwner, bool isBot)
		{
			Result hr = ResultCode::SUCCESS;

			// disable, not implemented yet
			svrCheckPtr(m_GameOwner = pGameOwner);

			svrCheck(m_ComponentManager.AddComponent<GameInstancePlayerComponentMovement>(GetHeap(), this));

			m_ComponentManager.InitializeComponents();

			// cache component reference
			m_pMovementManager = m_ComponentManager.GetComponent<GameInstancePlayerComponentMovement>();

			m_IsBot = isBot;

			return hr;
		}

		Result GameInstancePlayer::SetCharacterData(const VariableTable& characterVisual, const VariableTable& characterAttribute)
		{
			m_CharacterVisual = characterVisual;
			m_CharacterAttribute = characterAttribute;

			return ResultCode::SUCCESS;
		}

		// Update Game Player 
		Result GameInstancePlayer::UpdateGamePlayer(TimeStampMS ulCurTime, uint32_t movementFrame)
		{
			Result hr = ResultCode::SUCCESS;

			m_CurTime = ulCurTime;
			m_CurMovementFrame = movementFrame;

			for (auto itComponent : m_ComponentManager)
			{
				auto pComponent = static_cast<GameInstancePlayerComponent*>(itComponent.GetValue());
				pComponent->TickUpdate();
			}

			return hr;
		}

	} // Svr
} // namespace SF

