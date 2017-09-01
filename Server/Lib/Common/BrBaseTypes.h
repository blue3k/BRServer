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
#include "SFAssert.h"
#include "Common/MathUtil.h"
#include "String/StrUtil.h"


namespace BR {

#pragma pack(push)
#pragma pack(4)

	// context ID type
	typedef uint64_t Context;

	// Account ID type
	typedef uint64_t AccountID;

	// Facebook UID
	typedef uint64_t FacebookUID;

	// Player ID
	typedef AccountID PlayerID;

	// Authentication ticket
	typedef uint64_t AuthTicket;

	// server ID
	typedef uint32_t ServerID;
	
	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Address information
	//

	
	// Network class definition
	enum class NetClass : uint32_t
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

	enum class SockFamily : UINT8
	{
		None = 0,
		IPV4,// = AF_INET,
		IPV6,// = AF_INET6
	};

	enum class SockType
	{
		Stream,// = SOCK_STREAM,       // TCP
		DataGram,// = SOCK_DGRAM,     // UDP
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
		UINT16 usPort;
		SockFamily SocketFamily;

		inline NetAddress();
		inline NetAddress(SockFamily sockFamily, const char* strAdr, UINT16 port = 0);
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


