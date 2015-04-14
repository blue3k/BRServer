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

#include "Common/Typedefs.h"
#include "Common/BrAssert.h"
#include "Common/MathUtil.h"
#include "Common/NullUtil.h"
#include "Common/StrUtil.h"


namespace BR {

#pragma pack(push)
#pragma pack(4)

	// context ID type
	typedef UINT64 Context;

	// Account ID type
	typedef UINT64 AccountID;

	// Facebook UID
	typedef UINT64 FacebookUID;

	// Player ID
	typedef AccountID PlayerID;

	// Authentication ticket
	typedef UINT64 AuthTicket;

	// server ID
	typedef UINT32 ServerID;
	
	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Address information
	//

	
	// Network class definition
	enum class NetClass : UINT32
	{
		Unknown,
		Client,
		Entity,
		Login,
		GameInstance,
		GameMaster,
		Game,
		Max
	};



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Address information
	//

	struct NetAddress
	{
		enum {
			MAX_NETNAME = 70,
		};
		char strAddr[MAX_NETNAME];
		USHORT usPort;

		inline NetAddress();
		inline NetAddress( const char* strAdr, USHORT port = 0 );
		inline NetAddress( int );

		inline bool operator == ( const NetAddress& op ) const;
	};

#pragma pack(pop)

	
#pragma pack(push)
#pragma pack(4)


	// Entity ID faculty, this means group of Entity
	enum class EntityFaculty : UINT8
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
			UINT32			EntityLID	: 24;	// Local entity ID
			UINT32			FacultyID	: 8;	// Local faculty ID
			UINT32						:0;		// Local entity ID
		};
		UINT32 ID;


		inline EntityID();
		inline EntityID( const EntityUID& entityUID );
		inline EntityID( const EntityID& entityID );
#if !defined(SWIG)
		inline EntityID( EntityFaculty facultyID, UINT32 entityLID );
#endif
		inline EntityID( UINT32 uiID );


		inline EntityID& operator = ( const EntityID& entityID );

		//inline bool operator == ( const EntityID& src ) const;
		//inline bool operator != ( const EntityID& src ) const;
#if !defined(SWIG)
		inline operator UINT32() const;
#endif
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity Universal unique ID
	//
	union EntityUID
	{
		struct {
			EntityID	EntityID;		// Local entity ID
			ServerID	SvrID;		// Server ID
		};
		UINT64 UID;

		inline EntityUID();
		inline EntityUID( const EntityUID& entityUID );
		inline EntityUID( ServerID serverID, UINT32 entityID );
		inline EntityUID( const Context& context );

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


