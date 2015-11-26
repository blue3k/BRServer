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
		HRESULT NetPolicyGame::HeartBitC2SEvt(  )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::HeartBitC2SEvt::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::HeartBitC2SEvt(  )
		// Cmd: Player connected from a login server and moved to game server
		HRESULT NetPolicyGame::JoinGameServerCmd( const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::JoinGameServerCmd::BuildIMsg(pMsg, InAccID, InTicket, InLoginEntityUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::JoinGameServerCmd( const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )
		// Cmd: player complition statues
		HRESULT NetPolicyGame::GetComplitionStateCmd(  )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetComplitionStateCmd::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::GetComplitionStateCmd(  )
		// Cmd: Player complition state
		HRESULT NetPolicyGame::SetComplitionStateCmd( const char* InComplitionState )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::SetComplitionStateCmd::BuildIMsg(pMsg, InComplitionState));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::SetComplitionStateCmd( const char* InComplitionState )
		// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
		HRESULT NetPolicyGame::RegisterGCMCmd( const char* InGCMRegisteredID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RegisterGCMCmd::BuildIMsg(pMsg, InGCMRegisteredID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::RegisterGCMCmd( const char* InGCMRegisteredID )
		// Cmd: Unregister Google notification service ID
		HRESULT NetPolicyGame::UnregisterGCMCmd( const char* InGCMRegisteredID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::UnregisterGCMCmd::BuildIMsg(pMsg, InGCMRegisteredID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::UnregisterGCMCmd( const char* InGCMRegisteredID )
		// Cmd: Invite friend
		HRESULT NetPolicyGame::InviteFriendCmd( const PlayerID &InFriendID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::InviteFriendCmd::BuildIMsg(pMsg, InFriendID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::InviteFriendCmd( const PlayerID &InFriendID )
		// Cmd: Accept friend request
		HRESULT NetPolicyGame::AcceptFriendRequestCmd( const PlayerID &InInviterID, const FacebookUID &InInviterFacebookUID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::AcceptFriendRequestCmd::BuildIMsg(pMsg, InInviterID, InInviterFacebookUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::AcceptFriendRequestCmd( const PlayerID &InInviterID, const FacebookUID &InInviterFacebookUID )
		// Cmd: Remove friden form the friend list
		HRESULT NetPolicyGame::RemoveFriendCmd( const PlayerID &InFriendID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RemoveFriendCmd::BuildIMsg(pMsg, InFriendID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::RemoveFriendCmd( const PlayerID &InFriendID )
		// Cmd: Get friend list
		HRESULT NetPolicyGame::GetFriendListCmd( const UINT16 &InStartIndex, const UINT16 &InCount )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetFriendListCmd::BuildIMsg(pMsg, InStartIndex, InCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::GetFriendListCmd( const UINT16 &InStartIndex, const UINT16 &InCount )
		// Cmd: Query notification list
		HRESULT NetPolicyGame::GetNotificationListCmd(  )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetNotificationListCmd::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::GetNotificationListCmd(  )
		// Cmd: Delete notification
		HRESULT NetPolicyGame::DeleteNotificationCmd( const UINT32 &InNotificationID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::DeleteNotificationCmd::BuildIMsg(pMsg, InNotificationID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::DeleteNotificationCmd( const UINT32 &InNotificationID )
		// Cmd: Set notification is read
		HRESULT NetPolicyGame::SetNotificationReadCmd( const UINT32 &InNotificationID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::SetNotificationReadCmd::BuildIMsg(pMsg, InNotificationID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::SetNotificationReadCmd( const UINT32 &InNotificationID )
		// Cmd: Accept notification
		HRESULT NetPolicyGame::AcceptNotificationCmd( const UINT32 &InNotificationID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::AcceptNotificationCmd::BuildIMsg(pMsg, InNotificationID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::AcceptNotificationCmd( const UINT32 &InNotificationID )
		// Cmd: Query playerID list
		HRESULT NetPolicyGame::FindPlayerByEMailCmd( const char* InPlayerEMail )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::FindPlayerByEMailCmd::BuildIMsg(pMsg, InPlayerEMail));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::FindPlayerByEMailCmd( const char* InPlayerEMail )
		// Cmd: Query playerID list
		HRESULT NetPolicyGame::FindPlayerByPlayerIDCmd( const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::FindPlayerByPlayerIDCmd::BuildIMsg(pMsg, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::FindPlayerByPlayerIDCmd( const PlayerID &InPlayerID )
		// Cmd: *Request Player Status Update
		HRESULT NetPolicyGame::RequestPlayerStatusUpdateCmd( const Array<PlayerID>& InTargetPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RequestPlayerStatusUpdateCmd::BuildIMsg(pMsg, InTargetPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::RequestPlayerStatusUpdateCmd( const Array<PlayerID>& InTargetPlayerID )
		// Cmd: Get Ranking lise
		HRESULT NetPolicyGame::GetRankingListCmd( const RankingType &InRankingType, const BYTE &InBaseRanking, const BYTE &InCount )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetRankingListCmd::BuildIMsg(pMsg, InRankingType, InBaseRanking, InCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::GetRankingListCmd( const RankingType &InRankingType, const BYTE &InBaseRanking, const BYTE &InCount )
		// Cmd: Game user game play information
		HRESULT NetPolicyGame::GetUserGamePlayerInfoCmd(  )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetUserGamePlayerInfoCmd::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::GetUserGamePlayerInfoCmd(  )
		// Cmd: Game game play information
		HRESULT NetPolicyGame::GetGamePlayerInfoCmd( const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetGamePlayerInfoCmd::BuildIMsg(pMsg, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::GetGamePlayerInfoCmd( const PlayerID &InPlayerID )
		// Cmd: Change NickName
		HRESULT NetPolicyGame::SetNickNameCmd( const char* InNickName, const BYTE &InIsCostFree )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::SetNickNameCmd::BuildIMsg(pMsg, InNickName, InIsCostFree));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::SetNickNameCmd( const char* InNickName, const BYTE &InIsCostFree )
		// Cmd: Create Party
		HRESULT NetPolicyGame::CreatePartyCmd(  )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::CreatePartyCmd::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::CreatePartyCmd(  )
		// Cmd: Join party
		HRESULT NetPolicyGame::JoinPartyCmd( const PartyUID &InPartyUID, const PlayerID &InInviterID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::JoinPartyCmd::BuildIMsg(pMsg, InPartyUID, InInviterID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::JoinPartyCmd( const PartyUID &InPartyUID, const PlayerID &InInviterID )
		// Cmd: Leave party command
		HRESULT NetPolicyGame::LeavePartyCmd( const PartyUID &InPartyUID, const PlayerID &InPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::LeavePartyCmd::BuildIMsg(pMsg, InPartyUID, InPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::LeavePartyCmd( const PartyUID &InPartyUID, const PlayerID &InPlayerID )
		// Cmd: Kick player from the party
		HRESULT NetPolicyGame::PartyKickPlayerCmd( const PartyUID &InPartyUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyKickPlayerCmd::BuildIMsg(pMsg, InPartyUID, InPlayerID, InPlayerToKick));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::PartyKickPlayerCmd( const PartyUID &InPartyUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// Cmd: Invite a player to the party
		HRESULT NetPolicyGame::PartyInviteCmd( const PlayerID &InInviteTargetID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyInviteCmd::BuildIMsg(pMsg, InInviteTargetID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::PartyInviteCmd( const PlayerID &InInviteTargetID )
		// Cmd: Send Party quick chat message
		HRESULT NetPolicyGame::PartyQuickChatMessageCmd( const UINT32 &InQuickChatID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyQuickChatMessageCmd::BuildIMsg(pMsg, InQuickChatID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::PartyQuickChatMessageCmd( const UINT32 &InQuickChatID )
		// Cmd: Party chatting
		HRESULT NetPolicyGame::PartyChatMessageCmd( const char* InChatMessage )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyChatMessageCmd::BuildIMsg(pMsg, InChatMessage));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::PartyChatMessageCmd( const char* InChatMessage )
		// Cmd: Join to a game
		HRESULT NetPolicyGame::JoinGameCmd( const PlayerID &InPlayerID, const AuthTicket &InTicket, const GameInsUID &InInsUID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::JoinGameCmd::BuildIMsg(pMsg, InPlayerID, InTicket, InInsUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::JoinGameCmd( const PlayerID &InPlayerID, const AuthTicket &InTicket, const GameInsUID &InInsUID )
		// Cmd: Leave Game
		HRESULT NetPolicyGame::LeaveGameCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::LeaveGameCmd::BuildIMsg(pMsg, InGameInsUID, InPlayerID, InTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::LeaveGameCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
		// Cmd: Kick player
		HRESULT NetPolicyGame::KickPlayerCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::KickPlayerCmd::BuildIMsg(pMsg, InGameInsUID, InPlayerID, InPlayerToKick));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::KickPlayerCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
		// Cmd: Assign role + Game state reset
		HRESULT NetPolicyGame::AssignRoleCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::AssignRoleCmd::BuildIMsg(pMsg, InGameInsUID, InPlayerID, InTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::AssignRoleCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
		// Cmd: Send chatting message to the game
		HRESULT NetPolicyGame::ChatMessageCmd( const char* InChatMessage, const PlayerRole &InRole )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::ChatMessageCmd::BuildIMsg(pMsg, InChatMessage, InRole));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::ChatMessageCmd( const char* InChatMessage, const PlayerRole &InRole )
		// Cmd: Advance game
		HRESULT NetPolicyGame::AdvanceGameCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::AdvanceGameCmd::BuildIMsg(pMsg, InGameInsUID, InPlayerID, InTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::AdvanceGameCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
		// Cmd: Vote game advance
		HRESULT NetPolicyGame::VoteGameAdvanceCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::VoteGameAdvanceCmd::BuildIMsg(pMsg, InGameInsUID, InPlayerID, InTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::VoteGameAdvanceCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
		// Cmd: Vote
		HRESULT NetPolicyGame::VoteCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const PlayerID &InVoteTarget, const UINT32 &InActionSerial )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::VoteCmd::BuildIMsg(pMsg, InGameInsUID, InPlayerID, InTicket, InVoteTarget, InActionSerial));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::VoteCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const PlayerID &InVoteTarget, const UINT32 &InActionSerial )
		// Cmd: Play again with the current players
		HRESULT NetPolicyGame::GamePlayAgainCmd(  )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GamePlayAgainCmd::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::GamePlayAgainCmd(  )
		// Cmd: Player. reveal a player
		HRESULT NetPolicyGame::GameRevealPlayerCmd( const Array<PlayerID>& InTargetPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameRevealPlayerCmd::BuildIMsg(pMsg, InTargetPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::GameRevealPlayerCmd( const Array<PlayerID>& InTargetPlayerID )
		// Cmd: Player. revive himself
		HRESULT NetPolicyGame::GamePlayerReviveCmd(  )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GamePlayerReviveCmd::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::GamePlayerReviveCmd(  )
		// Cmd: Player. reset ranking
		HRESULT NetPolicyGame::GamePlayerResetRankCmd(  )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GamePlayerResetRankCmd::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::GamePlayerResetRankCmd(  )
		// Cmd: Request Game match
		HRESULT NetPolicyGame::RequestGameMatchCmd( const BYTE &InNumPlayer, const PlayerRole &InRequestRole )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RequestGameMatchCmd::BuildIMsg(pMsg, InNumPlayer, InRequestRole));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::RequestGameMatchCmd( const BYTE &InNumPlayer, const PlayerRole &InRequestRole )
		// Cmd: Cancel Game match
		HRESULT NetPolicyGame::CancelGameMatchCmd(  )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::CancelGameMatchCmd::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::CancelGameMatchCmd(  )
		// Cmd: Buy shop item prepare
		HRESULT NetPolicyGame::BuyShopItemPrepareCmd( const UINT32 &InShopItemID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::BuyShopItemPrepareCmd::BuildIMsg(pMsg, InShopItemID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::BuyShopItemPrepareCmd( const UINT32 &InShopItemID )
		// Cmd: Buy shop item
		HRESULT NetPolicyGame::BuyShopItemCmd( const UINT32 &InShopItemID, const char* InPlatform, const char* InPackageName, const char* InPurchaseTransactionID, const Array<BYTE>& InPurchaseToken )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::BuyShopItemCmd::BuildIMsg(pMsg, InShopItemID, InPlatform, InPackageName, InPurchaseTransactionID, InPurchaseToken));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::BuyShopItemCmd( const UINT32 &InShopItemID, const char* InPlatform, const char* InPackageName, const char* InPurchaseTransactionID, const Array<BYTE>& InPurchaseToken )
		// Cmd: Give my stamina to other player
		HRESULT NetPolicyGame::GiveStaminaCmd( const PlayerID &InTargetPlayer )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GiveStaminaCmd::BuildIMsg(pMsg, InTargetPlayer));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::GiveStaminaCmd( const PlayerID &InTargetPlayer )
		// Cmd: For debug, Change configue preset
		HRESULT NetPolicyGame::SetPresetGameConfigIDCmd( const UINT32 &InPresetID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::SetPresetGameConfigIDCmd::BuildIMsg(pMsg, InPresetID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::SetPresetGameConfigIDCmd( const UINT32 &InPresetID )
		// Cmd: For Debug
		HRESULT NetPolicyGame::GainGameResourceCmd( const INT32 &InResource, const INT32 &InValue )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GainGameResourceCmd::BuildIMsg(pMsg, InResource, InValue));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyGame::GainGameResourceCmd( const INT32 &InResource, const INT32 &InValue )


		// Cmd: Player connected from a login server and moved to game server
		HRESULT NetSvrPolicyGame::JoinGameServerRes( const HRESULT &InResult, const char* InNickName, const GameInsUID &InGameUID, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const MatchingQueueTicket &InMatchingTicket )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::JoinGameServerRes::BuildIMsg(pMsg, InResult, InNickName, InGameUID, InPartyUID, InPartyLeaderID, InMatchingTicket));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::JoinGameServerRes( const HRESULT &InResult, const char* InNickName, const GameInsUID &InGameUID, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const MatchingQueueTicket &InMatchingTicket )
		// Cmd: player complition statues
		HRESULT NetSvrPolicyGame::GetComplitionStateRes( const HRESULT &InResult, const char* InComplitionState )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetComplitionStateRes::BuildIMsg(pMsg, InResult, InComplitionState));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GetComplitionStateRes( const HRESULT &InResult, const char* InComplitionState )
		// Cmd: Player complition state
		HRESULT NetSvrPolicyGame::SetComplitionStateRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::SetComplitionStateRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::SetComplitionStateRes( const HRESULT &InResult )
		// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
		HRESULT NetSvrPolicyGame::RegisterGCMRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RegisterGCMRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::RegisterGCMRes( const HRESULT &InResult )
		// Cmd: Unregister Google notification service ID
		HRESULT NetSvrPolicyGame::UnregisterGCMRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::UnregisterGCMRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::UnregisterGCMRes( const HRESULT &InResult )
		// Cmd: Invite friend
		HRESULT NetSvrPolicyGame::InviteFriendRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::InviteFriendRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::InviteFriendRes( const HRESULT &InResult )
		// Cmd: Accept friend request
		HRESULT NetSvrPolicyGame::AcceptFriendRequestRes( const HRESULT &InResult, const FriendInformation &InNewFriend )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::AcceptFriendRequestRes::BuildIMsg(pMsg, InResult, InNewFriend));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::AcceptFriendRequestRes( const HRESULT &InResult, const FriendInformation &InNewFriend )
		// S2C: Notification for friend request is accepted
		HRESULT NetSvrPolicyGame::FriendRequestAcceptedS2CEvt( const FriendInformation &InAccepter )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::FriendRequestAcceptedS2CEvt::BuildIMsg(pMsg, InAccepter));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::FriendRequestAcceptedS2CEvt( const FriendInformation &InAccepter )
		// Cmd: Remove friden form the friend list
		HRESULT NetSvrPolicyGame::RemoveFriendRes( const HRESULT &InResult, const PlayerID &InFriendID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RemoveFriendRes::BuildIMsg(pMsg, InResult, InFriendID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::RemoveFriendRes( const HRESULT &InResult, const PlayerID &InFriendID )
		// S2C: Friend removed
		HRESULT NetSvrPolicyGame::FriendRemovedS2CEvt( const PlayerID &InFriendID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::FriendRemovedS2CEvt::BuildIMsg(pMsg, InFriendID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::FriendRemovedS2CEvt( const PlayerID &InFriendID )
		// Cmd: Get friend list
		HRESULT NetSvrPolicyGame::GetFriendListRes( const HRESULT &InResult, const UINT16 &InMaxFriendSlot, const UINT16 &InTotalNumberOfFriends, const UINT16 &InStartIndex, const Array<FriendInformation>& InFriendList )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetFriendListRes::BuildIMsg(pMsg, InResult, InMaxFriendSlot, InTotalNumberOfFriends, InStartIndex, InFriendList));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GetFriendListRes( const HRESULT &InResult, const UINT16 &InMaxFriendSlot, const UINT16 &InTotalNumberOfFriends, const UINT16 &InStartIndex, const Array<FriendInformation>& InFriendList )
		// Cmd: Query notification list
		HRESULT NetSvrPolicyGame::GetNotificationListRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetNotificationListRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GetNotificationListRes( const HRESULT &InResult )
		// Cmd: Delete notification
		HRESULT NetSvrPolicyGame::DeleteNotificationRes( const HRESULT &InResult, const UINT32 &InNotificationID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::DeleteNotificationRes::BuildIMsg(pMsg, InResult, InNotificationID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::DeleteNotificationRes( const HRESULT &InResult, const UINT32 &InNotificationID )
		// Cmd: Set notification is read
		HRESULT NetSvrPolicyGame::SetNotificationReadRes( const HRESULT &InResult, const UINT32 &InNotificationID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::SetNotificationReadRes::BuildIMsg(pMsg, InResult, InNotificationID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::SetNotificationReadRes( const HRESULT &InResult, const UINT32 &InNotificationID )
		// Cmd: Accept notification
		HRESULT NetSvrPolicyGame::AcceptNotificationRes( const HRESULT &InResult, const UINT32 &InNotificationID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::AcceptNotificationRes::BuildIMsg(pMsg, InResult, InNotificationID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::AcceptNotificationRes( const HRESULT &InResult, const UINT32 &InNotificationID )
		// S2C: Notify new notification
		HRESULT NetSvrPolicyGame::NotifyS2CEvt( const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const BYTE &InIsRead, const UINT64 &InTimeStamp )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::NotifyS2CEvt::BuildIMsg(pMsg, InNotificationID, InMessageID, InMessageParam0, InMessageParam1, InMessageText, InIsRead, InTimeStamp));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::NotifyS2CEvt( const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const BYTE &InIsRead, const UINT64 &InTimeStamp )
		// Cmd: Query playerID list
		HRESULT NetSvrPolicyGame::FindPlayerByEMailRes( const HRESULT &InResult, const PlayerInformation &InPlayer )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::FindPlayerByEMailRes::BuildIMsg(pMsg, InResult, InPlayer));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::FindPlayerByEMailRes( const HRESULT &InResult, const PlayerInformation &InPlayer )
		// Cmd: Query playerID list
		HRESULT NetSvrPolicyGame::FindPlayerByPlayerIDRes( const HRESULT &InResult, const PlayerInformation &InPlayer )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::FindPlayerByPlayerIDRes::BuildIMsg(pMsg, InResult, InPlayer));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::FindPlayerByPlayerIDRes( const HRESULT &InResult, const PlayerInformation &InPlayer )
		// Cmd: *Request Player Status Update
		HRESULT NetSvrPolicyGame::RequestPlayerStatusUpdateRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RequestPlayerStatusUpdateRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::RequestPlayerStatusUpdateRes( const HRESULT &InResult )
		// S2C: *Notify Player Status Updated
		HRESULT NetSvrPolicyGame::NotifyPlayerStatusUpdatedS2CEvt( const PlayerID &InPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::NotifyPlayerStatusUpdatedS2CEvt::BuildIMsg(pMsg, InPlayerID, InLatestActiveTime, InIsInGame));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::NotifyPlayerStatusUpdatedS2CEvt( const PlayerID &InPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame )
		// Cmd: Get Ranking lise
		HRESULT NetSvrPolicyGame::GetRankingListRes( const HRESULT &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetRankingListRes::BuildIMsg(pMsg, InResult, InRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GetRankingListRes( const HRESULT &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
		// Cmd: Game user game play information
		HRESULT NetSvrPolicyGame::GetUserGamePlayerInfoRes( const HRESULT &InResult, const INT16 &InLevel, const INT64 &InExp, const INT64 &InGameMoney, const INT64 &InGem, const INT16 &InStamina, const UINT32 &InLastUpdateTime, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetUserGamePlayerInfoRes::BuildIMsg(pMsg, InResult, InLevel, InExp, InGameMoney, InGem, InStamina, InLastUpdateTime, InTotalPlayed, InWinPlaySC, InWinPlaySM, InWinPlaySS, InLosePlaySC, InLosePlaySM, InLosePlaySS, InWinPlayNC, InWinPlayNM, InWinPlayNS, InLosePlayNC, InLosePlayNM, InLosePlayNS, InWeeklyWin, InWeeklyLose));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GetUserGamePlayerInfoRes( const HRESULT &InResult, const INT16 &InLevel, const INT64 &InExp, const INT64 &InGameMoney, const INT64 &InGem, const INT16 &InStamina, const UINT32 &InLastUpdateTime, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose )
		// Cmd: Game game play information
		HRESULT NetSvrPolicyGame::GetGamePlayerInfoRes( const HRESULT &InResult, const PlayerID &InPlayerID, const INT16 &InLevel, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GetGamePlayerInfoRes::BuildIMsg(pMsg, InResult, InPlayerID, InLevel, InTotalPlayed, InWinPlaySC, InWinPlaySM, InWinPlaySS, InLosePlaySC, InLosePlaySM, InLosePlaySS, InWinPlayNC, InWinPlayNM, InWinPlayNS, InLosePlayNC, InLosePlayNM, InLosePlayNS, InWeeklyWin, InWeeklyLose));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GetGamePlayerInfoRes( const HRESULT &InResult, const PlayerID &InPlayerID, const INT16 &InLevel, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose )
		// S2C: Player level up event
		HRESULT NetSvrPolicyGame::LevelUpS2CEvt( const UINT64 &InCurrentTotalExp, const UINT32 &InCurrentLevel )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::LevelUpS2CEvt::BuildIMsg(pMsg, InCurrentTotalExp, InCurrentLevel));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::LevelUpS2CEvt( const UINT64 &InCurrentTotalExp, const UINT32 &InCurrentLevel )
		// Cmd: Change NickName
		HRESULT NetSvrPolicyGame::SetNickNameRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::SetNickNameRes::BuildIMsg(pMsg, InResult, InTotalGem, InTotalGameMoney));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::SetNickNameRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		// Cmd: Create Party
		HRESULT NetSvrPolicyGame::CreatePartyRes( const HRESULT &InResult, const PartyUID &InPartyUID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::CreatePartyRes::BuildIMsg(pMsg, InResult, InPartyUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::CreatePartyRes( const HRESULT &InResult, const PartyUID &InPartyUID )
		// Cmd: Join party
		HRESULT NetSvrPolicyGame::JoinPartyRes( const HRESULT &InResult, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::JoinPartyRes::BuildIMsg(pMsg, InResult, InPartyUID, InPartyLeaderID, InChatHistoryData));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::JoinPartyRes( const HRESULT &InResult, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData )
		// S2C: Player Joined event
		HRESULT NetSvrPolicyGame::PartyPlayerJoinedS2CEvt( const PartyUID &InPartyUID, const PlayerInformation &InJoinedPlayer )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyPlayerJoinedS2CEvt::BuildIMsg(pMsg, InPartyUID, InJoinedPlayer));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::PartyPlayerJoinedS2CEvt( const PartyUID &InPartyUID, const PlayerInformation &InJoinedPlayer )
		// S2C: Party leader changed event
		HRESULT NetSvrPolicyGame::PartyLeaderChangedS2CEvt( const PartyUID &InPartyUID, const PlayerID &InNewLeaderID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyLeaderChangedS2CEvt::BuildIMsg(pMsg, InPartyUID, InNewLeaderID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::PartyLeaderChangedS2CEvt( const PartyUID &InPartyUID, const PlayerID &InNewLeaderID )
		// Cmd: Leave party command
		HRESULT NetSvrPolicyGame::LeavePartyRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::LeavePartyRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::LeavePartyRes( const HRESULT &InResult )
		// S2C: Party Player left event
		HRESULT NetSvrPolicyGame::PartyPlayerLeftS2CEvt( const PartyUID &InPartyUID, const PlayerID &InLeftPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyPlayerLeftS2CEvt::BuildIMsg(pMsg, InPartyUID, InLeftPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::PartyPlayerLeftS2CEvt( const PartyUID &InPartyUID, const PlayerID &InLeftPlayerID )
		// Cmd: Kick player from the party
		HRESULT NetSvrPolicyGame::PartyKickPlayerRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyKickPlayerRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::PartyKickPlayerRes( const HRESULT &InResult )
		// S2C: Party Player kicked message
		HRESULT NetSvrPolicyGame::PartyPlayerKickedS2CEvt( const PartyUID &InPartyUID, const PlayerID &InKickedPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyPlayerKickedS2CEvt::BuildIMsg(pMsg, InPartyUID, InKickedPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::PartyPlayerKickedS2CEvt( const PartyUID &InPartyUID, const PlayerID &InKickedPlayerID )
		// Cmd: Invite a player to the party
		HRESULT NetSvrPolicyGame::PartyInviteRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyInviteRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::PartyInviteRes( const HRESULT &InResult )
		// S2C: Party invite requested
		HRESULT NetSvrPolicyGame::PartyInviteRequestedS2CEvt( const PlayerID &InInviterID, const char* InInviterName, const PartyUID &InPartyToJoinUID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyInviteRequestedS2CEvt::BuildIMsg(pMsg, InInviterID, InInviterName, InPartyToJoinUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::PartyInviteRequestedS2CEvt( const PlayerID &InInviterID, const char* InInviterName, const PartyUID &InPartyToJoinUID )
		// Cmd: Send Party quick chat message
		HRESULT NetSvrPolicyGame::PartyQuickChatMessageRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyQuickChatMessageRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::PartyQuickChatMessageRes( const HRESULT &InResult )
		// S2C: Party Chatting message event
		HRESULT NetSvrPolicyGame::PartyQuickChatMessageS2CEvt( const PlayerID &InSenderID, const UINT32 &InQuickChatID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyQuickChatMessageS2CEvt::BuildIMsg(pMsg, InSenderID, InQuickChatID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::PartyQuickChatMessageS2CEvt( const PlayerID &InSenderID, const UINT32 &InQuickChatID )
		// Cmd: Party chatting
		HRESULT NetSvrPolicyGame::PartyChatMessageRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyChatMessageRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::PartyChatMessageRes( const HRESULT &InResult )
		// S2C: Party Chatting message event
		HRESULT NetSvrPolicyGame::PartyChatMessageS2CEvt( const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PartyChatMessageS2CEvt::BuildIMsg(pMsg, InSenderID, InSenderName, InChatMessage));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::PartyChatMessageS2CEvt( const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
		// Cmd: Join to a game
		HRESULT NetSvrPolicyGame::JoinGameRes( const HRESULT &InResult, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::JoinGameRes::BuildIMsg(pMsg, InResult, InInsUID, InTimeStamp, InGameState, InDay, InMaxPlayer, InPlayerIndex, InPlayerCharacter, InRole, InDead, InChatHistoryData, InGameLogData));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::JoinGameRes( const HRESULT &InResult, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData )
		// S2C: Player Joined in the game
		HRESULT NetSvrPolicyGame::PlayerJoinedS2CEvt( const GameInsUID &InGameInsUID, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PlayerJoinedS2CEvt::BuildIMsg(pMsg, InGameInsUID, InJoinedPlayer, InJoinedPlayerRole, InJoinedPlayerDead, InJoinedPlayerIndex, InJoinedPlayerCharacter));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::PlayerJoinedS2CEvt( const GameInsUID &InGameInsUID, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter )
		// Cmd: Leave Game
		HRESULT NetSvrPolicyGame::LeaveGameRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::LeaveGameRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::LeaveGameRes( const HRESULT &InResult )
		// S2C: Player left event
		HRESULT NetSvrPolicyGame::PlayerLeftS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InLeftPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PlayerLeftS2CEvt::BuildIMsg(pMsg, InGameInsUID, InLeftPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::PlayerLeftS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InLeftPlayerID )
		// Cmd: Kick player
		HRESULT NetSvrPolicyGame::KickPlayerRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::KickPlayerRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::KickPlayerRes( const HRESULT &InResult )
		// S2C: Player kicked
		HRESULT NetSvrPolicyGame::PlayerKickedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InKickedPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PlayerKickedS2CEvt::BuildIMsg(pMsg, InGameInsUID, InKickedPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::PlayerKickedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InKickedPlayerID )
		// Cmd: Assign role + Game state reset
		HRESULT NetSvrPolicyGame::AssignRoleRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::AssignRoleRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::AssignRoleRes( const HRESULT &InResult )
		// S2C: Role assigned event
		HRESULT NetSvrPolicyGame::RoleAssignedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerRole &InRole )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RoleAssignedS2CEvt::BuildIMsg(pMsg, InGameInsUID, InPlayerID, InRole));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::RoleAssignedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerRole &InRole )
		// Cmd: Send chatting message to the game
		HRESULT NetSvrPolicyGame::ChatMessageRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::ChatMessageRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::ChatMessageRes( const HRESULT &InResult )
		// S2C: Chatting message event 
		HRESULT NetSvrPolicyGame::ChatMessageS2CEvt( const PlayerID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::ChatMessageS2CEvt::BuildIMsg(pMsg, InSenderID, InRole, InSenderName, InChatMessage));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::ChatMessageS2CEvt( const PlayerID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )
		// Cmd: Advance game
		HRESULT NetSvrPolicyGame::AdvanceGameRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::AdvanceGameRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::AdvanceGameRes( const HRESULT &InResult )
		// S2C: The game state is advanced
		HRESULT NetSvrPolicyGame::GameAdvancedS2CEvt( const GameInsUID &InGameInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameAdvancedS2CEvt::BuildIMsg(pMsg, InGameInsUID, InTimeStamp, InGameState, InDay));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GameAdvancedS2CEvt( const GameInsUID &InGameInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay )
		// S2C: Game is ended
		HRESULT NetSvrPolicyGame::GameEndedS2CEvt( const GameInsUID &InGameInsUID, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameEndedS2CEvt::BuildIMsg(pMsg, InGameInsUID, InWinner, InGainedExp, InGainedGameMoney));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GameEndedS2CEvt( const GameInsUID &InGameInsUID, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney )
		// Cmd: Vote game advance
		HRESULT NetSvrPolicyGame::VoteGameAdvanceRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::VoteGameAdvanceRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::VoteGameAdvanceRes( const HRESULT &InResult )
		// S2C: GameAdvance is Voted
		HRESULT NetSvrPolicyGame::GameAdvanceVotedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InVoter )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameAdvanceVotedS2CEvt::BuildIMsg(pMsg, InGameInsUID, InVoter));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GameAdvanceVotedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InVoter )
		// Cmd: Vote
		HRESULT NetSvrPolicyGame::VoteRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::VoteRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::VoteRes( const HRESULT &InResult )
		// S2C: Player Voted
		HRESULT NetSvrPolicyGame::VotedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InVoter, const PlayerID &InVotedTarget )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::VotedS2CEvt::BuildIMsg(pMsg, InGameInsUID, InVoter, InVotedTarget));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::VotedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InVoter, const PlayerID &InVotedTarget )
		// S2C: Vote is ended
		HRESULT NetSvrPolicyGame::VoteEndS2CEvt( const GameInsUID &InGameInsUID, const Array<PlayerID>& InVoted )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::VoteEndS2CEvt::BuildIMsg(pMsg, InGameInsUID, InVoted));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::VoteEndS2CEvt( const GameInsUID &InGameInsUID, const Array<PlayerID>& InVoted )
		// S2C: Player Killed
		HRESULT NetSvrPolicyGame::PlayerKilledS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PlayerKilledS2CEvt::BuildIMsg(pMsg, InGameInsUID, InKilledPlayer, InReason));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::PlayerKilledS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason )
		// S2C: Player Voted
		HRESULT NetSvrPolicyGame::PlayerRevealedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::PlayerRevealedS2CEvt::BuildIMsg(pMsg, InGameInsUID, InRevealedPlayerID, InRole, InReason));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::PlayerRevealedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason )
		// Cmd: Play again with the current players
		HRESULT NetSvrPolicyGame::GamePlayAgainRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GamePlayAgainRes::BuildIMsg(pMsg, InResult, InTotalGem, InTotalGameMoney));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GamePlayAgainRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
		HRESULT NetSvrPolicyGame::GamePlayAgainS2CEvt( const PartyUID &InPartyUID, const PlayerID &InLeadPlayer )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GamePlayAgainS2CEvt::BuildIMsg(pMsg, InPartyUID, InLeadPlayer));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GamePlayAgainS2CEvt( const PartyUID &InPartyUID, const PlayerID &InLeadPlayer )
		// Cmd: Player. reveal a player
		HRESULT NetSvrPolicyGame::GameRevealPlayerRes( const HRESULT &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameRevealPlayerRes::BuildIMsg(pMsg, InResult, InRevealedPlayerID, InRevealedRole, InTotalGem, InTotalGameMoney));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GameRevealPlayerRes( const HRESULT &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		// Cmd: Player. revive himself
		HRESULT NetSvrPolicyGame::GamePlayerReviveRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GamePlayerReviveRes::BuildIMsg(pMsg, InResult, InTotalGem, InTotalGameMoney));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GamePlayerReviveRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		// S2C: Player is revived
		HRESULT NetSvrPolicyGame::GamePlayerRevivedS2CEvt( const PlayerID &InRevivedPlayerID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GamePlayerRevivedS2CEvt::BuildIMsg(pMsg, InRevivedPlayerID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GamePlayerRevivedS2CEvt( const PlayerID &InRevivedPlayerID )
		// Cmd: Player. reset ranking
		HRESULT NetSvrPolicyGame::GamePlayerResetRankRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GamePlayerResetRankRes::BuildIMsg(pMsg, InResult, InTotalGem, InTotalGameMoney));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GamePlayerResetRankRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		// Cmd: Request Game match
		HRESULT NetSvrPolicyGame::RequestGameMatchRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::RequestGameMatchRes::BuildIMsg(pMsg, InResult, InTotalGem, InTotalGameMoney));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::RequestGameMatchRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		// S2C: Game matched
		HRESULT NetSvrPolicyGame::GameMatchedS2CEvt( const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData, const UINT32 &InStamina, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameMatchedS2CEvt::BuildIMsg(pMsg, InInsUID, InTimeStamp, InGameState, InDay, InMaxPlayer, InPlayerIndex, InPlayerCharacter, InRole, InDead, InChatHistoryData, InGameLogData, InStamina, InTotalGem, InTotalGameMoney));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GameMatchedS2CEvt( const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData, const UINT32 &InStamina, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
		// S2C: Game match failed
		HRESULT NetSvrPolicyGame::GameMatchFailedS2CEvt( const HRESULT &InFailedReason )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameMatchFailedS2CEvt::BuildIMsg(pMsg, InFailedReason));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GameMatchFailedS2CEvt( const HRESULT &InFailedReason )
		// S2C: Game matching started
		HRESULT NetSvrPolicyGame::GameMatchingStartedS2CEvt(  )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameMatchingStartedS2CEvt::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GameMatchingStartedS2CEvt(  )
		// Cmd: Cancel Game match
		HRESULT NetSvrPolicyGame::CancelGameMatchRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::CancelGameMatchRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::CancelGameMatchRes( const HRESULT &InResult )
		// S2C: game matching canceled
		HRESULT NetSvrPolicyGame::GameMatchingCanceledS2CEvt(  )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GameMatchingCanceledS2CEvt::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GameMatchingCanceledS2CEvt(  )
		// Cmd: Buy shop item prepare
		HRESULT NetSvrPolicyGame::BuyShopItemPrepareRes( const HRESULT &InResult, const UINT32 &InShopItemID, const char* InPurchaseID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::BuyShopItemPrepareRes::BuildIMsg(pMsg, InResult, InShopItemID, InPurchaseID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::BuyShopItemPrepareRes( const HRESULT &InResult, const UINT32 &InShopItemID, const char* InPurchaseID )
		// Cmd: Buy shop item
		HRESULT NetSvrPolicyGame::BuyShopItemRes( const HRESULT &InResult, const UINT32 &InShopItemID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::BuyShopItemRes::BuildIMsg(pMsg, InResult, InShopItemID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::BuyShopItemRes( const HRESULT &InResult, const UINT32 &InShopItemID )
		// Cmd: Give my stamina to other player
		HRESULT NetSvrPolicyGame::GiveStaminaRes( const HRESULT &InResult, const PlayerID &InTargetPlayer, const UINT64 &InTimeStamp )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GiveStaminaRes::BuildIMsg(pMsg, InResult, InTargetPlayer, InTimeStamp));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GiveStaminaRes( const HRESULT &InResult, const PlayerID &InTargetPlayer, const UINT64 &InTimeStamp )
		// Cmd: For debug, Change configue preset
		HRESULT NetSvrPolicyGame::SetPresetGameConfigIDRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::SetPresetGameConfigIDRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::SetPresetGameConfigIDRes( const HRESULT &InResult )
		// Cmd: For Debug
		HRESULT NetSvrPolicyGame::GainGameResourceRes( const HRESULT &InResult )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Game::GainGameResourceRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyGame::GainGameResourceRes( const HRESULT &InResult )


	}; // namespace Policy
}; // namespace BR


