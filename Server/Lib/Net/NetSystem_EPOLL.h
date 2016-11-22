////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
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


#if EPOLL



namespace BR {
namespace Net {

	class Connection;
	enum class IOBUFFER_OPERATION : UINT8;



	////////////////////////////////////////////////////////////////////////////////
	//
	//	EPOLL thread worker
	//


	class EPOLLWorker : public Thread
	{
	public:

		enum {
			MAX_EPOLL_EVENTS = 1000,
			MAX_EPOLL_WAIT = 500,
		};

	private:
		// Epoll handle
		int m_hEpoll;
		bool m_HandleSend;

	public:
		// Constructor/destructor
		EPOLLWorker(bool bHandleSend, int hEpoll = 0);
		~EPOLLWorker();

		int GetEpollHandle() {
			return m_hEpoll;
		}

		Result RegisterSocket(INetIOCallBack* cbInstance);
		Result UnregisterSocket(INetIOCallBack* cbInstance);

		virtual void Run() override;


		Result HandleAccept(SOCKET sock, INetIOCallBack* pCallBack);
		Result HandleRW(SOCKET sock, unsigned int events, INetIOCallBack* pCallBack);
	};



	class EPOLLSendWorker : public Thread
	{
	public:

	private:
		// Epoll handle
		
		WriteBufferQueue m_WriteQueue;

	public:
		// Constructor/destructor
		EPOLLSendWorker();
		~EPOLLSendWorker();

		WriteBufferQueue& GetWriteQueue() { return m_WriteQueue; }

		virtual void Run() override;
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	
	//

	class EPOLLSystem
	{
	private:

		static EPOLLSystem stm_Instance;

		EPOLLWorker* m_ListenWorker;
		// workers for TCP
		std::atomic<int> m_iTCPAssignIndex;
		DynamicArray<EPOLLWorker*> m_WorkerTCP;

		// workers for UDP
		EPOLLSendWorker* m_UDPSendWorker;
		DynamicArray<EPOLLWorker*> m_WorkerUDP;

	public:

		EPOLLSystem();

		Result Initialize(UINT netThreadCount);
		void Terminate();

		Result MakeSocketNonBlocking(SOCKET sfd);
		WriteBufferQueue* GetWriteBufferQueue();
		//Result RegisterSharedSocket(SockType sockType, INetIOCallBack* cbInstance);

		// Register the socket to EPOLL
		Result RegisterToNETIO(SockType sockType, INetIOCallBack* cbInstance);
		Result UnregisterFromNETIO(INetIOCallBack* cbInstance);

		const char* EventFlagToString(int32_t bufferSize, char* stringBuffer, uint32_t eventFlags);

		static EPOLLSystem& GetInstance() { return stm_Instance; }
	};

	// Get system instance
	inline EPOLLSystem& GetNetIOSystem()
	{
		return EPOLLSystem::GetInstance();
	}


#include "NetSystem_EPOLL.inl"

} // namespace Net
} // namespace BR


#endif
