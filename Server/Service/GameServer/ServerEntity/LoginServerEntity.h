////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Entity server entity definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"
#include "Common/Thread.h"
#include "Net/NetDef.h"
#include "ServerSystem/ServerEntity.h"


namespace BR {
namespace GameServer
{


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity server entity entity class
	//

	class LoginServerEntity : public Svr::ServerEntity
	{
	private:

	protected:

	public:
		LoginServerEntity();
		virtual ~LoginServerEntity();


		// Initialize entity to proceed new connection
		HRESULT InitializeEntity( EntityID newEntityID );

	};




}; // namespace GameServer
}; // namespace BR




