////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : KyungKun Ko
//
// Description : NetPolicy. 
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Net/NetPolicy.h"


namespace BR {
namespace Net {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Connection Interface
	//

	INetPolicy::INetPolicy(UINT uiPolicyID, BR::Net::IConnection *pConn)
		: IPolicyBase(uiPolicyID)
		, m_pConnection(pConn)
	{
		Assert(m_pConnection != nullptr);
	}

	INetPolicy::~INetPolicy()
	{
		Assert(m_pConnection == nullptr);
		//if (m_pConnection != nullptr)
		//	m_pConnection->m_pPolicy[GetPolicyID()] = nullptr;
	}

} // namespace Net
} // namespace BR

