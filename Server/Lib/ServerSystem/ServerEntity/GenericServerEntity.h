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
		HRESULT InitializeEntity( EntityID newEntityID );

	};




}; // namespace Svr
}; // namespace BR




