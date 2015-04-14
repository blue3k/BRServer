////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server constants class implementation. 
//
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrConstDefault.h"


namespace BR {
namespace Svr {
namespace Const {


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Category : Server system
	//

	INT		TRANSACTION_TIMEOUT			= Default::TRANSACTION_TIMEOUT;
	INT		SVRMAINENTITY_RESPROC_MAX	= Default::SVRMAINENTITY_RESPROC_MAX;
	INT		PLUGIN_ENTITYID_BASE		= Default::PLUGIN_ENTITYID_BASE;


	INT		ENTITY_REMOTE_TRANS_QUEUE		= Default::ENTITY_REMOTE_TRANS_QUEUE;
	INT		ENTITY_REMOTE_TRANSRES_QUEUE	= Default::ENTITY_REMOTE_TRANSRES_QUEUE;

	INT		LOGIN_SESSION_EXPIRE_TIME	= Default::LOGIN_SESSION_EXPIRE_TIME;



}; // namespace Const
}; // namespace Svr
}; // namespace BR
