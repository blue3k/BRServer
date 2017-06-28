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
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Common/BrBaseTypes.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServerService/EntityServerService.h"
#include "ServerSystem/SvrTrace.h"



namespace BR
{
 	namespace Svr
	{
 		EntityServerService::EntityServerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyEntityServer::ID_POLICY)
		{
			static_assert((UINT)Policy::IPolicyEntityServer::ID_POLICY == (UINT)ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyEntityServer());
		}


		// Cmd: Register entity
		Result EntityServerService::RegisterEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName )
		{
 			Result hr;

			 svrChk(GetPolicyEntityServer()->RegisterEntityCmd( InTransactionID, InLocalEntID, InEntName ) );

		Proc_End:

			return hr;

		}; // Result EntityServerService::RegisterEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName )
		// Cmd: Find Entity
		Result EntityServerService::UnregisterEntityCmd( const TransactionID &InTransactionID, const EntityUID &InEntUID )
		{
 			Result hr;

			 svrChk(GetPolicyEntityServer()->UnregisterEntityCmd( InTransactionID, InEntUID ) );

		Proc_End:

			return hr;

		}; // Result EntityServerService::UnregisterEntityCmd( const TransactionID &InTransactionID, const EntityUID &InEntUID )
		// Cmd: Find Entity
		Result EntityServerService::FindEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID )
		{
 			Result hr;

			 svrChk(GetPolicyEntityServer()->FindEntityCmd( InTransactionID, InLocalEntID ) );

		Proc_End:

			return hr;

		}; // Result EntityServerService::FindEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID )


	}; // namespace Svr
}; // namespace BR


