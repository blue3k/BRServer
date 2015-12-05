////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Net server base type inline
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/HRESNet.h"
#include "Net/NetSvrDef.h"



namespace BR {
namespace Net {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Server network base interface
	//

	ServerNet::ServerNet( ServerID InServerID, NetClass localClass )
		: m_socket(INVALID_SOCKET)
		, m_LocalClass(localClass)
		, m_NetEventQueue(5 * 1024)
		, m_ServerID(InServerID)
		, m_IsEnableAccept(false)
	{
		memset( &m_LocalAddr, 0, sizeof(m_LocalAddr) );
		memset( &m_LocalSockAddr, 0, sizeof(m_LocalSockAddr) );
	}

	ServerNet::~ServerNet()
	{
	}
	

	// change status of new connection allowance 
	void ServerNet::SetIsEnableAccept(bool bIsEnable)
	{
		m_IsEnableAccept = bIsEnable;
	}

	// Add network event to queue
	HRESULT ServerNet::EnqueueNetEvent( INet::Event& curEvent )
	{
		return m_NetEventQueue.Enqueue( curEvent );
	}

	// Query Network event
	HRESULT ServerNet::DequeueNetEvent( Event& curEvent )
	{
		return m_NetEventQueue.Dequeue( curEvent );
	}
	
} // namespace Net
} // namespace BR


