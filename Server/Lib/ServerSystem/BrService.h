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
		HRESULT ServiceInstall( const WCHAR *strCfgPath );

		// Uninstall service
		HRESULT ServiceUninstall();

		// Run service main function
		HRESULT ServiceRun( int argc, WCHAR* argv[], BR::Svr::BrServer *pSvrInstance );
	};



}; // namespace BR
}; // namespace Svr


