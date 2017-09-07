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
#include "Types/SFEngineTypedefs.h"

#include "Transaction/MessageRoute.h"
#include "Entity/Entity.h"
#include "Server/BrServer.h"
#include "ServiceEntity/Game/GameSystem.h"
#include "Component/BrComponent.h"


#include "SvrConst.h"



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
		PlayerInformation m_PlayerInfo;

		TimeStampSec m_LatestServerUpTime;

		ServerEntity* m_ServerEntity;

		// User entity UID, this will be 0 if the user doesn't logged in.
		EntityUID m_PlayerEntityUID;

		bool m_IsActivePlayer;

	public:

		PlayerEntityInformation( const PlayerInformation& player );
		virtual ~PlayerEntityInformation();

		const PlayerInformation& GetPlayerInformation() const	{ return m_PlayerInfo; }
		const TimeStampSec& GetLatestServerUpTime() const { return m_LatestServerUpTime; }
		ServerEntity* GetServerEntity() const { return m_ServerEntity; }
		const EntityUID& GetPlayerEntityUID() const { return m_PlayerEntityUID; }
		bool GetIsActivePlayer() const { return m_IsActivePlayer; }

		PlayerID GetPlayerID() const							{ return m_PlayerInfo.PlayerID; }
		const char* GetPlayerName() const						{ return m_PlayerInfo.NickName; }
		Result SetPlayerName( const char* newName)				{ return StrUtil::StringCpy( m_PlayerInfo.NickName, newName ); }

		//FriendInformation GetFriendInformation() const			{ return FriendInformation(m_PlayerInfo.PlayerID, m_PlayerInfo.FacebookUID, m_PlayerInfo.NickName, m_PlayerInfo.Level, 0, 0, m_PlayerInfo.IsPlayingGame, m_PlayerInfo.LastActiveTime, 0); }

		// Check server status
		bool CheckServerStatus();

		// Set game server entity
		Result SetServerEntity( ServerEntity* pGameServerEntity, EntityUID playerUID );

		// Get route context
		RouteContext GetRouteContext( EntityUID uidFrom );

		// Query policy from game server entity
		Net::Connection* GetConnection() const		{ return m_ServerEntity != nullptr ? m_ServerEntity->GetConnection() : nullptr; }

	};

#include "PlayerEntityInformation.inl"

}; // namespace Svr
}; // namespace SF

