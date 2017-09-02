////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : EntityServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityInformation.h"
#include "Protocol/ServerService/EntityServerService.h"
#include "SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		EntityServerService::EntityServerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService)
		{
		}


		// Cmd: Register entity
		Result EntityServerService::RegisterEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName )
		{
 			Result hr;

			 svrChk(Policy::NetPolicyEntityServer(GetConnection()).RegisterEntityCmd( InTransactionID, InLocalEntID, InEntName ) );

		Proc_End:

			return hr;

		}; // Result EntityServerService::RegisterEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName )
		// Cmd: Find Entity
		Result EntityServerService::UnregisterEntityCmd( const TransactionID &InTransactionID, const uint64_t &InEntUID )
		{
 			Result hr;

			 svrChk(Policy::NetPolicyEntityServer(GetConnection()).UnregisterEntityCmd( InTransactionID, InEntUID ) );

		Proc_End:

			return hr;

		}; // Result EntityServerService::UnregisterEntityCmd( const TransactionID &InTransactionID, const uint64_t &InEntUID )
		// Cmd: Find Entity
		Result EntityServerService::FindEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID )
		{
 			Result hr;

			 svrChk(Policy::NetPolicyEntityServer(GetConnection()).FindEntityCmd( InTransactionID, InLocalEntID ) );

		Proc_End:

			return hr;

		}; // Result EntityServerService::FindEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID )


	}; // namespace Svr
}; // namespace SF


