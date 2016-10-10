////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Game Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/GameIPolicy.h"
#include "Protocol/Policy/GameNetPolicy.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// C2S: Client heart bit
		Result NetPolicyGame::HeartBitC2SEvt(  )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::HeartBitC2SEvt::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::HeartBitC2SEvt(  )
		// Cmd: Player connected from a login server and moved to game server
		Result NetPolicyGame::JoinGameServerCmd( const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::JoinGameServerCmd::BuildIMsg(pMsg, InAccID, InTicket, InLoginEntityUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::JoinGameServerCmd( const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )
		// Cmd: player complition statues
		Result NetPolicyGame::GetComplitionStateCmd(  )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetComplitionStateCmd::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::GetComplitionStateCmd(  )
		// Cmd: Player complition state
		Result NetPolicyGame::SetComplitionStateCmd( const char* InComplitionState )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::SetComplitionStateCmd::BuildIMsg(pMsg, InComplitionState));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::SetComplitionStateCmd( const char* InComplitionState )
		// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
		Result NetPolicyGame::RegisterGCMCmd( const char* InGCMRegisteredID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RegisterGCMCmd::BuildIMsg(pMsg, InGCMRegisteredID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::RegisterGCMCmd( const char* InGCMRegisteredID )
		// Cmd: Unregister Google notification service ID
		Result NetPolicyGame::UnregisterGCMCmd( const char* InGCMRegisteredID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::UnregisterGCMCmd::BuildIMsg(pMsg, InGCMRegisteredID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::UnregisterGCMCmd( const char* InGCMRegisteredID )
		// Cmd: Invite friend
		Result NetPolicyGame::InviteFriendCmd( const PlayerID &InFriendID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::InviteFriendCmd::BuildIMsg(pMsg, InFriendID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::InviteFriendCmd( const PlayerID &InFriendID )
		// Cmd: Accept friend request
		Result NetPolicyGame::AcceptFriendRequestCmd( const PlayerID &InInviterID, const FacebookUID &InInviterFacebookUID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::AcceptFriendRequestCmd::BuildIMsg(pMsg, InInviterID, InInviterFacebookUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::AcceptFriendRequestCmd( const PlayerID &InInviterID, const FacebookUID &InInviterFacebookUID )
		// Cmd: Remove friden form the friend list
		Result NetPolicyGame::RemoveFriendCmd( const PlayerID &InFriendID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RemoveFriendCmd::BuildIMsg(pMsg, InFriendID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::RemoveFriendCmd( const PlayerID &InFriendID )
		// Cmd: Get friend list
		Result NetPolicyGame::GetFriendListCmd( const UINT16 &InStartIndex, const UINT16 &InCount )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetFriendListCmd::BuildIMsg(pMsg, InStartIndex, InCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::GetFriendListCmd( const UINT16 &InStartIndex, const UINT16 &InCount )
		// Cmd: Query notification list
		Result NetPolicyGame::GetNotificationListCmd(  )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetNotificationListCmd::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::GetNotificationListCmd(  )
		// Cmd: Delete notification
		Result NetPolicyGame::DeleteNotificationCmd( const UINT32 &InNotificationID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::DeleteNotificationCmd::BuildIMsg(pMsg, InNotificationID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::DeleteNotificationCmd( const UINT32 &InNotificationID )
		// Cmd: Set notification is read
		Result NetPolicyGame::SetNotificationReadCmd( const UINT32 &InNotificationID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::SetNotificationReadCmd::BuildIMsg(pMsg, InNotificationID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::SetNotificationReadCmd( const UINT32 &InNotificationID )
		// Cmd: Accept notification
		Result NetPolicyGame::AcceptNotificationCmd( const UINT32 &InNotificationID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::AcceptNotificationCmd::BuildIMsg(pMsg, InNotificationID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::AcceptNotificationCmd( const UINT32 &InNotificationID )
		// Cmd: Query playerID list
		Result NetPolicyGame::FindPlayerByEMailCmd( const char* InPlayerEMail )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::FindPlayerByEMailCmd::BuildIMsg(pMsg, InPlayerEMail));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::FindPlayerByEMailCmd( const char* InPlayerEMail )
		// Cmd: Query playerID list
		Result NetPolicyGame::FindPlayerByPlayerIDCmd( const PlayerID &InPlayerID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::FindPlayerByPlayerIDCmd::BuildIMsg(pMsg, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::FindPlayerByPlayerIDCmd( const PlayerID &InPlayerID )
		// Cmd: *Request Player Status Update
		Result NetPolicyGame::RequestPlayerStatusUpdateCmd( const Array<PlayerID>& InTargetPlayerID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RequestPlayerStatusUpdateCmd::BuildIMsg(pMsg, InTargetPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::RequestPlayerStatusUpdateCmd( const Array<PlayerID>& InTargetPlayerID )
		// Cmd: Get Ranking lise
		Result NetPolicyGame::GetRankingListCmd( const RankingType &InRankingType, const BYTE &InBaseRanking, const BYTE &InCount )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetRankingListCmd::BuildIMsg(pMsg, InRankingType, InBaseRanking, InCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::GetRankingListCmd( const RankingType &InRankingType, const BYTE &InBaseRanking, const BYTE &InCount )
		// Cmd: Game user game play information
		Result NetPolicyGame::GetUserGamePlayerInfoCmd(  )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetUserGamePlayerInfoCmd::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::GetUserGamePlayerInfoCmd(  )
		// Cmd: Game game play information
		Result NetPolicyGame::GetGamePlayerInfoCmd( const PlayerID &InPlayerID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetGamePlayerInfoCmd::BuildIMsg(pMsg, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::GetGamePlayerInfoCmd( const PlayerID &InPlayerID )
		// Cmd: Change NickName
		Result NetPolicyGame::SetNickNameCmd( const char* InNickName, const BYTE &InIsCostFree )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::SetNickNameCmd::BuildIMsg(pMsg, InNickName, InIsCostFree));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::SetNickNameCmd( const char* InNickName, const BYTE &InIsCostFree )
		// Cmd: Create Party
		Result NetPolicyGame::CreatePartyCmd(  )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::CreatePartyCmd::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::CreatePartyCmd(  )
		// Cmd: Join party
		Result NetPolicyGame::JoinPartyCmd( const PartyUID &InPartyUID, const PlayerID &InInviterID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::JoinPartyCmd::BuildIMsg(pMsg, InPartyUID, InInviterID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::JoinPartyCmd( const PartyUID &InPartyUID, const PlayerID &InInviterID )
		// Cmd: Leave party command
		Result NetPolicyGame::LeavePartyCmd( const PartyUID &InPartyUID, const PlayerID &InPlayerID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::LeavePartyCmd::BuildIMsg(pMsg, InPartyUID, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::LeavePartyCmd( const PartyUID &InPartyUID, const PlayerID &InPlayerID )
		// Cmd: Kick player from the party
		Result NetPolicyGame::PartyKickPlayerCmd( const PartyUID &InPartyUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyKickPlayerCmd::BuildIMsg(pMsg, InPartyUID, InPlayerID, InPlayerToKick));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::PartyKickPlayerCmd( const PartyUID &InPartyUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// Cmd: Invite a player to the party
		Result NetPolicyGame::PartyInviteCmd( const PlayerID &InInviteTargetID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyInviteCmd::BuildIMsg(pMsg, InInviteTargetID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::PartyInviteCmd( const PlayerID &InInviteTargetID )
		// Cmd: Send Party quick chat message
		Result NetPolicyGame::PartyQuickChatMessageCmd( const UINT32 &InQuickChatID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyQuickChatMessageCmd::BuildIMsg(pMsg, InQuickChatID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::PartyQuickChatMessageCmd( const UINT32 &InQuickChatID )
		// Cmd: Party chatting
		Result NetPolicyGame::PartyChatMessageCmd( const char* InChatMessage )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyChatMessageCmd::BuildIMsg(pMsg, InChatMessage));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::PartyChatMessageCmd( const char* InChatMessage )
		// Cmd: Join to a game
		Result NetPolicyGame::JoinGameCmd( const PlayerID &InPlayerID, const AuthTicket &InTicket, const GameInsUID &InInsUID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::JoinGameCmd::BuildIMsg(pMsg, InPlayerID, InTicket, InInsUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::JoinGameCmd( const PlayerID &InPlayerID, const AuthTicket &InTicket, const GameInsUID &InInsUID )
		// Cmd: Leave Game
		Result NetPolicyGame::LeaveGameCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::LeaveGameCmd::BuildIMsg(pMsg, InGameInsUID, InPlayerID, InTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::LeaveGameCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
		// Cmd: Kick player
		Result NetPolicyGame::KickPlayerCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::KickPlayerCmd::BuildIMsg(pMsg, InGameInsUID, InPlayerID, InPlayerToKick));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::KickPlayerCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// Cmd: Assign role + Game state reset
		Result NetPolicyGame::AssignRoleCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::AssignRoleCmd::BuildIMsg(pMsg, InGameInsUID, InPlayerID, InTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::AssignRoleCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
		// Cmd: Send chatting message to the game
		Result NetPolicyGame::ChatMessageCmd( const char* InChatMessage, const PlayerRole &InRole )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::ChatMessageCmd::BuildIMsg(pMsg, InChatMessage, InRole));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::ChatMessageCmd( const char* InChatMessage, const PlayerRole &InRole )
		// Cmd: Advance game
		Result NetPolicyGame::AdvanceGameCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::AdvanceGameCmd::BuildIMsg(pMsg, InGameInsUID, InPlayerID, InTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::AdvanceGameCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
		// Cmd: Vote game advance
		Result NetPolicyGame::VoteGameAdvanceCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::VoteGameAdvanceCmd::BuildIMsg(pMsg, InGameInsUID, InPlayerID, InTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::VoteGameAdvanceCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
		// Cmd: Vote
		Result NetPolicyGame::VoteCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const PlayerID &InVoteTarget, const UINT32 &InActionSerial )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::VoteCmd::BuildIMsg(pMsg, InGameInsUID, InPlayerID, InTicket, InVoteTarget, InActionSerial));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::VoteCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const PlayerID &InVoteTarget, const UINT32 &InActionSerial )
		// Cmd: Play again with the current players
		Result NetPolicyGame::GamePlayAgainCmd(  )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GamePlayAgainCmd::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::GamePlayAgainCmd(  )
		// Cmd: Player. reveal a player
		Result NetPolicyGame::GameRevealPlayerCmd( const Array<PlayerID>& InTargetPlayerID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameRevealPlayerCmd::BuildIMsg(pMsg, InTargetPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::GameRevealPlayerCmd( const Array<PlayerID>& InTargetPlayerID )
		// Cmd: Player. revive himself
		Result NetPolicyGame::GamePlayerReviveCmd(  )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GamePlayerReviveCmd::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::GamePlayerReviveCmd(  )
		// Cmd: Player. reset ranking
		Result NetPolicyGame::GamePlayerResetRankCmd(  )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GamePlayerResetRankCmd::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::GamePlayerResetRankCmd(  )
		// Cmd: Request Game match
		Result NetPolicyGame::RequestGameMatchCmd( const BYTE &InNumPlayer, const PlayerRole &InRequestRole )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RequestGameMatchCmd::BuildIMsg(pMsg, InNumPlayer, InRequestRole));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::RequestGameMatchCmd( const BYTE &InNumPlayer, const PlayerRole &InRequestRole )
		// Cmd: Cancel Game match
		Result NetPolicyGame::CancelGameMatchCmd(  )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::CancelGameMatchCmd::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::CancelGameMatchCmd(  )
		// Cmd: Buy shop item prepare
		Result NetPolicyGame::BuyShopItemPrepareCmd( const UINT32 &InShopItemID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::BuyShopItemPrepareCmd::BuildIMsg(pMsg, InShopItemID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::BuyShopItemPrepareCmd( const UINT32 &InShopItemID )
		// Cmd: Buy shop item
		Result NetPolicyGame::BuyShopItemCmd( const UINT32 &InShopItemID, const char* InPlatform, const char* InPackageName, const char* InPurchaseTransactionID, const Array<BYTE>& InPurchaseToken )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::BuyShopItemCmd::BuildIMsg(pMsg, InShopItemID, InPlatform, InPackageName, InPurchaseTransactionID, InPurchaseToken));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::BuyShopItemCmd( const UINT32 &InShopItemID, const char* InPlatform, const char* InPackageName, const char* InPurchaseTransactionID, const Array<BYTE>& InPurchaseToken )
		// Cmd: Give my stamina to other player
		Result NetPolicyGame::GiveStaminaCmd( const PlayerID &InTargetPlayer )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GiveStaminaCmd::BuildIMsg(pMsg, InTargetPlayer));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::GiveStaminaCmd( const PlayerID &InTargetPlayer )
		// Cmd: For debug, Change configue preset
		Result NetPolicyGame::SetPresetGameConfigIDCmd( const UINT32 &InPresetID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::SetPresetGameConfigIDCmd::BuildIMsg(pMsg, InPresetID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::SetPresetGameConfigIDCmd( const UINT32 &InPresetID )
		// Cmd: For Debug
		Result NetPolicyGame::GainGameResourceCmd( const INT32 &InResource, const INT32 &InValue )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GainGameResourceCmd::BuildIMsg(pMsg, InResource, InValue));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyGame::GainGameResourceCmd( const INT32 &InResource, const INT32 &InValue )


		// Cmd: Player connected from a login server and moved to game server
		Result NetSvrPolicyGame::JoinGameServerRes( const Result &InResult, const char* InNickName, const GameInsUID &InGameUID, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const MatchingQueueTicket &InMatchingTicket )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::JoinGameServerRes::BuildIMsg(pMsg, InResult, InNickName, InGameUID, InPartyUID, InPartyLeaderID, InMatchingTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::JoinGameServerRes( const Result &InResult, const char* InNickName, const GameInsUID &InGameUID, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: player complition statues
		Result NetSvrPolicyGame::GetComplitionStateRes( const Result &InResult, const char* InComplitionState )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetComplitionStateRes::BuildIMsg(pMsg, InResult, InComplitionState));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GetComplitionStateRes( const Result &InResult, const char* InComplitionState )
		// Cmd: Player complition state
		Result NetSvrPolicyGame::SetComplitionStateRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::SetComplitionStateRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::SetComplitionStateRes( const Result &InResult )
		// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
		Result NetSvrPolicyGame::RegisterGCMRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RegisterGCMRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::RegisterGCMRes( const Result &InResult )
		// Cmd: Unregister Google notification service ID
		Result NetSvrPolicyGame::UnregisterGCMRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::UnregisterGCMRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::UnregisterGCMRes( const Result &InResult )
		// Cmd: Invite friend
		Result NetSvrPolicyGame::InviteFriendRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::InviteFriendRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::InviteFriendRes( const Result &InResult )
		// Cmd: Accept friend request
		Result NetSvrPolicyGame::AcceptFriendRequestRes( const Result &InResult, const FriendInformation &InNewFriend )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::AcceptFriendRequestRes::BuildIMsg(pMsg, InResult, InNewFriend));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::AcceptFriendRequestRes( const Result &InResult, const FriendInformation &InNewFriend )
		// S2C: Notification for friend request is accepted
		Result NetSvrPolicyGame::FriendRequestAcceptedS2CEvt( const FriendInformation &InAccepter )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::FriendRequestAcceptedS2CEvt::BuildIMsg(pMsg, InAccepter));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::FriendRequestAcceptedS2CEvt( const FriendInformation &InAccepter )
		// Cmd: Remove friden form the friend list
		Result NetSvrPolicyGame::RemoveFriendRes( const Result &InResult, const PlayerID &InFriendID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RemoveFriendRes::BuildIMsg(pMsg, InResult, InFriendID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::RemoveFriendRes( const Result &InResult, const PlayerID &InFriendID )
		// S2C: Friend removed
		Result NetSvrPolicyGame::FriendRemovedS2CEvt( const PlayerID &InFriendID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::FriendRemovedS2CEvt::BuildIMsg(pMsg, InFriendID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::FriendRemovedS2CEvt( const PlayerID &InFriendID )
		// Cmd: Get friend list
		Result NetSvrPolicyGame::GetFriendListRes( const Result &InResult, const UINT16 &InMaxFriendSlot, const UINT16 &InTotalNumberOfFriends, const UINT16 &InStartIndex, const Array<FriendInformation>& InFriendList )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetFriendListRes::BuildIMsg(pMsg, InResult, InMaxFriendSlot, InTotalNumberOfFriends, InStartIndex, InFriendList));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GetFriendListRes( const Result &InResult, const UINT16 &InMaxFriendSlot, const UINT16 &InTotalNumberOfFriends, const UINT16 &InStartIndex, const Array<FriendInformation>& InFriendList )
		// Cmd: Query notification list
		Result NetSvrPolicyGame::GetNotificationListRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetNotificationListRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GetNotificationListRes( const Result &InResult )
		// Cmd: Delete notification
		Result NetSvrPolicyGame::DeleteNotificationRes( const Result &InResult, const UINT32 &InNotificationID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::DeleteNotificationRes::BuildIMsg(pMsg, InResult, InNotificationID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::DeleteNotificationRes( const Result &InResult, const UINT32 &InNotificationID )
		// Cmd: Set notification is read
		Result NetSvrPolicyGame::SetNotificationReadRes( const Result &InResult, const UINT32 &InNotificationID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::SetNotificationReadRes::BuildIMsg(pMsg, InResult, InNotificationID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::SetNotificationReadRes( const Result &InResult, const UINT32 &InNotificationID )
		// Cmd: Accept notification
		Result NetSvrPolicyGame::AcceptNotificationRes( const Result &InResult, const UINT32 &InNotificationID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::AcceptNotificationRes::BuildIMsg(pMsg, InResult, InNotificationID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::AcceptNotificationRes( const Result &InResult, const UINT32 &InNotificationID )
		// S2C: Notify new notification
		Result NetSvrPolicyGame::NotifyS2CEvt( const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const BYTE &InIsRead, const UINT64 &InTimeStamp )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::NotifyS2CEvt::BuildIMsg(pMsg, InNotificationID, InMessageID, InMessageParam0, InMessageParam1, InMessageText, InIsRead, InTimeStamp));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::NotifyS2CEvt( const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const BYTE &InIsRead, const UINT64 &InTimeStamp )
		// Cmd: Query playerID list
		Result NetSvrPolicyGame::FindPlayerByEMailRes( const Result &InResult, const PlayerInformation &InPlayer )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::FindPlayerByEMailRes::BuildIMsg(pMsg, InResult, InPlayer));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::FindPlayerByEMailRes( const Result &InResult, const PlayerInformation &InPlayer )
		// Cmd: Query playerID list
		Result NetSvrPolicyGame::FindPlayerByPlayerIDRes( const Result &InResult, const PlayerInformation &InPlayer )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::FindPlayerByPlayerIDRes::BuildIMsg(pMsg, InResult, InPlayer));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::FindPlayerByPlayerIDRes( const Result &InResult, const PlayerInformation &InPlayer )
		// Cmd: *Request Player Status Update
		Result NetSvrPolicyGame::RequestPlayerStatusUpdateRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RequestPlayerStatusUpdateRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::RequestPlayerStatusUpdateRes( const Result &InResult )
		// S2C: *Notify Player Status Updated
		Result NetSvrPolicyGame::NotifyPlayerStatusUpdatedS2CEvt( const PlayerID &InPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::NotifyPlayerStatusUpdatedS2CEvt::BuildIMsg(pMsg, InPlayerID, InLatestActiveTime, InIsInGame));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::NotifyPlayerStatusUpdatedS2CEvt( const PlayerID &InPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame )
		// Cmd: Get Ranking lise
		Result NetSvrPolicyGame::GetRankingListRes( const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetRankingListRes::BuildIMsg(pMsg, InResult, InRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GetRankingListRes( const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
		// Cmd: Game user game play information
		Result NetSvrPolicyGame::GetUserGamePlayerInfoRes( const Result &InResult, const INT16 &InLevel, const INT64 &InExp, const INT64 &InGameMoney, const INT64 &InGem, const INT16 &InStamina, const UINT32 &InLastUpdateTime, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetUserGamePlayerInfoRes::BuildIMsg(pMsg, InResult, InLevel, InExp, InGameMoney, InGem, InStamina, InLastUpdateTime, InTotalPlayed, InWinPlaySC, InWinPlaySM, InWinPlaySS, InLosePlaySC, InLosePlaySM, InLosePlaySS, InWinPlayNC, InWinPlayNM, InWinPlayNS, InLosePlayNC, InLosePlayNM, InLosePlayNS, InWeeklyWin, InWeeklyLose));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GetUserGamePlayerInfoRes( const Result &InResult, const INT16 &InLevel, const INT64 &InExp, const INT64 &InGameMoney, const INT64 &InGem, const INT16 &InStamina, const UINT32 &InLastUpdateTime, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose )
		// Cmd: Game game play information
		Result NetSvrPolicyGame::GetGamePlayerInfoRes( const Result &InResult, const PlayerID &InPlayerID, const INT16 &InLevel, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetGamePlayerInfoRes::BuildIMsg(pMsg, InResult, InPlayerID, InLevel, InTotalPlayed, InWinPlaySC, InWinPlaySM, InWinPlaySS, InLosePlaySC, InLosePlaySM, InLosePlaySS, InWinPlayNC, InWinPlayNM, InWinPlayNS, InLosePlayNC, InLosePlayNM, InLosePlayNS, InWeeklyWin, InWeeklyLose));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GetGamePlayerInfoRes( const Result &InResult, const PlayerID &InPlayerID, const INT16 &InLevel, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose )
		// S2C: Player level up event
		Result NetSvrPolicyGame::LevelUpS2CEvt( const UINT64 &InCurrentTotalExp, const UINT32 &InCurrentLevel )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::LevelUpS2CEvt::BuildIMsg(pMsg, InCurrentTotalExp, InCurrentLevel));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::LevelUpS2CEvt( const UINT64 &InCurrentTotalExp, const UINT32 &InCurrentLevel )
		// Cmd: Change NickName
		Result NetSvrPolicyGame::SetNickNameRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::SetNickNameRes::BuildIMsg(pMsg, InResult, InTotalGem, InTotalGameMoney));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::SetNickNameRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		// Cmd: Create Party
		Result NetSvrPolicyGame::CreatePartyRes( const Result &InResult, const PartyUID &InPartyUID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::CreatePartyRes::BuildIMsg(pMsg, InResult, InPartyUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::CreatePartyRes( const Result &InResult, const PartyUID &InPartyUID )
		// Cmd: Join party
		Result NetSvrPolicyGame::JoinPartyRes( const Result &InResult, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::JoinPartyRes::BuildIMsg(pMsg, InResult, InPartyUID, InPartyLeaderID, InChatHistoryData));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::JoinPartyRes( const Result &InResult, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData )
		// S2C: Player Joined event
		Result NetSvrPolicyGame::PartyPlayerJoinedS2CEvt( const PartyUID &InPartyUID, const PlayerInformation &InJoinedPlayer )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyPlayerJoinedS2CEvt::BuildIMsg(pMsg, InPartyUID, InJoinedPlayer));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::PartyPlayerJoinedS2CEvt( const PartyUID &InPartyUID, const PlayerInformation &InJoinedPlayer )
		// S2C: Party leader changed event
		Result NetSvrPolicyGame::PartyLeaderChangedS2CEvt( const PartyUID &InPartyUID, const PlayerID &InNewLeaderID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyLeaderChangedS2CEvt::BuildIMsg(pMsg, InPartyUID, InNewLeaderID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::PartyLeaderChangedS2CEvt( const PartyUID &InPartyUID, const PlayerID &InNewLeaderID )
		// Cmd: Leave party command
		Result NetSvrPolicyGame::LeavePartyRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::LeavePartyRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::LeavePartyRes( const Result &InResult )
		// S2C: Party Player left event
		Result NetSvrPolicyGame::PartyPlayerLeftS2CEvt( const PartyUID &InPartyUID, const PlayerID &InLeftPlayerID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyPlayerLeftS2CEvt::BuildIMsg(pMsg, InPartyUID, InLeftPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::PartyPlayerLeftS2CEvt( const PartyUID &InPartyUID, const PlayerID &InLeftPlayerID )
		// Cmd: Kick player from the party
		Result NetSvrPolicyGame::PartyKickPlayerRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyKickPlayerRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::PartyKickPlayerRes( const Result &InResult )
		// S2C: Party Player kicked message
		Result NetSvrPolicyGame::PartyPlayerKickedS2CEvt( const PartyUID &InPartyUID, const PlayerID &InKickedPlayerID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyPlayerKickedS2CEvt::BuildIMsg(pMsg, InPartyUID, InKickedPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::PartyPlayerKickedS2CEvt( const PartyUID &InPartyUID, const PlayerID &InKickedPlayerID )
		// Cmd: Invite a player to the party
		Result NetSvrPolicyGame::PartyInviteRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyInviteRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::PartyInviteRes( const Result &InResult )
		// S2C: Party invite requested
		Result NetSvrPolicyGame::PartyInviteRequestedS2CEvt( const PlayerID &InInviterID, const char* InInviterName, const PartyUID &InPartyToJoinUID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyInviteRequestedS2CEvt::BuildIMsg(pMsg, InInviterID, InInviterName, InPartyToJoinUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::PartyInviteRequestedS2CEvt( const PlayerID &InInviterID, const char* InInviterName, const PartyUID &InPartyToJoinUID )
		// Cmd: Send Party quick chat message
		Result NetSvrPolicyGame::PartyQuickChatMessageRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyQuickChatMessageRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::PartyQuickChatMessageRes( const Result &InResult )
		// S2C: Party Chatting message event
		Result NetSvrPolicyGame::PartyQuickChatMessageS2CEvt( const PlayerID &InSenderID, const UINT32 &InQuickChatID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyQuickChatMessageS2CEvt::BuildIMsg(pMsg, InSenderID, InQuickChatID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::PartyQuickChatMessageS2CEvt( const PlayerID &InSenderID, const UINT32 &InQuickChatID )
		// Cmd: Party chatting
		Result NetSvrPolicyGame::PartyChatMessageRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyChatMessageRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::PartyChatMessageRes( const Result &InResult )
		// S2C: Party Chatting message event
		Result NetSvrPolicyGame::PartyChatMessageS2CEvt( const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyChatMessageS2CEvt::BuildIMsg(pMsg, InSenderID, InSenderName, InChatMessage));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::PartyChatMessageS2CEvt( const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
		// Cmd: Join to a game
		Result NetSvrPolicyGame::JoinGameRes( const Result &InResult, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::JoinGameRes::BuildIMsg(pMsg, InResult, InInsUID, InTimeStamp, InGameState, InDay, InMaxPlayer, InPlayerIndex, InPlayerCharacter, InRole, InDead, InChatHistoryData, InGameLogData));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::JoinGameRes( const Result &InResult, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData )
		// S2C: Player Joined in the game
		Result NetSvrPolicyGame::PlayerJoinedS2CEvt( const GameInsUID &InGameInsUID, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PlayerJoinedS2CEvt::BuildIMsg(pMsg, InGameInsUID, InJoinedPlayer, InJoinedPlayerRole, InJoinedPlayerDead, InJoinedPlayerIndex, InJoinedPlayerCharacter));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::PlayerJoinedS2CEvt( const GameInsUID &InGameInsUID, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter )
		// Cmd: Leave Game
		Result NetSvrPolicyGame::LeaveGameRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::LeaveGameRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::LeaveGameRes( const Result &InResult )
		// S2C: Player left event
		Result NetSvrPolicyGame::PlayerLeftS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InLeftPlayerID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PlayerLeftS2CEvt::BuildIMsg(pMsg, InGameInsUID, InLeftPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::PlayerLeftS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InLeftPlayerID )
		// Cmd: Kick player
		Result NetSvrPolicyGame::KickPlayerRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::KickPlayerRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::KickPlayerRes( const Result &InResult )
		// S2C: Player kicked
		Result NetSvrPolicyGame::PlayerKickedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InKickedPlayerID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PlayerKickedS2CEvt::BuildIMsg(pMsg, InGameInsUID, InKickedPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::PlayerKickedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InKickedPlayerID )
		// Cmd: Assign role + Game state reset
		Result NetSvrPolicyGame::AssignRoleRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::AssignRoleRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::AssignRoleRes( const Result &InResult )
		// S2C: Role assigned event
		Result NetSvrPolicyGame::RoleAssignedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerRole &InRole )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RoleAssignedS2CEvt::BuildIMsg(pMsg, InGameInsUID, InPlayerID, InRole));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::RoleAssignedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerRole &InRole )
		// Cmd: Send chatting message to the game
		Result NetSvrPolicyGame::ChatMessageRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::ChatMessageRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::ChatMessageRes( const Result &InResult )
		// S2C: Chatting message event 
		Result NetSvrPolicyGame::ChatMessageS2CEvt( const PlayerID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::ChatMessageS2CEvt::BuildIMsg(pMsg, InSenderID, InRole, InSenderName, InChatMessage));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::ChatMessageS2CEvt( const PlayerID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )
		// Cmd: Advance game
		Result NetSvrPolicyGame::AdvanceGameRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::AdvanceGameRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::AdvanceGameRes( const Result &InResult )
		// S2C: The game state is advanced
		Result NetSvrPolicyGame::GameAdvancedS2CEvt( const GameInsUID &InGameInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameAdvancedS2CEvt::BuildIMsg(pMsg, InGameInsUID, InTimeStamp, InGameState, InDay));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GameAdvancedS2CEvt( const GameInsUID &InGameInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay )
		// S2C: Game is ended
		Result NetSvrPolicyGame::GameEndedS2CEvt( const GameInsUID &InGameInsUID, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameEndedS2CEvt::BuildIMsg(pMsg, InGameInsUID, InWinner, InGainedExp, InGainedGameMoney));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GameEndedS2CEvt( const GameInsUID &InGameInsUID, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney )
		// Cmd: Vote game advance
		Result NetSvrPolicyGame::VoteGameAdvanceRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::VoteGameAdvanceRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::VoteGameAdvanceRes( const Result &InResult )
		// S2C: GameAdvance is Voted
		Result NetSvrPolicyGame::GameAdvanceVotedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InVoter )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameAdvanceVotedS2CEvt::BuildIMsg(pMsg, InGameInsUID, InVoter));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GameAdvanceVotedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InVoter )
		// Cmd: Vote
		Result NetSvrPolicyGame::VoteRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::VoteRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::VoteRes( const Result &InResult )
		// S2C: Player Voted
		Result NetSvrPolicyGame::VotedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InVoter, const PlayerID &InVotedTarget )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::VotedS2CEvt::BuildIMsg(pMsg, InGameInsUID, InVoter, InVotedTarget));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::VotedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InVoter, const PlayerID &InVotedTarget )
		// S2C: Vote is ended
		Result NetSvrPolicyGame::VoteEndS2CEvt( const GameInsUID &InGameInsUID, const Array<PlayerID>& InVoted )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::VoteEndS2CEvt::BuildIMsg(pMsg, InGameInsUID, InVoted));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::VoteEndS2CEvt( const GameInsUID &InGameInsUID, const Array<PlayerID>& InVoted )
		// S2C: Player Killed
		Result NetSvrPolicyGame::PlayerKilledS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PlayerKilledS2CEvt::BuildIMsg(pMsg, InGameInsUID, InKilledPlayer, InReason));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::PlayerKilledS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason )
		// S2C: Player Voted
		Result NetSvrPolicyGame::PlayerRevealedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PlayerRevealedS2CEvt::BuildIMsg(pMsg, InGameInsUID, InRevealedPlayerID, InRole, InReason));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::PlayerRevealedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason )
		// Cmd: Play again with the current players
		Result NetSvrPolicyGame::GamePlayAgainRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GamePlayAgainRes::BuildIMsg(pMsg, InResult, InTotalGem, InTotalGameMoney));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GamePlayAgainRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
		Result NetSvrPolicyGame::GamePlayAgainS2CEvt( const PartyUID &InPartyUID, const PlayerID &InLeadPlayer )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GamePlayAgainS2CEvt::BuildIMsg(pMsg, InPartyUID, InLeadPlayer));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GamePlayAgainS2CEvt( const PartyUID &InPartyUID, const PlayerID &InLeadPlayer )
		// Cmd: Player. reveal a player
		Result NetSvrPolicyGame::GameRevealPlayerRes( const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameRevealPlayerRes::BuildIMsg(pMsg, InResult, InRevealedPlayerID, InRevealedRole, InTotalGem, InTotalGameMoney));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GameRevealPlayerRes( const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		// Cmd: Player. revive himself
		Result NetSvrPolicyGame::GamePlayerReviveRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GamePlayerReviveRes::BuildIMsg(pMsg, InResult, InTotalGem, InTotalGameMoney));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GamePlayerReviveRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		// S2C: Player is revived
		Result NetSvrPolicyGame::GamePlayerRevivedS2CEvt( const PlayerID &InRevivedPlayerID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GamePlayerRevivedS2CEvt::BuildIMsg(pMsg, InRevivedPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GamePlayerRevivedS2CEvt( const PlayerID &InRevivedPlayerID )
		// Cmd: Player. reset ranking
		Result NetSvrPolicyGame::GamePlayerResetRankRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GamePlayerResetRankRes::BuildIMsg(pMsg, InResult, InTotalGem, InTotalGameMoney));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GamePlayerResetRankRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		// Cmd: Request Game match
		Result NetSvrPolicyGame::RequestGameMatchRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RequestGameMatchRes::BuildIMsg(pMsg, InResult, InTotalGem, InTotalGameMoney));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::RequestGameMatchRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		// S2C: Game matched
		Result NetSvrPolicyGame::GameMatchedS2CEvt( const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData, const UINT32 &InStamina, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameMatchedS2CEvt::BuildIMsg(pMsg, InInsUID, InTimeStamp, InGameState, InDay, InMaxPlayer, InPlayerIndex, InPlayerCharacter, InRole, InDead, InChatHistoryData, InGameLogData, InStamina, InTotalGem, InTotalGameMoney));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GameMatchedS2CEvt( const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData, const UINT32 &InStamina, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		// S2C: Game match failed
		Result NetSvrPolicyGame::GameMatchFailedS2CEvt( const Result &InFailedReason )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameMatchFailedS2CEvt::BuildIMsg(pMsg, InFailedReason));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GameMatchFailedS2CEvt( const Result &InFailedReason )
		// S2C: Game matching started
		Result NetSvrPolicyGame::GameMatchingStartedS2CEvt(  )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameMatchingStartedS2CEvt::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GameMatchingStartedS2CEvt(  )
		// Cmd: Cancel Game match
		Result NetSvrPolicyGame::CancelGameMatchRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::CancelGameMatchRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::CancelGameMatchRes( const Result &InResult )
		// S2C: game matching canceled
		Result NetSvrPolicyGame::GameMatchingCanceledS2CEvt(  )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameMatchingCanceledS2CEvt::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GameMatchingCanceledS2CEvt(  )
		// Cmd: Buy shop item prepare
		Result NetSvrPolicyGame::BuyShopItemPrepareRes( const Result &InResult, const UINT32 &InShopItemID, const char* InPurchaseID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::BuyShopItemPrepareRes::BuildIMsg(pMsg, InResult, InShopItemID, InPurchaseID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::BuyShopItemPrepareRes( const Result &InResult, const UINT32 &InShopItemID, const char* InPurchaseID )
		// Cmd: Buy shop item
		Result NetSvrPolicyGame::BuyShopItemRes( const Result &InResult, const UINT32 &InShopItemID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::BuyShopItemRes::BuildIMsg(pMsg, InResult, InShopItemID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::BuyShopItemRes( const Result &InResult, const UINT32 &InShopItemID )
		// Cmd: Give my stamina to other player
		Result NetSvrPolicyGame::GiveStaminaRes( const Result &InResult, const PlayerID &InTargetPlayer, const UINT64 &InTimeStamp )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GiveStaminaRes::BuildIMsg(pMsg, InResult, InTargetPlayer, InTimeStamp));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GiveStaminaRes( const Result &InResult, const PlayerID &InTargetPlayer, const UINT64 &InTimeStamp )
		// Cmd: For debug, Change configue preset
		Result NetSvrPolicyGame::SetPresetGameConfigIDRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::SetPresetGameConfigIDRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::SetPresetGameConfigIDRes( const Result &InResult )
		// Cmd: For Debug
		Result NetSvrPolicyGame::GainGameResourceRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GainGameResourceRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyGame::GainGameResourceRes( const Result &InResult )


	}; // namespace Policy
}; // namespace BR


