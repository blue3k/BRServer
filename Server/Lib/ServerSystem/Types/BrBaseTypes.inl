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

EntityUID::EntityUID( ServerID serverID, uint32_t entityID )
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
EntityID::EntityID( EntityFaculty facultyID, uint32_t entityLID )
{
	Components.FacultyID = (UINT)facultyID;
	Components.EntityLID = entityLID & 0xFFFFFF; // to avoid wconversion warning
}
#endif

EntityID::EntityID( uint32_t uiID )
	:ID(uiID)
{
}

EntityID& EntityID::operator = ( const EntityID& entityID )
{
	ID = entityID.ID;

	return *this;
}


EntityID::operator uint32_t() const
{
	return (uint32_t)ID;
}


