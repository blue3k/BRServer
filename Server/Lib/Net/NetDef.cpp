////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : KyungKun Ko
//
// Description : Network base type definitions. 
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/TimeUtil.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"


namespace BR {
namespace Net {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Connection Interface
	//

	IConnection::tag_Event IConnection::tag_Event::NullValue;

	template class SharedPointerT <IConnection>;


	//////////////////////////////////////////////////////////////////////////
	//
	//	basic implementation
	//

	// Constructor
	IConnection::IConnection()
		: m_CID(0)
		, m_UData(0)
		, m_ConnectionState(STATE_DISCONNECTED)
		, m_tConnectionTime(0)
		, m_pINet(nullptr)
		, m_pEventHandler(nullptr)
	{
		memset( &m_ConnectInfo, 0, sizeof(m_ConnectInfo) );
		memset( &m_pPolicy, 0, sizeof(m_pPolicy) );
	}

	IConnection::~IConnection()
	{
		for (int iPolicy = 0; iPolicy < _countof(m_pPolicy); iPolicy++)
		{
			if (m_pPolicy[iPolicy] != nullptr)
			{
				((INetPolicy*)m_pPolicy[iPolicy])->m_pConnection = nullptr;
			}
			Util::SafeDelete(m_pPolicy[iPolicy]);
		}
	}

	void IConnection::SetConnectionState(ConnectionState newState)
	{
		m_ConnectionState = newState;
	}

	void IConnection::UpdateConnectionTime()
	{
		m_tConnectionTime = Util::Time.GetTimeMs();
	}

	HRESULT IConnection::InitConnection(const NetAddress& Addr, NetClass netClass)
	{

		m_ConnectInfo.Local = Addr;
		m_ConnectInfo.LocalClass = netClass;
		m_ConnectInfo.Remote = Addr;
		m_ConnectInfo.RemoteClass = netClass;


		m_ConnectionState = STATE_CONNECTING;

		m_tConnectionTime = Util::Time.GetTimeMs();

		return S_OK;
	}



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Interface
	//

	INet::Event INet::Event::NullValue;


	INet::INet()
		:m_NetClass(NetClass::Unknown)
	{
	}

} // namespace Net
} // namespace BR

