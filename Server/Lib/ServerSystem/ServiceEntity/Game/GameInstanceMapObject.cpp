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

#include "SvrConst.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"
#include "ServiceEntity/Game/GameInstanceMapObject.h"
#include "ServiceEntity/Game/GameInstanceEntity.h"



namespace SF {
	namespace Svr {


		//////////////////////////////////////////////////////////////////////////
		//
		//	Entity server class
		//


		GameInstanceMapObject::GameInstanceMapObject(GameInstanceEntity* pGameOwner, StringCrc32 mapObjectId)
			: m_GameOwner(pGameOwner)
			, m_MapObjectId(mapObjectId)
			, m_Attributes(GetHeap())
		{
		}

		GameInstanceMapObject::~GameInstanceMapObject()
		{
		}

		IHeap& GameInstanceMapObject::GetHeap()
		{
			return m_GameOwner->GetHeap();
		}

		Result GameInstanceMapObject::SetOccupiedPlayer(PlayerID playerId)
		{
			if (m_OccupiedPlayerId != 0)
			{
				if (m_OccupiedPlayerId == playerId)
					return ResultCode::SUCCESS_FALSE;

				return ResultCode::MAPOBJECT_ALREADY_IN_USE;
			}

			m_OccupiedPlayerId = playerId;
			return ResultCode::SUCCESS;
		}

		Result GameInstanceMapObject::UseObject(GameInstancePlayer* pPlayer)
		{
			// TODO: test use conditions
			return ResultCode::NOT_IMPLEMENTED;
		}

	} // Svr
} // namespace SF

