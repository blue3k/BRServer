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
#include "Net/NetMessage.h"
#include "Net/NetSystem.h"


namespace BR {
namespace Net {


	

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Client base class
	//

	class RawUDP : public IOCPSystem::IOCallBack
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

		OVERLAPPED_BUFFER_READ *m_pRecvBuffers;

		MessageHandler *m_pMessageHandler;

	protected:

	public:

		RawUDP();
		~RawUDP();

		HRESULT InitializeNet(const NetAddress& localAddress, MessageHandler *pHandler);
		HRESULT TerminateNet();

		SOCKET GetSocket()								{ return m_Socket; }

		HRESULT PendingRecv(OVERLAPPED_BUFFER_READ *pOver);
		HRESULT SendMsg(const sockaddr_in6& dest, Message::MessageData *pMsg);

		// called when incomming message occure
		HRESULT OnRecv(const sockaddr_in6& remoteAddr, UINT uiBuffSize, const BYTE* pBuff);
		//HRESULT OnRecv(Message::MessageData *pMsg);

		// called when New connection TCP accepted
		virtual HRESULT OnIOAccept(HRESULT hrRes, OVERLAPPED_BUFFER_ACCEPT *pAcceptInfo) override;

		// called when reciving messag is completed
		virtual HRESULT OnIORecvCompleted(HRESULT hrRes, OVERLAPPED_BUFFER_READ *pIOBuffer, DWORD dwTransferred) override;

		// called when send completed
		virtual HRESULT OnIOSendCompleted(HRESULT hrRes, OVERLAPPED_BUFFER_WRITE *pIOBuffer, DWORD dwTransferred) override;
	};






} // namespace Net
} // namespace BR



