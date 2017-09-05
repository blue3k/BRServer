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


#include "SFTypedefs.h"
#include "Thread/Thread.h"
#include "Net/NetDef.h"
#include "ServerEntity/SvrEntityServerEntity.h"


namespace SF {
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


		virtual Result ProcessConnectionEvent( const Net::ConnectionEvent& conEvent ) override;

	};



} // namespace EntityServer
} // namespace SF




