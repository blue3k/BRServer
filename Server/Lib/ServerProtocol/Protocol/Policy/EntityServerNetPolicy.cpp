////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
// 
// Author : Generated
// 
// Description : EntityServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Net/NetDef.h"
#include "Net/Message.h"
#include "Net/Connection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/EntityServerNetPolicy.h"
#include "Protocol/Message/EntityServerMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Register entity
		Result NetPolicyEntityServer::RegisterEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::EntityServer::RegisterEntityCmd::Create(m_pConnection->GetMemoryManager(), InTransactionID, InLocalEntID, InEntName);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyEntityServer::RegisterEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName )
		// Cmd: Find Entity
		Result NetPolicyEntityServer::UnregisterEntityCmd( const TransactionID &InTransactionID, const EntityUID &InEntUID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::EntityServer::UnregisterEntityCmd::Create(m_pConnection->GetMemoryManager(), InTransactionID, InEntUID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyEntityServer::UnregisterEntityCmd( const TransactionID &InTransactionID, const EntityUID &InEntUID )
		// Cmd: Find Entity
		Result NetPolicyEntityServer::FindEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::EntityServer::FindEntityCmd::Create(m_pConnection->GetMemoryManager(), InTransactionID, InLocalEntID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyEntityServer::FindEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID )


		// Cmd: Register entity
		Result NetSvrPolicyEntityServer::RegisterEntityRes( const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InEntUID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::EntityServer::RegisterEntityRes::Create(m_pConnection->GetMemoryManager(), InTransactionID, InResult, InEntUID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyEntityServer::RegisterEntityRes( const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InEntUID )
		// Cmd: Find Entity
		Result NetSvrPolicyEntityServer::UnregisterEntityRes( const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::EntityServer::UnregisterEntityRes::Create(m_pConnection->GetMemoryManager(), InTransactionID, InResult);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyEntityServer::UnregisterEntityRes( const TransactionID &InTransactionID, const Result &InResult )
		// Cmd: Find Entity
		Result NetSvrPolicyEntityServer::FindEntityRes( const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InEntUID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::EntityServer::FindEntityRes::Create(m_pConnection->GetMemoryManager(), InTransactionID, InResult, InEntUID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyEntityServer::FindEntityRes( const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InEntUID )


	}; // namespace Policy
}; // namespace SF


