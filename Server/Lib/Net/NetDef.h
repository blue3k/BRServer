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

			// connection result
			Result hr;
			// State changed
			ConnectionState state;

			Event(void* ptr = nullptr) : EventType(EventTypes::EVT_NONE) { assert(ptr == nullptr); }
			Event(EventTypes eventType, Result hrRes) :EventType(eventType) { hr = hrRes; }
			Event(EventTypes eventType, ConnectionState InState) :EventType(eventType) { state = InState; }
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


		virtual Result SendRaw(Message::MessageData* &pMsg) = 0;

	public:
		//////////////////////////////////////////////////////////////////////////
		//
		//	basic implementation
		//

		// Constructor
		IConnection();
		virtual ~IConnection();

		// setup Net
		inline Result SetupNet( INet *pNetInstance, uintptr_t uiCID );



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
		Result CreatePolicy( UINT uiPolicy );

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

		virtual Result InitConnection(const NetAddress& Addr, NetClass netClass);

		// Disconnect connection
		virtual Result Disconnect(const char* reason) = 0;

		// Close connection immediately without notify
		virtual Result CloseConnection() = 0;

		// Send message to connected entity
		virtual Result Send( Message::MessageData* &pMsg ) = 0;

		// Message count currently in recv queue
		virtual SysUInt GetRecvMessageCount() = 0;

		// Query connection event
		virtual CounterType GetConnectionEventCount() = 0;
		virtual Result DequeueConnectionEvent(Event& curEvent) = 0;

		// Get received Message
		virtual Result GetRecvMessage( Message::MessageData* &pIMsg ) = 0;

		// Update net control, process connection heartbit, ... etc
		virtual Result UpdateNetCtrl(  ) = 0;

		// Update send queue, Reliable UDP
		virtual Result UpdateSendQueue() = 0;
		// Update Send buffer Queue, TCP and UDP client connection
		virtual Result UpdateSendBufferQueue() = 0;
	};



	////////////////////////////////////////////////////////////////////////////////
	//
	//	IConnectionEventHandler Interface
	//

	class IConnectionEventHandler
	{
	public:
		virtual ~IConnectionEventHandler();

		virtual void OnConnectionEvent(IConnection* pConn, const IConnection::Event& evt) = 0;
		virtual Result OnRecvMessage(IConnection* pConn, Message::MessageData* pMsg) = 0;
		virtual Result OnNetSyncMessage(IConnection* pConn);
		// Net send message
		virtual Result OnNetSendReadyMessage(IConnection* pConn);
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
		virtual Result DequeueNetEvent( Event& curEvent ) = 0;

		virtual Result EnqueueNetEvent( Event& curEvent ) = 0;

		// Make a connection to another server
		virtual Result Connect(IConnection* pIConn, UINT remoteID, NetClass netClass, const NetAddress& destAddress) = 0;

		// Release Connection
		virtual Result ReleaseConnection(IConnection* pIConnection) = 0;

		// take over connection management
		virtual Result TakeOverConnection(IConnection* pIConnection) = 0;
	};





	////////////////////////////////////////////////////////////////////////////////
	//
	//	Client network Interface
	//

	class IClient : public INet
	{
	public:
		// Get connection from connection ID
		virtual Result GetConnection( uintptr_t uiCID, SharedPointerT<IConnection> &pIConnection) = 0;

		// Connect to server
		virtual Result ConnectCli(const NetAddress& destAddress, IConnection* &pINewConnection ) = 0;
	};


#include "NetDef.inl"

} // namespace Net



#ifndef SWIG
extern template class SharedPointerT <Net::IConnection>;
#endif


} // namespace BR



