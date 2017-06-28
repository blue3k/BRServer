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
#include "ServerSystem/ServerEntity/EntityServerEntity.h"


namespace BR {
namespace EntityServer {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity server entity class
	//

	class EntityServerEntity : public Svr::EntityServerEntity
	{
	public:
		EntityServerEntity();
		virtual ~EntityServerEntity();


		virtual Result ProcessConnectionEvent( const BR::Net::IConnection::Event& conEvent ) override;

	};



} // namespace EntityServer
} // namespace BR




