////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Network classes definitions
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Net/NetDef.h"
#include "Net/NetConst.h"
#include "Common/StrUtil.h"
#include "Common/HashTable.h"
#include "Common/memorypool.h"
#include "Common/Thread.h"
#include "Common/BrBaseTypes.h"



namespace BR {
namespace Net {

	class Connection;

	

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Packetes for Network Control definitions
	//

	enum NetCtrlIDs
	{
		NetCtrlCode_None,
		NetCtrlCode_Ack,
		NetCtrlCode_Nack,
		NetCtrlCode_HeartBit,
		NetCtrlCode_Disconnect,
		NetCtrlCode_Connect,
		NetCtrlCode_ConnectPeer,
		NetCtrlCode_SyncReliable,
		NetCtrlCode_TimeSync,
	};

	static const Message::MessageID PACKET_NETCTRL_NONE			= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE, Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_None);
	static const Message::MessageID PACKET_NETCTRL_ACK			= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE, Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_Ack);
	static const Message::MessageID PACKET_NETCTRL_NACK			= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE, Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_Nack);
	static const Message::MessageID PACKET_NETCTRL_HEARTBIT		= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE, Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_HeartBit);
	static const Message::MessageID PACKET_NETCTRL_DISCONNECT	= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE, Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_Disconnect);
	static const Message::MessageID PACKET_NETCTRL_CONNECT		= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE, Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_Connect);
	static const Message::MessageID PACKET_NETCTRL_CONNECTPEER	= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE, Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_ConnectPeer);
	static const Message::MessageID PACKET_NETCTRL_SYNCRELIABLE	= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE, Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_SyncReliable);
	static const Message::MessageID PACKET_NETCTRL_TIMESYNC		= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE, Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_TimeSync);


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Utility functions
	//
	void SockAddr2Addr( const sockaddr_in6 &sockAddr, NetAddress &addr );
	void Addr2SockAddr( const NetAddress &addr, sockaddr_in6 &sockAddr );
	void SetSockAddr( sockaddr_in6& sockAddr, const char *strAddr, USHORT usPort );
	bool GetLocalAddressIPv4(NetAddress &addr);
	bool GetLocalAddressIPv6(NetAddress &addr);

	bool operator == ( const sockaddr_in6 &op1, const sockaddr_in6 &op2 );
	bool operator != ( const sockaddr_in6 &op1, const sockaddr_in6 &op2 );

	bool operator == ( const NetAddress &op1, const NetAddress &op2 );
	bool operator != ( const NetAddress &op1, const NetAddress &op2 );



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Overlapped I/O structures
	//

	typedef struct tag_OVERLAPPED_BUFFER : public WSAOVERLAPPED
	{
		enum OPERATION {
			OP_NONE = 0,
			OP_TCPREAD,
			OP_TCPREADPENDING,
			OP_TCPWRITE,
			OP_TCPACCEPT,
			OP_UDPREAD,
			OP_UDPWRITE,
			OP_PEERUDPREAD,
			OP_PEERUDPWRITE,
		} Operation;

		// Clear Buffer
		void ClearBuffer();

	} OVERLAPPED_BUFFER;


	// UDP/TCP read/write overlapped base
	typedef struct tag_OVERLAPPED_BUFFER_BASE : public OVERLAPPED_BUFFER
	{
		// IOCP buffer 
		WSABUF	wsaBuff;

		// Operated buffer size
		DWORD dwOperateSize;


		// Constructor
		tag_OVERLAPPED_BUFFER_BASE();


	} OVERLAPPED_BUFFER_BASE;


	// UDP/TCP write overlapped
	struct OVERLAPPED_BUFFER_WRITE : public OVERLAPPED_BUFFER_BASE, public MemoryPoolObject<OVERLAPPED_BUFFER_WRITE>
	{
		// Message pointer to send
		Message::MessageData *pMsgs;

		// Message buffer pointer to send
		BYTE *pSendBuff;

		// Constructor
		OVERLAPPED_BUFFER_WRITE();
		~OVERLAPPED_BUFFER_WRITE();

		// Initialize for IO
		inline void InitForIO();
		inline void InitMsg( Message::MessageData *pMsg );
		inline void InitBuff( UINT uiBuffSize, BYTE* pBuff );

		// Setup sending mode
		inline void SetupSendUDP( Message::MessageData *pMsg );
		inline void SetupSendUDP( UINT uiBuffSize, BYTE* pBuff );
		inline void SetupSendPeer( Message::MessageData *pMsg );
		inline void SetupSendPeer( UINT uiBuffSize, BYTE* pBuff );
		inline void SetupSendTCP( Message::MessageData *pMsg );
		inline void SetupSendTCP( UINT uiBuffSize, BYTE* pBuff );

	};


	// UDP/TCP read overlapped
	typedef struct tag_OVERLAPPED_BUFFER_READ : public OVERLAPPED_BUFFER_BASE
	{
		// Read flag
		DWORD dwFlags;

		// UDP Read from
		struct sockaddr_in6 From;

		// UDP Recv socket length
		INT iSockLen;

		// Recv connection ID for error check
		UINT_PTR CID;

		// Recv buffer
		CHAR buffer[Const::INTER_PACKET_SIZE_MAX];

		bool bIsPending;

		// constructor
		tag_OVERLAPPED_BUFFER_READ();
		~tag_OVERLAPPED_BUFFER_READ();

		// Initialize for IO
		inline void InitForIO();
		inline void InitRecv( UINT_PTR iCID );

		// Setup recving mode
		inline void SetupRecvUDP( UINT_PTR iCID );
		inline void SetupRecvPeer( UINT_PTR iCID );
		inline void SetupRecvTCP( UINT_PTR iCID );
		inline void SetupRecvTCPPending( UINT_PTR iCID );

	} OVERLAPPED_BUFFER_READ;


	// TCP accept overlapped
	typedef struct tag_OVERLAPPED_BUFFER_ACCEPT : public OVERLAPPED_BUFFER
	{
		//Connection	*pConnection;
		SOCKET sockAccept;
		BYTE pAcceptInfo[(sizeof(sockaddr_in6)+16)*2];

		// Constructor
		tag_OVERLAPPED_BUFFER_ACCEPT();
		~tag_OVERLAPPED_BUFFER_ACCEPT();

		// Setup accept
		inline void SetupAccept( SOCKET sock );

	} OVERLAPPED_BUFFER_ACCEPT;







	////////////////////////////////////////////////////////////////////////////////
	//
	//	Windows network system
	//
	namespace WSASystem
	{

		// Open network system
		HRESULT OpenSystem( UINT uiOverBufferCount );

		void CloseSystem();

		// Network buffer operation
		//HRESULT AllocBufferTry( OVERLAPPED_BUFFER_WRITE* &pIOBuffer );
		HRESULT AllocBuffer( OVERLAPPED_BUFFER_WRITE* &pIOBuffer );
		HRESULT FreeBuffer( OVERLAPPED_BUFFER_WRITE *pIOBuffer );
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	IO operations
	//

	namespace IOCPSystem
	{
		// NetIO callback interface
		class IOCallBack
		{
		public:
			// called when New connection TCP accepted
			virtual HRESULT OnIOAccept( HRESULT hrRes, OVERLAPPED_BUFFER_ACCEPT *pAcceptInfo ) = 0;

			// called when reciving messag is completed
			virtual HRESULT OnIORecvCompleted( HRESULT hrRes, OVERLAPPED_BUFFER_READ *pIOBuffer, DWORD dwTransferred ) = 0;

			// called when send completed
			virtual HRESULT OnIOSendCompleted( HRESULT hrRes, OVERLAPPED_BUFFER_WRITE *pIOBuffer, DWORD dwTransferred ) = 0;
		};


		
		////////////////////////////////////////////////////////////////////////////////
		//
		//	IOCP thread worker
		//
		class IOCPWorker : public BR::Thread
		{
		private:
			// IOCP handle
			HANDLE m_hIOCP;

		public:
			// Constructor/destructor
			inline IOCPWorker();
			inline ~IOCPWorker();

			inline void SetIOCPHandle( HANDLE hIOCP );

			virtual bool Run();
		};

		
		
		//////////////////////////////////////////////////////////////////
		//
		//	network IOCP System
		//

		class IOCPSystem
		{
		private:
			// IOCP handle
			HANDLE m_hIOCP;

			// IOCP system open reference count
			BR::SyncCounter m_RefCount;

			// IOCP worker thread list
			std::vector<IOCPWorker*> m_pWorkers;

			// global statistic
			BR::SyncCounter m_NumReadWait;
			BR::SyncCounter m_NumWriteWait;

			// Singletone instance
			static IOCPSystem stm_Instance;

		public:
			IOCPSystem();
			~IOCPSystem();

			// GetInstance
			inline static IOCPSystem& GetInstance();


			// Query IOCP handle
			inline HANDLE GetIOCP();

			// Initialize IOCP
			HRESULT InitIOCP( UINT uiNumIOCPThread );

			// Close IOCP
			HRESULT CloseIOCP();
		};


		// Get IOCP System
		IOCPSystem& GetSystem();


	}; // namespace IOCPSystem





#include "NetSystem.inl"

} // namespace Net
} // namespace BR



