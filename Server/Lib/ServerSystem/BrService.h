////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server service definitions. 
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/Memory.h"
#include "Common/BrAssert.h"
#include "Common/Utility.h"
#include "Common/StrUtil.h"
#include "ServerSystem/BrServer.h"



namespace BR {
namespace Svr
{

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Service control
	//

	namespace Service
	{
		// Install Service
		HRESULT ServiceInstall( const char *strCfgPath, const char *strUser, const char *strPWD);

		// Uninstall service
		HRESULT ServiceUninstall();

		// prepare service running
		HRESULT ServicePrepare();

		// Run service main function
		HRESULT ServiceRun(std::vector<std::string>& cmdArgs, BR::Svr::BrServer *pSvrInstance );
	};



}; // namespace BR
}; // namespace Svr


