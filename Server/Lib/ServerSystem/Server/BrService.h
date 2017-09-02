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

#include "SFTypedefs.h"
#include "Memory/SFMemory.h"
#include "SFAssert.h"
#include "Util/Utility.h"
#include "String/StrUtil.h"
#include "Server/BrServer.h"



namespace SF {
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
		Result ServiceRun(SF::Svr::BrServer *pSvrInstance );
	};



}; // namespace SF
}; // namespace Svr


