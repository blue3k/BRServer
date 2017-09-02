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
#include "ServerSystem/ServiceEntity/EntityInformation.h"


namespace SF {
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

		ServerServiceInformation* m_ServerServiceInformation;
		uint m_ProtocolID;

	public:
		ServerServiceBase( ServerServiceInformation* pService, uint policyID )
			:m_ServerServiceInformation(pService)
			, m_ProtocolID(policyID)
		{
			Assert(m_ServerServiceInformation);
		}

		ServerServiceInformation* GetServerServiceInformation() { return m_ServerServiceInformation; }
		uint GetProtocolID() { return m_ProtocolID; }

		EntityUID GetServiceEntityUID()
		{
			return m_ServerServiceInformation->GetEntityUID();
		}

		// Get Policy
		template< class PolicyType >
		FORCEINLINE PolicyType* GetPolicy() const
		{
			return m_ServerServiceInformation->GetPolicy<PolicyType>();
		}

	};



}; // namespace Svr
}; // namespace SF




