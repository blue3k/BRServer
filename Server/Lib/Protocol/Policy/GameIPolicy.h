////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : Game Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BrBaseTypes.h"
#include "Common/BrGameTypes.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"



namespace BR
{
 	namespace Policy
	{
 
		class ISvrPolicyGame : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_SVR_GAME };

			// Constructor
			ISvrPolicyGame( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// C2S: Client heart bit
			// Cmd: Player connected from a login server and moved to game server
			virtual Result JoinGameServerRes( const Result &InResult, const char* InNickName, const GameInsUID &InGameUID, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// Cmd: player complition statues
			virtual Result GetComplitionStateRes( const Result &InResult, const char* InComplitionState ) = 0;
			// Cmd: Player complition state
			virtual Result SetComplitionStateRes( const Result &InResult ) = 0;
			// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
			virtual Result RegisterGCMRes( const Result &InResult ) = 0;
			// Cmd: Unregister Google notification service ID
			virtual Result UnregisterGCMRes( const Result &InResult ) = 0;
			// Cmd: Invite friend
			virtual Result InviteFriendRes( const Result &InResult ) = 0;
			// Cmd: Accept friend request
			virtual Result AcceptFriendRequestRes( const Result &InResult, const FriendInformation &InNewFriend ) = 0;
			// S2C: Notification for friend request is accepted
			virtual Result FriendRequestAcceptedS2CEvt( const FriendInformation &InAccepter ) = 0;
			// Cmd: Remove friden form the friend list
			virtual Result RemoveFriendRes( const Result &InResult, const PlayerID &InFriendID ) = 0;
			// S2C: Friend removed
			virtual Result FriendRemovedS2CEvt( const PlayerID &InFriendID ) = 0;
			// Cmd: Get friend list
			virtual Result GetFriendListRes( const Result &InResult, const UINT16 &InMaxFriendSlot, const UINT16 &InTotalNumberOfFriends, const UINT16 &InStartIndex, const Array<FriendInformation>& InFriendList ) = 0;
			// Cmd: Query notification list
			virtual Result GetNotificationListRes( const Result &InResult ) = 0;
			// Cmd: Delete notification
			virtual Result DeleteNotificationRes( const Result &InResult, const UINT32 &InNotificationID ) = 0;
			// Cmd: Set notification is read
			virtual Result SetNotificationReadRes( const Result &InResult, const UINT32 &InNotificationID ) = 0;
			// Cmd: Accept notification
			virtual Result AcceptNotificationRes( const Result &InResult, const UINT32 &InNotificationID ) = 0;
			// S2C: Notify new notification
			virtual Result NotifyS2CEvt( const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const BYTE &InIsRead, const UINT64 &InTimeStamp ) = 0;
			// Cmd: Query playerID list
			virtual Result FindPlayerByEMailRes( const Result &InResult, const PlayerInformation &InPlayer ) = 0;
			// Cmd: Query playerID list
			virtual Result FindPlayerByPlayerIDRes( const Result &InResult, const PlayerInformation &InPlayer ) = 0;
			// Cmd: *Request Player Status Update
			virtual Result RequestPlayerStatusUpdateRes( const Result &InResult ) = 0;
			// S2C: *Notify Player Status Updated
			virtual Result NotifyPlayerStatusUpdatedS2CEvt( const PlayerID &InPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame ) = 0;
			// Cmd: Get Ranking lise
			virtual Result GetRankingListRes( const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking ) = 0;
			// Cmd: Game user game play information
			virtual Result GetUserGamePlayerInfoRes( const Result &InResult, const INT16 &InLevel, const INT64 &InExp, const INT64 &InGameMoney, const INT64 &InGem, const INT16 &InStamina, const UINT32 &InLastUpdateTime, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose ) = 0;
			// Cmd: Game game play information
			virtual Result GetGamePlayerInfoRes( const Result &InResult, const PlayerID &InPlayerID, const INT16 &InLevel, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose ) = 0;
			// S2C: Player level up event
			virtual Result LevelUpS2CEvt( const UINT64 &InCurrentTotalExp, const UINT32 &InCurrentLevel ) = 0;
			// Cmd: Change NickName
			virtual Result SetNickNameRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney ) = 0;
			// Cmd: Create Party
			virtual Result CreatePartyRes( const Result &InResult, const PartyUID &InPartyUID ) = 0;
			// Cmd: Join party
			virtual Result JoinPartyRes( const Result &InResult, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData ) = 0;
			// S2C: Player Joined event
			virtual Result PartyPlayerJoinedS2CEvt( const PartyUID &InPartyUID, const PlayerInformation &InJoinedPlayer ) = 0;
			// S2C: Party leader changed event
			virtual Result PartyLeaderChangedS2CEvt( const PartyUID &InPartyUID, const PlayerID &InNewLeaderID ) = 0;
			// Cmd: Leave party command
			virtual Result LeavePartyRes( const Result &InResult ) = 0;
			// S2C: Party Player left event
			virtual Result PartyPlayerLeftS2CEvt( const PartyUID &InPartyUID, const PlayerID &InLeftPlayerID ) = 0;
			// Cmd: Kick player from the party
			virtual Result PartyKickPlayerRes( const Result &InResult ) = 0;
			// S2C: Party Player kicked message
			virtual Result PartyPlayerKickedS2CEvt( const PartyUID &InPartyUID, const PlayerID &InKickedPlayerID ) = 0;
			// Cmd: Invite a player to the party
			virtual Result PartyInviteRes( const Result &InResult ) = 0;
			// S2C: Party invite requested
			virtual Result PartyInviteRequestedS2CEvt( const PlayerID &InInviterID, const char* InInviterName, const PartyUID &InPartyToJoinUID ) = 0;
			// Cmd: Send Party quick chat message
			virtual Result PartyQuickChatMessageRes( const Result &InResult ) = 0;
			// S2C: Party Chatting message event
			virtual Result PartyQuickChatMessageS2CEvt( const PlayerID &InSenderID, const UINT32 &InQuickChatID ) = 0;
			// Cmd: Party chatting
			virtual Result PartyChatMessageRes( const Result &InResult ) = 0;
			// S2C: Party Chatting message event
			virtual Result PartyChatMessageS2CEvt( const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage ) = 0;
			// Cmd: Join to a game
			virtual Result JoinGameRes( const Result &InResult, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData ) = 0;
			// S2C: Player Joined in the game
			virtual Result PlayerJoinedS2CEvt( const GameInsUID &InGameInsUID, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter ) = 0;
			// Cmd: Leave Game
			virtual Result LeaveGameRes( const Result &InResult ) = 0;
			// S2C: Player left event
			virtual Result PlayerLeftS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InLeftPlayerID ) = 0;
			// Cmd: Kick player
			virtual Result KickPlayerRes( const Result &InResult ) = 0;
			// S2C: Player kicked
			virtual Result PlayerKickedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InKickedPlayerID ) = 0;
			// Cmd: Assign role + Game state reset
			virtual Result AssignRoleRes( const Result &InResult ) = 0;
			// S2C: Role assigned event
			virtual Result RoleAssignedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerRole &InRole ) = 0;
			// Cmd: Send chatting message to the game
			virtual Result ChatMessageRes( const Result &InResult ) = 0;
			// S2C: Chatting message event 
			virtual Result ChatMessageS2CEvt( const PlayerID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage ) = 0;
			// Cmd: Advance game
			virtual Result AdvanceGameRes( const Result &InResult ) = 0;
			// S2C: The game state is advanced
			virtual Result GameAdvancedS2CEvt( const GameInsUID &InGameInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay ) = 0;
			// S2C: Game is ended
			virtual Result GameEndedS2CEvt( const GameInsUID &InGameInsUID, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney ) = 0;
			// Cmd: Vote game advance
			virtual Result VoteGameAdvanceRes( const Result &InResult ) = 0;
			// S2C: GameAdvance is Voted
			virtual Result GameAdvanceVotedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InVoter ) = 0;
			// Cmd: Vote
			virtual Result VoteRes( const Result &InResult ) = 0;
			// S2C: Player Voted
			virtual Result VotedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InVoter, const PlayerID &InVotedTarget ) = 0;
			// S2C: Vote is ended
			virtual Result VoteEndS2CEvt( const GameInsUID &InGameInsUID, const Array<PlayerID>& InVoted ) = 0;
			// S2C: Player Killed
			virtual Result PlayerKilledS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason ) = 0;
			// S2C: Player Voted
			virtual Result PlayerRevealedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason ) = 0;
			// Cmd: Play again with the current players
			virtual Result GamePlayAgainRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney ) = 0;
			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			virtual Result GamePlayAgainS2CEvt( const PartyUID &InPartyUID, const PlayerID &InLeadPlayer ) = 0;
			// Cmd: Player. reveal a player
			virtual Result GameRevealPlayerRes( const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney ) = 0;
			// Cmd: Player. revive himself
			virtual Result GamePlayerReviveRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney ) = 0;
			// S2C: Player is revived
			virtual Result GamePlayerRevivedS2CEvt( const PlayerID &InRevivedPlayerID ) = 0;
			// Cmd: Player. reset ranking
			virtual Result GamePlayerResetRankRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney ) = 0;
			// Cmd: Request Game match
			virtual Result RequestGameMatchRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney ) = 0;
			// S2C: Game matched
			virtual Result GameMatchedS2CEvt( const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData, const UINT32 &InStamina, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney ) = 0;
			// S2C: Game match failed
			virtual Result GameMatchFailedS2CEvt( const Result &InFailedReason ) = 0;
			// S2C: Game matching started
			virtual Result GameMatchingStartedS2CEvt(  ) = 0;
			// Cmd: Cancel Game match
			virtual Result CancelGameMatchRes( const Result &InResult ) = 0;
			// S2C: game matching canceled
			virtual Result GameMatchingCanceledS2CEvt(  ) = 0;
			// Cmd: Buy shop item prepare
			virtual Result BuyShopItemPrepareRes( const Result &InResult, const UINT32 &InShopItemID, const char* InPurchaseID ) = 0;
			// Cmd: Buy shop item
			virtual Result BuyShopItemRes( const Result &InResult, const UINT32 &InShopItemID ) = 0;
			// Cmd: Give my stamina to other player
			virtual Result GiveStaminaRes( const Result &InResult, const PlayerID &InTargetPlayer, const UINT64 &InTimeStamp ) = 0;
			// Cmd: For debug, Change configue preset
			virtual Result SetPresetGameConfigIDRes( const Result &InResult ) = 0;
			// Cmd: For Debug
			virtual Result GainGameResourceRes( const Result &InResult ) = 0;

		}; // class ISvrPolicyGame : public Net::INetPolicy


		class IPolicyGame : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_GAME };

			// Constructor
			IPolicyGame( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// C2S: Client heart bit
			virtual Result HeartBitC2SEvt(  ) = 0;
			// Cmd: Player connected from a login server and moved to game server
			virtual Result JoinGameServerCmd( const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID ) = 0;
			// Cmd: player complition statues
			virtual Result GetComplitionStateCmd(  ) = 0;
			// Cmd: Player complition state
			virtual Result SetComplitionStateCmd( const char* InComplitionState ) = 0;
			// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
			virtual Result RegisterGCMCmd( const char* InGCMRegisteredID ) = 0;
			// Cmd: Unregister Google notification service ID
			virtual Result UnregisterGCMCmd( const char* InGCMRegisteredID ) = 0;
			// Cmd: Invite friend
			virtual Result InviteFriendCmd( const PlayerID &InFriendID ) = 0;
			// Cmd: Accept friend request
			virtual Result AcceptFriendRequestCmd( const PlayerID &InInviterID, const FacebookUID &InInviterFacebookUID ) = 0;
			// S2C: Notification for friend request is accepted
			// Cmd: Remove friden form the friend list
			virtual Result RemoveFriendCmd( const PlayerID &InFriendID ) = 0;
			// S2C: Friend removed
			// Cmd: Get friend list
			virtual Result GetFriendListCmd( const UINT16 &InStartIndex, const UINT16 &InCount ) = 0;
			// Cmd: Query notification list
			virtual Result GetNotificationListCmd(  ) = 0;
			// Cmd: Delete notification
			virtual Result DeleteNotificationCmd( const UINT32 &InNotificationID ) = 0;
			// Cmd: Set notification is read
			virtual Result SetNotificationReadCmd( const UINT32 &InNotificationID ) = 0;
			// Cmd: Accept notification
			virtual Result AcceptNotificationCmd( const UINT32 &InNotificationID ) = 0;
			// S2C: Notify new notification
			// Cmd: Query playerID list
			virtual Result FindPlayerByEMailCmd( const char* InPlayerEMail ) = 0;
			// Cmd: Query playerID list
			virtual Result FindPlayerByPlayerIDCmd( const PlayerID &InPlayerID ) = 0;
			// Cmd: *Request Player Status Update
			virtual Result RequestPlayerStatusUpdateCmd( const Array<PlayerID>& InTargetPlayerID ) = 0;
			// S2C: *Notify Player Status Updated
			// Cmd: Get Ranking lise
			virtual Result GetRankingListCmd( const RankingType &InRankingType, const BYTE &InBaseRanking, const BYTE &InCount ) = 0;
			// Cmd: Game user game play information
			virtual Result GetUserGamePlayerInfoCmd(  ) = 0;
			// Cmd: Game game play information
			virtual Result GetGamePlayerInfoCmd( const PlayerID &InPlayerID ) = 0;
			// S2C: Player level up event
			// Cmd: Change NickName
			virtual Result SetNickNameCmd( const char* InNickName, const BYTE &InIsCostFree ) = 0;
			// Cmd: Create Party
			virtual Result CreatePartyCmd(  ) = 0;
			// Cmd: Join party
			virtual Result JoinPartyCmd( const PartyUID &InPartyUID, const PlayerID &InInviterID ) = 0;
			// S2C: Player Joined event
			// S2C: Party leader changed event
			// Cmd: Leave party command
			virtual Result LeavePartyCmd( const PartyUID &InPartyUID, const PlayerID &InPlayerID ) = 0;
			// S2C: Party Player left event
			// Cmd: Kick player from the party
			virtual Result PartyKickPlayerCmd( const PartyUID &InPartyUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick ) = 0;
			// S2C: Party Player kicked message
			// Cmd: Invite a player to the party
			virtual Result PartyInviteCmd( const PlayerID &InInviteTargetID ) = 0;
			// S2C: Party invite requested
			// Cmd: Send Party quick chat message
			virtual Result PartyQuickChatMessageCmd( const UINT32 &InQuickChatID ) = 0;
			// S2C: Party Chatting message event
			// Cmd: Party chatting
			virtual Result PartyChatMessageCmd( const char* InChatMessage ) = 0;
			// S2C: Party Chatting message event
			// Cmd: Join to a game
			virtual Result JoinGameCmd( const PlayerID &InPlayerID, const AuthTicket &InTicket, const GameInsUID &InInsUID ) = 0;
			// S2C: Player Joined in the game
			// Cmd: Leave Game
			virtual Result LeaveGameCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket ) = 0;
			// S2C: Player left event
			// Cmd: Kick player
			virtual Result KickPlayerCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick ) = 0;
			// S2C: Player kicked
			// Cmd: Assign role + Game state reset
			virtual Result AssignRoleCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket ) = 0;
			// S2C: Role assigned event
			// Cmd: Send chatting message to the game
			virtual Result ChatMessageCmd( const char* InChatMessage, const PlayerRole &InRole ) = 0;
			// S2C: Chatting message event 
			// Cmd: Advance game
			virtual Result AdvanceGameCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket ) = 0;
			// S2C: The game state is advanced
			// S2C: Game is ended
			// Cmd: Vote game advance
			virtual Result VoteGameAdvanceCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket ) = 0;
			// S2C: GameAdvance is Voted
			// Cmd: Vote
			virtual Result VoteCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const PlayerID &InVoteTarget, const UINT32 &InActionSerial ) = 0;
			// S2C: Player Voted
			// S2C: Vote is ended
			// S2C: Player Killed
			// S2C: Player Voted
			// Cmd: Play again with the current players
			virtual Result GamePlayAgainCmd(  ) = 0;
			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			// Cmd: Player. reveal a player
			virtual Result GameRevealPlayerCmd( const Array<PlayerID>& InTargetPlayerID ) = 0;
			// Cmd: Player. revive himself
			virtual Result GamePlayerReviveCmd(  ) = 0;
			// S2C: Player is revived
			// Cmd: Player. reset ranking
			virtual Result GamePlayerResetRankCmd(  ) = 0;
			// Cmd: Request Game match
			virtual Result RequestGameMatchCmd( const BYTE &InNumPlayer, const PlayerRole &InRequestRole ) = 0;
			// S2C: Game matched
			// S2C: Game match failed
			// S2C: Game matching started
			// Cmd: Cancel Game match
			virtual Result CancelGameMatchCmd(  ) = 0;
			// S2C: game matching canceled
			// Cmd: Buy shop item prepare
			virtual Result BuyShopItemPrepareCmd( const UINT32 &InShopItemID ) = 0;
			// Cmd: Buy shop item
			virtual Result BuyShopItemCmd( const UINT32 &InShopItemID, const char* InPlatform, const char* InPackageName, const char* InPurchaseTransactionID, const Array<BYTE>& InPurchaseToken ) = 0;
			// Cmd: Give my stamina to other player
			virtual Result GiveStaminaCmd( const PlayerID &InTargetPlayer ) = 0;
			// Cmd: For debug, Change configue preset
			virtual Result SetPresetGameConfigIDCmd( const UINT32 &InPresetID ) = 0;
			// Cmd: For Debug
			virtual Result GainGameResourceCmd( const INT32 &InResource, const INT32 &InValue ) = 0;

		}; // class IPolicyGame : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


