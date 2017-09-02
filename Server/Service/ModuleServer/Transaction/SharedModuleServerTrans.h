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
#include "ServerSystem/ServerTransaction.h"
#include "Memory/MemoryPool.h"
#include "Types/BrBaseTypes.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "SharedModuleServerClass.h"


namespace SF {
namespace SharedModuleServer {

	
	class SharedModuleServerStartProcess : public Svr::TransactionT<SharedModuleServer,SharedModuleServerStartProcess>
	{
	private:
		typedef Svr::TransactionT<SharedModuleServer, SharedModuleServerStartProcess> super;

		enum class StartingStep 
		{
			WaitEntityServer,
			WaitInitializeComponents,
			Done
		};

		StartingStep m_Step;

	public:
		SharedModuleServerStartProcess();
		~SharedModuleServerStartProcess();

		// Override delete function
		virtual void Release();

		Result OnTimer(Svr::TransactionResult* pRes);

		Result InitializeServices();

		virtual Result OnCloseTransaction( Result hrRes ) override;

		// Start Transaction
		virtual Result StartTransaction();
	};





};// namespace SharedModuleServer 
};// namespace SF 

