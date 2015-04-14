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
#include "Common/Synchronize.h"
#include "Common/UniqueEntityIDGenerator.h"
#include "Net/NetDef.h"
#include "Net/Connection.h"

#include "ServerSystem/TaskManager.h"


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

		class Sockaddress : public sockaddr_in6
		{
		public:
			Sockaddress() = default;
			Sockaddress(const Sockaddress& src) = default;
			Sockaddress(const sockaddr_in6& src);
			Sockaddress(int value);
			~Sockaddress() = default;

			Sockaddress& operator = (const Sockaddress& src) = default;

			bool operator > (const Sockaddress& src);
			bool operator == (const Sockaddress& src);

			operator const sockaddr_in6& () { return *this; }
		};

		//////////////////////////////////////////////////////////////////
		// Hash definition
		typedef Hash::HashTable2< Sockaddress, WeakPointerT<Connection>, Hash::UniqueKeyTrait, ThreadSyncTraitMT, Hash::hash < sockaddr_in6 >
									//, Indexing::MemData<AddrConMapItem,ULONGLONG,&AddrConMapItem::Addr64>
									> AddrMap;


		typedef Hash::HashTable2<	UINT_PTR, SharedPointerT<Connection>
									//Indexing::ConstMemFunc<IConnection,UINT_PTR,&Net::IConnection::GetCID>
									> CIDMap;

		//typedef CIDMap::iterator CIDIterator;


		typedef Hash::HashTable2<	UINT64, WeakPointerT<Connection>
									//Indexing::ConstMemFunc<IConnection,UINT64,&Net::IConnection::GetPeerID>
									> PeerIDMap;

		// Connection set
		typedef DualSortedMap<UINT_PTR,SharedPointerT<Connection>>	ConnectionSet;


		//////////////////////////////////////////////////////////////////
		// Queue operations
		struct Operation
		{
			static const Operation NullValue;


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
			sockaddr_in6 addrOrg;
			sockaddr_in6 addrNew;

			// Used when UDP connection
			MsgMobileNetCtrl	MobileNetCtrl;
			MsgNetCtrlConnect	NetCtrlConnect;

			ULONG EnqueuedTime;

			// Constructor
			inline Operation();
			inline Operation( Operation&& src );
			inline Operation(const sockaddr_in6& sockAddr, MsgNetCtrlConnect *pNetCtrl);
			inline Operation(OperationCode code, const sockaddr_in6& sockAddr, MsgMobileNetCtrl *pNetCtrl);
			//inline Operation( OperationCode code, const sockaddr_in6& sockAddr, Connection *pCo );
			inline Operation(OperationCode code, const sockaddr_in6& sockAddrOrg, const sockaddr_in6& sockAddrNew, Connection *pCo);
			inline Operation(OperationCode code, AuthTicket ticket, Connection *pCo);
			inline Operation( OperationCode code, Connection *pCo );
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
		BR::PageQueue<Operation>	m_PendingOperations;

		// Managed for Add/Remove connections
		ConnectionSet				m_ManagedConnections;

		ULONG GetAddressHash(const sockaddr_in6& sockAddr);

	protected:

		// mapping Add/Remove
		virtual HRESULT AddMap( Connection *pConn );
		HRESULT AddressRemap(Connection *pConn, const sockaddr_in6 &addressOrg, const sockaddr_in6 &newAddress);
		HRESULT RemoveMap( Connection *pConn );

		// Remap PeerID
		HRESULT RemapPeerID( Connection *pConn, AuthTicket ticket );


		////////////////////////////////////////////////////////////////////////////////
		//
		//	Update functions
		//

		// thread implementation
		virtual bool Run();

		// Update managed connections
		virtual void UpdateManagedConnections();

		// Update Operation queue
		virtual void UpdateOperationQueue();

		// Update Disconnect queue
		//virtual void UpdateDisconnectQueue();


		// Disconnect all connection
		virtual HRESULT DisconnectAllConnection();


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
		virtual HRESULT InitManager( UINT poolCacheCount );

		// Release all connection and terminate manager
		virtual HRESULT TerminateManager();

		// Prevent connection allocation or not
		inline void EnableNewConnection( bool bIsEnable );

		// Active connection
		inline CounterType GetNumActiveConnection();

		// just allocate connection from connection pool for TCP wait
		virtual Connection* NewConnection() = 0;

		// Free connection directly
		virtual void FreeConnection( Connection* pConn ) = 0;


		// Add connection to connecting process
		virtual HRESULT PendingWaitConnection( Connection* pConnection );

		// Add connection to connecting process
		virtual HRESULT PendingConnection( Connection* pConnection );

		// Create new connection from connection pool with UDP address and add to connecting process
		HRESULT PendingNewConnection(const sockaddr_in6& sockAddr, MsgNetCtrlConnect *pNetCtrl);
		HRESULT PendingNewConnection(const sockaddr_in6& sockAddr, MsgMobileNetCtrl *pNetCtrl);

		// Change address mapping of connection
		HRESULT PendingRemapPeerID( Connection* pConnection, AuthTicket ticket );

		// Change address mapping of connection
		HRESULT PendingAddressRemap(Connection* pConnection, const sockaddr_in6& sockAddrOrg, const sockaddr_in6& sockAddrNew);

		// Pending Init connection
		HRESULT PendingInitConnection( Connection* pConnection );

		// Managed connection is taken by other entity
		HRESULT PendingManagedConnectionTakenOver(Connection* pConnection);

		// Close and release connection
		virtual HRESULT PendingReleaseConnection( Connection* pConnection );

		// Wait disconnect and release
		//virtual HRESULT PendingDisconnectNReleaseConnection( Connection* pConnection );

		// Close all current connection
		virtual HRESULT PendingCloseAllConnection();


		// Find and return connection
		HRESULT GetConnectionByAddr(const sockaddr_in6& sockAddr, SharedPointerT<Connection> &pFound);
		HRESULT GetConnectionByCID( UINT_PTR uiCID, SharedPointerT<Connection> &pConn );
		HRESULT GetConnectionByPeerID(UINT64 peerID, SharedPointerT<Connection> &pConn);
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
		virtual HRESULT InitManager( UINT poolCacheCount );

		// create connection
		virtual Connection* NewConnection();
		virtual void FreeConnection( Connection* pConn );
	};
	





#include "ConnectionManager.inl"

} // namespace Net
} // namespace BR



