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

#include "SFTypedefs.h"
#include "Server/BrServer.h"
#include "MemoryManager/SFMemoryPool.h"

#include "Net/SFLoopbackConnection.h"





namespace SF {
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

		virtual Result SendRaw(const MessageDataPtr &pMsg) override { unused(pMsg);  return ResultCode::FAIL; }

		// Send message to connected entity
		virtual Result Send(const MessageDataPtr &pMsg ) override;

	};




}; // namespace Svr
}; // namespace SF







