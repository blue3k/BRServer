////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game master server
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"
#include "Common/Thread.h"
#include "Net/NetDef.h"
#include "ServerSystem/ServerEntity/GameMasterServerEntity.h"



namespace BR {
namespace EntityServer
{



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Zone server Zone class
	//

	class GameMasterServerEntity : public Svr::GameMasterServerEntity
	{
	private:

	public:
		GameMasterServerEntity();
		virtual ~GameMasterServerEntity();


		// Initialize Zone to proceed new connection
		HRESULT InitializeEntity( EntityID newEntityID );

	};



}; // namespace EntityServer
}; // namespace BR




