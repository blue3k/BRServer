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
			virtual HRESULT OnRecv(const sockaddr_in6& remoteAddr, Message::MessageData *pMsg) = 0;
		};

	private:

		NetAddress m_LocalAddress;
		sockaddr_in6 m_LocalSockAddress;
		SOCKET m_Socket;

		IOBUFFER_READ *m_pRecvBuffers;

		MessageHandler *m_pMessageHandler;

	protected:

	public:

		RawUDP();
		virtual ~RawUDP();

		HRESULT InitializeNet(const NetAddress& localAddress, MessageHandler *pHandler);
		HRESULT TerminateNet();

		SOCKET GetSocket()								{ return m_Socket; }

		HRESULT PendingRecv(IOBUFFER_READ *pOver);
		virtual HRESULT SendBuffer(IOBUFFER_WRITE *pSendBuffer) override;
		HRESULT SendMsg(const sockaddr_in6& dest, Message::MessageData *pMsg);

		virtual SOCKET GetIOSocket() override { return GetSocket(); }

		// called when incomming message occure
		HRESULT OnRecv(const sockaddr_in6& remoteAddr, UINT uiBuffSize, const BYTE* pBuff);

		// called when reciving messag is completed
		HRESULT Recv(IOBUFFER_READ* pIOBuffer) override;
		virtual HRESULT OnIORecvCompleted(HRESULT hrRes, IOBUFFER_READ* &pIOBuffer) override;

		// called when send completed
		virtual HRESULT OnIOSendCompleted(HRESULT hrRes, IOBUFFER_WRITE *pIOBuffer) override;
	};






} // namespace Net
} // namespace BR



