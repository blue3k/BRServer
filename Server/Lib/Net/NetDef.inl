////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : KyungKun Ko
//
// Description : Net base type definitions. 
//	
//
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
//
//	connection object
//


// Get connection state
inline const IConnection::ConnectionState IConnection::GetConnectionState() const
{
	return m_ConnectionState.load(std::memory_order_relaxed);
}

inline void IConnection::SetConnectionState(ConnectionState newState)
{
	m_ConnectionState.store(newState, std::memory_order_relaxed);
}

// set local info
inline void IConnection::ConnectionInformation::SetLocalInfo( NetClass Class, const NetAddress& Addr, UINT64 UID )
{
	Local = Addr;
	LocalClass = Class;
	LocalID = UID;
}

// set remote info
inline void IConnection::ConnectionInformation::SetRemoteInfo( NetClass Class, UINT64 UID )
{
	//Remote = Addr;
	RemoteClass = Class;
	RemoteID = UID;
}



inline IConnection::Event& IConnection::Event::operator =(const IConnection::Event& src)
{
	memcpy( this, &src, sizeof(Event) );
	return *this;
}

inline bool IConnection::Event::operator == (const IConnection::Event& src) const
{
	if( src.EventType != EventType )
		return false;

	assert(EventType != EVT_NONE); // who access this?

	return hr == src.hr;
}

inline bool IConnection::Event::operator != (const IConnection::Event& src) const
{
	return src.EventType != EventType || hr != src.hr;
}


// setup Net
Result IConnection::SetupNet( INet *pNetInstance, uintptr_t uiCID )
{
	m_CID = uiCID;
	m_pINet = pNetInstance;
	return ResultCode::SUCCESS;
}


// Get Connection info
inline const IConnection::ConnectionInformation& IConnection::GetConnectionInfo() const
{
	return m_ConnectInfo;
}

inline void IConnection::SetRemoteID(UINT64 newID)
{
	m_ConnectInfo.RemoteID = newID;
}


// Get Connection ID
inline uintptr_t IConnection::GetCID() const
{
	return m_CID;
}

inline void IConnection::ClearCID()
{
	m_CID = 0;
}

// Get Connection PeerID
UINT64 IConnection::GetPeerID() const
{
	return m_ConnectInfo.RemoteID;
}

// Set local class
inline void IConnection::SetLocalClass(NetClass uiLocalClass)
{
	m_ConnectInfo.LocalClass = uiLocalClass;
	m_ConnectInfo.LocalID = (UINT)(-1);
}



// register message policy handler
template< typename IPolicyType >
IPolicyType* IConnection::GetPolicy()
{
	if( ((UINT32)IPolicyType::ID_POLICY) >= POLICY_NETMAX )
		return nullptr;

	if( m_pPolicy[IPolicyType::ID_POLICY] == nullptr )
		CreatePolicy(IPolicyType::ID_POLICY);

	if( m_pPolicy[IPolicyType::ID_POLICY] == nullptr )
	{
		// Not registered policy?
		Assert(false);
		return nullptr;
	}

	return (IPolicyType*)m_pPolicy[IPolicyType::ID_POLICY];
}

template< typename IPolicyType >
IPolicyType* IConnection::GetPolicyByID( UINT policyID )
{
	if( ((UINT32)policyID) >= POLICY_NETMAX || m_pPolicy[policyID] == NULL )
		return NULL;

	return (IPolicyType*)m_pPolicy[policyID];
}

// User data
inline uintptr_t IConnection::GetUData()
{
	return m_UData;
}

inline void IConnection::SetUData(uintptr_t UData)
{
	m_UData = UData;
}

// Get connection time
inline TimeStampMS IConnection::GetConnectionTime()
{
	return m_tConnectionTime;
}

// Get Net Instance
inline INet* IConnection::GetNet()
{
	return m_pINet;
}

inline IConnectionEventHandler* IConnection::GetEventHandler()
{
	return m_pEventHandler.load(std::memory_order_acquire);
}

inline void IConnection::SetEventHandler(IConnectionEventHandler* pEventHandler)
{
	m_pEventHandler.store(pEventHandler, std::memory_order_release);
}


////////////////////////////////////////////////////////////////////////////////
//
//	Net object
//

inline INet::Event::Event(void* ptr)
	:EventType(INet::Event::EVT_NONE),
	EventConnection()
{
	assert(ptr == nullptr);
}

inline INet::Event::Event(EventTypes eventType)
	:EventType(eventType),
	EventConnection()
{
}





inline bool INet::Event::operator == ( const INet::Event& src ) const
{
	return EventType == src.EventType && EventConnection == src.EventConnection;
}

inline bool INet::Event::operator != (const INet::Event& src) const
{
	return EventType != src.EventType || EventConnection != src.EventConnection;
}




NetClass INet::GetNetClass() const
{
	return m_NetClass;
}

