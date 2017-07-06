////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author : KyungKun Ko
//
// Description : Network connection manager
//	
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/HashTable2.h"
#include "Common/Thread.h"
#include "Common/Synchronization.h"
#include "Common/UniqueEntityIDGenerator.h"
#include "Net/NetDef.h"
#include "Net/Connection.h"
#include "Net/ConnectionTCP.h"
#include "Net/ConnectionUDP.h"
#include "Common/Task/TaskManager.h"


namespace BR {
namespace Net {


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Network address to connection map
	//

	class ConnectionManager : public Thread
	{
	public:

		////////////////////////////////////////////////////////////////////////////////
		//
		//	Address to connection map
		//

		class Sockaddress : public sockaddr_storage
		{
		public:
			Sockaddress() = default;
			Sockaddress(const Sockaddress& src) = default;
			Sockaddress(const sockaddr_storage& src);
			Sockaddress(int value);
			~Sockaddress() = default;

			Sockaddress& operator = (const Sockaddress& src) = default;

			bool operator > (const Sockaddress& src);
			bool operator == (const Sockaddress& src);
			bool operator != (const Sockaddress& src);

			//operator const sockaddr_storage& () { return *this; }
		};

		//////////////////////////////////////////////////////////////////
		// Hash definition
		typedef Hash::HashTable2< Sockaddress, WeakPointerT<Connection>, Hash::UniqueKeyTrait, ThreadSyncTraitReadWriteT<Sockaddress, WeakPointerT<Connection>>, Hash::hash < sockaddr_storage >
									> AddrMap;


		typedef Hash::HashTable2<	uintptr_t, SharedPointerT<Connection>
									> CIDMap;


		typedef Hash::HashTable2<	UINT64, WeakPointerT<Connection>
									> PeerIDMap;

		// Connection set
		typedef DualSortedMap<uintptr_t,SharedPointerT<Connection>>	ConnectionSet;


		//////////////////////////////////////////////////////////////////
		// Queue operations
		struct Operation
		{
			// Operation code
			enum OperationCode : UINT32
			{
				OP_NONE,
				OP_WAITING_CONNECTION,
				OP_PENDING_CONNECTION,
				OP_RELEASE_CONNECTION,
				OP_PENDING_INITCONNECTION,
				OP_PENDING_ADDR,
				OP_PENDING_MOBILECONNECTION,
				OP_PENDING_MOBILEREMAP,
				OP_PENDING_REMAP_PEERID,
				OP_CLOSEALL_CONNECTION,
				OP_MANAGED_CONNECTION_TAKENOVER,
			};
			std::atomic<OperationCode> OpCode;


			// address or connection data
			SharedPointerT<Connection> pConn;
			sockaddr_storage addrOrg;
			sockaddr_storage addrNew;

			// Used when UDP connection
			MsgMobileNetCtrl	MobileNetCtrl;
			MsgNetCtrlConnect	NetCtrlConnect;

			TimeStampMS EnqueuedTime;

			// Constructor
			inline Operation(void* ptr = nullptr);
			inline Operation( Operation&& src );
			inline Operation(const sockaddr_storage& sockAddr, MsgNetCtrlConnect *pNetCtrl);
			inline Operation(OperationCode code, const sockaddr_storage& sockAddr, MsgMobileNetCtrl *pNetCtrl);
			inline Operation(OperationCode code, const sockaddr_storage& sockAddrOrg, const sockaddr_storage& sockAddrNew, ConnectionPtr& pCo);
			inline Operation(OperationCode code, AuthTicket ticket, ConnectionPtr& pCo);
			inline Operation( OperationCode code, ConnectionPtr& pCo );
			inline ~Operation();

			inline void Clear();

			// Copy operator
			Operation& operator = ( const Operation& src );
			Operation& operator = (Operation&& src);

			// These operation is used for empty value os just compare with OpCode
			inline bool operator != ( const Operation& src ) const;
			inline bool operator == ( const Operation& src ) const;
		};

	private:
		// Address map
		AddrMap		m_AddrMap;

		// Use address map
		bool		m_UseAddressMap;
		bool		m_UsePeerIDMap;

		// Connection ID map
		CIDMap		m_CIDMap;

		// Peer ID map
		PeerIDMap	m_PeerIDMap;

		// Net interface that owner of this manager
		INet		*m_pINetOwner;

		// CID generator
		UniqueEntityIDGenerator	m_CIDGen;


	protected:
		// No new connection
		bool		m_bNoNewConnection;

		// Add/Pending/Release operation queue
		PageQueue<Operation>	m_PendingOperations;

		// Managed for Add/Remove connections
		ConnectionSet				m_ManagedConnections;

		//ULONG GetAddressHash(const sockaddr_in6& sockAddr);

	protected:

		// mapping Add/Remove
		virtual Result AddMap( Connection *pConn );
		Result AddressRemap(Connection *pConn, const sockaddr_storage &addressOrg, const sockaddr_storage &newAddress);
		Result RemoveMap( Connection *pConn );

		// Remap PeerID
		Result RemapPeerID( Connection *pConn, AuthTicket ticket );


		////////////////////////////////////////////////////////////////////////////////
		//
		//	Update functions
		//

		// thread implementation
		virtual void Run() override;

		// Update managed connections
		virtual void UpdateManagedConnections();

		// Update Operation queue
		virtual void UpdateOperationQueue();

		// Update Disconnect queue
		//virtual void UpdateDisconnectQueue();


		// Disconnect all connection
		virtual Result DisconnectAllConnection();


	public:
		// Constructor/Destructor
		// @param uiBucketSize : Hashmap bucket size
		ConnectionManager( UINT uiBucketSize );
		virtual ~ConnectionManager();

		// Get net owner
		inline void SetNetOwner( INet *pOwner );
		inline INet* GetNetOwner();

		inline void SetUseAddressMap(bool bUseAddressMap);
		inline void SetPeerIDMap(bool bUsePeerIDMap);

		// CID generation
		inline UINT NewCID();
		inline void FreeCID( UINT uiCID );

		// Initialize Manager
		virtual Result InitManager( UINT poolCacheCount );

		// Release all connection and terminate manager
		virtual Result TerminateManager();

		// Prevent connection allocation or not
		inline void EnableNewConnection( bool bIsEnable );

		// Active connection
		inline CounterType GetNumActiveConnection();

		// just allocate connection from connection pool for TCP wait
		virtual Connection* NewConnection() = 0;

		// Free connection directly
		virtual void FreeConnection(ConnectionPtr& pConn ) = 0;


		// Add connection to connecting process
		virtual Result PendingWaitConnection(ConnectionPtr& pConnection );

		// Add connection to connecting process
		virtual Result PendingConnection(ConnectionPtr& pConnection );

		// Create new connection from connection pool with UDP address and add to connecting process
		Result PendingNewConnection(const sockaddr_storage& sockAddr, MsgNetCtrlConnect *pNetCtrl);
		Result PendingNewConnection(const sockaddr_storage& sockAddr, MsgMobileNetCtrl *pNetCtrl);

		// Change address mapping of connection
		Result PendingRemapPeerID(ConnectionPtr& pConnection, AuthTicket ticket );

		// Change address mapping of connection
		Result PendingAddressRemap(ConnectionPtr& pConnection, const sockaddr_storage& sockAddrOrg, const sockaddr_storage& sockAddrNew);

		// Pending Init connection
		Result PendingInitConnection(ConnectionPtr& pConnection );

		// Managed connection is taken by other entity
		Result PendingManagedConnectionTakenOver(ConnectionPtr& pConnection);

		// Close and release connection
		virtual Result PendingReleaseConnection( ConnectionPtr& pConnection );

		// Wait disconnect and release
		//virtual Result PendingDisconnectNReleaseConnection( Connection* pConnection );

		// Close all current connection
		virtual Result PendingCloseAllConnection();


		// Find and return connection
		Result GetConnectionByAddr(const sockaddr_storage& sockAddr, SharedPointerT<Connection> &pFound);
		Result GetConnectionByCID( uintptr_t uiCID, SharedPointerT<Connection> &pConn );
		Result GetConnectionByPeerID(UINT64 peerID, SharedPointerT<Connection> &pConn);
	};




	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ConnectionManagerT
	//

	template< class ConnectionType >
	class ConnectionManagerT : public ConnectionManager
	{
	private:

	public:
		// Constructor
		ConnectionManagerT( UINT uiBucketSize );

		// Initialize Manager
		virtual Result InitManager( UINT poolCacheCount );

		// create connection
		virtual Connection* NewConnection();
		virtual void FreeConnection( ConnectionPtr& pConn );
	};
	





#include "ConnectionManager.inl"

} // namespace Net
} // namespace BR



