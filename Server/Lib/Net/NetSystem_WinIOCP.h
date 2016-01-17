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

#if WINDOWS

#include "Common/Typedefs.h"
#include "Common/StackWalker.h"
#include "Net/NetDef.h"
#include "Net/NetConst.h"

namespace BR {
namespace Net {

	class Connection;
	enum class IOBUFFER_OPERATION : UINT8;
	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Overlapped I/O structures
	//

	struct IOBUFFER : public WSAOVERLAPPED
	{
		IOBUFFER_OPERATION Operation;

		// Clear Buffer
		void ClearBuffer();

	} ;


	// UDP/TCP read/write overlapped base
	struct IOBUFFER_RWBASE : public IOBUFFER
	{
		// IOCP buffer 
		WSABUF	wsaBuff;

		// Transferred buffer size
		DWORD TransferredSize;

		union {
			// UDP Read from
			struct sockaddr_storage From;
			struct sockaddr_storage To;
		} NetAddr;


		// Constructor
		IOBUFFER_RWBASE();


	} ;


	// UDP/TCP write overlapped
	struct IOBUFFER_WRITE : public IOBUFFER_RWBASE, public MemoryPoolObject<IOBUFFER_WRITE>
	{
		// Message pointer to send
		Message::MessageData *pMsgs;

		// Message buffer pointer to send
		BYTE *pSendBuff;

		// Constructor
		IOBUFFER_WRITE();
		~IOBUFFER_WRITE();

		// Initialize for IO
		inline void InitForIO(SOCKET sockWrite);
		inline void InitMsg( Message::MessageData *pMsg );
		inline void InitBuff( UINT uiBuffSize, BYTE* pBuff );

		// Setup sending mode
		inline void SetupSendUDP(SOCKET sockWrite, const sockaddr_storage& to, Message::MessageData *pMsg );
		inline void SetupSendUDP(SOCKET sockWrite, const sockaddr_storage& to, UINT uiBuffSize, BYTE* pBuff );
		inline void SetupSendTCP( Message::MessageData *pMsg );
		inline void SetupSendTCP( UINT uiBuffSize, BYTE* pBuff );

	};


	// UDP/TCP read overlapped
	struct IOBUFFER_READ : public IOBUFFER_RWBASE, public MemoryPoolObject<IOBUFFER_READ>
	{
		// Read flag
		DWORD dwFlags;
		DWORD dwNumberOfByte;

		// UDP Recv socket length
		INT iSockLen;

		// Recv connection ID for error check
		uintptr_t CID;

		// Recv buffer
		char buffer[Const::INTER_PACKET_SIZE_MAX];

		// Mark wheather this buffer is in use
		std::atomic<bool> bIsPending;
		//CallStackTrace PendingTrace;


		// constructor
		IOBUFFER_READ();
		~IOBUFFER_READ();

		// Initialize for IO
		inline void InitForIO();
		inline void InitRecv( uintptr_t iCID );

		// Setup recving mode
		inline void SetupRecvUDP( uintptr_t iCID );
		inline void SetupRecvTCP( uintptr_t iCID );

		HRESULT SetPendingTrue();
		HRESULT SetPendingFalse();

	};


	// TCP accept overlapped
	struct IOBUFFER_ACCEPT : public IOBUFFER, public MemoryPoolObject<IOBUFFER_ACCEPT>
	{
		//Connection	*pConnection;
		SOCKET sockAccept;
		BYTE pAcceptInfo[sizeof(sockaddr_storage)*2];
		DWORD dwByteReceived;

		// Constructor
		IOBUFFER_ACCEPT();
		~IOBUFFER_ACCEPT();

		// Setup accept
		inline void SetupAccept( SOCKET sock );

	} ;





	////////////////////////////////////////////////////////////////////////////////
	//
	//	IO operations
	//

	namespace IOCPSystem
	{
		
		////////////////////////////////////////////////////////////////////////////////
		//
		//	IOCP thread worker
		//
		class IOCPWorker : public Thread
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
			SyncCounter m_RefCount;

			// IOCP worker thread list
			std::vector<IOCPWorker*> m_pWorkers;

			// global statistic
			SyncCounter m_NumReadWait;
			SyncCounter m_NumWriteWait;

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



#include "NetSystem_WinIOCP.inl"

} // namespace Net
} // namespace BR

#endif

