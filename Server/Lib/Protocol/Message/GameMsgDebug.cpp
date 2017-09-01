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
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/Message/GameMsgDebug.h"
#include "Protocol/Message/GameMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// Game Debug trace mappping
			static std::unordered_map<UINT,std::function<Result(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapGame;

			void RegisterDebugTraceGame()
			{
 				// C2S: Client heart bit
				MessageDebugTraceMapGame.insert(std::make_pair(Game::HeartBitC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::HeartBitC2SEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Player connected from a login server and moved to game server
				MessageDebugTraceMapGame.insert(std::make_pair(Game::JoinGameServerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::JoinGameServerCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::JoinGameServerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::JoinGameServerRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: player complition statues
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetComplitionStateCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GetComplitionStateCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetComplitionStateRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GetComplitionStateRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Player complition state
				MessageDebugTraceMapGame.insert(std::make_pair(Game::SetComplitionStateCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::SetComplitionStateCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::SetComplitionStateRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::SetComplitionStateRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RegisterGCMCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::RegisterGCMCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RegisterGCMRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::RegisterGCMRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Unregister Google notification service ID
				MessageDebugTraceMapGame.insert(std::make_pair(Game::UnregisterGCMCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::UnregisterGCMCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::UnregisterGCMRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::UnregisterGCMRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Invite friend
				MessageDebugTraceMapGame.insert(std::make_pair(Game::InviteFriendCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::InviteFriendCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::InviteFriendRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::InviteFriendRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Accept friend request
				MessageDebugTraceMapGame.insert(std::make_pair(Game::AcceptFriendRequestCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::AcceptFriendRequestCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::AcceptFriendRequestRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::AcceptFriendRequestRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Notification for friend request is accepted
				MessageDebugTraceMapGame.insert(std::make_pair(Game::FriendRequestAcceptedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::FriendRequestAcceptedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Remove friden form the friend list
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RemoveFriendCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::RemoveFriendCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RemoveFriendRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::RemoveFriendRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Friend removed
				MessageDebugTraceMapGame.insert(std::make_pair(Game::FriendRemovedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::FriendRemovedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Get friend list
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetFriendListCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GetFriendListCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetFriendListRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GetFriendListRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Query notification list
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetNotificationListCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GetNotificationListCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetNotificationListRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GetNotificationListRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Delete notification
				MessageDebugTraceMapGame.insert(std::make_pair(Game::DeleteNotificationCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::DeleteNotificationCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::DeleteNotificationRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::DeleteNotificationRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Set notification is read
				MessageDebugTraceMapGame.insert(std::make_pair(Game::SetNotificationReadCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::SetNotificationReadCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::SetNotificationReadRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::SetNotificationReadRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Accept notification
				MessageDebugTraceMapGame.insert(std::make_pair(Game::AcceptNotificationCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::AcceptNotificationCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::AcceptNotificationRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::AcceptNotificationRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Notify new notification
				MessageDebugTraceMapGame.insert(std::make_pair(Game::NotifyS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::NotifyS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Query playerID list
				MessageDebugTraceMapGame.insert(std::make_pair(Game::FindPlayerByEMailCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::FindPlayerByEMailCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::FindPlayerByEMailRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::FindPlayerByEMailRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Query playerID list
				MessageDebugTraceMapGame.insert(std::make_pair(Game::FindPlayerByPlayerIDCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::FindPlayerByPlayerIDCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::FindPlayerByPlayerIDRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::FindPlayerByPlayerIDRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: *Request Player Status Update
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RequestPlayerStatusUpdateCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::RequestPlayerStatusUpdateCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RequestPlayerStatusUpdateRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::RequestPlayerStatusUpdateRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: *Notify Player Status Updated
				MessageDebugTraceMapGame.insert(std::make_pair(Game::NotifyPlayerStatusUpdatedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::NotifyPlayerStatusUpdatedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Get Ranking lise
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetRankingListCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GetRankingListCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetRankingListRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GetRankingListRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Game user game play information
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetUserGamePlayerInfoCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GetUserGamePlayerInfoCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetUserGamePlayerInfoRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GetUserGamePlayerInfoRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Game game play information
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetGamePlayerInfoCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GetGamePlayerInfoCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetGamePlayerInfoRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GetGamePlayerInfoRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player level up event
				MessageDebugTraceMapGame.insert(std::make_pair(Game::LevelUpS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::LevelUpS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Change NickName
				MessageDebugTraceMapGame.insert(std::make_pair(Game::SetNickNameCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::SetNickNameCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::SetNickNameRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::SetNickNameRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Create Party
				MessageDebugTraceMapGame.insert(std::make_pair(Game::CreatePartyCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::CreatePartyCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::CreatePartyRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::CreatePartyRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Join party
				MessageDebugTraceMapGame.insert(std::make_pair(Game::JoinPartyCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::JoinPartyCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::JoinPartyRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::JoinPartyRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player Joined event
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyPlayerJoinedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PartyPlayerJoinedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Party leader changed event
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyLeaderChangedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PartyLeaderChangedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Leave party command
				MessageDebugTraceMapGame.insert(std::make_pair(Game::LeavePartyCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::LeavePartyCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::LeavePartyRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::LeavePartyRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Party Player left event
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyPlayerLeftS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PartyPlayerLeftS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Kick player from the party
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyKickPlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PartyKickPlayerCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyKickPlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PartyKickPlayerRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Party Player kicked message
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyPlayerKickedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PartyPlayerKickedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Invite a player to the party
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyInviteCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PartyInviteCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyInviteRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PartyInviteRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Party invite requested
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyInviteRequestedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PartyInviteRequestedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Send Party quick chat message
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyQuickChatMessageCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PartyQuickChatMessageCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyQuickChatMessageRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PartyQuickChatMessageRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Party Chatting message event
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyQuickChatMessageS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PartyQuickChatMessageS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Party chatting
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyChatMessageCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PartyChatMessageCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyChatMessageRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PartyChatMessageRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Party Chatting message event
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyChatMessageS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PartyChatMessageS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Join to a game
				MessageDebugTraceMapGame.insert(std::make_pair(Game::JoinGameCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::JoinGameCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::JoinGameRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::JoinGameRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player Joined in the game
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PlayerJoinedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PlayerJoinedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Leave Game
				MessageDebugTraceMapGame.insert(std::make_pair(Game::LeaveGameCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::LeaveGameCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::LeaveGameRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::LeaveGameRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player left event
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PlayerLeftS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PlayerLeftS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Kick player
				MessageDebugTraceMapGame.insert(std::make_pair(Game::KickPlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::KickPlayerCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::KickPlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::KickPlayerRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player kicked
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PlayerKickedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PlayerKickedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Assign role + Game state reset
				MessageDebugTraceMapGame.insert(std::make_pair(Game::AssignRoleCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::AssignRoleCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::AssignRoleRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::AssignRoleRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Role assigned event
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RoleAssignedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::RoleAssignedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Send chatting message to the game
				MessageDebugTraceMapGame.insert(std::make_pair(Game::ChatMessageCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::ChatMessageCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::ChatMessageRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::ChatMessageRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Chatting message event 
				MessageDebugTraceMapGame.insert(std::make_pair(Game::ChatMessageS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::ChatMessageS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Advance game
				MessageDebugTraceMapGame.insert(std::make_pair(Game::AdvanceGameCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::AdvanceGameCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::AdvanceGameRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::AdvanceGameRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: The game state is advanced
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameAdvancedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GameAdvancedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Game is ended
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameEndedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GameEndedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Vote game advance
				MessageDebugTraceMapGame.insert(std::make_pair(Game::VoteGameAdvanceCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::VoteGameAdvanceCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::VoteGameAdvanceRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::VoteGameAdvanceRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: GameAdvance is Voted
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameAdvanceVotedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GameAdvanceVotedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Vote
				MessageDebugTraceMapGame.insert(std::make_pair(Game::VoteCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::VoteCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::VoteRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::VoteRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player Voted
				MessageDebugTraceMapGame.insert(std::make_pair(Game::VotedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::VotedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Vote is ended
				MessageDebugTraceMapGame.insert(std::make_pair(Game::VoteEndS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::VoteEndS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player Killed
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PlayerKilledS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PlayerKilledS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player Voted
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PlayerRevealedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::PlayerRevealedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Play again with the current players
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GamePlayAgainCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GamePlayAgainCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GamePlayAgainRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GamePlayAgainRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GamePlayAgainS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GamePlayAgainS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Player. reveal a player
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameRevealPlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GameRevealPlayerCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameRevealPlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GameRevealPlayerRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Player. revive himself
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GamePlayerReviveCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GamePlayerReviveCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GamePlayerReviveRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GamePlayerReviveRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player is revived
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GamePlayerRevivedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GamePlayerRevivedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Player. reset ranking
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GamePlayerResetRankCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GamePlayerResetRankCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GamePlayerResetRankRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GamePlayerResetRankRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Request Game match
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RequestGameMatchCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::RequestGameMatchCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RequestGameMatchRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::RequestGameMatchRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Game matched
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameMatchedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GameMatchedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Game match failed
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameMatchFailedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GameMatchFailedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Game matching started
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameMatchingStartedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GameMatchingStartedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Cancel Game match
				MessageDebugTraceMapGame.insert(std::make_pair(Game::CancelGameMatchCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::CancelGameMatchCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::CancelGameMatchRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::CancelGameMatchRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: game matching canceled
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameMatchingCanceledS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GameMatchingCanceledS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Buy shop item prepare
				MessageDebugTraceMapGame.insert(std::make_pair(Game::BuyShopItemPrepareCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::BuyShopItemPrepareCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::BuyShopItemPrepareRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::BuyShopItemPrepareRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Buy shop item
				MessageDebugTraceMapGame.insert(std::make_pair(Game::BuyShopItemCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::BuyShopItemCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::BuyShopItemRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::BuyShopItemRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Give my stamina to other player
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GiveStaminaCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GiveStaminaCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GiveStaminaRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GiveStaminaRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: For debug, Change configue preset
				MessageDebugTraceMapGame.insert(std::make_pair(Game::SetPresetGameConfigIDCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::SetPresetGameConfigIDCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::SetPresetGameConfigIDRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::SetPresetGameConfigIDRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: For Debug
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GainGameResourceCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GainGameResourceCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GainGameResourceRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Game::GainGameResourceRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
			}; // void RegisterDebugTraceGame()


			///////////////////////////////////////////////////////////////
			// Game Debug trace
			Result DebugOutGame( const char *Prefix, MessageData *pMsg )
			{
 
				Result hr;
				auto itFount = MessageDebugTraceMapGame.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// Game Debug trace
				itFount = MessageDebugTraceMapGame.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapGame.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // Result DebugOutGame( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


