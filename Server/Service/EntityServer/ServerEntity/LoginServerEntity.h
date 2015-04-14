////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game server entity
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"
#include "Common/Thread.h"
#include "Net/NetDef.h"
#include "ServerSystem/ServerEntity.h"


namespace BR {
namespace EntityServer {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game server entity entity class
	//

	class LoginServerEntity : public Svr::ServerEntity
	{
	public:
		LoginServerEntity();
		virtual ~LoginServerEntity();


		// Initialize entity to proceed new connection
		HRESULT InitializeEntity( EntityID newEntityID );

	};



} // namespace EntityServer
} // namespace BR




