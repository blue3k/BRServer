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
#include "Transaction/ServerTransaction.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "Protocol/GameMsgClass.h"
#include "Protocol/GameNetPolicy.h"
#include "SharedModuleServerClass.h"


namespace SF {
	namespace SharedModuleServer {


		class SharedModuleServerStartProcess : public Svr::TransactionT<SharedModuleServer>
		{
		private:
			typedef Svr::TransactionT<SharedModuleServer> super;

			enum class StartingStep
			{
				StartInitialization,
				WaitInitializeComponents,
				Done
			};

			StartingStep m_Step;

		public:
			SharedModuleServerStartProcess(IHeap& heap);
			~SharedModuleServerStartProcess();

			// Override delete function
			virtual void Release();

			Result OnTimer(Svr::TransactionResult* pRes);

			Result InitializeServices();

			virtual Result OnCloseTransaction(Result hrRes) override;

			// Start Transaction
			virtual Result StartTransaction() override;
		};


	} // namespace SharedModuleServer 
} // namespace SF 

