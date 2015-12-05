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
#include "Common/BrBaseTypes.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/GameIPolicy.h"

namespace BR {
namespace LoginServer {

	
	class LoginServerStartProcess : public Svr::TransactionT<LoginServer,LoginServerStartProcess>
	{
	private:
		typedef Svr::TransactionT<LoginServer, LoginServerStartProcess> super;
		
		enum class StartingStep 
		{
			WaitEntityServer,
			WaitInitializeComponents,
			Done
		};

		StartingStep m_Step;

	public:
		LoginServerStartProcess();
		~LoginServerStartProcess();
		
		// Override delete function
		virtual void Release();
		
		HRESULT OnTimer(Svr::TransactionResult* pRes);

		HRESULT InitializeServices();

		virtual HRESULT OnCloseTransaction( HRESULT hrRes ) override;

		// Start Transaction
		virtual HRESULT StartTransaction();

	};



};// namespace LoginServer 
};// namespace BR 

