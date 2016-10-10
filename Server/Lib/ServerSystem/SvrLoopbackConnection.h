////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Loopback connection
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "ServerSystem/BrServer.h"
#include "Common/MemoryPool.h"

#include "Net/LoopbackConnection.h"





namespace BR {
namespace Svr
{



	//////////////////////////////////////////////////////////////////////////
	//
	//	LoopbackConnection class
	//

	class LoopbackConnection : public Net::LoopbackConnection
	{
	private:
		// local remote entity that linked with this connection
		Svr::ServerEntity*		m_pServerEntity;

	public:
		LoopbackConnection(NetClass netClass, Svr::ServerEntity* pServerEntity);
		~LoopbackConnection();

		virtual Result SendRaw(Message::MessageData* &pMsg) override { unused(pMsg);  return ResultCode::FAIL; }

		// Send message to connected entity
		virtual Result Send( Message::MessageData* &pMsg );

	};




}; // namespace Svr
}; // namespace BR







