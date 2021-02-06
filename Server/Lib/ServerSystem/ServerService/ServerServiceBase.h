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
#include "Entity/EntityInformation.h"


namespace SF {



	class ServerEntity;
	//class ServerServiceInformation;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	IServerComponent base class
	//

	class ServerServiceBase
	{
	protected:

		ServerServiceInformation* m_ServerServiceInformation;

	public:
		ServerServiceBase( ServerServiceInformation* pService )
			:m_ServerServiceInformation(pService)
		{
			Assert(m_ServerServiceInformation);
		}

		ServerServiceInformation* GetServerServiceInformation() { return m_ServerServiceInformation; }

		SF_FORCEINLINE EntityUID GetServiceEntityUID() { return m_ServerServiceInformation->GetEntityUID(); }

		// TODO: I may need to add new one
		SF_FORCEINLINE const SharedPointerT<MessageEndpoint>& GetTargetEndpoint() { return m_ServerServiceInformation->GetTargetEndpoint(); }

	};


} // namespace SF




