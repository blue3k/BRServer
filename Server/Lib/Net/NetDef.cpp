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
#include "Net/Connection.h"


namespace BR {

	template class SharedPointerT <Net::IConnection>;

namespace Net {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Connection Interface
	//




	//////////////////////////////////////////////////////////////////////////
	//
	//	basic implementation
	//

	// Constructor
	IConnection::IConnection()
		: m_CID(0)
		, m_UData(0)
		, m_ConnectionState(STATE_DISCONNECTED)
		, m_tConnectionTime(DurationMS(0))
		, m_pINet(nullptr)
		, m_pEventHandler(nullptr)
	{
		memset( &m_ConnectInfo, 0, sizeof(m_ConnectInfo) );
		memset( &m_pPolicy, 0, sizeof(m_pPolicy) );
	}

	IConnection::~IConnection()
	{
		for (UINT iPolicy = 0; iPolicy < countof(m_pPolicy); iPolicy++)
		{
			if (m_pPolicy[iPolicy] != nullptr)
			{
				((INetPolicy*)m_pPolicy[iPolicy])->m_pConnection = nullptr;
			}
			Util::SafeDelete(m_pPolicy[iPolicy]);
		}
	}

	void IConnection::UpdateConnectionTime()
	{
		m_tConnectionTime = Util::Time.GetTimeMs();
	}

	Result IConnection::InitConnectionNClass(const NetAddress& Addr, NetClass netClass)
	{

		m_ConnectInfo.Local = Addr;
		m_ConnectInfo.LocalClass = netClass;
		m_ConnectInfo.Remote = Addr;
		m_ConnectInfo.RemoteClass = netClass;


		m_ConnectionState = STATE_CONNECTING;

		m_tConnectionTime = Util::Time.GetTimeMs();

		return ResultCode::SUCCESS;
	}




	////////////////////////////////////////////////////////////////////////////////
	//
	//	IConnectionEventHandler Interface
	//

	IConnectionEventHandler::~IConnectionEventHandler()
	{

	}

	Result IConnectionEventHandler::OnNetSyncMessage(IConnection* pConn)
	{
		unused(pConn);
		Assert(false);
		return ResultCode::NOT_IMPLEMENTED;
	};


	// Net send message
	Result IConnectionEventHandler::OnNetSendReadyMessage(IConnection* pConn)
	{
		unused(pConn);
		Assert(false);
		return ResultCode::NOT_IMPLEMENTED;
	};



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Interface
	//


	INet::Event::Event(INet::Event::EventTypes eventType, SharedPointerT<Connection>& pCon)
		:EventType(eventType),
		EventConnection(pCon)
	{
	}


	INet::Event& INet::Event::operator =(const INet::Event& src)
	{
		EventType = src.EventType;
		EventConnection = src.EventConnection;
		return *this;
	}


	INet::INet()
		:m_NetClass(NetClass::Unknown)
	{
	}



} // namespace Net
} // namespace BR

