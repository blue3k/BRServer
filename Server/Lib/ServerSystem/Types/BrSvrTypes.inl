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
//	Cluster service information
//


ServiceInformation::ServiceInformation()
	: UID(0)
	, Membership(ClusterMembership::Slave)
	, Status(ServiceStatus::Offline)
	, ServerClass(NetClass::Unknown)
	, ServerUpTime(TimeStampSec::max())
	, Workload(0)
{
}

ServiceInformation::ServiceInformation( const ServiceInformation& src )
	:UID(src.UID),Membership(src.Membership),Status(src.Status),ServerClass(src.ServerClass), ServerAddress(src.ServerAddress),ServerUpTime(src.ServerUpTime), Workload(src.Workload)
{
}

ServiceInformation::ServiceInformation( EntityUID entityUID, ClusterMembership membership, ServiceStatus status, NetClass netClass, const NetAddress& address, TimeStampSec serverUpTime, uint32_t workload )
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
	Membership = ClusterMembership::Slave;
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

#if __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif

GlobalUID::GlobalUID( uint serverID, uint32_t time, uint32_t id )
	: Time(time), SvrID(serverID), ID(id)
{
	Assert( serverID < 256 );
}
#if __GNUC__
#pragma GCC diagnostic pop
#endif

GlobalUID::GlobalUID( uint64_t initValue )
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

GlobalUID::operator uint64_t () const
{
	return UID;
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
	, NumPlayers(std::max((uint32_t)MAX_NUM_PLAYER, src.NumPlayers))
{
	memcpy( Players, src.Players, sizeof(MatchingPlayerInformation)*NumPlayers );
}

MatchingQueueItem::MatchingQueueItem( EntityUID registerUID, PlayerID registerID, uint numPlayer, const MatchingPlayerInformation* playerInformations )
	: RegisterUID(registerUID)
	, RegisterID(registerID)
	, NumPlayers(std::max((uint32_t)MAX_NUM_PLAYER, numPlayer))
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

Result MatchingQueueItem::SetQueueItem( EntityUID registerUID, PlayerID registerID, uint numPlayer, const MatchingPlayerInformation* playerInformations )
{
	Assert(playerInformations);
	if( playerInformations == nullptr) 
		return ResultCode::INVALID_ARG;
	
	RegisterUID = registerUID;
	RegisterID = registerID;
	NumPlayers = std::min((uint32_t)MAX_NUM_PLAYER,numPlayer);
	memcpy( Players, playerInformations, sizeof(MatchingPlayerInformation)*NumPlayers );
	return ResultCode::SUCCESS;
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
	return StrUtil::StringCmpLwr(CounterName, (INT)countof(CounterName), op.CounterName, (INT)countof(CounterName));
}

inline bool PerformanceCounterInstanceInfo::operator == (const PerformanceCounterInstanceInfo& op) const
{
	return InstanceUID == op.InstanceUID;
}




