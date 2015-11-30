////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Player entity information class
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/ClassUtil.h"
#include "Common/BrBaseTypes.h"

#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/GameSystem.h"
#include "ServerSystem/Component.h"


#include "ServerSystem/SvrConst.h"
#include "Common/Indexing.h"
#include "Common/StaticHashTable.h"


namespace BR {
namespace Svr {

	class ServerEntity;

	//////////////////////////////////////////////////////////////////////////////
	//
	//	Party player class
	//
	
	class PlayerEntityInformation
	{
	private:
		// Player's UID
		BRCLASS_ATTRIBUTE_READONLY(PlayerInformation,PlayerInfo);

		BRCLASS_ATTRIBUTE_READONLY(TimeStampSec,LatestServerUpTime);

		BRCLASS_ATTRIBUTE_READONLY(ServerEntity*,ServerEntity);

		// User entity UID, this will be 0 if the user doesn't logged in.
		BRCLASS_ATTRIBUTE_READONLY(EntityUID,PlayerEntityUID);

		BRCLASS_ATTRIBUTE_READONLY(bool,IsActivePlayer);

	public:

		PlayerEntityInformation( const PlayerInformation& player );
		virtual ~PlayerEntityInformation();


		FORCEINLINE PlayerID GetPlayerID() const							{ return m_PlayerInfo.PlayerID; }
		FORCEINLINE const char* GetPlayerName() const						{ return m_PlayerInfo.NickName; }
		FORCEINLINE HRESULT SetPlayerName( const char* newName)				{ return StrUtil::StringCpy( m_PlayerInfo.NickName, newName ); }
		FORCEINLINE const PlayerInformation& GetPlayerInformation() const	{ return m_PlayerInfo; }
		//FORCEINLINE FriendInformation GetFriendInformation() const			{ return FriendInformation(m_PlayerInfo.PlayerID, m_PlayerInfo.FacebookUID, m_PlayerInfo.NickName, m_PlayerInfo.Level, 0, 0, m_PlayerInfo.IsPlayingGame, m_PlayerInfo.LastActiveTime, 0); }

		// Check server status
		FORCEINLINE bool CheckServerStatus();

		// Set game server entity
		HRESULT SetServerEntity( ServerEntity* pGameServerEntity, EntityUID playerUID );

		// Get route context
		FORCEINLINE RouteContext GetRouteContext( EntityUID uidFrom );

		// Query policy from game server entity
		template< class PolicyType >
		FORCEINLINE PolicyType* GetPolicy();

	};

#include "PlayerEntityInformation.inl"

}; // namespace Svr
}; // namespace BR

