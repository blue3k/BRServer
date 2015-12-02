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
namespace LoginServer
{


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity server entity entity class
	//

	class GameServerEntity : public Svr::ServerEntity
	{
	private:
		typedef Svr::ServerEntity super;

		StaticAllocator< sizeof(Svr::EntityMessageHandlerItem)*30 > m_Allocator;

	protected:

		virtual MemoryAllocator& GetAllocator()			{ return m_Allocator; }

	public:
		GameServerEntity();
		virtual ~GameServerEntity();


		// Initialize entity to proceed new connection
		HRESULT InitializeEntity( EntityID newEntityID );

	};




}; // namespace LoginServer
}; // namespace BR




