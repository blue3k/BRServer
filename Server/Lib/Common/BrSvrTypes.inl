////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Br Base type definitions
//	
//
////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Transaction ID
//


TransactionID::TransactionID()
	: ID(0)
{
	Components.EntID = -1;
	Components.TransID = 0;
}

TransactionID::TransactionID( const TransactionID& transID )
	: ID(transID.ID)
{
}

TransactionID::TransactionID( EntityID entityID, UINT32 transID )
{
	Components.EntID = entityID;
	Components.TransID = transID;
}

TransactionID::TransactionID( Context context )
	:ID(context)
{
}

bool TransactionID::IsValid() const
{
	TransactionID trans;
	return trans.ID != ID;
}

TransactionID& TransactionID::operator = ( const TransactionID& transID )
{
	ID = transID.ID;

	return *this;
}

bool TransactionID::operator == ( const TransactionID& src ) const
{
	return ID == src.ID;
}

TransactionID::operator Context() const
{
	return (Context)ID;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Server Route context
//


RouteContext::RouteContext()
{
	memset(ContextValue,0,sizeof(ContextValue));
}

RouteContext::RouteContext( const RouteContext& routeContext )
{
	memcpy(ContextValue, routeContext.ContextValue, sizeof(ContextValue));
}

RouteContext::RouteContext( EntityUID InFromID, EntityUID InToID )
{
	Components.From = InFromID;
	Components.To = InToID;
}

RouteContext::RouteContext( int initValue )
{
	Assert( initValue == 0 );
	memset(ContextValue,0,sizeof(ContextValue));
}

RouteContext& RouteContext::operator = ( const RouteContext& src )
{
	memcpy(ContextValue, src.ContextValue, sizeof(ContextValue));
	return *this;
}

bool RouteContext::operator == ( const RouteContext& routeContext ) const
{
	return ContextValue[0] == routeContext.ContextValue[0] && ContextValue[1] == routeContext.ContextValue[1];
}



// Get swaped context( From <==> To )
RouteContext RouteContext::GetSwaped() const
{
	return RouteContext(Components.To, Components.From);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Cluster service information
//


ServiceInformation::ServiceInformation()
	:UID(0),Membership(ClusterMembership::StatusWatcher),Status(ServiceStatus::Offline),ServerClass(NetClass::Unknown), ServerAddress(0), ServerUpTime(TimeStampSec::max()), Workload(0)
{
}

ServiceInformation::ServiceInformation( const ServiceInformation& src )
	:UID(src.UID),Membership(src.Membership),Status(src.Status),ServerClass(src.ServerClass), ServerAddress(src.ServerAddress),ServerUpTime(src.ServerUpTime), Workload(src.Workload)
{
}

ServiceInformation::ServiceInformation( EntityUID entityUID, ClusterMembership membership, ServiceStatus status, NetClass netClass, const NetAddress& address, TimeStampSec serverUpTime, UINT32 workload )
{
	UID = entityUID;
	Membership = membership;
	Status = status;
	ServerClass = netClass;
	ServerAddress = address;
	ServerUpTime = serverUpTime;
	Workload = workload;
}

ServiceInformation::ServiceInformation( int initValue )
{
	Assert( initValue == 0 );
	UID = 0;
	Membership = ClusterMembership::StatusWatcher;
	ServerClass = NetClass::Unknown;
	Status = ServiceStatus::Offline;
	ServerAddress = NetAddress(0);
	ServerUpTime = TimeStampSec::min();
	Workload = 0;
}

ServiceInformation& ServiceInformation::operator = ( const ServiceInformation& src )
{
	UID = src.UID;
	Membership = src.Membership;
	Status = src.Status;
	ServerClass = src.ServerClass;
	ServerAddress = src.ServerAddress;
	ServerUpTime = src.ServerUpTime;
	Workload = src.Workload;
	return *this;
}

bool ServiceInformation::operator == ( const ServiceInformation& op ) const
{
	return UID == op.UID && Membership == op.Membership && Status == op.Status && ServerClass == op.ServerClass && ServerAddress == op.ServerAddress && ServerUpTime == op.ServerUpTime && Workload == op.Workload;
}




GlobalUID::GlobalUID()
	:UID(0)
{
}

GlobalUID::GlobalUID( const GlobalUID& src )
	:UID(src.UID)
{
}

GlobalUID::GlobalUID( UINT serverID, UINT32 time, UINT32 id )
	: Time(time), SvrID(serverID), ID(id)
{
	Assert( serverID < 256 );
}

GlobalUID::GlobalUID( UINT64 initValue )
	:UID(initValue)
{
}

GlobalUID& GlobalUID::operator = ( const GlobalUID& src )
{
	UID = src.UID;
	return *this;
}

bool GlobalUID::operator == ( const GlobalUID& op ) const
{
	return UID == op.UID;
}

bool GlobalUID::operator != ( const GlobalUID& op ) const
{
	return UID != op.UID;
}

GlobalUID::operator UINT64 () const
{
	return UID;
}



LocalUID::LocalUID()
	:UID(0)
{
}

LocalUID::LocalUID( const LocalUID& src )
	:UID(src.UID)
{
}

LocalUID::LocalUID( UINT32 time, UINT32 id )
	:Time(time), ID(id)
{
}

LocalUID::LocalUID( UINT64 initValue )
	:UID(initValue)
{
}

LocalUID& LocalUID::operator = ( const LocalUID& src )
{
	UID = src.UID;
	return *this;
}

bool LocalUID::operator == ( const LocalUID& op ) const
{
	return UID == op.UID;
}

bool LocalUID::operator != ( const LocalUID& op ) const
{
	return UID != op.UID;
}

LocalUID::operator UINT64 () const
{
	return UID;
}




MatchingQueueTicket::MatchingQueueTicket()
	:QueueUID(0), QueueItemID(0)
{
}

MatchingQueueTicket::MatchingQueueTicket( const MatchingQueueTicket& src )
	:QueueUID(src.QueueUID), QueueItemID(src.QueueItemID)
{
}

MatchingQueueTicket::MatchingQueueTicket( EntityUID queueUID, LocalUID itemID )
{
	QueueUID = queueUID;
	QueueItemID = itemID;
}

MatchingQueueTicket::MatchingQueueTicket( int initValue )
	:QueueUID(0), QueueItemID(0)
{
}

MatchingQueueTicket& MatchingQueueTicket::operator = ( const MatchingQueueTicket& src )
{
	QueueUID = src.QueueUID;
	QueueItemID = src.QueueItemID;

	return *this;
}

bool MatchingQueueTicket::operator == ( const MatchingQueueTicket& op ) const
{
	return QueueUID == op.QueueUID && QueueItemID == op.QueueItemID;
}

bool MatchingQueueTicket::operator != ( const MatchingQueueTicket& op ) const
{
	return QueueUID != op.QueueUID || QueueItemID != op.QueueItemID;
}





MatchingPlayerInformation::MatchingPlayerInformation()
	:PlayerUID(0), PlayerID(0), RequestedRole(PlayerRole::None)
{
}

MatchingPlayerInformation::MatchingPlayerInformation( const MatchingPlayerInformation& src )
	: PlayerUID(src.PlayerUID), PlayerID(src.PlayerID), RequestedRole(src.RequestedRole)
{
}

MatchingPlayerInformation::MatchingPlayerInformation(EntityUID playerUID, AccountID playerID, PlayerRole requestedRole)
{
	PlayerUID = playerUID;
	PlayerID = playerID;
	RequestedRole = requestedRole;
}

MatchingPlayerInformation::MatchingPlayerInformation( int initValue )
	:PlayerUID(0), PlayerID(0)
{
}

MatchingPlayerInformation& MatchingPlayerInformation::operator = ( const MatchingPlayerInformation& src )
{
	PlayerUID = src.PlayerUID;
	PlayerID = src.PlayerID;
	RequestedRole = src.RequestedRole;

	return *this;
}

bool MatchingPlayerInformation::operator == ( const MatchingPlayerInformation& op ) const
{
	return PlayerUID == op.PlayerUID && PlayerID == op.PlayerID && RequestedRole == op.RequestedRole;
}





MatchingQueueItem::MatchingQueueItem()
	: RegisterUID(0)
	, RegisterID(0)
	, NumPlayers(0)
{
	memset( Players, 0, sizeof(Players) );
}

MatchingQueueItem::MatchingQueueItem( const MatchingQueueItem& src )
	: RegisterUID(src.RegisterUID)
	, RegisterID(src.RegisterID)
	, NumPlayers(std::max((UINT32)MAX_NUM_PLAYER, src.NumPlayers))
{
	memcpy( Players, src.Players, sizeof(MatchingPlayerInformation)*NumPlayers );
}

MatchingQueueItem::MatchingQueueItem( EntityUID registerUID, PlayerID registerID, UINT numPlayer, const MatchingPlayerInformation* playerInformations )
	: RegisterUID(registerUID)
	, RegisterID(registerID)
	, NumPlayers(std::max((UINT32)MAX_NUM_PLAYER, numPlayer))
{
	memcpy( Players, playerInformations, sizeof(MatchingPlayerInformation)*NumPlayers );
}

MatchingQueueItem::MatchingQueueItem( int initValue )
	: RegisterUID(0)
	, RegisterID(0)
	, NumPlayers(0)
{
	memset( Players, 0, sizeof(Players) );
}

HRESULT MatchingQueueItem::SetQueueItem( EntityUID registerUID, PlayerID registerID, UINT numPlayer, const MatchingPlayerInformation* playerInformations )
{
	Assert(playerInformations);
	if( playerInformations == nullptr) 
		return E_SYSTEM_INVALIDARG;
	
	RegisterUID = registerUID;
	RegisterID = registerID;
	NumPlayers = std::min((UINT32)MAX_NUM_PLAYER,numPlayer);
	memcpy( Players, playerInformations, sizeof(MatchingPlayerInformation)*NumPlayers );
	return S_OK;
}

MatchingQueueItem& MatchingQueueItem::operator = ( const MatchingQueueItem& src )
{
	RegisterUID = src.RegisterUID;
	RegisterID = src.RegisterID;
	NumPlayers = src.NumPlayers;
	memcpy( Players, src.Players, sizeof(MatchingPlayerInformation)*NumPlayers );

	return *this;
}

bool MatchingQueueItem::operator == ( const MatchingQueueItem& op ) const
{
	return NumPlayers == op.NumPlayers && RegisterUID == op.RegisterUID && RegisterID == op.RegisterID;
}



inline bool PerformanceCounterInfo::operator == (const PerformanceCounterInfo& op) const
{
	return StrUtil::StringCmpLwr(CounterName, (INT)countof(CounterName), op.CounterName, (INT)countof(CounterName)) == 0;
}

inline bool PerformanceCounterInstanceInfo::operator == (const PerformanceCounterInstanceInfo& op) const
{
	return InstanceUID == op.InstanceUID;
}




