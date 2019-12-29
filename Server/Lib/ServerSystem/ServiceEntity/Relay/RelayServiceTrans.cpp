////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2019 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "Memory/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"

#include "Protocol/Message/GameInstanceManagerMsgClass.h"


#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/Relay/RelayServiceTrans.h"
#include "ServiceEntity/Relay/RelayServiceEntity.h"



SF_MEMORYPOOL_IMPLEMENT(SF::Svr::RelayInstanceTrans);



namespace SF {
namespace Svr {



	RelayInstanceTrans::RelayInstanceTrans(IHeap& memoryManager)
		: TransactionT( memoryManager, TransactionID() )
	{
	}

	// Start Transaction
	Result RelayInstanceTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );


	Proc_End:

		if( !(hr) )
		{
			// We need to retry until it's successed
			CloseTransaction(hr);
		}

		return hr;
	}
	

};// namespace Svr 
};// namespace SF 

