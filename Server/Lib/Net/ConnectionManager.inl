////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author : KyungKun Ko
//
// Description : Net connection Map. 
//	
//
////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
//
//	ConnectionManager
//



ConnectionManager::Operation::Operation(void* ptr)
	:OpCode(OP_NONE), pConn(0), EnqueuedTime(DurationMS(0))
{
	assert(ptr == nullptr);
}

ConnectionManager::Operation::Operation( Operation&& src )
	: addrOrg(src.addrOrg)
	, addrNew(src.addrNew)
	, MobileNetCtrl(src.MobileNetCtrl)
	, NetCtrlConnect(src.NetCtrlConnect)
	, EnqueuedTime(src.EnqueuedTime)
{
	pConn = std::forward<SharedPointerT<Connection>>(src.pConn);
	OpCode.store(src.OpCode.load(std::memory_order_relaxed), std::memory_order_release);
}

ConnectionManager::Operation::Operation( const sockaddr_storage& sockAddr, MsgNetCtrlConnect *pNetCtrl )
	: OpCode(OP_PENDING_ADDR)
	, pConn(nullptr)
	, addrOrg(sockAddr)
	, NetCtrlConnect(*pNetCtrl)
	, EnqueuedTime(Util::Time.GetTimeMs())
{
}

ConnectionManager::Operation::Operation( OperationCode code, const sockaddr_storage& sockAddr, MsgMobileNetCtrl *pNetCtrl )
	: OpCode(code)
	, pConn(nullptr)
	, addrOrg(sockAddr)
	, MobileNetCtrl(*pNetCtrl)
	, EnqueuedTime(Util::Time.GetTimeMs())
{
}

ConnectionManager::Operation::Operation(OperationCode code, const sockaddr_storage& sockAddrOrg, const sockaddr_storage& sockAddrNew, Connection *pCo)
	: OpCode(code)
	, pConn(pCo)
	, addrOrg(sockAddrOrg)
	, addrNew(sockAddrNew)
	, EnqueuedTime(Util::Time.GetTimeMs())
{
}

ConnectionManager::Operation::Operation( OperationCode code, AuthTicket ticket, Connection *pCo )
	: OpCode(code)
	, pConn(pCo)
	, EnqueuedTime(Util::Time.GetTimeMs())
{
	MobileNetCtrl.PeerID = ticket;
}
			
ConnectionManager::Operation::Operation( OperationCode code, Connection *pCo )
	: OpCode(code)
	, pConn(pCo)
	, EnqueuedTime(Util::Time.GetTimeMs())
{
}

ConnectionManager::Operation::~Operation()
{
	Clear();
}

void ConnectionManager::Operation::Clear()
{
	pConn = SharedPointerT<Connection>();
}

// These operation is used for empty value os just compare with OpCode
bool ConnectionManager::Operation::operator != ( const Operation& src ) const
{
	return OpCode.load(std::memory_order_relaxed) != src.OpCode.load(std::memory_order_relaxed);
}

bool ConnectionManager::Operation::operator == ( const Operation& src ) const
{
	return OpCode.load(std::memory_order_relaxed) == src.OpCode.load(std::memory_order_relaxed);
}







//////////////////////////////////////////////////////////////////////////////////////////////////
//
//	ConnectionManager
//

void ConnectionManager::SetNetOwner( INet *pOwner )
{
	m_pINetOwner = pOwner;
}

// Get net owner
INet* ConnectionManager::GetNetOwner()
{
	return m_pINetOwner;
}

UINT ConnectionManager::NewCID()
{
	UINT counter = m_CIDGen.NewID();

	AssertRel( counter < INT_MAX );// If got this assertion that will be some thing going wrong with connection manage

	return counter;
}

void ConnectionManager::FreeCID( UINT uiCID )
{
	m_CIDGen.FreeID( uiCID );
}

void ConnectionManager::SetUseAddressMap(bool bUseAddressMap)
{
	m_UseAddressMap = bUseAddressMap;
}

void ConnectionManager::SetPeerIDMap(bool bUsePeerIDMap)
{
	m_UsePeerIDMap = bUsePeerIDMap;
}

// Prevent connection allocation or not
void ConnectionManager::EnableNewConnection( bool bIsEnable )
{
	m_bNoNewConnection = bIsEnable;
}

// Active connection
CounterType ConnectionManager::GetNumActiveConnection()
{
	return m_AddrMap.GetItemCount();
}




//////////////////////////////////////////////////////////////////////////////////////////////////
//
//	ConnectionManagerT
//

template< class ConnectionType >
ConnectionManagerT<ConnectionType>::ConnectionManagerT( UINT uiBucketSize )
	: ConnectionManager(uiBucketSize)
{
}

// Initialize Manager
template< class ConnectionType >
Result ConnectionManagerT<ConnectionType>::InitManager( UINT poolCacheCount )
{
	Result hr = ResultCode::SUCCESS;

	//if( !( hr = m_ConnectionPool.PoolCache( poolCacheCount ) ) )
	//	return hr;

	Start();

	return hr;
}


// create connection
template< class ConnectionType >
Connection* ConnectionManagerT<ConnectionType>::NewConnection()
{
	ConnectionType* pNewCon = new ConnectionType;
	AssertRel(pNewCon->GetConnectionState() == IConnection::STATE_DISCONNECTED);
	if( pNewCon && pNewCon->GetNet() == nullptr )
	{
		Assert( GetNetOwner() );
		pNewCon->SetupNet( GetNetOwner(), (uintptr_t)NewCID() );
	}

	return pNewCon;
}

template< class ConnectionType >
void ConnectionManagerT<ConnectionType>::FreeConnection( Connection* pConn )
{
	if (pConn == nullptr)
		return;

	//ConnectionType* pConnT = dynamic_cast<ConnectionType*>( pConn );

	RemoveMap( pConn );

	if (pConn->GetCID() != 0 )
	{
		FreeCID((UINT)pConn->GetCID());
		pConn->ClearCID(); // clear after free
	}

	AssertRel(pConn->GetConnectionState() == IConnection::STATE_DISCONNECTED);
	//delete pConnT;
}




