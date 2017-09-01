////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author : KyungKun Ko
//
// Description : Net Client
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Net/NetDef.h"
#include "Net/NetCtrl.h"
#include "Net/NetSystem.h"


namespace BR {
namespace Net {


	

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Client base class
	//

	class RawUDP : public INetIOCallBack
	{
	public :

		class MessageHandler
		{
		public:
			virtual Result OnRecv(const sockaddr_storage& remoteAddr, Message::MessageData *pMsg) = 0;
		};

	private:

		NetAddress m_LocalAddress;
		sockaddr_storage m_LocalSockAddress;
		SOCKET m_Socket;

		IOBUFFER_READ *m_pRecvBuffers;

		MessageHandler *m_pMessageHandler;

	protected:

	public:

		RawUDP();
		virtual ~RawUDP();

		Result InitializeNet(const NetAddress& localAddress, MessageHandler *pHandler);
		Result TerminateNet();

		SOCKET GetSocket()								{ return m_Socket; }

		Result PendingRecv(IOBUFFER_READ *pOver);
		virtual Result SendBuffer(IOBUFFER_WRITE *pSendBuffer) override;
		Result SendMsg(const sockaddr_storage& dest, Message::MessageData *pMsg);


		// called when incomming message occure
		Result OnRecv(const sockaddr_storage& remoteAddr, UINT uiBuffSize, const uint8_t* pBuff);

		virtual Result OnSendReady() override;
		// called when reciving messag is completed
		Result Recv(IOBUFFER_READ* pIOBuffer) override;
		virtual Result OnIORecvCompleted(Result hrRes, IOBUFFER_READ* &pIOBuffer) override;

		// called when send completed
		virtual Result OnIOSendCompleted(Result hrRes, IOBUFFER_WRITE *pIOBuffer) override;
	};






} // namespace Net
} // namespace BR



