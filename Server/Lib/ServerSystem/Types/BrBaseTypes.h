////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Brave base types
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "SFAssert.h"
#include "String/StrUtil.h"


namespace BR {

#pragma pack(push)
#pragma pack(4)


	// Entity ID faculty, this means group of Entity
	enum class EntityFaculty : uint8_t
	{
		None,				// Faculty undefined
		Server,				// Faculty Server
		Service,			// Faculty Service
		User,				// Faculty User
		Game,				// Faculty Game
		GameInstance,		// Faculty Game Instance
		Party,				// Faculty Party
		MatchInstance,		// Faculty Match Instance
		Manager,			// Faculty Manager Instance
		Max,
	};


	union EntityUID;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity unique ID in local
	//
	union EntityID
	{
		enum {
			MAX_IDBIT = 24,
		};


		struct {
			uint32_t			EntityLID	: 24;	// Local entity ID
			uint32_t			FacultyID	: 8;	// Local faculty ID
			uint32_t						:0;		// Local entity ID
		} Components;
		uint32_t ID;


		inline EntityID();
		inline EntityID( const EntityUID& entityUID );
		inline EntityID( const EntityID& entityID );
#if !defined(SWIG)
		inline EntityID( EntityFaculty facultyID, uint32_t entityLID );
#endif
		inline EntityID( uint32_t uiID );

		uint32_t GetEntityLID() const { return Components.EntityLID; }
		uint32_t GetFacultyID() const { return Components.FacultyID; }

		inline EntityID& operator = ( const EntityID& entityID );

		//inline bool operator == ( const EntityID& src ) const;
		//inline bool operator != ( const EntityID& src ) const;
#if !defined(SWIG)
		inline operator uint32_t() const;
#endif
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity Universal unique ID
	//
	union EntityUID
	{
		struct {
			EntityID	EntID;		// Local entity ID
			ServerID	SvrID;		// Server ID
		} Components;
		uint64_t UID;

		inline EntityUID();
		inline EntityUID( const EntityUID& entityUID );
		inline EntityUID( ServerID serverID, uint32_t entityID );
		inline EntityUID( const Context& context );

		EntityID GetEntityID() const { return Components.EntID; }
		ServerID GetServerID() const { return Components.SvrID; }

		inline EntityUID& operator = ( const EntityUID& entityUID );

		inline bool operator == ( const EntityUID& src ) const;
		inline bool operator != ( const EntityUID& src ) const;

#if !defined(SWIG)
		inline explicit operator Context() const;
#endif
	};


	typedef EntityID	PartyID;
	typedef EntityUID	PartyUID;
	
	// Game Instance
	typedef EntityID	GameInsID;
	typedef EntityUID	GameInsUID;



#pragma pack(pop)

#include "BrBaseTypes.inl"


};


