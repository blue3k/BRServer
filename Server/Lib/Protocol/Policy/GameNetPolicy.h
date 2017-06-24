﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Game Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"
#include "Protocol/Policy/GameIPolicy.h"



namespace BR
{
 	namespace Policy
	{
 		class NetPolicyGame : public IPolicyGame
		{
 			public:
			// Constructor
			NetPolicyGame ( Net::IConnection* pConn ) : IPolicyGame(pConn)
			{}

			// C2S: Client heart bit
			Result HeartBitC2SEvt(  );
			// Cmd: Player connected from a login server and moved to game server
			Result JoinGameServerCmd( const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID );
			// Cmd: player complition statues
			Result GetComplitionStateCmd(  );
			// Cmd: Player complition state
			Result SetComplitionStateCmd( const char* InComplitionState );
			// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
			Result RegisterGCMCmd( const char* InGCMRegisteredID );
			// Cmd: Unregister Google notification service ID
			Result UnregisterGCMCmd( const char* InGCMRegisteredID );
			// Cmd: Invite friend
			Result InviteFriendCmd( const PlayerID &InFriendID );
			// Cmd: Accept friend request
			Result AcceptFriendRequestCmd( const PlayerID &InInviterID, const FacebookUID &InInviterFacebookUID );
			// Cmd: Remove friden form the friend list
			Result RemoveFriendCmd( const PlayerID &InFriendID );
			// Cmd: Get friend list
			Result GetFriendListCmd( const uint16_t &InStartIndex, const uint16_t &InCount );
			// Cmd: Query notification list
			Result GetNotificationListCmd(  );
			// Cmd: Delete notification
			Result DeleteNotificationCmd( const uint32_t &InNotificationID );
			// Cmd: Set notification is read
			Result SetNotificationReadCmd( const uint32_t &InNotificationID );
			// Cmd: Accept notification
			Result AcceptNotificationCmd( const uint32_t &InNotificationID );
			// Cmd: Query playerID list
			Result FindPlayerByEMailCmd( const char* InPlayerEMail );
			// Cmd: Query playerID list
			Result FindPlayerByPlayerIDCmd( const PlayerID &InPlayerID );
			// Cmd: *Request Player Status Update
			Result RequestPlayerStatusUpdateCmd( const Array<PlayerID>& InTargetPlayerID );
			// Cmd: Get Ranking lise
			Result GetRankingListCmd( const RankingType &InRankingType, const uint8_t &InBaseRanking, const uint8_t &InCount );
			// Cmd: Game user game play information
			Result GetUserGamePlayerInfoCmd(  );
			// Cmd: Game game play information
			Result GetGamePlayerInfoCmd( const PlayerID &InPlayerID );
			// Cmd: Change NickName
			Result SetNickNameCmd( const char* InNickName, const uint8_t &InIsCostFree );
			// Cmd: Create Party
			Result CreatePartyCmd(  );
			// Cmd: Join party
			Result JoinPartyCmd( const PartyUID &InPartyUID, const PlayerID &InInviterID );
			// Cmd: Leave party command
			Result LeavePartyCmd( const PartyUID &InPartyUID, const PlayerID &InPlayerID );
			// Cmd: Kick player from the party
			Result PartyKickPlayerCmd( const PartyUID &InPartyUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
			// Cmd: Invite a player to the party
			Result PartyInviteCmd( const PlayerID &InInviteTargetID );
			// Cmd: Send Party quick chat message
			Result PartyQuickChatMessageCmd( const uint32_t &InQuickChatID );
			// Cmd: Party chatting
			Result PartyChatMessageCmd( const char* InChatMessage );
			// Cmd: Join to a game
			Result JoinGameCmd( const PlayerID &InPlayerID, const AuthTicket &InTicket, const GameInsUID &InInsUID );
			// Cmd: Leave Game
			Result LeaveGameCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket );
			// Cmd: Kick player
			Result KickPlayerCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
			// Cmd: Assign role + Game state reset
			Result AssignRoleCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket );
			// Cmd: Send chatting message to the game
			Result ChatMessageCmd( const char* InChatMessage, const PlayerRole &InRole );
			// Cmd: Advance game
			Result AdvanceGameCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket );
			// Cmd: Vote game advance
			Result VoteGameAdvanceCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket );
			// Cmd: Vote
			Result VoteCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const PlayerID &InVoteTarget, const uint32_t &InActionSerial );
			// Cmd: Play again with the current players
			Result GamePlayAgainCmd(  );
			// Cmd: Player. reveal a player
			Result GameRevealPlayerCmd( const Array<PlayerID>& InTargetPlayerID );
			// Cmd: Player. revive himself
			Result GamePlayerReviveCmd(  );
			// Cmd: Player. reset ranking
			Result GamePlayerResetRankCmd(  );
			// Cmd: Request Game match
			Result RequestGameMatchCmd( const uint8_t &InNumPlayer, const PlayerRole &InRequestRole );
			// Cmd: Cancel Game match
			Result CancelGameMatchCmd(  );
			// Cmd: Buy shop item prepare
			Result BuyShopItemPrepareCmd( const uint32_t &InShopItemID );
			// Cmd: Buy shop item
			Result BuyShopItemCmd( const uint32_t &InShopItemID, const char* InPlatform, const char* InPackageName, const char* InPurchaseTransactionID, const Array<uint8_t>& InPurchaseToken );
			// Cmd: Give my stamina to other player
			Result GiveStaminaCmd( const PlayerID &InTargetPlayer );
			// Cmd: For debug, Change configue preset
			Result SetPresetGameConfigIDCmd( const uint32_t &InPresetID );
			// Cmd: For Debug
			Result GainGameResourceCmd( const int32_t &InResource, const int32_t &InValue );

		}; // class NetPolicyGame : public IPolicyGame


		class NetSvrPolicyGame : public ISvrPolicyGame
		{
 			public:
			// Constructor
			NetSvrPolicyGame ( Net::IConnection* pConn ) : ISvrPolicyGame(pConn)
			{}

			// Cmd: Player connected from a login server and moved to game server
			Result JoinGameServerRes( const Result &InResult, const char* InNickName, const GameInsUID &InGameUID, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const MatchingQueueTicket &InMatchingTicket );
			// Cmd: player complition statues
			Result GetComplitionStateRes( const Result &InResult, const char* InComplitionState );
			// Cmd: Player complition state
			Result SetComplitionStateRes( const Result &InResult );
			// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
			Result RegisterGCMRes( const Result &InResult );
			// Cmd: Unregister Google notification service ID
			Result UnregisterGCMRes( const Result &InResult );
			// Cmd: Invite friend
			Result InviteFriendRes( const Result &InResult );
			// Cmd: Accept friend request
			Result AcceptFriendRequestRes( const Result &InResult, const FriendInformation &InNewFriend );
			// S2C: Notification for friend request is accepted
			Result FriendRequestAcceptedS2CEvt( const FriendInformation &InAccepter );
			// Cmd: Remove friden form the friend list
			Result RemoveFriendRes( const Result &InResult, const PlayerID &InFriendID );
			// S2C: Friend removed
			Result FriendRemovedS2CEvt( const PlayerID &InFriendID );
			// Cmd: Get friend list
			Result GetFriendListRes( const Result &InResult, const uint16_t &InMaxFriendSlot, const uint16_t &InTotalNumberOfFriends, const uint16_t &InStartIndex, const Array<FriendInformation>& InFriendList );
			// Cmd: Query notification list
			Result GetNotificationListRes( const Result &InResult );
			// Cmd: Delete notification
			Result DeleteNotificationRes( const Result &InResult, const uint32_t &InNotificationID );
			// Cmd: Set notification is read
			Result SetNotificationReadRes( const Result &InResult, const uint32_t &InNotificationID );
			// Cmd: Accept notification
			Result AcceptNotificationRes( const Result &InResult, const uint32_t &InNotificationID );
			// S2C: Notify new notification
			Result NotifyS2CEvt( const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint8_t &InIsRead, const uint64_t &InTimeStamp );
			// Cmd: Query playerID list
			Result FindPlayerByEMailRes( const Result &InResult, const PlayerInformation &InPlayer );
			// Cmd: Query playerID list
			Result FindPlayerByPlayerIDRes( const Result &InResult, const PlayerInformation &InPlayer );
			// Cmd: *Request Player Status Update
			Result RequestPlayerStatusUpdateRes( const Result &InResult );
			// S2C: *Notify Player Status Updated
			Result NotifyPlayerStatusUpdatedS2CEvt( const PlayerID &InPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame );
			// Cmd: Get Ranking lise
			Result GetRankingListRes( const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking );
			// Cmd: Game user game play information
			Result GetUserGamePlayerInfoRes( const Result &InResult, const int16_t &InLevel, const int64_t &InExp, const int64_t &InGameMoney, const int64_t &InGem, const int16_t &InStamina, const uint32_t &InLastUpdateTime, const int32_t &InTotalPlayed, const int32_t &InWinPlaySC, const int32_t &InWinPlaySM, const int32_t &InWinPlaySS, const int32_t &InLosePlaySC, const int32_t &InLosePlaySM, const int32_t &InLosePlaySS, const int32_t &InWinPlayNC, const int32_t &InWinPlayNM, const int32_t &InWinPlayNS, const int32_t &InLosePlayNC, const int32_t &InLosePlayNM, const int32_t &InLosePlayNS, const int32_t &InWeeklyWin, const int32_t &InWeeklyLose );
			// Cmd: Game game play information
			Result GetGamePlayerInfoRes( const Result &InResult, const PlayerID &InPlayerID, const int16_t &InLevel, const int32_t &InTotalPlayed, const int32_t &InWinPlaySC, const int32_t &InWinPlaySM, const int32_t &InWinPlaySS, const int32_t &InLosePlaySC, const int32_t &InLosePlaySM, const int32_t &InLosePlaySS, const int32_t &InWinPlayNC, const int32_t &InWinPlayNM, const int32_t &InWinPlayNS, const int32_t &InLosePlayNC, const int32_t &InLosePlayNM, const int32_t &InLosePlayNS, const int32_t &InWeeklyWin, const int32_t &InWeeklyLose );
			// S2C: Player level up event
			Result LevelUpS2CEvt( const uint64_t &InCurrentTotalExp, const uint32_t &InCurrentLevel );
			// Cmd: Change NickName
			Result SetNickNameRes( const Result &InResult, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney );
			// Cmd: Create Party
			Result CreatePartyRes( const Result &InResult, const PartyUID &InPartyUID );
			// Cmd: Join party
			Result JoinPartyRes( const Result &InResult, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const Array<uint8_t>& InChatHistoryData );
			// S2C: Player Joined event
			Result PartyPlayerJoinedS2CEvt( const PartyUID &InPartyUID, const PlayerInformation &InJoinedPlayer );
			// S2C: Party leader changed event
			Result PartyLeaderChangedS2CEvt( const PartyUID &InPartyUID, const PlayerID &InNewLeaderID );
			// Cmd: Leave party command
			Result LeavePartyRes( const Result &InResult );
			// S2C: Party Player left event
			Result PartyPlayerLeftS2CEvt( const PartyUID &InPartyUID, const PlayerID &InLeftPlayerID );
			// Cmd: Kick player from the party
			Result PartyKickPlayerRes( const Result &InResult );
			// S2C: Party Player kicked message
			Result PartyPlayerKickedS2CEvt( const PartyUID &InPartyUID, const PlayerID &InKickedPlayerID );
			// Cmd: Invite a player to the party
			Result PartyInviteRes( const Result &InResult );
			// S2C: Party invite requested
			Result PartyInviteRequestedS2CEvt( const PlayerID &InInviterID, const char* InInviterName, const PartyUID &InPartyToJoinUID );
			// Cmd: Send Party quick chat message
			Result PartyQuickChatMessageRes( const Result &InResult );
			// S2C: Party Chatting message event
			Result PartyQuickChatMessageS2CEvt( const PlayerID &InSenderID, const uint32_t &InQuickChatID );
			// Cmd: Party chatting
			Result PartyChatMessageRes( const Result &InResult );
			// S2C: Party Chatting message event
			Result PartyChatMessageS2CEvt( const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage );
			// Cmd: Join to a game
			Result JoinGameRes( const Result &InResult, const GameInsUID &InInsUID, const uint32_t &InTimeStamp, const GameStateID &InGameState, const uint8_t &InDay, const uint8_t &InMaxPlayer, const uint8_t &InPlayerIndex, const uint8_t &InPlayerCharacter, const PlayerRole &InRole, const uint8_t &InDead, const Array<uint8_t>& InChatHistoryData, const Array<uint8_t>& InGameLogData );
			// S2C: Player Joined in the game
			Result PlayerJoinedS2CEvt( const GameInsUID &InGameInsUID, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const uint8_t &InJoinedPlayerDead, const uint8_t &InJoinedPlayerIndex, const uint8_t &InJoinedPlayerCharacter );
			// Cmd: Leave Game
			Result LeaveGameRes( const Result &InResult );
			// S2C: Player left event
			Result PlayerLeftS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InLeftPlayerID );
			// Cmd: Kick player
			Result KickPlayerRes( const Result &InResult );
			// S2C: Player kicked
			Result PlayerKickedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InKickedPlayerID );
			// Cmd: Assign role + Game state reset
			Result AssignRoleRes( const Result &InResult );
			// S2C: Role assigned event
			Result RoleAssignedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerRole &InRole );
			// Cmd: Send chatting message to the game
			Result ChatMessageRes( const Result &InResult );
			// S2C: Chatting message event 
			Result ChatMessageS2CEvt( const PlayerID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage );
			// Cmd: Advance game
			Result AdvanceGameRes( const Result &InResult );
			// S2C: The game state is advanced
			Result GameAdvancedS2CEvt( const GameInsUID &InGameInsUID, const uint32_t &InTimeStamp, const GameStateID &InGameState, const uint8_t &InDay );
			// S2C: Game is ended
			Result GameEndedS2CEvt( const GameInsUID &InGameInsUID, const GameWinner &InWinner, const uint32_t &InGainedExp, const uint32_t &InGainedGameMoney );
			// Cmd: Vote game advance
			Result VoteGameAdvanceRes( const Result &InResult );
			// S2C: GameAdvance is Voted
			Result GameAdvanceVotedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InVoter );
			// Cmd: Vote
			Result VoteRes( const Result &InResult );
			// S2C: Player Voted
			Result VotedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InVoter, const PlayerID &InVotedTarget );
			// S2C: Vote is ended
			Result VoteEndS2CEvt( const GameInsUID &InGameInsUID, const Array<PlayerID>& InVoted );
			// S2C: Player Killed
			Result PlayerKilledS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason );
			// S2C: Player Voted
			Result PlayerRevealedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason );
			// Cmd: Play again with the current players
			Result GamePlayAgainRes( const Result &InResult, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney );
			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			Result GamePlayAgainS2CEvt( const PartyUID &InPartyUID, const PlayerID &InLeadPlayer );
			// Cmd: Player. reveal a player
			Result GameRevealPlayerRes( const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney );
			// Cmd: Player. revive himself
			Result GamePlayerReviveRes( const Result &InResult, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney );
			// S2C: Player is revived
			Result GamePlayerRevivedS2CEvt( const PlayerID &InRevivedPlayerID );
			// Cmd: Player. reset ranking
			Result GamePlayerResetRankRes( const Result &InResult, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney );
			// Cmd: Request Game match
			Result RequestGameMatchRes( const Result &InResult, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney );
			// S2C: Game matched
			Result GameMatchedS2CEvt( const GameInsUID &InInsUID, const uint32_t &InTimeStamp, const GameStateID &InGameState, const uint8_t &InDay, const uint8_t &InMaxPlayer, const uint8_t &InPlayerIndex, const uint8_t &InPlayerCharacter, const PlayerRole &InRole, const uint8_t &InDead, const Array<uint8_t>& InChatHistoryData, const Array<uint8_t>& InGameLogData, const uint32_t &InStamina, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney );
			// S2C: Game match failed
			Result GameMatchFailedS2CEvt( const Result &InFailedReason );
			// S2C: Game matching started
			Result GameMatchingStartedS2CEvt(  );
			// Cmd: Cancel Game match
			Result CancelGameMatchRes( const Result &InResult );
			// S2C: game matching canceled
			Result GameMatchingCanceledS2CEvt(  );
			// Cmd: Buy shop item prepare
			Result BuyShopItemPrepareRes( const Result &InResult, const uint32_t &InShopItemID, const char* InPurchaseID );
			// Cmd: Buy shop item
			Result BuyShopItemRes( const Result &InResult, const uint32_t &InShopItemID );
			// Cmd: Give my stamina to other player
			Result GiveStaminaRes( const Result &InResult, const PlayerID &InTargetPlayer, const uint64_t &InTimeStamp );
			// Cmd: For debug, Change configue preset
			Result SetPresetGameConfigIDRes( const Result &InResult );
			// Cmd: For Debug
			Result GainGameResourceRes( const Result &InResult );

		}; // class NetSvrPolicyGame : public ISvrPolicyGame


	}; // namespace Policy
}; // namespace BR


