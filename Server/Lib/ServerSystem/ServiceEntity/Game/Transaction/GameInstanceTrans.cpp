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

			svrCheck(GetMyOwner()->LeavePlayer(GetPlayerID()));

			return hr;
		}



		// Start Transaction
		Result GameEntityTransPlayerMovement::StartTransaction()
		{
			ScopeContext hr([this](Result hr)
				{
					CloseTransaction(hr);
				});

			svrCheck(super::StartTransaction());

			svrCheck(GetMyOwner()->NewPlayerMovement(GetPlayerID(), GetMovement()));

			return hr;
		}


}// namespace SF 

