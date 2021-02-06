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

		UTCTimeStampSec m_LatestServerUpTime;

		SharedPointerT<MessageEndpoint> m_RemoteEndpoint;

		// User entity UID, this will be 0 if the user doesn't logged in.
		EntityUID m_PlayerEntityUID;

		bool m_IsActivePlayer;

	public:

		PlayerEntityInformation( const PlayerInformation& player );
		virtual ~PlayerEntityInformation();

		const PlayerInformation& GetPlayerInformation() const	{ return m_PlayerInfo; }
		const UTCTimeStampSec& GetLatestServerUpTime() const { return m_LatestServerUpTime; }
		const SharedPointerT<MessageEndpoint>& GetRemoteEndpoint() const { return m_RemoteEndpoint; }
		const EntityUID& GetPlayerEntityUID() const { return m_PlayerEntityUID; }
		bool GetIsActivePlayer() const { return m_IsActivePlayer; }

		PlayerID GetPlayerID() const							{ return m_PlayerInfo.PlayerID; }
		const char* GetPlayerName() const						{ return m_PlayerInfo.NickName; }
		Result SetPlayerName( const char* newName)				{ return StrUtil::StringCopy( m_PlayerInfo.NickName, newName ); }

		// Set game server entity
		Result SetRemoteEndpoint(const SharedPointerT<MessageEndpoint>& remoteEndpoint, EntityUID playerUID );

		// Get route context
		RouteContext GetRouteContext( EntityUID uidFrom );

		// Query policy from game server entity
		//const SharedPointerAtomicT<Net::Connection>& GetConnection() const { static const SharedPointerAtomicT<Net::Connection> Dymmy;  return m_ServerEntity != nullptr ? m_ServerEntity->GetConnection() : Dymmy; }

	};

#include "PlayerEntityInformation.inl"

}; // namespace Svr
}; // namespace SF

