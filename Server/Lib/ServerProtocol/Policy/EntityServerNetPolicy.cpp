////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : EntityServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Net/Message.h"
#include "Types/BrSvrTypes.h"
#include "Types/BrGameTypes.h"
#include "Protocol/Policy/EntityServerIPolicy.h"
#include "Protocol/Policy/EntityServerNetPolicy.h"
#include "Protocol/Message/EntityServerMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: Register entity
		Result NetPolicyEntityServer::RegisterEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::EntityServer::RegisterEntityCmd::BuildIMsg(pMsg, InTransactionID, InLocalEntID, InEntName));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyEntityServer::RegisterEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName )
		// Cmd: Find Entity
		Result NetPolicyEntityServer::UnregisterEntityCmd( const TransactionID &InTransactionID, const EntityUID &InEntUID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::EntityServer::UnregisterEntityCmd::BuildIMsg(pMsg, InTransactionID, InEntUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyEntityServer::UnregisterEntityCmd( const TransactionID &InTransactionID, const EntityUID &InEntUID )
		// Cmd: Find Entity
		Result NetPolicyEntityServer::FindEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::EntityServer::FindEntityCmd::BuildIMsg(pMsg, InTransactionID, InLocalEntID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyEntityServer::FindEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID )


		// Cmd: Register entity
		Result NetSvrPolicyEntityServer::RegisterEntityRes( const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InEntUID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::EntityServer::RegisterEntityRes::BuildIMsg(pMsg, InTransactionID, InResult, InEntUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyEntityServer::RegisterEntityRes( const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InEntUID )
		// Cmd: Find Entity
		Result NetSvrPolicyEntityServer::UnregisterEntityRes( const TransactionID &InTransactionID, const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::EntityServer::UnregisterEntityRes::BuildIMsg(pMsg, InTransactionID, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyEntityServer::UnregisterEntityRes( const TransactionID &InTransactionID, const Result &InResult )
		// Cmd: Find Entity
		Result NetSvrPolicyEntityServer::FindEntityRes( const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InEntUID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::EntityServer::FindEntityRes::BuildIMsg(pMsg, InTransactionID, InResult, InEntUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyEntityServer::FindEntityRes( const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InEntUID )


	}; // namespace Policy
}; // namespace BR


