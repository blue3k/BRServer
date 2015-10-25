////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author : KyungKun Ko
//
// Description : Net connection classes. 
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Net/NetDef.h"
#include "Net/NetCtrl.h"
#include "Net/NetSystem.h"
#include "Common/HRESNet.h"
#include "Net/Connection.h"



namespace BR {
namespace Net {



	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Network connection class
	//

	class ConnectionTCP : public Connection, public IOCPSystem::IOCallBack
	{
	public:

	private:
		// Guaranted send/ack count
		std::atomic<LONG> m_lGuarantedSent;
		std::atomic<LONG> m_lGuarantedAck;


		// Tempory incomming buffer for fragmented incomming packet buffer
		UINT	m_uiRecvTemUsed;
		std::vector<BYTE>	m_bufRecvTem;

		// Recv overlapped buffer. recv 
		OVERLAPPED_BUFFER_READ m_RecvBuffer;

	protected:
		// Special net control packet count on the same connection state. ex)Connect packet
		UINT m_uiSendNetCtrlCount;

	public:
		// Constructor
		ConnectionTCP();
		virtual ~ConnectionTCP();


		inline OVERLAPPED_BUFFER_READ* GetRecvBuffer();

		// Process network control message
		HRESULT ProcNetCtrl( const MsgNetCtrl* pNetCtrl );

		/////////////////////////////////////////////////////////////////////////////////////////////
		// callback
		// called when New connection TCP accepted
		virtual HRESULT OnIOAccept( HRESULT hrRes, OVERLAPPED_BUFFER_ACCEPT *pAcceptInfo );

		// called when reciving TCP message
		virtual HRESULT OnIORecvCompleted( HRESULT hrRes, OVERLAPPED_BUFFER_READ *pIOBuffer, DWORD dwTransferred );

		// called when Send completed
		virtual HRESULT OnIOSendCompleted( HRESULT hrRes, OVERLAPPED_BUFFER_WRITE *pIOBuffer, DWORD dwTransferred );


		// Pending recv New one
		HRESULT PendingRecv();
		
		// Clear Queue
		virtual HRESULT ClearQueues();


		// Called on connection result
		virtual void OnConnectionResult( HRESULT hrConnect );

		// Initialize connection
		virtual HRESULT InitConnection( SOCKET socket, const ConnectionInformation &connectInfo );

		HRESULT Connect();

		virtual HRESULT WaitConnect();

		// Close connection
		virtual HRESULT CloseConnection() override;

		// called when incomming message occure
		virtual HRESULT OnRecv(UINT uiBuffSize, const BYTE* pBuff) override;
		virtual HRESULT OnRecv(Message::MessageData *pMsg) override;

		// Send message to connected entity
		virtual HRESULT Send(Message::MessageData* &pMsg) override;

		virtual HRESULT SendNetCtrl(UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID = 0) override;

	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Network client connection class
	//

	class ConnectionTCPClient : public ConnectionTCP, public MemoryPoolObject<ConnectionTCPClient>
	{
	public:

	public:
		// Constructor
		ConnectionTCPClient();
		~ConnectionTCPClient();
		
		// Initialize connection
		virtual HRESULT InitConnection( SOCKET socket, const ConnectionInformation &connectInfo );

		// Wait connection event
		HRESULT WaitConnect();

		// Update net control, process connection heartbit, ... etc
		virtual HRESULT UpdateNetCtrl();

	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Network server connection class
	//

	class ConnectionTCPServer : public ConnectionTCP, public MemoryPoolObject<ConnectionTCPServer>
	{
	public:

	private:

	public:
		// Constructor
		ConnectionTCPServer();
		~ConnectionTCPServer();

		// Update net control, process connection heartbit, ... etc
		virtual HRESULT UpdateNetCtrl();

	};



	////////////////////////////////////////////////////////////////////////////////
	//
	//	TCP Network server connection class
	//

	class ConnectionPeerTCP : public ConnectionTCP, public MemoryPoolObject<ConnectionPeerTCP>
	{
	public:

	private:

	public:
		// Constructor
		ConnectionPeerTCP();
		~ConnectionPeerTCP();

		virtual void OnConnectionResult(HRESULT hrConnect) override;

		// Update net control, process connection heartbit, ... etc
		virtual HRESULT UpdateNetCtrl();

	};

#include "ConnectionTCP.inl"

}  // namespace Net
}; // namespace BR


