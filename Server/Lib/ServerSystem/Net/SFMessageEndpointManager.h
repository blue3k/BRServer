////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Message endpoint
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Service/MessageEndpointManagerService.h"
#include "Container/SFDualSortedMap.h"

namespace SF {

	//////////////////////////////////////////////////////////////////////////////////////////////
	// MessageEndpointManager class
	class MessageEndpointManager : public SharedObject, public MessageEndpointManagerService
	{
	public:

		static constexpr StringCrc64 TypeName = "MessageEndpointManagerService";

	public:

		CriticalSection m_TableLock;
		DualSortedMap<uint64_t, MessageEndpoint*> m_MessageEndpointByUID;
		DualSortedMap<uint32_t, MessageEndpoint*> m_MessageEndpointByServerID;

	public:

		MessageEndpointManager();
		virtual ~MessageEndpointManager();

		// Initialize entity to proceed new connection
		Result InitializeComponent();

		// Close entity and clear transaction
		Result DeinitializeComponent();


		virtual Result AddOrGetRemoteEndpoint(const EntityUID& entityUID, const ServerConfig::MessageEndpoint& messageEndpoint, MessageEndpoint*& pEndpoint) override;

		virtual MessageEndpoint* GetEndpoint(const EntityUID& entityUID) override;
	};



} // namespace SF


