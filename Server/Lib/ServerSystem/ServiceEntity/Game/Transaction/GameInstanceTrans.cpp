////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : game player implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "Util/SFRandom.h"
#include "GameConst.h"

#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerNetPolicy.h"

#include "GameInstanceTrans.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/Game/GameInstancePlayer.h"
#include "ServiceEntity/Game/GameInstanceEntity.h"
#include "ServiceEntity/Game/GameInstanceMapObject.h"


namespace SF {


		/////////////////////////////////////////////////////////////////////////////
		//
		//	Game command transaction
		//


		// Start Transaction
		Result GameEntityTransJoinGameInstance::StartTransaction()
		{
			ScopeContext hr([this](Result hr)
				{
					CloseTransaction(hr);
				});

			svrCheck(super::StartTransaction());

			m_Addr4 = GetMyOwner()->GetAddressIPV4();
			m_Addr6 = GetMyOwner()->GetAddressIPV6();
			m_TimeStamp = Util::Time.GetRawUTCSec();

			svrCheck(GetMyOwner()->AddPlayerToJoin(GetRouteContext().GetFrom(), GetPlayer(), GetCharacterVisual(), GetCharacterAttribute()));

			return hr;
		}



		// Start Transaction
		Result GameEntityTransLeaveGameInstance::StartTransaction()
		{
			ScopeContext hr([this](Result hr) 
				{
					CloseTransaction(hr);
				});

			svrCheck(super::StartTransaction());

			svrCheck(GetMyOwner()->PendingLeavePlayer(GetPlayerID()));

			return hr;
		}



		// Start Transaction
		Result GameInstanceTransPlayerMovement::StartTransaction()
		{
			ScopeContext hr([this](Result hr)
				{
					CloseTransaction(hr);
				});

			svrCheck(super::StartTransaction());

			svrCheck(GetMyOwner()->NewPlayerMovement(GetPlayerID(), GetMovement()));

			return hr;
		}



		Result GameInstanceTransOccupyMapObject::StartTransaction()
		{
			ScopeContext hr([this](Result hr)
				{
					CloseTransaction(hr);
				});

			svrCheck(super::StartTransaction());

			Svr::GameInstanceMapObject* pMapObject{};
			svrCheck(GetMyOwner()->GetMapObject(GetMapObjectId(), pMapObject));
			if (pMapObject->IsOccupied())
			{
				svrCheck(ResultCode::MAPOBJECT_ALREADY_IN_USE);
			}

			Svr::GameInstancePlayer* pGamePlayer{};
			svrCheck(GetMyOwner()->FindPlayer(GetPlayerID(), pGamePlayer));

			svrCheck(pGamePlayer->OccupyObject(pMapObject->GetMapObjectID()));
			auto hrTem = pMapObject->SetOccupiedPlayer(GetPlayerID());
			if (!hrTem)
			{
				if (pGamePlayer && pGamePlayer->GetOccupiedObjectId() == GetMapObjectId())
				{
					pGamePlayer->OccupyObject(nullptr);
				}
				if (pMapObject)
				{
					if (pMapObject->GetOccupiedPlayerID() == GetPlayerID())
						pMapObject->SetOccupiedPlayer(0);

					Svr::GameInstancePlayer* pOccupiedPlayer{};
					if (!GetMyOwner()->FindPlayer(pMapObject->GetOccupiedPlayerID(), pOccupiedPlayer))
					{
						// the player might be left or something. erase occupied info
						pMapObject->SetOccupiedPlayer(0);
					}
				}

				svrCheck(hrTem);
			}


			return hr;
		}

		Result GameInstanceTransUnoccupyMapObject::StartTransaction()
		{
			ScopeContext hr([this](Result hr)
				{
					CloseTransaction(hr);
				});

			svrCheck(super::StartTransaction());

			Svr::GameInstanceMapObject* pMapObject{};
			svrCheck(GetMyOwner()->GetMapObject(GetMapObjectId(), pMapObject));
			if (!pMapObject->IsOccupied())
			{
				svrCheck(ResultCode::MAPOBJECT_NOT_IN_USE);
			}

			Svr::GameInstancePlayer* pOccupiedPlayer{};
			if (pMapObject->GetOccupiedPlayerID() != GetPlayerID())
			{
				// validate occupant
				if (!GetMyOwner()->FindPlayer(pMapObject->GetOccupiedPlayerID(), pOccupiedPlayer))
				{
					// the player might be left or something. erase occupied info
					pMapObject->SetOccupiedPlayer(0);
				}

				svrCheck(ResultCode::MAPOBJECT_NOT_OCCUPIED_BY_PLAYER);
			}

			if (GetMyOwner()->FindPlayer(GetPlayerID(), pOccupiedPlayer))
			{
				pOccupiedPlayer->OccupyObject(nullptr);
			}

			pMapObject->SetOccupiedPlayer(0);

			return hr;
		}

		Result GameInstanceTransUseMapObject::StartTransaction()
		{
			ScopeContext hr([this](Result hr)
				{
					CloseTransaction(hr);
				});

			svrCheck(super::StartTransaction());

			Svr::GameInstanceMapObject* pMapObject{};
			svrCheck(GetMyOwner()->GetMapObject(GetMapObjectId(), pMapObject));

			Svr::GameInstancePlayer* pPlayer{};
			svrCheck(GetMyOwner()->FindPlayer(GetPlayerID(), pPlayer));

			svrCheck(pMapObject->UseObject(pPlayer));

			return hr;
		}

		Result GameInstanceTransHarvest::StartTransaction()
		{
			ScopeContext hr([this](Result hr)
				{
					CloseTransaction(hr);
				});

			svrCheck(super::StartTransaction());



			return hr;
		}


}// namespace SF 

