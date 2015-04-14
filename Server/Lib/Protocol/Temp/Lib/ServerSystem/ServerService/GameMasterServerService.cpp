////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameMasterServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Common/BRBaseTypes.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServerService/GameMasterServerService.h"
#include "ServerSystem/SvrTrace.h"



namespace BR
{
 	namespace Svr
	{
 		GameMasterServerService::GameMasterServerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyGameMasterServer::ID_POLICY)
		{
			static_assert(Policy::IPolicyGameMasterServer::ID_POLICY == ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyGameMasterServer());
		}


		// C2S: Player entered
		HRESULT GameMasterServerService::PlayerEnteredC2SEvt( const AccountID &InPlayerID, const char* InPlayerName )
		{
 			HRESULT hr = S_OK;

			 svrChk(GetPolicyGameMasterServer()->PlayerEnteredC2SEvt( InPlayerID, InPlayerName ) );

		Proc_End:

			return hr;

		}; // HRESULT GameMasterServerService::PlayerEnteredC2SEvt( const AccountID &InPlayerID, const char* InPlayerName )
		// C2S: Player leaved
		HRESULT GameMasterServerService::PlayerLeavedC2SEvt( const AccountID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			 svrChk(GetPolicyGameMasterServer()->PlayerLeavedC2SEvt( InPlayerID ) );

		Proc_End:

			return hr;

		}; // HRESULT GameMasterServerService::PlayerLeavedC2SEvt( const AccountID &InPlayerID )


	}; // namespace Svr
}; // namespace BR


