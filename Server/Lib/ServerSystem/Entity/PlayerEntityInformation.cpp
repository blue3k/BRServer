////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Player class
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "SvrConst.h"
#include "SvrTrace.h"
#include "Util/SFTimeUtil.h"
#include "Types/SFEngineTypedefs.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "Net/SFMessage.h"

#include "ServerEntity/ServerEntity.h"
#include "Entity/PlayerEntityInformation.h"


namespace SF {
	namespace Svr {


		//////////////////////////////////////////////////////////////////////////
		//
		//	Remote Player class
		//

		PlayerEntityInformation::PlayerEntityInformation(IHeap& heap, EntityUID playerEntityUID, const PlayerInformation& player)
			: m_Heap("PlayerEntityInformation", heap)
			, m_PlayerInfo(player)
			, m_PlayerEntityUID(playerEntityUID)
			, m_IsActivePlayer(true)
		{
		}

		PlayerEntityInformation::~PlayerEntityInformation()
		{
		}


		// Set game server entity
		Result PlayerEntityInformation::SetRemoteEndpoint(EntityUID playerEntityUID, const SharedPointerT<MessageEndpoint>& remoteEndpoint)
		{
			m_PlayerEntityUID = playerEntityUID;
			m_RemoteEndpoint = remoteEndpoint;

			m_IsActivePlayer = true;

			return ResultCode::SUCCESS;
		}

		void PlayerEntityInformation::ReleaseConnection(const char* reason)
		{
			if (m_Connection != nullptr)
			{
				m_Connection->DisconnectNRelease(reason);
				m_Connection.reset();
			}
		}

		Result PlayerEntityInformation::SetRemoteConnection(const SharedPointerT<Net::Connection>& connection)
		{
			Result hr;

			m_RemoteEndpoint = new(GetHeap()) Net::MessageEndpointConnection(connection.get());
			m_Connection = connection;

			m_IsActivePlayer = true;

			return ResultCode::SUCCESS;
		}



	} // namespace Svr
} // namespace SF

