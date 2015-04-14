////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server constants definitions. 
//	This constants used to initialize Constant Instance
//
////////////////////////////////////////////////////////////////////////////////

#pragma once




namespace BR{
namespace Svr{
namespace Const{
namespace Default { 

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Category : Server system
	//

	enum {
#if defined(DEBUG)
	TRANSACTION_TIMEOUT				= 1000*60,
#else
	TRANSACTION_TIMEOUT				= 1000*30,
#endif
		
	SVRMAINENTITY_RESPROC_MAX		= 500,

	PLUGIN_ENTITYID_BASE			= 900000,

	ENTITY_SIMPLE_TRANS_QUEUE		= 4,
	ENTITY_SIMPLE_TRANSRES_QUEUE	= 8,

	ENTITY_REMOTE_TRANS_QUEUE		= 1024,
	ENTITY_REMOTE_TRANSRES_QUEUE	= 1024,

	SERVER_TRANS_QUEUE		= 1024,
	SERVER_TRANSRES_QUEUE	= 1024,


	LOGIN_SESSION_EXPIRE_TIME		= 60*60*2, // 2 min
	};


}; // namespace Default 
}; // namespace Const
}; // namespace Svr
}; // namespace BR


