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
		Result ServiceInstall( const char *strCfgPath, const char *strUser, const char *strPWD);

		// Uninstall service
		Result ServiceUninstall();

		// prepare service running
		Result ServicePrepare();

		// Run service main function
		Result ServiceRun(BR::Svr::BrServer *pSvrInstance );
	};



}; // namespace BR
}; // namespace Svr


