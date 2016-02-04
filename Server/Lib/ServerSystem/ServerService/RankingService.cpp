﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Ranking Message debug implementations
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
#include "ServerSystem/ServerService/RankingService.h"
#include "ServerSystem/SvrTrace.h"



namespace BR
{
 	namespace Svr
	{
 		RankingService::RankingService( ServerServiceInformation* pService)
			: ServerServiceBase(pService, Policy::IPolicyRanking::ID_POLICY)
		{
			static_assert((UINT)Policy::IPolicyRanking::ID_POLICY == (UINT)ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyRanking());
		}


		// Cmd: Add a player to ranking
		HRESULT RankingService::AddPlayerCmd( const Context &InContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
		{
 			HRESULT hr = S_SYSTEM_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyRanking()->AddPlayerCmd( InRouteContext, InContext, InPlayerID, InRankingScore, InPlayerInfo ) );

		Proc_End:

			return hr;

		}; // HRESULT RankingService::AddPlayerCmd( const Context &InContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
		// Cmd: Remove a player to ranking
		HRESULT RankingService::RemovePlayerCmd( const Context &InContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyRanking()->RemovePlayerCmd( InRouteContext, InContext, InPlayerID ) );

		Proc_End:

			return hr;

		}; // HRESULT RankingService::RemovePlayerCmd( const Context &InContext, const PlayerID &InPlayerID )
		// Cmd: Remove a player to ranking
		HRESULT RankingService::GetPlayerRankingCmd( const Context &InContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_SYSTEM_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyRanking()->GetPlayerRankingCmd( InRouteContext, InContext, InPlayerID ) );

		Proc_End:

			return hr;

		}; // HRESULT RankingService::GetPlayerRankingCmd( const Context &InContext, const PlayerID &InPlayerID )
		// Cmd: Update a player to ranking
		HRESULT RankingService::UpdatePlayerCmd( const Context &InContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
		{
 			HRESULT hr = S_SYSTEM_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyRanking()->UpdatePlayerCmd( InRouteContext, InContext, InPlayerID, InRankingScore, InPlayerInfo ) );

		Proc_End:

			return hr;

		}; // HRESULT RankingService::UpdatePlayerCmd( const Context &InContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
		// Cmd: Remove a player to ranking
		HRESULT RankingService::GetRankingCmd( const Context &InContext, const UINT32 &InFrom, const UINT32 &InCount )
		{
 			HRESULT hr = S_SYSTEM_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.GetEntityID()), GetServiceEntityUID() );
			svrChk(GetPolicyRanking()->GetRankingCmd( InRouteContext, InContext, InFrom, InCount ) );

		Proc_End:

			return hr;

		}; // HRESULT RankingService::GetRankingCmd( const Context &InContext, const UINT32 &InFrom, const UINT32 &InCount )


	}; // namespace Svr
}; // namespace BR


