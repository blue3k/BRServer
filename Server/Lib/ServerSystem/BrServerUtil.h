////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server Utility definition
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/StrUtil.h"
#include "Net/Netdef.h"
#include "Net/NetServer.h"
//#include "ServerSystem/PlugIn.h"
#include "ServerSystem/Transaction.h"
#include "ServerSystem/SvrTypes.h"
#include "ServerSystem/TaskManager.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/EntityTable.h"
#include "ServerSystem/SvrConfig.h"
#include "ServerSystem/BrServer.h"




namespace BR {
namespace SvrUtil
{
	class SimpleUserEntity;

	
	//////////////////////////////////////////////////////////////////////////
	//
	//	Utility helper function
	//


	// Get Server UID
	inline UINT GetServerUID();

	// Make Route Context
	inline RouteContext Route( UINT SvrTo = 0 );


#include "BRServerUtil.inl"


}; // namespace SvrUtil
}; // namespace BR





