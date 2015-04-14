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


// set local info
void IConnection::ConnectionInformation::SetLocalInfo( NetClass Class, const NetAddress& Addr, UINT64 UID )
{
	Local = Addr;
	LocalClass = Class;
	LocalID = UID;
}

// set remote info
void IConnection::ConnectionInformation::SetRemoteInfo( NetClass Class, UINT64 UID )
{
	//Remote = Addr;
	RemoteClass = Class;
	RemoteID = UID;
}



IConnection::tag_Event& IConnection::tag_Event::operator =(const IConnection::tag_Event& src)
{
	memcpy( this, &src, sizeof(Event) );
	return *this;
}

bool IConnection::tag_Event::operator == (const IConnection::tag_Event& src) const
{
	if( src.EventType != EventType )
		return false;

	if( EventType == EVT_NONE ) 
		return true;

	return Value.hr == src.Value.hr;
}


// setup Net
HRESULT IConnection::SetupNet( INet *pNetInstance, UINT_PTR uiCID )
{
	m_CID = uiCID;
	m_pINet = pNetInstance;
	return S_OK;
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

// Get connection state
inline const IConnection::ConnectionState IConnection::GetConnectionState() const
{
	return m_ConnectionState;
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
	if( ((UINT32)IPolicyType::ID_POLICY) >= BR::POLICY_NETMAX )
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
	if( ((UINT32)policyID) >= BR::POLICY_NETMAX || m_pPolicy[policyID] == NULL )
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
inline ULONG IConnection::GetConnectionTime()
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
	return m_pEventHandler;
}

inline void IConnection::SetEventHandler(IConnectionEventHandler* pEventHandler)
{
	m_pEventHandler = pEventHandler;
}


////////////////////////////////////////////////////////////////////////////////
//
//	Net object
//

INet::Event::Event()
	:EventType(INet::Event::EVT_NONE),
	EventConnection(NULL)
{
}

INet::Event::Event( INet::Event::EventTypes eventType, BR::Net::IConnection *pCon )
	:EventType(eventType),
	EventConnection(pCon)
{
}

INet::Event& INet::Event::operator =( const INet::Event& src )
{
	EventType = src.EventType;
	EventConnection = src.EventConnection;
	return *this;
}

bool INet::Event::operator == ( const INet::Event& src ) const
{
	return EventType == src.EventType && EventConnection == src.EventConnection;
}



NetClass INet::GetNetClass() const
{
	return m_NetClass;
}

