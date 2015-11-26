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
#include "Common/MemoryPool.h"
#include "Common/Thread.h"
#include "Common/BrBaseTypes.h"
#include "Net/NetUtil.h"


namespace BR {
namespace Net {

	class Connection;

	
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
		uintptr_t CID;

		// Recv buffer
		char buffer[Const::INTER_PACKET_SIZE_MAX];

		bool bIsPending;

		// constructor
		tag_OVERLAPPED_BUFFER_READ();
		~tag_OVERLAPPED_BUFFER_READ();

		// Initialize for IO
		inline void InitForIO();
		inline void InitRecv( uintptr_t iCID );

		// Setup recving mode
		inline void SetupRecvUDP( uintptr_t iCID );
		inline void SetupRecvPeer( uintptr_t iCID );
		inline void SetupRecvTCP( uintptr_t iCID );
		inline void SetupRecvTCPPending( uintptr_t iCID );

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

			virtual void Run() override;
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



