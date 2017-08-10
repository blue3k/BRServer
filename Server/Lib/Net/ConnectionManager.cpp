////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Network connection manager
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/ResultCode/BRResultCodeNet.h"
#include "Common/Thread.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Net/Connection.h"
#include "Net/ConnectionUDP.h"
#include "Net/NetTrace.h"
#include "Net/NetDef.h"
#include "Net/ConnectionManager.h"
#include "Net/NetServer.h"





namespace BR {
namespace Net {



	ConnectionManager::Operation& ConnectionManager::Operation::operator = (const ConnectionManager::Operation& src)
	{
		addrOrg = src.addrOrg;
		addrNew = src.addrNew;
		pConn = src.pConn;
		MobileNetCtrl = src.MobileNetCtrl;
		NetCtrlConnect = src.NetCtrlConnect;
		EnqueuedTime = src.EnqueuedTime;

		OpCode.store(src.OpCode.load(std::memory_order_relaxed), std::memory_order_release);

		return *this;
	}

	ConnectionManager::Operation& ConnectionManager::Operation::operator = (ConnectionManager::Operation&& src)
	{
		addrOrg = src.addrOrg;
		addrNew = src.addrNew;
		pConn = std::forward<SharedPointerT<Connection>>(src.pConn);
		MobileNetCtrl = src.MobileNetCtrl;
		NetCtrlConnect = src.NetCtrlConnect;
		EnqueuedTime = src.EnqueuedTime;

		OpCode.store(src.OpCode.load(std::memory_order_relaxed), std::memory_order_release);

		return *this;
	}



	ConnectionManager::Sockaddress::Sockaddress(const sockaddr_storage& src)
	{
		memcpy(this, &src, sizeof(src));
	}

	ConnectionManager::Sockaddress::Sockaddress(int value)
	{
		memset(this, 0, sizeof(sockaddr_storage));
	}

	bool ConnectionManager::Sockaddress::operator > (const Sockaddress& src)
	{
		if (ss_family > src.ss_family) return true;

		if (ss_family == AF_INET6)
		{
			auto op1 = (sockaddr_in6*)this;
			auto op2 = (sockaddr_in6*)&src;

#if WINDOWS
			auto& rawAddress = op1->sin6_addr.s6_addr;
			auto& rawAddressSrc = op2->sin6_addr.s6_addr;
			for (UINT iAddr = 0; iAddr < countof(rawAddress); iAddr++)
				if (rawAddress[iAddr] > rawAddressSrc[iAddr]) return true;
#elif LINUX
			auto& rawAddress = op1->sin6_addr.s6_addr32;
			auto& rawAddressSrc = op2->sin6_addr.s6_addr32;
			for (UINT iAddr = 0; iAddr < countof(rawAddress); iAddr++)
				if (rawAddress[iAddr] > rawAddressSrc[iAddr]) return true;
#endif
			if (op1->sin6_port > op2->sin6_port) return true;
		}
		else
		{
			auto op1 = (sockaddr_in*)this;
			auto op2 = (sockaddr_in*)&src;

			if (op1->sin_addr.s_addr > op2->sin_addr.s_addr) return true;
			if (op1->sin_port > op2->sin_port) return true;
		}


		return false;
	}

	bool ConnectionManager::Sockaddress::operator == (const Sockaddress& src)
	{
		if (ss_family != src.ss_family) return false;

		if (ss_family == AF_INET6)
		{
			auto op1 = (sockaddr_in6*)this;
			auto op2 = (sockaddr_in6*)&src;

#if WINDOWS
			auto& rawAddress = op1->sin6_addr.s6_addr;
			auto& rawAddressSrc = op2->sin6_addr.s6_addr;
			for (UINT iAddr = 0; iAddr < countof(rawAddress); iAddr++)
				if (rawAddress[iAddr] != rawAddressSrc[iAddr]) return false;
#else
			auto& rawAddress = op1->sin6_addr.s6_addr32;
			auto& rawAddressSrc = op2->sin6_addr.s6_addr32;
			for (UINT iAddr = 0; iAddr < countof(rawAddress); iAddr++)
				if (rawAddress[iAddr] != rawAddressSrc[iAddr]) return false;
#endif

			if (op1->sin6_port != op2->sin6_port) return false;
		}
		else
		{
			auto op1 = (sockaddr_in*)this;
			auto op2 = (sockaddr_in*)&src;

			if (op1->sin_addr.s_addr != op2->sin_addr.s_addr) return false;
			if (op1->sin_port != op2->sin_port) return false;
		}

		return true;
	}

	bool ConnectionManager::Sockaddress::operator != (const Sockaddress& src)
	{
		return !((*this) == src);
	}


	////////////////////////////////////////////////////////////////////////////////
	//
	//	ConnectionManager
	//


	ConnectionManager::ConnectionManager( UINT uiBucketSize )
		: m_AddrMap(uiBucketSize)
		, m_UseAddressMap(true)
		, m_UsePeerIDMap(true)
		, m_pINetOwner(nullptr)
		, m_bNoNewConnection(true)
	{
	}

	ConnectionManager::~ConnectionManager()
	{
		m_AddrMap.Clear();
		m_CIDMap.Clear();
	}


	// thread implementation
	void ConnectionManager::Run()
	{
		//Util::Time.UpdateTimer();

		while( 1 )
		{
			auto loopInterval = UpdateInterval(DurationMS(Const::CONMGR_THREAD_INTERVAL) );

			if( CheckKillEvent(loopInterval) ) 
			{
				// Kill Event signaled
				break;
			}

			UpdateOperationQueue();

			UpdateManagedConnections();
		}
	}


	// Update managed connections
	void ConnectionManager::UpdateManagedConnections()
	{
		m_ManagedConnections.CommitChanges();

		// Update Managed connections
		m_ManagedConnections.ForeachOrder(0, (UINT)m_ManagedConnections.GetItemCount(), [&](const uintptr_t& key, const ConnectionPtr& pConn)->bool
		{
			//SharedPointerT<Connection> pConn = *itCur;
			if( !(pConn->UpdateNetCtrl()) )
			{
				netTrace( TRC_CONNECTION, "Managed Connection Update failed CID:{0}", pConn->GetCID() );
			}

			switch (pConn->GetConnectionState())
			{
			case IConnection::STATE_DISCONNECTED:
			{
				auto pendingRecv = pConn->GetPendingRecvCount();
				auto pendingSend = pConn->GetPendingSendCount();

				if ((pendingRecv + pendingSend) == 0)
				{
					ConnectionPtr temp = pConn;
					PendingReleaseConnection(temp);
					//SharedPointerT<Connection> pOrg;
					//m_ManagedConnections.Remove(key, pOrg);
					//Assert(pConn == pOrg);
					////itCur = m_ManagedConnections.erase(itCur);
					//netTrace(TRC_CONNECTION, "Managed Connection Disconnected Free CID:{0}", pConn->GetCID());
					//FreeConnection((Connection*)pConn);
				}
			}
				break;
			case IConnection::STATE_CONNECTED:
				// removing from here must be explicit now
				break;
			default:
				//itCur++;
				break;
			};

			return true;
		});
	}


	// Update Operation queue
	void ConnectionManager::UpdateOperationQueue()
	{
		Result hr = ResultCode::SUCCESS;
		Operation oper;

		auto loopCount = m_PendingOperations.GetEnqueCount();
		for (decltype(loopCount) iLoop = 0; iLoop < loopCount && (m_PendingOperations.Dequeue(oper)); iLoop++)
		{
			SharedPointerT<Connection>& pConn = oper.pConn;

			switch( oper.OpCode )
			{
			case Operation::OP_CLOSEALL_CONNECTION:
				DisconnectAllConnection();
				break;

			case Operation::OP_WAITING_CONNECTION:
				if (pConn == nullptr)
				{
					netTrace( Trace::TRC_ERROR, "Invalid Operation for Pending connection nullptr" );
					break;
				}

				netChk( m_ManagedConnections.Insert(pConn->GetCID(), pConn) );
				netTrace(TRC_CONNECTION, "Connection management started CID:{0}", pConn->GetCID());

				if (!(AddMap((Connection*)pConn)))
				{
					netTrace(Trace::TRC_WARN, "Managed Connection AddMap failed CID:{0}", pConn->GetCID());
				}
				break;
			case Operation::OP_PENDING_CONNECTION:
				if (pConn == nullptr)
				{
					netTrace( Trace::TRC_ERROR, "Invalid Operation for Pending connection nullptr" );
					break;
				}

				if( !(AddMap( (Connection*)pConn )) )
				{
					netTrace(Trace::TRC_WARN, "Managed Connection AddMap failed CID:{0}", pConn->GetCID());
				}
				break;
			case Operation::OP_PENDING_INITCONNECTION:
				if (pConn == nullptr)
				{
					netTrace( Trace::TRC_ERROR, "Invalid Operation for Pending init connection nullptr" );
					break;
				}

				if (pConn->GetConnectionState() == IConnection::STATE_DISCONNECTED)
				{
					pConn->InitConnection(pConn->GetSocket(), pConn->GetConnectionInfo());
					netTrace(TRC_CONNECTION, "Initialize connection CID:{0}, Addr:{1}:{2}", pConn->GetCID(), pConn->GetConnectionInfo().Remote.strAddr, pConn->GetConnectionInfo().Remote.usPort);
				}

				break;
			case Operation::OP_MANAGED_CONNECTION_TAKENOVER:
			{
				if (pConn == nullptr)
				{
					netTrace(Trace::TRC_ERROR, "Invalid Operation for Release connection nullptr");
					break;
				}

				//m_ManagedConnections.CommitChanges();

				SharedPointerT<Connection> pPtr;
				if ((m_ManagedConnections.Remove(pConn->GetCID(), pPtr)))
				{
					netTrace(TRC_CONNECTION, "Connection management is handed over CID:{0}", pConn->GetCID());
				}
				else
				{
					netTrace(Trace::TRC_WARN, "Connection management is failed to be handed over CID:{0}", pConn->GetCID());
				}

				oper.Clear();
				break;
			}
			case Operation::OP_RELEASE_CONNECTION:
				{
					if( pConn == nullptr )
					{
						netTrace( Trace::TRC_ERROR, "Invalid Operation for Release connection nullptr" );
						break;
					}

					if ((pConn->GetPendingRecvCount() + pConn->GetPendingSendCount()) > 0)
					{
						if (pConn->GetConnectionState() == IConnection::STATE_CONNECTED)
							pConn->CloseConnection();

						// close socket all the time
						pConn->CloseSocket();

						if (Util::TimeSince(oper.EnqueuedTime) < DurationMS(30 * 1000))
						{
							// Wait for a bit if there is something we need to send or recv
							// leave this release for a while
							m_PendingOperations.Enqueue(std::forward<Operation>(oper));
							break;
						}

						netTrace(Trace::TRC_WARN, "Timeout, force clean up connection CID:{0}", pConn->GetCID());
					}

					auto cid = pConn->GetCID();
					SharedPointerT<Connection> pPtr;
					if ((m_ManagedConnections.Remove(cid, pPtr)))
					{
						Assert(pPtr == pConn);
					}

					netTrace(TRC_CONNECTION, "ConnectionManager Operation Release CID:{0}", cid);

					FreeConnection(pConn);

					oper.Clear();
					break;
				}
			case Operation::OP_PENDING_ADDR:
				{
					WeakPointerT<Connection> pConnOrg;
					if ((m_AddrMap.Find(oper.addrOrg, pConnOrg)))
					{
						// already in map
						break;
					}

					pConn = SharedPointerT<Connection>(NewConnection());
					if (pConn != nullptr && pConn->GetNet())
					{
						IConnection::ConnectionInformation connectionInfo;
						ServerNet *pIServer = dynamic_cast<ServerNet*>(pConn->GetNet());
						netChkPtr( pIServer );

						memset( &connectionInfo, 0, sizeof(connectionInfo) );

						connectionInfo.SetLocalInfo( pIServer->GetNetClass(), pIServer->GetLocalAddress(), pIServer->GetServerID() );

						SockAddr2Addr( oper.addrOrg, connectionInfo.Remote );
						connectionInfo.SetRemoteInfo(NetClass::Unknown, 0);

						netChk( pConn->InitConnection( pIServer->GetSocket(), connectionInfo ) );
						netTrace(TRC_CONNECTION, "Initialize connection CID:{0}, Addr:{1}:{2}", pConn->GetCID(), pConn->GetConnectionInfo().Remote.strAddr, pConn->GetConnectionInfo().Remote.usPort);

						m_ManagedConnections.Insert(pConn->GetCID(), pConn);

						if( !(AddMap( (Connection*)pConn )) )
						{
							netTrace( TRC_CONNECTION, "Managed Connection AddMap failed CID:{0}", pConn->GetCID() );
						}

						//pConn = nullptr;
					}
					else
					{
						netTrace( Trace::TRC_ERROR, "Invalid Operation for creat connection" );
					}
				}
				break;
			case Operation::OP_PENDING_MOBILECONNECTION:
				{
					WeakPointerT<Connection> pConnOrg;

					if ((m_AddrMap.Find(oper.addrOrg, pConnOrg)))
					{
						//pConn = nullptr;
						// already in map
						break;
					}

					if( (m_PeerIDMap.Find( oper.MobileNetCtrl.PeerID, pConnOrg )) )
					{
						// already
						break;
					}

					pConn = SharedPointerT<Connection>(NewConnection());
					if( pConn != nullptr && pConn->GetNet() )
					{
						IConnection::ConnectionInformation connectionInfo;
						ServerNet *pIServer = dynamic_cast<ServerNet*>(pConn->GetNet());

						netChkPtr( pIServer );

						memset( &connectionInfo, 0, sizeof(connectionInfo) );

						connectionInfo.SetLocalInfo( pIServer->GetNetClass(), pIServer->GetLocalAddress(), pIServer->GetServerID() );

						connectionInfo.SetRemoteInfo(NetClass::Client, oper.MobileNetCtrl.PeerID);
						SockAddr2Addr( oper.addrOrg, connectionInfo.Remote );
						Assert(oper.MobileNetCtrl.PeerID == 0);

						netChk( pConn->InitConnection( pIServer->GetSocket(), connectionInfo ) );
						netTrace(TRC_CONNECTION, "Initialize connection CID:{0}, Addr:{1}:{2}", pConn->GetCID(), pConn->GetConnectionInfo().Remote.strAddr, pConn->GetConnectionInfo().Remote.usPort);

						m_ManagedConnections.Insert(pConn->GetCID(), pConn);

						if (!(AddMap((Connection*)pConn)))
						{
							netTrace( TRC_CONNECTION, "Managed Connection AddMap failed CID:{0}", pConn->GetCID() );
						}

						netChk( pConn->OnRecv( sizeof(oper.MobileNetCtrl), (const BYTE*)&oper.MobileNetCtrl ) );
					}
					else
					{
						netTrace( Trace::TRC_ERROR, "Invalid Operation while creating a connection" );
					}
				}
				break;
			case Operation::OP_PENDING_MOBILEREMAP:
				{
					ConnectionUDPBase *pUDPConn = dynamic_cast<ConnectionUDPBase*>((Connection*)pConn);
					if( pConn == nullptr || pUDPConn == nullptr )
					{
						netTrace( TRC_CONNECTION, "Address remapping is failed null connection" );
						break;
					}

					//CIDMap::iterator itCon;
					SharedPointerT<Connection> pConnOrg;
					if( !(m_CIDMap.Find( pConn->GetCID(), pConnOrg )) )
					{
						netTrace( TRC_CONNECTION, "Address remapping is failed: invalid CID:{0}", pConn->GetCID() );
						break;
					}
					//itCon = nullptr;

					if (!(AddressRemap((Connection*)pConn, oper.addrOrg, oper.addrNew)))
					{
						netTrace(Trace::TRC_ERROR, "Address remapping is failed CID:{0}, from:{1}, to:{2}", pConn->GetCID(), oper.addrOrg, oper.addrNew);
					}
					else
					{
						netTrace( TRC_CONNECTION, "Address is mapped from {0} to {1}", oper.addrOrg, oper.addrNew );
					}
				}
				break;
			case Operation::OP_PENDING_REMAP_PEERID:
					if( pConn == nullptr )
						break;

					if( !(RemapPeerID( (Connection*)pConn, oper.MobileNetCtrl.PeerID )) )
					{
						netTrace( TRC_CONNECTION, "Address remapping is failed CID:{0}", pConn->GetCID() );
					}
				break;
			default:
				netTrace( Trace::TRC_WARN, "Unknown Operation {0}", (INT32)oper.OpCode );
				break;
			};
		}

	Proc_End:

		return;
	}

	// mapping Add/Remove
	Result ConnectionManager::AddMap( Connection *pConn )
	{
		Result hr = ResultCode::SUCCESS;
		SharedPointerT<Connection> pConnPtr;
		WeakPointerT<Connection> pPtr;

		netChkPtr( pConn );

		if (m_UseAddressMap)
		{
			if (pConn->GetRemoteSockAddr().ss_family != 0 && (m_AddrMap.Find(pConn->GetRemoteSockAddr(), pPtr)))
			{
				// already in map
				netErr(ResultCode::INVALID_ARG);
			}
		}

		if ((m_CIDMap.Find(pConn->GetCID(), pConnPtr)))
		{
			// already in map
			netErr( ResultCode::INVALID_ARG );
		}

		if (m_UseAddressMap && pConn->GetRemoteSockAddr().ss_family != 0)
		{
			netChk(m_AddrMap.Insert(pConn->GetRemoteSockAddr(), WeakPointerT<Connection>(pConn)));
		}

		if (!(m_CIDMap.Insert(pConn->GetCID(), pConn)))
		{
			// remove
			m_AddrMap.Erase(pConn->GetRemoteSockAddr(), pPtr);
			netErr( ResultCode::UNEXPECTED );
		}

		if ((m_UsePeerIDMap && pConn->GetPeerID() != 0 && !(m_PeerIDMap.Insert(pConn->GetPeerID(), WeakPointerT<Connection>(pConn)))))
		{
			// remove
			m_AddrMap.Erase(pConn->GetRemoteSockAddr(), pPtr);
			m_CIDMap.Erase(pConn->GetCID(), pConnPtr);
			netErr( ResultCode::UNEXPECTED );
		}

	Proc_End:

		return hr;
	}

	Result ConnectionManager::AddressRemap(Connection *pConn, const sockaddr_storage &addressOrg, const sockaddr_storage &newAddress)
	{
		Result hr = ResultCode::SUCCESS;
		WeakPointerT<Connection> pPtr;
		ConnectionUDPBase *pConnUDP = dynamic_cast<ConnectionUDPBase*>(pConn);
		netChkPtr(pConnUDP);

		if (!(m_AddrMap.Erase(addressOrg, pPtr)))
		{
			netTrace( Trace::TRC_WARN, "Old address is not found ignoring:{0}", pConn->GetConnectionInfo().Remote );
		}

		netChk(m_AddrMap.Insert(addressOrg, WeakPointerT<Connection>(pConn)));

	Proc_End:

		return hr;
	}

	Result ConnectionManager::RemoveMap( Connection *pConn )
	{
		Result hr = ResultCode::SUCCESS;
		Result hrTem = ResultCode::SUCCESS;
		SharedPointerT<Connection> pConnPtr;
		WeakPointerT<Connection> pPtr;

		if (pConn->GetCID() != 0)
		{
			if (m_UseAddressMap)
				hr = m_AddrMap.Erase(pConn->GetRemoteSockAddr(), pPtr);

			if (pConn->GetCID() != 0)
				hrTem = m_CIDMap.Erase(pConn->GetCID(), pConnPtr);
		}

		if( pConn->GetConnectionInfo().RemoteID != 0 )
			m_PeerIDMap.Erase(pConn->GetPeerID(), pPtr);

		netChk( hr );
		netChk( hrTem );

	Proc_End:

		return hr;
	}

	// Remap PeerID
	Result ConnectionManager::RemapPeerID( Connection *pConn, AuthTicket ticket )
	{
		Result hr = ResultCode::SUCCESS;
		Result hrTem = ResultCode::SUCCESS;
		WeakPointerT<Connection> pConnMapPtr;

		if( pConn->GetConnectionInfo().RemoteID != 0 )
		{
			if ((m_PeerIDMap.Erase(pConn->GetPeerID(), pConnMapPtr)))
			{
				//Assert(pConn == pConnMapPtr);
			}
		}

#ifdef DEBUG
		Assert(pConn->GetConnectionInfo().RemoteID != 0xcdcdcdcdcdcdcdcd);
#endif

		pConn->SetRemoteID(ticket);

		if( ticket != 0 )
		{
			//m_PeerIDMap.erase( pConn );
			netChk(m_PeerIDMap.Insert(pConn->GetPeerID(), WeakPointerT<Connection>(pConn)));
		}

		netChk( hr );
		netChk( hrTem );

	Proc_End:

		return hr;
	}


	// Initialize Manager
	Result ConnectionManager::InitManager( UINT poolCacheCount )
	{
		Result hr = ResultCode::SUCCESS;

		m_bNoNewConnection = false;

		Start();

	//Proc_End:
		return hr;
	}


	// Release all connection and terminate manager
	Result ConnectionManager::TerminateManager()
	{
		Result hr = ResultCode::SUCCESS;
		Operation oper;


		m_bNoNewConnection = true;

		Stop(true);


		// Clear pending operation queue
		while( m_PendingOperations.Dequeue( oper ) )
		{
			SharedPointerT<Connection>& pConn = oper.pConn;

			//if (oper.pConn != nullptr)
			//	oper.pConn.GetSharedPointer(pConn);

			switch( oper.OpCode )
			{
			case Operation::OP_WAITING_CONNECTION:
			case Operation::OP_PENDING_CONNECTION:
			case Operation::OP_PENDING_INITCONNECTION:
			case Operation::OP_RELEASE_CONNECTION:
				if( pConn != nullptr )
				{
					netTrace(TRC_CONNECTION, "TerminateManager ConnectionManager Operation Release CID:{0}", pConn->GetCID());
					oper.Clear();
					FreeConnection( pConn );
				}
				break;
			case Operation::OP_PENDING_ADDR:
			default:
				break;
			};
		}

		// clear connection queue
		
		m_ManagedConnections.ForeachOrder(0, (UINT)m_ManagedConnections.GetItemCount(), [&](const uintptr_t& key, SharedPointerT<Connection> pConn) -> bool
		{
			netTrace( TRC_CONNECTION, "TerminateManager ConnectionManager Managed connection Release CID:{0}", pConn->GetCID() );
			if (pConn->GetConnectionState() != IConnection::STATE_DISCONNECTED)
				pConn->CloseConnection();
			FreeConnection( pConn );
			return true;
		});

		m_ManagedConnections.ClearMap();


	//Proc_End:


		return hr;
	}


	// Disconnect all connection
	Result ConnectionManager::DisconnectAllConnection()
	{
		//Result hr  = ResultCode::SUCCESS;
		Operation oper;

		// Clear pending operation queue
		while( m_PendingOperations.Dequeue( oper ) )
		{
			switch( oper.OpCode )
			{
			case Operation::OP_WAITING_CONNECTION:
			case Operation::OP_PENDING_CONNECTION:
			case Operation::OP_PENDING_INITCONNECTION:
			case Operation::OP_RELEASE_CONNECTION:
				if( oper.pConn != nullptr )
				{
					SharedPointerT<Connection> pConn = oper.pConn;

					//if (oper.pConn != nullptr)
					//	oper.pConn.GetSharedPointer(pConn);

					if (pConn != nullptr)
						pConn->Disconnect("DisconnectAllConnection");
				}
				break;
			case Operation::OP_PENDING_ADDR:
			default:
				break;
			};
		}

		// clear connection queue
		m_ManagedConnections.ForeachOrder(0, (UINT)m_ManagedConnections.GetItemCount(), [&](const uintptr_t& key, SharedPointerT<Connection> pConn)-> bool
		{
			pConn->Disconnect("DisconnectAllConnection");
			return true;
		});

		return ResultCode::SUCCESS;
	}

	// Add connection to wait connect process
	Result ConnectionManager::PendingWaitConnection( ConnectionPtr& pConnection )
	{
		return m_PendingOperations.Enqueue( Operation(Operation::OP_WAITING_CONNECTION,pConnection) );
	}


	// Add connection to connecting process
	Result ConnectionManager::PendingConnection(ConnectionPtr& pConnection )
	{
		return m_PendingOperations.Enqueue( Operation(Operation::OP_PENDING_CONNECTION,pConnection) );
	}

	// Create new connection from connection pool with UDP address and add to connecting process
	Result ConnectionManager::PendingNewConnection(const sockaddr_storage& sockAddr, MsgNetCtrlConnect *pNetCtrl)
	{
		return m_PendingOperations.Enqueue( Operation(sockAddr,pNetCtrl) );
	}

	// Create new connection from connection pool with UDP address and add to connecting process
	Result ConnectionManager::PendingNewConnection(const sockaddr_storage& sockAddr, MsgMobileNetCtrl *pNetCtrl)
	{
		return m_PendingOperations.Enqueue( Operation(Operation::OP_PENDING_MOBILECONNECTION, sockAddr, pNetCtrl) );
	}

	// Change address mapping of connection
	Result ConnectionManager::PendingRemapPeerID(ConnectionPtr& pConnection, AuthTicket ticket )
	{
		return m_PendingOperations.Enqueue( Operation(Operation::OP_PENDING_REMAP_PEERID, ticket, pConnection) );
	}

	// Change address mapping of connection
	Result ConnectionManager::PendingAddressRemap(ConnectionPtr& pConnection, const sockaddr_storage& sockAddrOrg, const sockaddr_storage& sockAddrNew)
	{
		return m_PendingOperations.Enqueue(Operation(Operation::OP_PENDING_MOBILEREMAP, sockAddrOrg, sockAddrNew, pConnection));
	}

	// Pending Init connection
	Result ConnectionManager::PendingInitConnection(ConnectionPtr& pConnection )
	{
		return m_PendingOperations.Enqueue( Operation(Operation::OP_PENDING_INITCONNECTION,pConnection) );
	}

	// Managed connection is taken by other entity
	Result ConnectionManager::PendingManagedConnectionTakenOver(ConnectionPtr& pConnection)
	{
		return m_PendingOperations.Enqueue(Operation(Operation::OP_MANAGED_CONNECTION_TAKENOVER, pConnection));
	}

	// Close and release connection
	Result ConnectionManager::PendingReleaseConnection( ConnectionPtr& pConnection )
	{
		Result hr = ResultCode::SUCCESS;

		// force to close
		if (pConnection->GetConnectionState() != IConnection::STATE_DISCONNECTED)
		{
			pConnection->Disconnect("ConnectionManager::PendingReleaseConnection");
			pConnection->CloseConnection();
		}

		// force to close socket before it goes into queue so that we can save time to wait
		if (pConnection->GetSocket() != INVALID_SOCKET)
			pConnection->CloseSocket();

		Assert(pConnection->GetReferenceCount() > 0);
		hr = m_PendingOperations.Enqueue( Operation(Operation::OP_RELEASE_CONNECTION, pConnection) );

		Assert((hr));
		if( (hr) )
			pConnection = nullptr;

		return hr;
	}

	// Close and release connection
	Result ConnectionManager::PendingCloseAllConnection()
	{
		ConnectionPtr tempPtr;
		return m_PendingOperations.Enqueue( Operation(Operation::OP_CLOSEALL_CONNECTION, tempPtr) );
	}

	// Find and return connection
	Result ConnectionManager::GetConnectionByAddr(const sockaddr_storage& sockAddr, SharedPointerT<Connection> &pConn)
	{
		WeakPointerT<Connection> pPtr;
		if((m_AddrMap.Find(sockAddr, pPtr)))
		{
			pPtr.GetSharedPointer(pConn);
		}
		return pConn != nullptr ? ResultCode::SUCCESS : ResultCode::FAIL;
	}


	// Find and return connection
	Result ConnectionManager::GetConnectionByCID(uintptr_t uiCID, SharedPointerT<Connection> &pConn)
	{
		return m_CIDMap.Find(uiCID, pConn);
	}
	
	// Find and return connection
	Result ConnectionManager::GetConnectionByPeerID(UINT64 peerID, SharedPointerT<Connection> &pConn)
	{
		WeakPointerT<Connection> pPtr;
		if ((m_PeerIDMap.Find(peerID, pPtr)))
		{
			pPtr.GetSharedPointer(pConn);
		}
		return pConn != nullptr ? ResultCode::SUCCESS : ResultCode::FAIL;
	}


} // namespace Net
} // namespace BR

