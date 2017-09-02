﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GameMasterServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/ServiceEntity/EntityInformation.h"
#include "ServerSystem/ServerService/GameMasterServerService.h"
#include "ServerSystem/SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		GameMasterServerService::GameMasterServerService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyGameMasterServer::ID_POLICY)
		{
			static_assert((UINT)Policy::IPolicyGameMasterServer::ID_POLICY == (UINT)ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyGameMasterServer());
		}


		// C2S: Player entered
		Result GameMasterServerService::PlayerEnteredC2SEvt( const AccountID &InPlayerID, const char* InPlayerName )
		{
 			Result hr;

			 svrChk(GetPolicyGameMasterServer()->PlayerEnteredC2SEvt( InPlayerID, InPlayerName ) );

		Proc_End:

			return hr;

		}; // Result GameMasterServerService::PlayerEnteredC2SEvt( const AccountID &InPlayerID, const char* InPlayerName )
		// C2S: Player leaved
		Result GameMasterServerService::PlayerLeavedC2SEvt( const AccountID &InPlayerID )
		{
 			Result hr;

			 svrChk(GetPolicyGameMasterServer()->PlayerLeavedC2SEvt( InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result GameMasterServerService::PlayerLeavedC2SEvt( const AccountID &InPlayerID )


	}; // namespace Svr
}; // namespace SF


