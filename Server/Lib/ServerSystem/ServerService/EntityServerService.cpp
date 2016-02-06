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
		HRESULT EntityServerService::RegisterEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 svrChk(GetPolicyEntityServer()->RegisterEntityCmd( InTransactionID, InLocalEntID, InEntName ) );

		Proc_End:

			return hr;

		}; // HRESULT EntityServerService::RegisterEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName )
		// Cmd: Find Entity
		HRESULT EntityServerService::UnregisterEntityCmd( const TransactionID &InTransactionID, const EntityUID &InEntUID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 svrChk(GetPolicyEntityServer()->UnregisterEntityCmd( InTransactionID, InEntUID ) );

		Proc_End:

			return hr;

		}; // HRESULT EntityServerService::UnregisterEntityCmd( const TransactionID &InTransactionID, const EntityUID &InEntUID )
		// Cmd: Find Entity
		HRESULT EntityServerService::FindEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			 svrChk(GetPolicyEntityServer()->FindEntityCmd( InTransactionID, InLocalEntID ) );

		Proc_End:

			return hr;

		}; // HRESULT EntityServerService::FindEntityCmd( const TransactionID &InTransactionID, const EntityID &InLocalEntID )


	}; // namespace Svr
}; // namespace BR


