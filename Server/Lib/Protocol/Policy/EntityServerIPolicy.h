﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : EntityServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Net/NetPolicy.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Policy
	{
 
		class ISvrPolicyEntityServer : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_SVR_ENTITYSERVER };

			// Constructor
			ISvrPolicyEntityServer( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Register entity
			virtual HRESULT RegisterEntityRes( const Context &InContext, const HRESULT &InResult, const EntityUID &InEntUID ) = 0;
			// Cmd: Find Entity
			virtual HRESULT UnregisterEntityRes( const Context &InContext, const HRESULT &InResult ) = 0;
			// Cmd: Find Entity
			virtual HRESULT FindEntityRes( const Context &InContext, const HRESULT &InResult, const EntityUID &InEntUID ) = 0;

		}; // class ISvrPolicyEntityServer : public Net::INetPolicy


		class IPolicyEntityServer : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_ENTITYSERVER };

			// Constructor
			IPolicyEntityServer( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Register entity
			virtual HRESULT RegisterEntityCmd( const Context &InContext, const EntityID &InLocalEntID, const char* InEntName ) = 0;
			// Cmd: Find Entity
			virtual HRESULT UnregisterEntityCmd( const Context &InContext, const EntityUID &InEntUID ) = 0;
			// Cmd: Find Entity
			virtual HRESULT FindEntityCmd( const Context &InContext, const EntityID &InLocalEntID ) = 0;

		}; // class IPolicyEntityServer : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR

