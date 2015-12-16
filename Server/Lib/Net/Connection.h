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
#include "Net/NetUtil.h"
#include "Net/NetSystem.h"
#include "Common/HRESNet.h"
#include "Common/PageQueue.h"



namespace BR {
namespace Net {





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

			TimeStampMS	ThroughputStartMS;			// Throughput check Start time
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



	protected:

		// Sending wait queue
		MsgQueue			m_SendGuaQueue;


		// NetCtrl control time
		TimeStampMS m_ulNetCtrlTime;
		TimeStampMS m_ulNetCtrlTryTime;

		// Set socket handle
		inline void SetSocket( SOCKET socket );


		// Add network event to queue
		virtual HRESULT EnqueueConnectionEvent( const IConnection::Event& evt );


	public:
		Connection();
		virtual ~Connection();

		// Get socket handle
		inline SOCKET GetSocket() const;

		// Close connections socket handle
		void CloseSocket();

		// Get remote address
		inline const sockaddr_in6& GetRemoteSockAddr() const;

		// Get Recv queue
		MsgQueue& GetRecvQueue();

		MsgQueue& GetSendGuaQueue();

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

	#include "Connection.inl"

}  // namespace Net

	extern template class SharedPointerT <Net::Connection>;
	extern template class WeakPointerT < Net::Connection >;

}; // namespace BR



