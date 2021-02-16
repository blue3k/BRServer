////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Transaction/Transaction.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Util/Memento.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"



#include "conspiracy/ShopTbl.h"

#include "Transaction/MessageRoute.h"

#include "GameServerClass.h"

#include "GameInstance/GamePlayerEntity.h"
#include "Net/SFMessage.h"

#include "GameSystem/UserGamePlayerInfoSystem.h"


namespace SF {

	namespace DB{
		struct QueryGetPlayerInfoData;
	};

namespace GameServer {



	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//




} // namespace GameServer 
} // namespace SF 

