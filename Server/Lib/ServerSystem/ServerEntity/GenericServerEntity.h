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
#include "Thread/SFThread.h"
#include "Net/SFNetDef.h"
#include "ServerEntity/ServerEntity.h"



namespace SF {

namespace Svr {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Game server entity entity class
	//

	class GenericServerEntity : public ServerEntity
	{
	private:
		
	protected:


	public:
		GenericServerEntity();
		virtual ~GenericServerEntity();

		// Initialize entity to proceed new connection
		Result InitializeEntity( EntityID newEntityID );

	};




}; // namespace Svr
}; // namespace SF




