////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "SvrTrace.h"
#include "String/SFStrUtil.h"
#include "Net/SFMessageEndpoint.h"
#include "Net/SFMessageEndpointManager.h"
#include "Service/ServerService.h"


namespace SF {

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	MessageEndpointManager class
	//

	constexpr StringCrc64 MessageEndpointManager::TypeName;


	MessageEndpointManager::MessageEndpointManager()
		: m_MessageEndpointByUID(GetSystemHeap())
		, m_MessageEndpointByServerID(GetSystemHeap())
	{

	}

	MessageEndpointManager::~MessageEndpointManager()
	{
	}

	// Initialize entity to proceed new connection
	Result MessageEndpointManager::InitializeComponent()
	{
		Service::MessageEndpointManager = this;

		return ResultCode::SUCCESS;
	}

	// Close entity and clear transaction
	Result MessageEndpointManager::DeinitializeComponent()
	{

		MutexScopeLock lock(m_TableLock);

		m_MessageEndpointByUID.ForeachOrder(0, m_MessageEndpointByUID.size(), 
			[](uint64_t key, MessageEndpoint*& value)
			{
				delete value;
				return true;
			});

		Service::MessageEndpointManager = nullptr;

		return ResultCode::SUCCESS;
	}


	Result MessageEndpointManager::AddOrGetRemoteEndpoint(const EntityUID& entityUID, const ServerConfig::MessageEndpoint& messageEndpoint, MessageEndpoint*& pEndpoint)
	{
		MutexScopeLock lock(m_TableLock);

		pEndpoint = nullptr;
		if (m_MessageEndpointByUID.Find(entityUID, pEndpoint))
		{
			// TODO: validate whether they are same
			assert(pEndpoint);
		}
		else
		{
			auto pStreamEndpoint = new(GetEngineHeap()) MessageEndpointStreamDB();
			SharedReferenceInc inc(pStreamEndpoint);
			pStreamEndpoint->InitializeEndpoint(messageEndpoint.MessageServer, messageEndpoint.Channel);
			auto res = m_MessageEndpointByUID.Insert(entityUID, pStreamEndpoint);
			if (!res)
			{
				SharedReferenceDec dec(pStreamEndpoint);
				return ResultCode::OUT_OF_MEMORY;
			}
			m_MessageEndpointByUID.CommitChanges();

			m_MessageEndpointByServerID.Insert(entityUID.GetServerID(), pStreamEndpoint);
			m_MessageEndpointByServerID.CommitChanges();

			pEndpoint = pStreamEndpoint;
		}

		return ResultCode::SUCCESS;
	}

	MessageEndpoint* MessageEndpointManager::GetEndpoint(const EntityUID& entityUID)
	{
		MessageEndpoint* pEndpoint{};
		if (!m_MessageEndpointByUID.Find(entityUID, pEndpoint)) // search with entityUID first
		{
			// If we fail the UID should be non-service entity. search with server ID
			m_MessageEndpointByServerID.Find(entityUID.GetServerID(), pEndpoint);
		}
		return pEndpoint;
	}


} // namespace SF

