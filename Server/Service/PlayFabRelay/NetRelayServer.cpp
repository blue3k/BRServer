////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2019 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Relay Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "String/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFThread.h"
#include "Memory/SFMemory.h"
#include "GameConst.h"
#include "Net/SFNetDef.h"
#include "Net/SFNetServerUDP.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "Entity/EntityManager.h"
#include "NetRelayServer.h"
#include "ServiceEntity/Relay/RelayPlayer.h"
#include "ServiceEntity/RankingServiceTrans.h"
#include "Server/BrServer.h"

#include "SvrTrace.h"
#include "SvrConst.h"
#include "ServerConfig/SFServerConfig.h"
#include "Service/ServerService.h"
#include "ServiceEntity/Relay/RelayInstance.h"
#include "ServiceEntity/Relay/RelayPlayer.h"

#include "Protocol/Message/RelayMsgClass.h"
#include "Protocol/Policy/RelayNetPolicy.h"



namespace SF {
namespace Net {


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//


	RelayServer::RelayServer(GameID gameID, uint32_t maximumRelayInstances)
		: m_GameID(gameID)
		, m_MaxInstances(maximumRelayInstances)
		, m_RealyInstanceByID(m_NetRawUDP.GetHeap())
		, m_HandlerTable(m_NetRawUDP.GetHeap())
	{
	}

	RelayServer::~RelayServer()
	{
		TerminateNet();
	}

	Result RelayServer::InitializeNet(const NetAddress& listenAddr)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(m_NetRawUDP.InitializeNet(listenAddr, this));

	Proc_End:

		return hr;
	}

	// clear 
	void RelayServer::TerminateNet()
	{
		m_NetRawUDP.TerminateNet();
	}


	Result RelayServer::TickUpdate()
	{
		FunctionContext hr;


		return hr;
	}

	Result RelayServer::RegisterMessageHandlers()
	{
		m_HandlerTable.Register<Message::Relay::JoinRelayInstanceC2SEvt>(__FILE__, __LINE__, &RelayServer::OnJoinRelayInstanceC2SEvt);
		m_HandlerTable.Register<Message::Relay::LeaveRelayInstanceC2SEvt>(__FILE__, __LINE__, &RelayServer::OnLeaveRelayInstanceC2SEvt);
		m_HandlerTable.Register<Message::Relay::RelayPacketC2SEvt>(__FILE__, __LINE__, &RelayServer::OnRelayPacketC2SEvt);

		return ResultCode::SUCCESS;
	}


	Result RelayServer::OnJoinRelayInstanceC2SEvt(MessageDataPtr&& pMsgData)
	{
		FunctionContext hr;

		Message::Relay::JoinRelayInstanceC2SEvt message(pMsgData);
		svrCheck(message.ParseMsg());

		


		return hr;
	}

	Result RelayServer::OnLeaveRelayInstanceC2SEvt(MessageDataPtr&& pMsgData)
	{
		FunctionContext hr;


		return hr;
	}

	Result RelayServer::OnRelayPacketC2SEvt(MessageDataPtr&& pMsgData)
	{
		FunctionContext hr;


		return hr;
	}

	Result RelayServer::OnRecv(const sockaddr_storage& remoteAddr, SharedPointerT<Message::MessageData>& pMsg)
	{
		FunctionContext hr;

		MessageHandlerType handler;

		if (m_HandlerTable.GetHandler(pMsg->GetMessageHeader()->msgID, handler))
		{
			(this->*handler)(std::forward<SharedPointerT<Message::MessageData>>(pMsg));
		}
		else
		{
			svrTrace(Error, "Unhandled Relaymessage {0}", pMsg->GetMessageHeader()->msgID);
		}

		return hr;
	}



}; // namespace Net {
}; // namespace SF {



