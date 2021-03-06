////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2019 The Braves
// 
// Author : KyungKun Ko
//
// Description : Network relay server
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"

#include "Memory/SFMemory.h"
#include "Types/BrSvrTypes.h"
#include "Container/SFPageQueue.h"
#include "Util/LocalUIDGenerator.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "Transaction/MessageRoute.h"
#include "Container/SFHashTable.h"
#include "Container/SFIndexing.h"

#include "Entity/EntityInformation.h"
#include "ServiceEntity/ClusteredServiceEntity.h"
#include "Net/SFNetRawUDP.h"

#include "Transaction/MessageHandlerTable.h"


namespace SF {
	namespace Net {
		class Connection;
		class ServerMUDP;
	};

	namespace Policy {
		class ISvrPolicyRelay;
	};
};




namespace SF {


	class RelayPlayer;
	class RelayInstance;


namespace Net {



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	RelayServiceEntity class
	//

	class RelayServer
	{
	public:

		//using MessageHandlerType = std::function<void(MessageDataPtr&)>;
		typedef Result(RelayServer::*MessageHandlerType)(const sockaddr_storage& remoteAddr, MessageDataPtr &&);
		//using MessageHandlerType = MessageHandlerFunc;


	public:

		RelayServer(GameID gameID, uint32_t maximumRelayInstances);
		~RelayServer();

		IHeap& GetHeap() { return m_NetRawUDP.GetHeap(); }
		GameID GetGameID() const { return m_GameID; }

		Result InitializeNet(const NetAddress& listenAddr);
		void TerminateNet();

		// TickUpdate 
		virtual Result TickUpdate();

		// send message
		Result SendMsg(const sockaddr_storage& dest, SharedPointerT<Message::MessageData>& pMsg);


	private:

		////////////////////////////////////////////////////////////////////////////////////
		//
		//	Network handling
		//

		// Register message handlers
		Result RegisterMessageHandlers();

		Result OnJoinRelayInstanceC2SEvt(const sockaddr_storage& remoteAddr, MessageDataPtr&& pMsgData);
		Result OnLeaveRelayInstanceC2SEvt(const sockaddr_storage& remoteAddr, MessageDataPtr&& pMsgData);
		Result OnRelayPacketC2SEvt(const sockaddr_storage& remoteAddr, MessageDataPtr&& pMsgData);

		Result OnRecv(const sockaddr_storage& remoteAddr, SharedPointerT<Message::MessageData>& pMsg);

	private:

		// Game ID
		GameID m_GameID;

		// Max available relay instance
		uint32_t m_MaxInstances;

		// Raw UDP network
		Net::RawUDP m_NetRawUDP;

		//
		HashTable<uint32_t, RelayInstance*> m_RelayInstanceByID;

		// Message handler table
		Svr::MessageHandlerTable<MessageHandlerType>	m_HandlerTable;

	};








}; // namespace Net
}; // namespace SF




