////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Entity server entity
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"
#include "Common/Thread.h"
#include "Net/NetDef.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/MessageRoute.h"


namespace BR {

	namespace Policy
	{
		class IPolicyEntityServer;
	};

namespace Svr
{



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity server entity
	//

	class EntityServerEntity : public ServerEntity
	{
	public:

	private:

		StaticAllocator< sizeof(Svr::EntityMessageHandlerItem)*30 > m_Allocator;

	protected:

		virtual MemoryAllocator& GetAllocator()			{ return m_Allocator; }


	public:
		EntityServerEntity( UINT uiTransQueueSize = 0, UINT TransResQueueSize = 0);
		virtual ~EntityServerEntity();



		// Initialize entity to proceed new connection
		HRESULT InitializeEntity( EntityID newEntityID );


		// Process Connection event
		virtual HRESULT ProcessConnectionEvent( const BR::Net::IConnection::Event& conEvent );
	};




}; // namespace Svr
}; // namespace BR



