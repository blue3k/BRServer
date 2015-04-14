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
#include "Net/NetMessage.h"
#include "Net/NetSystem.h"
#include "Common/HRESNet.h"
#include "Common/PageQueue.h"



namespace BR {
namespace Net {



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Packet message queue class
	//

	class MsgQueue : public BR::PageQueue<Message::MessageData*>
	{
	public:
		MsgQueue( UINT uiNumElePerPage = 512 );
		virtual ~MsgQueue();

		// Clear queue element
		virtual void ClearQueue();
	};



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network connection base class
	//

	class Connection : public IConnection
	{
	public:
		////////////////////////////////////////////////////////////////////////////////
		//
		//	Connection statistic calculation state
		//

		typedef struct tag_StatisticCalc : public Statistic
		{
			ULONG	PingStartMS;				// Ping start time to calculate roundtrip time

			ULONG	ThroughputStartMS;			// Throughput check Start time
			ULONG	ThroughputBytes;			// Packet data size sum

		} StatisticCalc;

	protected:

		// Remote Socket address
		struct sockaddr_in6	m_sockAddrRemote;

		std::atomic<LONG> m_PendingSend;


	private:

		// Connection socket
		SOCKET	m_socket;

		// Recv Message Queue
		MsgQueue m_RecvQueue;

		// Event queue
		PageQueue<IConnection::Event>	m_EventQueue;

		// heart bit time
		ULONG	m_ulHeartbitTry;
		ULONG	m_ulConnectingTimeOut;

		// Sequence 
		std::atomic<USHORT> m_usSeqNone;

		// Statistics
		StatisticCalc	m_Statistic;

		// ZeroLength recv count
		ULONG m_ulZeroLengthRecvCount;

		// Recv pending count
		std::atomic<LONG> m_lPendingRecvCount;

		// Pending Job count
		//SyncCounter m_PendingJobCount;


	protected:

		// NetCtrl control time
		ULONG m_ulNetCtrlTime;
		ULONG m_ulNetCtrlTryTime;

		// Set socket handle
		inline void SetSocket( SOCKET socket );


		// Add network event to queue
		virtual HRESULT EnqueueConnectionEvent( const IConnection::Event& evt );


	public:
		Connection();
		virtual ~Connection();

		//FORCEINLINE SyncCounter& GetPendingJobCounter();

		// from iSharedObj
		//virtual void DeleteThis() const;

		// Get socket handle
		inline SOCKET GetSocket() const;

		// Close connections socket handle
		inline void CloseSocket();

		// Get remote address
		inline const sockaddr_in6& GetRemoteSockAddr() const;

		// Get Recv queue
		inline MsgQueue& GetRecvQueue();

		// Get zero recv count
		inline ULONG GetZeroRecvCount();
		inline void IncZeroRecvCount();
		inline void ResetZeroRecvCount();

		// Get pending recv count
		inline LONG GetPendingRecvCount();
		inline void IncPendingRecvCount();
		inline void DecPendingRecvCount();
		inline void ResetPendingRecvCount();

		LONG GetPendingSendCount();

		// Sequence generation
		inline USHORT NewSeqNone();

		// Heartbit time
		inline ULONG GetHeartbitTry();
		inline void SetHeartbitTry( ULONG ulHeartbitTry );

		inline ULONG GetConnectingTimeOut();
		inline void SetConnectingTimeOut( ULONG ulConnectingTimeOut );

		// Make NetCtrl packet and send
		virtual HRESULT SendNetCtrl( UINT uiCtrlCode, UINT uiSequence, Message::MessageID msgID, UINT64 UID = 0 );

		// Clear Queue
		virtual HRESULT ClearQueues();

		// Message count currently in recv queue
		virtual SysUInt GetRecvMessageCount();

		// Called on connection result
		virtual void OnConnectionResult( HRESULT hrConnect );

		// Initialize connection
		virtual HRESULT InitConnection( SOCKET socket, const ConnectionInformation &connectInfo );

		// Initialize packet synchronization
		virtual HRESULT InitSynchronization();

		// Disconnect connection
		virtual HRESULT Disconnect();

		// Close connection
		virtual HRESULT CloseConnection();


		// Get Recived message
		virtual HRESULT GetRecv( Message::MessageData* &pMsg );

		// called when incomming message occure
		virtual HRESULT OnRecv( UINT uiBuffSize, const BYTE* pBuff ) = 0;
		virtual HRESULT OnRecv( Message::MessageData *pMsg );

		
		// Query connection event
		CounterType GetConnectionEventCount();
		HRESULT DequeueConnectionEvent(Event& curEvent);

		// Get received Message
		virtual HRESULT GetRecvMessage( Message::MessageData* &pIMsg );

		// Print Debug message
		static void PrintDebugMessage( const char* strPrefix, Message::MessageData *pMsg );
	};

	extern template class SharedPointerT <Connection>;
	extern template class WeakPointerT < Connection > ;

	#include "Connection.inl"

}  // namespace Net
}; // namespace BR



