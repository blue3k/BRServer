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
#include "ServerSystem/ServerEntity/EntityServerEntity.h"


namespace BR {
namespace SharedModuleServer
{


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity server entity entity class
	//

	class EntityServerEntity : public Svr::EntityServerEntity
	{
	private:

		StaticAllocator< sizeof(Svr::EntityMessageHandlerItem)*30 > m_Allocator;

	protected:

		virtual MemoryAllocator& GetAllocator()			{ return m_Allocator; }

	public:
		EntityServerEntity();
		virtual ~EntityServerEntity();

	};




}; // namespace SharedModuleServer
}; // namespace BR




