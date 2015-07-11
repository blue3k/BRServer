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
#include "Net/Netdef.h"
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
			virtual HRESULT JoinGameServerRes( const HRESULT &InResult, const char* InNickName, const GameInsUID &InGameUID, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const MatchingQueueTicket &InMatchingTicket ) = 0;
			// Cmd: player complition statues
			virtual HRESULT GetComplitionStateRes( const HRESULT &InResult, const char* InComplitionState ) = 0;
			// Cmd: Player complition state
			virtual HRESULT SetComplitionStateRes( const HRESULT &InResult ) = 0;
			// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
			virtual HRESULT RegisterGCMRes( const HRESULT &InResult ) = 0;
			// Cmd: Unregister Google notification service ID
			virtual HRESULT UnregisterGCMRes( const HRESULT &InResult ) = 0;
			// Cmd: Invite friend
			virtual HRESULT InviteFriendRes( const HRESULT &InResult ) = 0;
			// Cmd: Accept friend request
			virtual HRESULT AcceptFriendRequestRes( const HRESULT &InResult, const FriendInformation &InNewFriend ) = 0;
			// S2C: Notification for friend request is accepted
			virtual HRESULT FriendRequestAcceptedS2CEvt( const FriendInformation &InAccepter ) = 0;
			// Cmd: Remove friden form the friend list
			virtual HRESULT RemoveFriendRes( const HRESULT &InResult, const PlayerID &InFriendID ) = 0;
			// S2C: Friend removed
			virtual HRESULT FriendRemovedS2CEvt( const PlayerID &InFriendID ) = 0;
			// Cmd: Get friend list
			virtual HRESULT GetFriendListRes( const HRESULT &InResult, const UINT16 &InMaxFriendSlot, const UINT16 &InTotalNumberOfFriends, const UINT16 &InStartIndex, const Array<FriendInformation>& InFriendList ) = 0;
			// Cmd: Query notification list
			virtual HRESULT GetNotificationListRes( const HRESULT &InResult ) = 0;
			// Cmd: Delete notification
			virtual HRESULT DeleteNotificationRes( const HRESULT &InResult, const UINT32 &InNotificationID ) = 0;
			// Cmd: Set notification is read
			virtual HRESULT SetNotificationReadRes( const HRESULT &InResult, const UINT32 &InNotificationID ) = 0;
			// Cmd: Accept notification
			virtual HRESULT AcceptNotificationRes( const HRESULT &InResult, const UINT32 &InNotificationID ) = 0;
			// S2C: Notify new notification
			virtual HRESULT NotifyS2CEvt( const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const BYTE &InIsRead, const UINT64 &InTimeStamp ) = 0;
			// Cmd: Query playerID list
			virtual HRESULT FindPlayerByEMailRes( const HRESULT &InResult, const PlayerInformation &InPlayer ) = 0;
			// Cmd: *Request Player Status Update
			virtual HRESULT RequestPlayerStatusUpdateRes( const HRESULT &InResult ) = 0;
			// S2C: *Notify Player Status Updated
			virtual HRESULT NotifyPlayerStatusUpdatedS2CEvt( const PlayerID &InPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame ) = 0;
			// Cmd: Get Ranking lise
			virtual HRESULT GetRankingListRes( const HRESULT &InResult, const Array<TotalRankingPlayerInformation>& InRanking ) = 0;
			// Cmd: Game user game play information
			virtual HRESULT GetUserGamePlayerInfoRes( const HRESULT &InResult, const INT16 &InLevel, const INT64 &InExp, const INT64 &InGameMoney, const INT64 &InGem, const INT16 &InStamina, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose ) = 0;
			// Cmd: Game game play information
			virtual HRESULT GetGamePlayerInfoRes( const HRESULT &InResult, const PlayerID &InPlayerID, const INT16 &InLevel, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose ) = 0;
			// S2C: Player level up event
			virtual HRESULT LevelUpS2CEvt( const UINT64 &InCurrentTotalExp, const UINT32 &InCurrentLevel ) = 0;
			// Cmd: Change NickName
			virtual HRESULT SetNickNameRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney ) = 0;
			// Cmd: Create Party
			virtual HRESULT CreatePartyRes( const HRESULT &InResult, const PartyUID &InPartyUID ) = 0;
			// Cmd: Join party
			virtual HRESULT JoinPartyRes( const HRESULT &InResult, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData ) = 0;
			// S2C: Player Joined event
			virtual HRESULT PartyPlayerJoinedS2CEvt( const PartyUID &InPartyUID, const PlayerInformation &InJoinedPlayer ) = 0;
			// S2C: Party leader changed event
			virtual HRESULT PartyLeaderChangedS2CEvt( const PartyUID &InPartyUID, const PlayerID &InNewLeaderID ) = 0;
			// Cmd: Leave party command
			virtual HRESULT LeavePartyRes( const HRESULT &InResult ) = 0;
			// S2C: Party Player left event
			virtual HRESULT PartyPlayerLeftS2CEvt( const PartyUID &InPartyUID, const PlayerID &InLeftPlayerID ) = 0;
			// Cmd: Kick player from the party
			virtual HRESULT PartyKickPlayerRes( const HRESULT &InResult ) = 0;
			// S2C: Party Player kicked message
			virtual HRESULT PartyPlayerKickedS2CEvt( const PartyUID &InPartyUID, const PlayerID &InKickedPlayerID ) = 0;
			// Cmd: Invite a player to the party
			virtual HRESULT PartyInviteRes( const HRESULT &InResult ) = 0;
			// S2C: Party invite requested
			virtual HRESULT PartyInviteRequestedS2CEvt( const PlayerID &InInviterID, const char* InInviterName, const PartyUID &InPartyToJoinUID ) = 0;
			// Cmd: Send Party quick chat message
			virtual HRESULT PartyQuickChatMessageRes( const HRESULT &InResult ) = 0;
			// S2C: Party Chatting message event
			virtual HRESULT PartyQuickChatMessageS2CEvt( const PlayerID &InSenderID, const UINT32 &InQuickChatID ) = 0;
			// Cmd: Party chatting
			virtual HRESULT PartyChatMessageRes( const HRESULT &InResult ) = 0;
			// S2C: Party Chatting message event
			virtual HRESULT PartyChatMessageS2CEvt( const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage ) = 0;
			// Cmd: Join to a game
			virtual HRESULT JoinGameRes( const HRESULT &InResult, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData ) = 0;
			// S2C: Player Joined in the game
			virtual HRESULT PlayerJoinedS2CEvt( const GameInsUID &InGameInsUID, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter ) = 0;
			// Cmd: Leave Game
			virtual HRESULT LeaveGameRes( const HRESULT &InResult ) = 0;
			// S2C: Player left event
			virtual HRESULT PlayerLeftS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InLeftPlayerID ) = 0;
			// Cmd: Kick player
			virtual HRESULT KickPlayerRes( const HRESULT &InResult ) = 0;
			// S2C: Player kicked
			virtual HRESULT PlayerKickedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InKickedPlayerID ) = 0;
			// Cmd: Assign role + Game state reset
			virtual HRESULT AssignRoleRes( const HRESULT &InResult ) = 0;
			// S2C: Role assigned event
			virtual HRESULT RoleAssignedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerRole &InRole ) = 0;
			// Cmd: Send chatting message to the game
			virtual HRESULT ChatMessageRes( const HRESULT &InResult ) = 0;
			// S2C: Chatting message event 
			virtual HRESULT ChatMessageS2CEvt( const PlayerID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage ) = 0;
			// Cmd: Advance game
			virtual HRESULT AdvanceGameRes( const HRESULT &InResult ) = 0;
			// S2C: The game state is advanced
			virtual HRESULT GameAdvancedS2CEvt( const GameInsUID &InGameInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay ) = 0;
			// S2C: Game is ended
			virtual HRESULT GameEndedS2CEvt( const GameInsUID &InGameInsUID, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney ) = 0;
			// Cmd: Vote game advance
			virtual HRESULT VoteGameAdvanceRes( const HRESULT &InResult ) = 0;
			// S2C: GameAdvance is Voted
			virtual HRESULT GameAdvanceVotedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InVoter ) = 0;
			// Cmd: Vote
			virtual HRESULT VoteRes( const HRESULT &InResult ) = 0;
			// S2C: Player Voted
			virtual HRESULT VotedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InVoter, const PlayerID &InVotedTarget ) = 0;
			// S2C: Vote is ended
			virtual HRESULT VoteEndS2CEvt( const GameInsUID &InGameInsUID, const Array<PlayerID>& InVoted ) = 0;
			// S2C: Player Killed
			virtual HRESULT PlayerKilledS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason ) = 0;
			// S2C: Player Voted
			virtual HRESULT PlayerRevealedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason ) = 0;
			// Cmd: Play again with the current players
			virtual HRESULT GamePlayAgainRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney ) = 0;
			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			virtual HRESULT GamePlayAgainS2CEvt( const PartyUID &InPartyUID, const PlayerID &InLeadPlayer ) = 0;
			// Cmd: Player. reveal a player
			virtual HRESULT GameRevealPlayerRes( const HRESULT &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney ) = 0;
			// Cmd: Player. revive himself
			virtual HRESULT GamePlayerReviveRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney ) = 0;
			// S2C: Player is revived
			virtual HRESULT GamePlayerRevivedS2CEvt( const PlayerID &InRevivedPlayerID ) = 0;
			// Cmd: Player. reset ranking
			virtual HRESULT GamePlayerResetRankRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney ) = 0;
			// Cmd: Request Game match
			virtual HRESULT RequestGameMatchRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney ) = 0;
			// S2C: Game matched
			virtual HRESULT GameMatchedS2CEvt( const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData, const UINT32 &InStamina, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney ) = 0;
			// S2C: Game match failed
			virtual HRESULT GameMatchFailedS2CEvt( const HRESULT &InFailedReason ) = 0;
			// S2C: Game matching started
			virtual HRESULT GameMatchingStartedS2CEvt(  ) = 0;
			// Cmd: Cancel Game match
			virtual HRESULT CancelGameMatchRes( const HRESULT &InResult ) = 0;
			// S2C: game matching canceled
			virtual HRESULT GameMatchingCanceledS2CEvt(  ) = 0;
			// Cmd: Buy shop item prepare
			virtual HRESULT BuyShopItemPrepareRes( const HRESULT &InResult, const UINT32 &InShopItemID, const char* InPurchaseID ) = 0;
			// Cmd: Buy shop item
			virtual HRESULT BuyShopItemRes( const HRESULT &InResult, const UINT32 &InShopItemID ) = 0;
			// Cmd: Give my stamina to other player
			virtual HRESULT GiveStaminaRes( const HRESULT &InResult, const PlayerID &InTargetPlayer, const UINT64 &InTimeStamp ) = 0;
			// Cmd: For debug, Change configue preset
			virtual HRESULT SetPresetGameConfigIDRes( const HRESULT &InResult ) = 0;
			// Cmd: For Debug
			virtual HRESULT GainGameResourceRes( const HRESULT &InResult ) = 0;

		}; // class ISvrPolicyGame : public Net::INetPolicy


		class IPolicyGame : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = POLICY_GAME };

			// Constructor
			IPolicyGame( Net::IConnection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// C2S: Client heart bit
			virtual HRESULT HeartBitC2SEvt(  ) = 0;
			// Cmd: Player connected from a login server and moved to game server
			virtual HRESULT JoinGameServerCmd( const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID ) = 0;
			// Cmd: player complition statues
			virtual HRESULT GetComplitionStateCmd(  ) = 0;
			// Cmd: Player complition state
			virtual HRESULT SetComplitionStateCmd( const char* InComplitionState ) = 0;
			// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
			virtual HRESULT RegisterGCMCmd( const char* InGCMRegisteredID ) = 0;
			// Cmd: Unregister Google notification service ID
			virtual HRESULT UnregisterGCMCmd( const char* InGCMRegisteredID ) = 0;
			// Cmd: Invite friend
			virtual HRESULT InviteFriendCmd( const PlayerID &InFriendID ) = 0;
			// Cmd: Accept friend request
			virtual HRESULT AcceptFriendRequestCmd( const PlayerID &InInviterID, const FacebookUID &InInviterFacebookUID ) = 0;
			// S2C: Notification for friend request is accepted
			// Cmd: Remove friden form the friend list
			virtual HRESULT RemoveFriendCmd( const PlayerID &InFriendID ) = 0;
			// S2C: Friend removed
			// Cmd: Get friend list
			virtual HRESULT GetFriendListCmd( const UINT16 &InStartIndex, const UINT16 &InCount ) = 0;
			// Cmd: Query notification list
			virtual HRESULT GetNotificationListCmd(  ) = 0;
			// Cmd: Delete notification
			virtual HRESULT DeleteNotificationCmd( const UINT32 &InNotificationID ) = 0;
			// Cmd: Set notification is read
			virtual HRESULT SetNotificationReadCmd( const UINT32 &InNotificationID ) = 0;
			// Cmd: Accept notification
			virtual HRESULT AcceptNotificationCmd( const UINT32 &InNotificationID ) = 0;
			// S2C: Notify new notification
			// Cmd: Query playerID list
			virtual HRESULT FindPlayerByEMailCmd( const char* InPlayerEMail ) = 0;
			// Cmd: *Request Player Status Update
			virtual HRESULT RequestPlayerStatusUpdateCmd( const Array<PlayerID>& InTargetPlayerID ) = 0;
			// S2C: *Notify Player Status Updated
			// Cmd: Get Ranking lise
			virtual HRESULT GetRankingListCmd( const RankingType &InRankingType, const BYTE &InBaseRanking, const BYTE &InCount ) = 0;
			// Cmd: Game user game play information
			virtual HRESULT GetUserGamePlayerInfoCmd(  ) = 0;
			// Cmd: Game game play information
			virtual HRESULT GetGamePlayerInfoCmd( const PlayerID &InPlayerID ) = 0;
			// S2C: Player level up event
			// Cmd: Change NickName
			virtual HRESULT SetNickNameCmd( const char* InNickName, const BYTE &InIsCostFree ) = 0;
			// Cmd: Create Party
			virtual HRESULT CreatePartyCmd(  ) = 0;
			// Cmd: Join party
			virtual HRESULT JoinPartyCmd( const PartyUID &InPartyUID, const PlayerID &InInviterID ) = 0;
			// S2C: Player Joined event
			// S2C: Party leader changed event
			// Cmd: Leave party command
			virtual HRESULT LeavePartyCmd( const PartyUID &InPartyUID, const PlayerID &InPlayerID ) = 0;
			// S2C: Party Player left event
			// Cmd: Kick player from the party
			virtual HRESULT PartyKickPlayerCmd( const PartyUID &InPartyUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick ) = 0;
			// S2C: Party Player kicked message
			// Cmd: Invite a player to the party
			virtual HRESULT PartyInviteCmd( const PlayerID &InInviteTargetID ) = 0;
			// S2C: Party invite requested
			// Cmd: Send Party quick chat message
			virtual HRESULT PartyQuickChatMessageCmd( const UINT32 &InQuickChatID ) = 0;
			// S2C: Party Chatting message event
			// Cmd: Party chatting
			virtual HRESULT PartyChatMessageCmd( const char* InChatMessage ) = 0;
			// S2C: Party Chatting message event
			// Cmd: Join to a game
			virtual HRESULT JoinGameCmd( const PlayerID &InPlayerID, const AuthTicket &InTicket, const GameInsUID &InInsUID ) = 0;
			// S2C: Player Joined in the game
			// Cmd: Leave Game
			virtual HRESULT LeaveGameCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket ) = 0;
			// S2C: Player left event
			// Cmd: Kick player
			virtual HRESULT KickPlayerCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick ) = 0;
			// S2C: Player kicked
			// Cmd: Assign role + Game state reset
			virtual HRESULT AssignRoleCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket ) = 0;
			// S2C: Role assigned event
			// Cmd: Send chatting message to the game
			virtual HRESULT ChatMessageCmd( const char* InChatMessage, const PlayerRole &InRole ) = 0;
			// S2C: Chatting message event 
			// Cmd: Advance game
			virtual HRESULT AdvanceGameCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket ) = 0;
			// S2C: The game state is advanced
			// S2C: Game is ended
			// Cmd: Vote game advance
			virtual HRESULT VoteGameAdvanceCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket ) = 0;
			// S2C: GameAdvance is Voted
			// Cmd: Vote
			virtual HRESULT VoteCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const PlayerID &InVoteTarget, const UINT32 &InActionSerial ) = 0;
			// S2C: Player Voted
			// S2C: Vote is ended
			// S2C: Player Killed
			// S2C: Player Voted
			// Cmd: Play again with the current players
			virtual HRESULT GamePlayAgainCmd(  ) = 0;
			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			// Cmd: Player. reveal a player
			virtual HRESULT GameRevealPlayerCmd( const Array<PlayerID>& InTargetPlayerID ) = 0;
			// Cmd: Player. revive himself
			virtual HRESULT GamePlayerReviveCmd(  ) = 0;
			// S2C: Player is revived
			// Cmd: Player. reset ranking
			virtual HRESULT GamePlayerResetRankCmd(  ) = 0;
			// Cmd: Request Game match
			virtual HRESULT RequestGameMatchCmd( const BYTE &InNumPlayer, const PlayerRole &InRequestRole ) = 0;
			// S2C: Game matched
			// S2C: Game match failed
			// S2C: Game matching started
			// Cmd: Cancel Game match
			virtual HRESULT CancelGameMatchCmd(  ) = 0;
			// S2C: game matching canceled
			// Cmd: Buy shop item prepare
			virtual HRESULT BuyShopItemPrepareCmd( const UINT32 &InShopItemID ) = 0;
			// Cmd: Buy shop item
			virtual HRESULT BuyShopItemCmd( const UINT32 &InShopItemID, const char* InPlatform, const char* InPackageName, const char* InPurchaseTransactionID, const Array<BYTE>& InPurchaseToken ) = 0;
			// Cmd: Give my stamina to other player
			virtual HRESULT GiveStaminaCmd( const PlayerID &InTargetPlayer ) = 0;
			// Cmd: For debug, Change configue preset
			virtual HRESULT SetPresetGameConfigIDCmd( const UINT32 &InPresetID ) = 0;
			// Cmd: For Debug
			virtual HRESULT GainGameResourceCmd( const INT32 &InResource, const INT32 &InValue ) = 0;

		}; // class IPolicyGame : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


