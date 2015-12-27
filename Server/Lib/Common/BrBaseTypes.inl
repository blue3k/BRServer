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



////////////////////////////////////////////////////////////////////////////////
//
//	TCP/IP Network Address information
//

NetAddress::NetAddress()
{
	memset( strAddr, 0, sizeof(strAddr) );
	usPort = 0;
	SocketFamily = SockFamily::None;
}

NetAddress::NetAddress(SockFamily sockFamily, const char* strAdr, UINT16 port )
{
	AssertRel(strAdr);
	StrUtil::StringCpy( strAddr, strAdr );
	usPort = port;
	SocketFamily = sockFamily;
}

NetAddress::NetAddress( int iVal )
{
	unused(iVal);
	memset( strAddr, 0, sizeof(strAddr) );
	usPort = 0;
	SocketFamily = SockFamily::None;
}

bool NetAddress::operator == ( const NetAddress& op ) const
{
	return memcmp( strAddr, op.strAddr, sizeof(strAddr) ) == 0 && usPort == op.usPort;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Entity Universal unique ID
//


EntityUID::EntityUID()
	:UID(0)
{
}

EntityUID::EntityUID( const EntityUID& entityUID )
	:UID(entityUID.UID)
{
}

EntityUID::EntityUID( ServerID serverID, UINT32 entityID )
{
	Components.SvrID = serverID;
	Components.EntID = entityID;
}

EntityUID::EntityUID( const Context& context )
	:UID(context)
{
}



EntityUID& EntityUID::operator = ( const EntityUID& entityUID )
{
	UID = entityUID.UID;

	return *this;
}

bool EntityUID::operator == ( const EntityUID& src ) const
{
	return UID == src.UID;
}

bool EntityUID::operator != ( const EntityUID& src ) const
{
	return UID != src.UID;
}

EntityUID::operator Context() const
{
	return (Context)UID;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Entity local unique ID
//


EntityID::EntityID()
	:ID(0)
{
}

EntityID::EntityID( const EntityUID& entityUID )
	:ID(entityUID.Components.EntID.ID)
{
}

EntityID::EntityID( const EntityID& entityID )
	:ID(entityID.ID)
{
}

#if !defined(SWIG)
EntityID::EntityID( EntityFaculty facultyID, UINT32 entityLID )
{
	Components.FacultyID = (UINT)facultyID;
	Components.EntityLID = entityLID;
}
#endif

EntityID::EntityID( UINT32 uiID )
	:ID(uiID)
{
}

EntityID& EntityID::operator = ( const EntityID& entityID )
{
	ID = entityID.ID;

	return *this;
}

//bool EntityID::operator == ( const EntityID& src ) const
//{
//	return ID == src.ID;
//}
//
//bool EntityID::operator != ( const EntityID& src ) const
//{
//	return ID != src.ID;
//}

EntityID::operator UINT32() const
{
	return (UINT32)ID;
}


