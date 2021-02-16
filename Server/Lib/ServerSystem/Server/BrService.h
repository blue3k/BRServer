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
#include "MemoryManager/SFMemory.h"
#include "SFAssert.h"
#include "Util/SFUtility.h"
#include "String/SFStrUtil.h"
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
		Result ServiceInstall();

		// Uninstall service
		Result ServiceUninstall();

		// prepare service running
		Result ServicePrepare();

		// Run service main function
		Result ServiceRun(SF::Svr::BrServer *pSvrInstance );
	};



}; // namespace SF
}; // namespace Svr


