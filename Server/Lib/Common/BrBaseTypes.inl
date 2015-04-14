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
}

NetAddress::NetAddress( const char* strAdr, USHORT port )
{
	AssertRel(strAdr);
	StrUtil::StringCpy( strAddr, strAdr );
	usPort = port;
}

NetAddress::NetAddress( int iVal )
{
	iVal;
	memset( strAddr, 0, sizeof(strAddr) );
	usPort = 0;
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
	:SvrID(serverID), 
	EntityID(entityID)
{
}

EntityUID::EntityUID( const BR::Context& context )
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

EntityUID::operator BR::Context() const
{
	return (BR::Context)UID;
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
	:ID(entityUID.EntityID.ID)
{
}

EntityID::EntityID( const EntityID& entityID )
	:ID(entityID.ID)
{
}

#if !defined(SWIG)
EntityID::EntityID( EntityFaculty facultyID, UINT32 entityLID )
	:FacultyID((UINT)facultyID), 
	EntityLID(entityLID)
{
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


