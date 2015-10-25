﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : EntityServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/EntityServerIPolicy.h"
#include "Protocol/Policy/EntityServerNetPolicy.h"
#include "Protocol/Message/EntityServerMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: Register entity
		HRESULT NetPolicyEntityServer::RegisterEntityCmd( const Context &InContext, const EntityID &InLocalEntID, const char* InEntName )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::EntityServer::RegisterEntityCmd::BuildIMsg(pMsg, InContext, InLocalEntID, InEntName));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyEntityServer::RegisterEntityCmd( const Context &InContext, const EntityID &InLocalEntID, const char* InEntName )
		// Cmd: Find Entity
		HRESULT NetPolicyEntityServer::UnregisterEntityCmd( const Context &InContext, const EntityUID &InEntUID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::EntityServer::UnregisterEntityCmd::BuildIMsg(pMsg, InContext, InEntUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyEntityServer::UnregisterEntityCmd( const Context &InContext, const EntityUID &InEntUID )
		// Cmd: Find Entity
		HRESULT NetPolicyEntityServer::FindEntityCmd( const Context &InContext, const EntityID &InLocalEntID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::EntityServer::FindEntityCmd::BuildIMsg(pMsg, InContext, InLocalEntID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyEntityServer::FindEntityCmd( const Context &InContext, const EntityID &InLocalEntID )


		// Cmd: Register entity
		HRESULT NetSvrPolicyEntityServer::RegisterEntityRes( const Context &InContext, const HRESULT &InResult, const EntityUID &InEntUID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::EntityServer::RegisterEntityRes::BuildIMsg(pMsg, InContext, InResult, InEntUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyEntityServer::RegisterEntityRes( const Context &InContext, const HRESULT &InResult, const EntityUID &InEntUID )
		// Cmd: Find Entity
		HRESULT NetSvrPolicyEntityServer::UnregisterEntityRes( const Context &InContext, const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::EntityServer::UnregisterEntityRes::BuildIMsg(pMsg, InContext, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyEntityServer::UnregisterEntityRes( const Context &InContext, const HRESULT &InResult )
		// Cmd: Find Entity
		HRESULT NetSvrPolicyEntityServer::FindEntityRes( const Context &InContext, const HRESULT &InResult, const EntityUID &InEntUID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::EntityServer::FindEntityRes::BuildIMsg(pMsg, InContext, InResult, InEntUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyEntityServer::FindEntityRes( const Context &InContext, const HRESULT &InResult, const EntityUID &InEntUID )


	}; // namespace Policy
}; // namespace BR

