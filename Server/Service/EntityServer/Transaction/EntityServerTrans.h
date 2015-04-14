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

#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "Common/MemoryPool.h"
#include "Common/BRBaseTypes.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameIPolicy.h"
#include "EntityServerClass.h"


namespace BR {
namespace EntityServer {

	
	class EntityServerStartProcess : public Svr::TransactionT<EntityServer,EntityServerStartProcess>
	{
	private:
		
		enum class StartingStep 
		{
			WaitEntityServer,
			WaitInitializeComponents,
			Done
		};

		StartingStep m_Step;

		// wait count for entity servers
		UINT m_WaitCount;

	public:
		EntityServerStartProcess();
		~EntityServerStartProcess();

		// Override delete function
		virtual void Release();

		HRESULT OnTimer(Svr::TransactionResult* pRes);

		HRESULT InitializeServices();

		//virtual HRESULT OnCloseTransaction( HRESULT hrRes ) override;

		// Start Transaction
		virtual HRESULT StartTransaction();
	};



};// namespace EntityServer 
};// namespace BR 

