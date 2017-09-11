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

	public:
		ServerServiceBase( ServerServiceInformation* pService )
			:m_ServerServiceInformation(pService)
		{
			Assert(m_ServerServiceInformation);
		}

		ServerServiceInformation* GetServerServiceInformation() { return m_ServerServiceInformation; }

		EntityUID GetServiceEntityUID() { return m_ServerServiceInformation->GetEntityUID(); }

		FORCEINLINE const SharedPointerT<Net::Connection>& GetConnection() { return m_ServerServiceInformation->GetConnection(); }

	};



}; // namespace Svr
}; // namespace SF




