////////////////////////////////////////////////////////////////////////////////
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
#include "Common/BRBaseTypes.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/BrServer.h"
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
			static_assert(Policy::IPolicyRanking::ID_POLICY == ID_SERVICEPOLICY,"Invalid Policy ID for a Servicebase ");
			Assert(GetPolicyRanking());
		}


		// Cmd: Add a player to ranking
		HRESULT RankingService::AddPlayerCmd( const Context &InContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyRanking()->AddPlayerCmd( InContext, InRouteContext, InPlayerID, InRankingScore, InPlayerInfo ) );

		Proc_End:

			return hr;

		}; // HRESULT RankingService::AddPlayerCmd( const Context &InContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
		// Cmd: Remove a player to ranking
		HRESULT RankingService::RemovePlayerCmd( const Context &InContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyRanking()->RemovePlayerCmd( InContext, InRouteContext, InPlayerID ) );

		Proc_End:

			return hr;

		}; // HRESULT RankingService::RemovePlayerCmd( const Context &InContext, const PlayerID &InPlayerID )
		// Cmd: Remove a player to ranking
		HRESULT RankingService::GetPlayerRankingCmd( const Context &InContext, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyRanking()->GetPlayerRankingCmd( InContext, InRouteContext, InPlayerID ) );

		Proc_End:

			return hr;

		}; // HRESULT RankingService::GetPlayerRankingCmd( const Context &InContext, const PlayerID &InPlayerID )
		// Cmd: Update a player to ranking
		HRESULT RankingService::UpdatePlayerCmd( const Context &InContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyRanking()->UpdatePlayerCmd( InContext, InRouteContext, InPlayerID, InRankingScore, InPlayerInfo ) );

		Proc_End:

			return hr;

		}; // HRESULT RankingService::UpdatePlayerCmd( const Context &InContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo )
		// Cmd: Remove a player to ranking
		HRESULT RankingService::GetRankingCmd( const Context &InContext, const UINT32 &InFrom, const UINT32 &InCount )
		{
 			HRESULT hr = S_OK;

			TransactionID localTransID(InContext);
			RouteContext InRouteContext( EntityUID(GetMyServerID(),localTransID.EntityID), GetServiceEntityUID() );
			svrChk(GetPolicyRanking()->GetRankingCmd( InContext, InRouteContext, InFrom, InCount ) );

		Proc_End:

			return hr;

		}; // HRESULT RankingService::GetRankingCmd( const Context &InContext, const UINT32 &InFrom, const UINT32 &InCount )


	}; // namespace Svr
}; // namespace BR


