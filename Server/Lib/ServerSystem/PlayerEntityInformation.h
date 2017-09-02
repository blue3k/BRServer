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

#include "SFTypedefs.h"
#include "Common/ClassUtil.h"
#include "Types/BrBaseTypes.h"

#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/GameSystem.h"
#include "Common/BrComponent.h"


#include "ServerSystem/SvrConst.h"



namespace SF {
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


		PlayerID GetPlayerID() const							{ return m_PlayerInfo.PlayerID; }
		const char* GetPlayerName() const						{ return m_PlayerInfo.NickName; }
		Result SetPlayerName( const char* newName)				{ return StrUtil::StringCpy( m_PlayerInfo.NickName, newName ); }
		const PlayerInformation& GetPlayerInformation() const	{ return m_PlayerInfo; }
		//FriendInformation GetFriendInformation() const			{ return FriendInformation(m_PlayerInfo.PlayerID, m_PlayerInfo.FacebookUID, m_PlayerInfo.NickName, m_PlayerInfo.Level, 0, 0, m_PlayerInfo.IsPlayingGame, m_PlayerInfo.LastActiveTime, 0); }

		// Check server status
		bool CheckServerStatus();

		// Set game server entity
		Result SetServerEntity( ServerEntity* pGameServerEntity, EntityUID playerUID );

		// Get route context
		RouteContext GetRouteContext( EntityUID uidFrom );

		// Query policy from game server entity
		template< class PolicyType >
		PolicyType* GetPolicy();

	};

#include "PlayerEntityInformation.inl"

}; // namespace Svr
}; // namespace SF

