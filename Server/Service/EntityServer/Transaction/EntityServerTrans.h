////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Transaction/Transaction.h"
#include "Memory/MemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "EntityServerClass.h"


namespace SF {
namespace EntityServer {

	
	class EntityServerStartProcess : public Svr::TransactionT<EntityServer>
	{
	private:
		typedef Svr::TransactionT<EntityServer> super;
		
		enum class StartingStep 
		{
			WaitEntityServer,
			WaitInitializeComponents,
			Done
		};

		StartingStep m_Step;

		// wait count for entity servers
		uint m_WaitCount;

	public:
		EntityServerStartProcess();
		~EntityServerStartProcess();

		// Override delete function
		virtual void Release();

		Result OnTimer(Svr::TransactionResult* pRes);

		Result InitializeServices();

		//virtual Result OnCloseTransaction( Result hrRes ) override;

		// Start Transaction
		virtual Result StartTransaction();
	};



};// namespace EntityServer 
};// namespace SF 

