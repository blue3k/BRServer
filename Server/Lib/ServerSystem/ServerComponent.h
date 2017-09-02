////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Component class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Memory/SFMemory.h"
#include "Component/BrComponent.h"
#include "ServerSystem/ServerServiceBase.h"
#include "ServerSystem/ServerComponentCarrier.h"


namespace SF {
namespace Svr {

	class Entity;
	class ServerEntity;
	class ServerServiceBase;


	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	IServerComponent base class
	//

	class IServerComponent : public Component
	{
	public:
		IServerComponent( UINT id ) : Component(id)
		{
		}

		virtual ~IServerComponent()
		{
		}
	};
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	IServerService Component base class
	//

	class IServerServiceComponent : public IServerComponent
	{
	public:
		IServerServiceComponent( UINT id ) : IServerComponent(id)
		{
		}

		virtual ~IServerServiceComponent()
		{
		}

		// Check server component
		virtual Result CheckService() { return ResultCode::SUCCESS; }

		
		// Get entity, default implementation is return null
		virtual ServerServiceBase* GetService( uint64_t serverSelector ) = 0;

		template< class ServerServiceType >
		ServerServiceType* GetService( uint64_t serverSelector )
		{
			ServerServiceBase *pService = GetService(serverSelector);
			Assert(pService != nullptr);
			//Assert(pService->GetServerEntity());
			if( pService == nullptr /*|| pService->GetServerEntity() == nullptr*/ )
				return nullptr;

			return dynamic_cast<ServerServiceType*>(pService);
		}

	};




	

#include "ServerComponent.inl"


}; // namespace Svr
}; // namespace SF




