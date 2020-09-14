////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : ChatChannel Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
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
		Result ChatChannelService::JoinCmd( const uint64_t &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyChatChannel(GetConnection()).JoinCmd( InRouteContext, InTransactionID, InPasscode, InJoiningPlayer ) );

			return hr;

		}; // Result ChatChannelService::JoinCmd( const uint64_t &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer )
		// Cmd: Leave chat channel
		Result ChatChannelService::LeaveCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyChatChannel(GetConnection()).LeaveCmd( InRouteContext, InTransactionID, InPlayerID ) );

			return hr;

		}; // Result ChatChannelService::LeaveCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID )
		// Cmd: Kick a player
		Result ChatChannelService::KickPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),TransactionID(InTransactionID).GetEntityID()), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyChatChannel(GetConnection()).KickPlayerCmd( InRouteContext, InTransactionID, InPlayerID, InPlayerToKick ) );

			return hr;

		}; // Result ChatChannelService::KickPlayerCmd( const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// C2S: Sending a chatting message
		Result ChatChannelService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InSenderID, const char* InChatMessage )
		{
 			FunctionContext hr;

			RouteContext InRouteContext( EntityUID(GetMyServerID(),InSenderEntityID), GetServiceEntityUID() );
			svrChk(Policy::NetPolicyChatChannel(GetConnection()).ChatMessageC2SEvt( InRouteContext, InSenderID, InChatMessage ) );

			return hr;

		}; // Result ChatChannelService::ChatMessageC2SEvt( const EntityID &InSenderEntityID, const PlayerID &InSenderID, const char* InChatMessage )


	}; // namespace Svr
}; // namespace SF


