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


#if KQUEUE



namespace BR {
namespace Net {

	class Connection;
	enum class IOBUFFER_OPERATION : UINT8;



	////////////////////////////////////////////////////////////////////////////////
	//
	//	KQUEUE thread worker
	//


	class KQUEUEWorker : public Thread
	{
	public:

		enum {
			MAX_KQUEUE_EVENTS = 1000,
			MAX_KQUEUE_WAIT = 500,
		};

	private:
		// KQUEUE handle
		int m_hKQUEUE;
		bool m_HandleSend;

	public:
		// Constructor/destructor
		KQUEUEWorker(bool bHandleSend, int hKQUEUE = 0);

		~KQUEUEWorker();

		int GetKQUEUEHandle() {
			return m_hKQUEUE;
		}

		HRESULT RegisterSocket(INetIOCallBack* cbInstance);
		HRESULT UnregisterSocket(INetIOCallBack* cbInstance);

		virtual void Run() override;


		HRESULT HandleAccept(SOCKET sock, INetIOCallBack* pCallBack);
		HRESULT HandleRW(SOCKET sock, unsigned int events, INetIOCallBack* pCallBack);
	};



	class KQUEUESendWorker : public Thread
	{
	public:

	private:
		// KQUEUE handle
		
		WriteBufferQueue m_WriteQueue;

	public:
		// Constructor/destructor
		KQUEUESendWorker();

		~KQUEUESendWorker();

		WriteBufferQueue& GetWriteQueue() { return m_WriteQueue; }

		virtual void Run() override;
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	
	//

	class KQUEUESystem
	{
	private:

		static KQUEUESystem stm_Instance;

		KQUEUEWorker* m_ListenWorker;
		// workers for TCP
		std::atomic<int> m_iTCPAssignIndex;
		DynamicArray<KQUEUEWorker*> m_WorkerTCP;

		// workers for UDP
		KQUEUESendWorker* m_UDPSendWorker;
		DynamicArray<KQUEUEWorker*> m_WorkerUDP;

	public:

		KQUEUESystem();

		HRESULT Initialize(UINT netThreadCount);
		void Terminate();

		HRESULT MakeSocketNonBlocking(SOCKET sfd);
		WriteBufferQueue* GetWriteBufferQueue();
		HRESULT RegisterSharedSocket(SockType sockType, INetIOCallBack* cbInstance);

		// Register the socket to KQUEUE
		HRESULT RegisterToNETIO(SockType sockType, INetIOCallBack* cbInstance);
		HRESULT UnregisterFromNETIO(SockType sockType, INetIOCallBack* cbInstance);


		static KQUEUESystem& GetInstance() { return stm_Instance; }
	};

	// Get system instance
	inline KQUEUESystem& GetNetIOSystem()
	{
		return KQUEUESystem::GetInstance();
	}


#include "NetSystem_KQUEUE.inl"

} // namespace Net
} // namespace BR


#endif