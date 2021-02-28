////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
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
#include "Component/SFComponent.h"


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

		Heap m_Heap;

		// Player's UID
		PlayerInformation m_PlayerInfo;

		SharedPointerT<Net::Connection> m_Connection;
		SharedPointerT<MessageEndpoint> m_RemoteEndpoint;

		// User entity UID, this will be 0 if the user doesn't logged in.
		EntityUID m_PlayerEntityUID;

		// if player disconnected and need to stay for a while
		bool m_IsActivePlayer{};

	public:

		PlayerEntityInformation(IHeap& heap, EntityUID playerEntityUID, const PlayerInformation& player);
		virtual ~PlayerEntityInformation();

		IHeap& GetHeap() { return m_Heap; }

		const PlayerInformation& GetPlayerInformation() const	{ return m_PlayerInfo; }
		const SharedPointerT<MessageEndpoint>& GetRemoteEndpoint() const { return m_RemoteEndpoint; }
		const EntityUID& GetPlayerEntityUID() const { return m_PlayerEntityUID; }
		bool GetIsActivePlayer() const { return m_IsActivePlayer; }

		PlayerID GetPlayerID() const							{ return m_PlayerInfo.PlayerID; }
		const char* GetPlayerName() const						{ return m_PlayerInfo.NickName; }
		Result SetPlayerName( const char* newName)				{ return StrUtil::StringCopy( m_PlayerInfo.NickName, newName ); }

		// Set game server entity
		Result SetRemoteEndpoint(EntityUID playerEntityUID, const SharedPointerT<MessageEndpoint>& remoteEndpoint);
		Result SetRemoteConnection(const SharedPointerT<Net::Connection>& connection);

		void ReleaseConnection(const char* reason);

		// Get route context
		RouteContext GetRouteContext(EntityUID uidFrom) {
			return RouteContext(uidFrom, m_PlayerEntityUID);
		}
	};


} // namespace Svr
} // namespace SF

