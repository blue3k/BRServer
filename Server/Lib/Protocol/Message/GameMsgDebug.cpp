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
			static std::unordered_map<UINT,std::function<HRESULT(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapGame;

			void RegisterDebugTraceGame()
			{
 				// C2S: Client heart bit
				MessageDebugTraceMapGame.insert(std::make_pair(Game::HeartBitC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::HeartBitC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Player connected from a login server and moved to game server
				MessageDebugTraceMapGame.insert(std::make_pair(Game::JoinGameServerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::JoinGameServerCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::JoinGameServerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::JoinGameServerRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: player complition statues
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetComplitionStateCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GetComplitionStateCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetComplitionStateRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GetComplitionStateRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Player complition state
				MessageDebugTraceMapGame.insert(std::make_pair(Game::SetComplitionStateCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::SetComplitionStateCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::SetComplitionStateRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::SetComplitionStateRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RegisterGCMCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::RegisterGCMCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RegisterGCMRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::RegisterGCMRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Unregister Google notification service ID
				MessageDebugTraceMapGame.insert(std::make_pair(Game::UnregisterGCMCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::UnregisterGCMCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::UnregisterGCMRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::UnregisterGCMRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Invite friend
				MessageDebugTraceMapGame.insert(std::make_pair(Game::InviteFriendCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::InviteFriendCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::InviteFriendRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::InviteFriendRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Accept friend request
				MessageDebugTraceMapGame.insert(std::make_pair(Game::AcceptFriendRequestCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::AcceptFriendRequestCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::AcceptFriendRequestRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::AcceptFriendRequestRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Notification for friend request is accepted
				MessageDebugTraceMapGame.insert(std::make_pair(Game::FriendRequestAcceptedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::FriendRequestAcceptedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Remove friden form the friend list
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RemoveFriendCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::RemoveFriendCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RemoveFriendRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::RemoveFriendRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Friend removed
				MessageDebugTraceMapGame.insert(std::make_pair(Game::FriendRemovedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::FriendRemovedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Get friend list
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetFriendListCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GetFriendListCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetFriendListRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GetFriendListRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Query notification list
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetNotificationListCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GetNotificationListCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetNotificationListRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GetNotificationListRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Delete notification
				MessageDebugTraceMapGame.insert(std::make_pair(Game::DeleteNotificationCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::DeleteNotificationCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::DeleteNotificationRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::DeleteNotificationRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Set notification is read
				MessageDebugTraceMapGame.insert(std::make_pair(Game::SetNotificationReadCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::SetNotificationReadCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::SetNotificationReadRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::SetNotificationReadRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Accept notification
				MessageDebugTraceMapGame.insert(std::make_pair(Game::AcceptNotificationCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::AcceptNotificationCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::AcceptNotificationRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::AcceptNotificationRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Notify new notification
				MessageDebugTraceMapGame.insert(std::make_pair(Game::NotifyS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::NotifyS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Query playerID list
				MessageDebugTraceMapGame.insert(std::make_pair(Game::FindPlayerByEMailCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::FindPlayerByEMailCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::FindPlayerByEMailRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::FindPlayerByEMailRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Query playerID list
				MessageDebugTraceMapGame.insert(std::make_pair(Game::FindPlayerByPlayerIDCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::FindPlayerByPlayerIDCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::FindPlayerByPlayerIDRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::FindPlayerByPlayerIDRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: *Request Player Status Update
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RequestPlayerStatusUpdateCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::RequestPlayerStatusUpdateCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RequestPlayerStatusUpdateRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::RequestPlayerStatusUpdateRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: *Notify Player Status Updated
				MessageDebugTraceMapGame.insert(std::make_pair(Game::NotifyPlayerStatusUpdatedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::NotifyPlayerStatusUpdatedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Get Ranking lise
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetRankingListCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GetRankingListCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetRankingListRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GetRankingListRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Game user game play information
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetUserGamePlayerInfoCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GetUserGamePlayerInfoCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetUserGamePlayerInfoRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GetUserGamePlayerInfoRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Game game play information
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetGamePlayerInfoCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GetGamePlayerInfoCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GetGamePlayerInfoRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GetGamePlayerInfoRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Player level up event
				MessageDebugTraceMapGame.insert(std::make_pair(Game::LevelUpS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::LevelUpS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Change NickName
				MessageDebugTraceMapGame.insert(std::make_pair(Game::SetNickNameCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::SetNickNameCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::SetNickNameRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::SetNickNameRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Create Party
				MessageDebugTraceMapGame.insert(std::make_pair(Game::CreatePartyCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::CreatePartyCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::CreatePartyRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::CreatePartyRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Join party
				MessageDebugTraceMapGame.insert(std::make_pair(Game::JoinPartyCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::JoinPartyCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::JoinPartyRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::JoinPartyRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Player Joined event
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyPlayerJoinedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PartyPlayerJoinedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Party leader changed event
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyLeaderChangedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PartyLeaderChangedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Leave party command
				MessageDebugTraceMapGame.insert(std::make_pair(Game::LeavePartyCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::LeavePartyCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::LeavePartyRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::LeavePartyRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Party Player left event
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyPlayerLeftS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PartyPlayerLeftS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Kick player from the party
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyKickPlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PartyKickPlayerCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyKickPlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PartyKickPlayerRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Party Player kicked message
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyPlayerKickedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PartyPlayerKickedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Invite a player to the party
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyInviteCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PartyInviteCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyInviteRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PartyInviteRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Party invite requested
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyInviteRequestedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PartyInviteRequestedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Send Party quick chat message
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyQuickChatMessageCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PartyQuickChatMessageCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyQuickChatMessageRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PartyQuickChatMessageRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Party Chatting message event
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyQuickChatMessageS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PartyQuickChatMessageS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Party chatting
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyChatMessageCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PartyChatMessageCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyChatMessageRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PartyChatMessageRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Party Chatting message event
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PartyChatMessageS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PartyChatMessageS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Join to a game
				MessageDebugTraceMapGame.insert(std::make_pair(Game::JoinGameCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::JoinGameCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::JoinGameRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::JoinGameRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Player Joined in the game
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PlayerJoinedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PlayerJoinedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Leave Game
				MessageDebugTraceMapGame.insert(std::make_pair(Game::LeaveGameCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::LeaveGameCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::LeaveGameRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::LeaveGameRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Player left event
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PlayerLeftS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PlayerLeftS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Kick player
				MessageDebugTraceMapGame.insert(std::make_pair(Game::KickPlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::KickPlayerCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::KickPlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::KickPlayerRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Player kicked
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PlayerKickedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PlayerKickedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Assign role + Game state reset
				MessageDebugTraceMapGame.insert(std::make_pair(Game::AssignRoleCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::AssignRoleCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::AssignRoleRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::AssignRoleRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Role assigned event
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RoleAssignedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::RoleAssignedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Send chatting message to the game
				MessageDebugTraceMapGame.insert(std::make_pair(Game::ChatMessageCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::ChatMessageCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::ChatMessageRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::ChatMessageRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Chatting message event 
				MessageDebugTraceMapGame.insert(std::make_pair(Game::ChatMessageS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::ChatMessageS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Advance game
				MessageDebugTraceMapGame.insert(std::make_pair(Game::AdvanceGameCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::AdvanceGameCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::AdvanceGameRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::AdvanceGameRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: The game state is advanced
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameAdvancedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GameAdvancedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Game is ended
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameEndedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GameEndedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Vote game advance
				MessageDebugTraceMapGame.insert(std::make_pair(Game::VoteGameAdvanceCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::VoteGameAdvanceCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::VoteGameAdvanceRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::VoteGameAdvanceRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: GameAdvance is Voted
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameAdvanceVotedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GameAdvanceVotedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Vote
				MessageDebugTraceMapGame.insert(std::make_pair(Game::VoteCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::VoteCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::VoteRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::VoteRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Player Voted
				MessageDebugTraceMapGame.insert(std::make_pair(Game::VotedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::VotedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Vote is ended
				MessageDebugTraceMapGame.insert(std::make_pair(Game::VoteEndS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::VoteEndS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Player Killed
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PlayerKilledS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PlayerKilledS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Player Voted
				MessageDebugTraceMapGame.insert(std::make_pair(Game::PlayerRevealedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::PlayerRevealedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Play again with the current players
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GamePlayAgainCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GamePlayAgainCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GamePlayAgainRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GamePlayAgainRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GamePlayAgainS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GamePlayAgainS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Player. reveal a player
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameRevealPlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GameRevealPlayerCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameRevealPlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GameRevealPlayerRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Player. revive himself
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GamePlayerReviveCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GamePlayerReviveCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GamePlayerReviveRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GamePlayerReviveRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Player is revived
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GamePlayerRevivedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GamePlayerRevivedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Player. reset ranking
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GamePlayerResetRankCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GamePlayerResetRankCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GamePlayerResetRankRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GamePlayerResetRankRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Request Game match
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RequestGameMatchCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::RequestGameMatchCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::RequestGameMatchRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::RequestGameMatchRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Game matched
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameMatchedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GameMatchedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Game match failed
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameMatchFailedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GameMatchFailedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: Game matching started
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameMatchingStartedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GameMatchingStartedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Cancel Game match
				MessageDebugTraceMapGame.insert(std::make_pair(Game::CancelGameMatchCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::CancelGameMatchCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::CancelGameMatchRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::CancelGameMatchRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// S2C: game matching canceled
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GameMatchingCanceledS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GameMatchingCanceledS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Buy shop item prepare
				MessageDebugTraceMapGame.insert(std::make_pair(Game::BuyShopItemPrepareCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::BuyShopItemPrepareCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::BuyShopItemPrepareRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::BuyShopItemPrepareRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Buy shop item
				MessageDebugTraceMapGame.insert(std::make_pair(Game::BuyShopItemCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::BuyShopItemCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::BuyShopItemRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::BuyShopItemRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Give my stamina to other player
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GiveStaminaCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GiveStaminaCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GiveStaminaRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GiveStaminaRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: For debug, Change configue preset
				MessageDebugTraceMapGame.insert(std::make_pair(Game::SetPresetGameConfigIDCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::SetPresetGameConfigIDCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::SetPresetGameConfigIDRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::SetPresetGameConfigIDRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: For Debug
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GainGameResourceCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GainGameResourceCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGame.insert(std::make_pair(Game::GainGameResourceRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Game::GainGameResourceRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
			}; // void RegisterDebugTraceGame()


			///////////////////////////////////////////////////////////////
			// Game Debug trace
			HRESULT DebugOutGame( const char *Prefix, MessageData *pMsg )
			{
 
				HRESULT hr = S_OK;
				auto itFount = MessageDebugTraceMapGame.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// Game Debug trace
				itFount = MessageDebugTraceMapGame.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapGame.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // HRESULT DebugOutGame( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


