////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : KyungKun Ko
//
// Description : Server interface
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common/MemoryPool.h"
#include "Common/TimeUtil.h"
#include "Common/Trace.h"
#include "Common/BrLibComponents.h"
#include "Common/TraceComponent.h"
#include "Common/BrXML.h"
#include "Common/Utility.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/ParameterSetting.h"
#include "Common/ResultCode/BRResultCodeSvr.h"

#include "IBRServer.h"





namespace BR {

	namespace Svr {

		IBRServer::IBRServer()
		{

		}

		IBRServer::~IBRServer()
		{

		}


		Result IBRServer::StartServer()
		{
			Result hr = ResultCode::SUCCESS;

			auto pServer = BrServer::GetInstance();
			if (pServer == nullptr) return ResultCode::FAIL;
			if (pServer->GetServerState() != ServerState::STOPED) return ResultCode::E_INVALID_STATE;

			svrTrace(Trace::TRC_TRACE, "<{0}> Starting", Util::GetServiceNameA());

			svrChk(pServer->StartServer());

		Proc_End:

			return hr;
		}

		Result IBRServer::StopServer()
		{
			Result hr = ResultCode::SUCCESS;

			auto pServer = BrServer::GetInstance();
			if (pServer == nullptr) return ResultCode::FAIL;
			if (pServer->GetServerState() != ServerState::RUNNING) return ResultCode::E_INVALID_STATE;

			svrChk(pServer->StopServer());

			svrTrace(Trace::TRC_TRACE, "<{0}> Closed", Util::GetServiceNameA());

		Proc_End:

			return hr;
		}

	}
}
