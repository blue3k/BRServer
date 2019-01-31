////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : ChatChannel Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerEntity/ServerEntity.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityInformation.h"
#include "Protocol/ServerService/ChatChannelService.h"
#include "SvrTrace.h"



namespace SF
{
 	namespace Svr
	{
 		ChatChannelService::ChatChannelService( ServerServiceInformation* pService)
			: ServerServiceBase(pService)
		{
		}


		// Cmd: Request to join chat channel
		Result ChatChannelService::JoinCmd( const TransactionID &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyChatChannel(GetConnection()).JoinCmd( InRouteContext, InTransactionID, InPasscode, InJoiningPlayer ) );

		Proc_End:

			return hr;

		}; // Result ChatChannelService::JoinCmd( const TransactionID &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer )
		// Cmd: Leave chat channel
		Result ChatChannelService::LeaveCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyChatChannel(GetConnection()).LeaveCmd( InRouteContext, InTransactionID, InPlayerID ) );

		Proc_End:

			return hr;

		}; // Result ChatChannelService::LeaveCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Kick a player
		Result ChatChannelService::KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InTransactionID.GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyChatChannel(GetConnection()).KickPlayerCmd( InRouteContext, InTransactionID, InPlayerID, InPlayerToKick ) );

		Proc_End:

			return hr;

		}; // Result ChatChannelService::KickPlayerCmd( const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// C2S: Sending a chatting message
		Result ChatChannelService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InSenderID, const char* InChatMessage )
		{
 			Result hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyChatChannel(GetConnection()).ChatMessageC2SEvt( InRouteContext, InSenderID, InChatMessage ) );

		Proc_End:

			return hr;

		}; // Result ChatChannelService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InSenderID, const char* InChatMessage )


	}; // namespace Svr
}; // namespace SF


