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


#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"


namespace BR {
namespace Svr {


	class ServerEntity;
	class ServerServiceInformation;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	IServerComponent base class
	//

	class ServerServiceBase
	{
	protected:

		BRCLASS_ATTRIBUTE_READONLY(ServerServiceInformation*,ServerServiceInformation);
		BRCLASS_ATTRIBUTE_READONLY(UINT,PolicyID);

	public:
		ServerServiceBase( ServerServiceInformation* pService, UINT policyID )
			:m_ServerServiceInformation(pService)
			,m_PolicyID(policyID)
		{
			Assert(m_ServerServiceInformation);
		}

		FORCEINLINE EntityUID GetServiceEntityUID();

		// Get Policy
		template< class PolicyType >
		FORCEINLINE PolicyType* GetPolicy() const
		{
			return m_ServerServiceInformation->GetPolicy<PolicyType>();
		}

	};




//#include "ServerComponent.inl"


}; // namespace Svr
}; // namespace BR




