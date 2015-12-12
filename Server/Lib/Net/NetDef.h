////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : KyungKun Ko
//
// Description : Net base type definitions. 
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/BrBaseTypes.h"
#include "Common/PolicyID.h"
#include "Common/Message.h"
#include "Common/SharedObject.h"
#include "Common/SharedPointer.h"
#include "Net/NetCtrlIDs.h"
#if !WINDOWS
#include <arpa/inet.h>
#include <netdb.h>
#endif


namespace BR {
namespace Net {
	

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Connection Interface
	//
#if !WINDOWS
	#define SOCKET_ERROR (-1)
#endif

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Connection Interface
	//

	class INet;
	class IConnection;
	class IConnectionEventHandler;

	class IConnection : public SharedObject
	{
	public:
		////////////////////////////////////////////////////////////////////////////////
		//
		//	connection state definitions
		//

		// Connection state
		enum ConnectionState {
			STATE_NONE,				// None just created
			STATE_WAITING,			// Waiting accept
			STATE_UDP_WAITING,		// UDP incomming Waiting
			STATE_CONNECTING,		// Connecting sequence
			STATE_CHECKVERSION,		// Checking protocol version
			STATE_CONNECTED,		// Connected 
			STATE_DISCONNECTING,	// Disconnecting
			STATE_DISCONNECTED,		// Disconnected
			STATE_SLEEP,			// For Mobile connection
		};


		////////////////////////////////////////////////////////////////////////////////
		//
		//	Event definitions
		//

		struct Event
		{
			// Type of connection event
			enum EventTypes
			{
				EVT_NONE,
				EVT_CONNECTION_RESULT,
				EVT_DISCONNECTED,
				EVT_STATE_CHANGE,
				EVT_ADDRESS_REMAPPED,
			};
			EventTypes		EventType;

			union {
				// connection result
				HRESULT hr;
				// State changed
				ConnectionState state;
			} Value;

			Event(void* ptr = nullptr)									{ assert(ptr == nullptr); }
			Event(EventTypes eventType, HRESULT hrRes) :EventType(eventType) { Value.hr = hrRes; }
			Event(EventTypes eventType, ConnectionState InState) :EventType(eventType) { Value.state = InState; }
			Event& operator =(const Event& src);
			bool operator == (const Event& src) const;
			bool operator != (const Event& src) const;
		};



		////////////////////////////////////////////////////////////////////////////////
		//
		//	Connection information
		//

		struct ConnectionInformation
		{
			// Local address
			NetAddress		Local;
			NetClass		LocalClass;

			// Local identification ID
			UINT64			LocalID;

			// Remote address
			NetAddress		Remote;
			NetClass		RemoteClass;

			// Remote identification ID
			UINT64			RemoteID;


			// set local info
			void SetLocalInfo( NetClass Class, const NetAddress& Addr, UINT64 UID );

			// set remote info
			void SetRemoteInfo( NetClass Class, UINT64 UID );
		};


		////////////////////////////////////////////////////////////////////////////////
		//
		//	Connection statistic
		//

		typedef struct tag_Statistic
		{
			UINT32 RoundTripLatencyMS;			// Milisecond
			UINT32 ThroughputBPS;				// Byte/Sec
			UINT32 PeakThroughputBPS;			// Byte/Sec

			UINT32 PacketsSentGuaranteed;		// Retry, Guaranteed, Sorted only
			UINT32 PacketsSentNonGuaranteed;	// Non Guaranteed only

			UINT32 PacketsRetried;				// Retry, Guaranteed, Sorted only
			UINT32 PacketsDropped;				// Non Guaranteed only

			UINT32 MessagesReceived;			// Total received messages

		} Statistic;


	private:
		//////////////////////////////////////////////////////////////////////////
		//
		//	basic members implementations
		//


		// Connection ID
		uintptr_t	m_CID;

		// User data
		uintptr_t	m_UData;

		// Connection state
		ConnectionState	m_ConnectionState;

		// Policy processor
		IPolicyBase* m_pPolicy[POLICY_NETMAX];

		// Connection time
		TimeStampMS	m_tConnectionTime;

		// Network interface
		INet	*m_pINet;

		IConnectionEventHandler *m_pEventHandler;

	protected:
		void SetConnectionState(ConnectionState newState);
		void UpdateConnectionTime();

		// Connection information
		ConnectionInformation	m_ConnectInfo;

	public:
		//////////////////////////////////////////////////////////////////////////
		//
		//	basic implementation
		//

		// Constructor
		IConnection();
		virtual ~IConnection();

		// setup Net
		inline HRESULT SetupNet( INet *pNetInstance, uintptr_t uiCID );



		//////////////////////////////////////////////////////////////////////////
		//
		//	inline implementations
		//

		// Get Connection info
		const ConnectionInformation& GetConnectionInfo() const;
		void SetRemoteID(UINT64 newID);


		// Get Connection ID
		uintptr_t GetCID() const;
		void ClearCID();
		
		// Get Connection PeerID
		inline UINT64 GetPeerID() const;


		// Get connection state
		const ConnectionState GetConnectionState() const;

		// Set local class
		inline void SetLocalClass( NetClass uiLocalClass );


		// Get Policy
		template< typename IPolicyType >
		inline IPolicyType* GetPolicy();

		template< typename IPolicyType >
		inline IPolicyType* GetPolicyByID( UINT policyID );

		// Create policy if not exist
		HRESULT CreatePolicy( UINT uiPolicy );

		// User data
		uintptr_t GetUData();
		void SetUData(uintptr_t UData);

		// Get connection time
		TimeStampMS GetConnectionTime();

		// Get Net Instance
		INet* GetNet();

		IConnectionEventHandler* GetEventHandler();
		void SetEventHandler(IConnectionEventHandler* pEventHandler);

		// Connection tvent handler
		void SetConnectionEventHandler(IConnectionEventHandler *pEventHandler) { m_pEventHandler = pEventHandler; }


		//////////////////////////////////////////////////////////////////////////
		//
		//	virtual implementations
		//

		virtual HRESULT InitConnection(const NetAddress& Addr, NetClass netClass);

		// Disconnect connection
		virtual HRESULT Disconnect() = 0;

		// Close connection immediately without notify
		virtual HRESULT CloseConnection() = 0;


		// Send message to connected entity
		virtual HRESULT Send( Message::MessageData* &pMsg ) = 0;

		// Message count currently in recv queue
		virtual SysUInt GetRecvMessageCount() = 0;

		// Query connection event
		virtual CounterType GetConnectionEventCount() = 0;
		virtual HRESULT DequeueConnectionEvent(Event& curEvent) = 0;

		// Get received Message
		virtual HRESULT GetRecvMessage( Message::MessageData* &pIMsg ) = 0;

		// Update net control, process connection heartbit, ... etc
		virtual HRESULT UpdateNetCtrl(  ) = 0;

	};



	class IConnectionEventHandler
	{
	public:
		virtual void OnConnectionEvent(IConnection* pConn, const IConnection::Event& evt) = 0;
		virtual HRESULT OnRecvMessage(IConnection* pConn, Message::MessageData* pMsg) = 0;
		virtual HRESULT OnNetSyncMessage(IConnection* pConn) = 0;
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Interface
	//

	class INet
	{
	public:
		////////////////////////////////////////////////////////////////////////////////
		//
		//	Event definitions
		//

		struct Event
		{

			// Type of connection event
			enum EventTypes
			{
				EVT_NONE,
				EVT_NET_INITIALIZED,
				EVT_NET_CLOSED,
				EVT_NEW_CONNECTION,
				EVT_CONNECTION_DISCONNECTED,
				EVT_MAX
			};
			EventTypes		EventType;

			// ID value that associated with event
			IConnection		*EventConnection;


			inline Event(void* ptr = nullptr);
			inline Event( EventTypes eventType, IConnection *pCon = NULL );
			inline Event& operator =( const Event& src );

			inline bool operator == (const Event& src) const;
			inline bool operator != (const Event& src) const;
		};



		///////////////////////////////////////////////////////////////////////////////////
		//
		//	Net class definition
		//

		// Net class
		NetClass m_NetClass;

		// Get network class
		inline NetClass GetNetClass() const;


		///////////////////////////////////////////////////////////////////////////////////
		//
		//	Implementations
		//

		// Constructor
		INet();
		virtual ~INet()	{}


		// Release instance
		virtual void Release() = 0;

		// check about initialize
		virtual bool IsReady() = 0;

		// Query Network event
		virtual HRESULT DequeueNetEvent( Event& curEvent ) = 0;

		virtual HRESULT EnqueueNetEvent( Event& curEvent ) = 0;

		// Make a connection to another server
		virtual HRESULT Connect(IConnection* pIConn, UINT remoteID, NetClass netClass, const char *strDstIP, USHORT usDstPort) = 0;

		// Release Connection
		virtual HRESULT ReleaseConnection(IConnection* pIConnection) = 0;

		// take over connection management
		virtual HRESULT TakeOverConnection(IConnection* pIConnection) = 0;


		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Used by connection

		// Send message to connection with network device
		virtual HRESULT SendMsg( IConnection *pConnection, Message::MessageData *pMsg ) = 0;

		// Send message to connection with network device to dst addr
		virtual HRESULT SendMsg( IConnection *pConnection, const sockaddr_in6& dstAddr, Message::MessageData *pMsg ) = 0;

		// Send array of message buffer to connection with network device
		virtual HRESULT SendMsg( IConnection *pConnection, UINT uiBuffSize, BYTE* pBuff ) = 0;
	};





	////////////////////////////////////////////////////////////////////////////////
	//
	//	Client network Interface
	//

	class IClient : public INet
	{
	public:
		// Get connection from connection ID
		virtual HRESULT GetConnection( uintptr_t uiCID, IConnection* &pConnection ) = 0;

		// Connect to server
		virtual HRESULT Connect( const char *strServerIP, USHORT usServerPort, IConnection* &pINewConnection ) = 0;
		//virtual HRESULT Connect(IConnection* pIConn, UINT remoteID, NetClass netClass, const char *strDstIP, USHORT usDstPort) { return E_NOTIMPL; }
	};


#include "NetDef.inl"

} // namespace Net



#ifndef SWIG
extern template class SharedPointerT <Net::IConnection>;
#endif


} // namespace BR



