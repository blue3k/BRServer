////////////////////////////////////////////////////////////////////////////////
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
			Result JoinGameServerCmd( const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID );
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
			Result GetFriendListCmd( const UINT16 &InStartIndex, const UINT16 &InCount );
			// Cmd: Query notification list
			Result GetNotificationListCmd(  );
			// Cmd: Delete notification
			Result DeleteNotificationCmd( const UINT32 &InNotificationID );
			// Cmd: Set notification is read
			Result SetNotificationReadCmd( const UINT32 &InNotificationID );
			// Cmd: Accept notification
			Result AcceptNotificationCmd( const UINT32 &InNotificationID );
			// Cmd: Query playerID list
			Result FindPlayerByEMailCmd( const char* InPlayerEMail );
			// Cmd: Query playerID list
			Result FindPlayerByPlayerIDCmd( const PlayerID &InPlayerID );
			// Cmd: *Request Player Status Update
			Result RequestPlayerStatusUpdateCmd( const Array<PlayerID>& InTargetPlayerID );
			// Cmd: Get Ranking lise
			Result GetRankingListCmd( const RankingType &InRankingType, const BYTE &InBaseRanking, const BYTE &InCount );
			// Cmd: Game user game play information
			Result GetUserGamePlayerInfoCmd(  );
			// Cmd: Game game play information
			Result GetGamePlayerInfoCmd( const PlayerID &InPlayerID );
			// Cmd: Change NickName
			Result SetNickNameCmd( const char* InNickName, const BYTE &InIsCostFree );
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
			Result PartyQuickChatMessageCmd( const UINT32 &InQuickChatID );
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
			Result VoteCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const PlayerID &InVoteTarget, const UINT32 &InActionSerial );
			// Cmd: Play again with the current players
			Result GamePlayAgainCmd(  );
			// Cmd: Player. reveal a player
			Result GameRevealPlayerCmd( const Array<PlayerID>& InTargetPlayerID );
			// Cmd: Player. revive himself
			Result GamePlayerReviveCmd(  );
			// Cmd: Player. reset ranking
			Result GamePlayerResetRankCmd(  );
			// Cmd: Request Game match
			Result RequestGameMatchCmd( const BYTE &InNumPlayer, const PlayerRole &InRequestRole );
			// Cmd: Cancel Game match
			Result CancelGameMatchCmd(  );
			// Cmd: Buy shop item prepare
			Result BuyShopItemPrepareCmd( const UINT32 &InShopItemID );
			// Cmd: Buy shop item
			Result BuyShopItemCmd( const UINT32 &InShopItemID, const char* InPlatform, const char* InPackageName, const char* InPurchaseTransactionID, const Array<BYTE>& InPurchaseToken );
			// Cmd: Give my stamina to other player
			Result GiveStaminaCmd( const PlayerID &InTargetPlayer );
			// Cmd: For debug, Change configue preset
			Result SetPresetGameConfigIDCmd( const UINT32 &InPresetID );
			// Cmd: For Debug
			Result GainGameResourceCmd( const INT32 &InResource, const INT32 &InValue );

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
			Result GetFriendListRes( const Result &InResult, const UINT16 &InMaxFriendSlot, const UINT16 &InTotalNumberOfFriends, const UINT16 &InStartIndex, const Array<FriendInformation>& InFriendList );
			// Cmd: Query notification list
			Result GetNotificationListRes( const Result &InResult );
			// Cmd: Delete notification
			Result DeleteNotificationRes( const Result &InResult, const UINT32 &InNotificationID );
			// Cmd: Set notification is read
			Result SetNotificationReadRes( const Result &InResult, const UINT32 &InNotificationID );
			// Cmd: Accept notification
			Result AcceptNotificationRes( const Result &InResult, const UINT32 &InNotificationID );
			// S2C: Notify new notification
			Result NotifyS2CEvt( const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const BYTE &InIsRead, const UINT64 &InTimeStamp );
			// Cmd: Query playerID list
			Result FindPlayerByEMailRes( const Result &InResult, const PlayerInformation &InPlayer );
			// Cmd: Query playerID list
			Result FindPlayerByPlayerIDRes( const Result &InResult, const PlayerInformation &InPlayer );
			// Cmd: *Request Player Status Update
			Result RequestPlayerStatusUpdateRes( const Result &InResult );
			// S2C: *Notify Player Status Updated
			Result NotifyPlayerStatusUpdatedS2CEvt( const PlayerID &InPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame );
			// Cmd: Get Ranking lise
			Result GetRankingListRes( const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking );
			// Cmd: Game user game play information
			Result GetUserGamePlayerInfoRes( const Result &InResult, const INT16 &InLevel, const INT64 &InExp, const INT64 &InGameMoney, const INT64 &InGem, const INT16 &InStamina, const UINT32 &InLastUpdateTime, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose );
			// Cmd: Game game play information
			Result GetGamePlayerInfoRes( const Result &InResult, const PlayerID &InPlayerID, const INT16 &InLevel, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose );
			// S2C: Player level up event
			Result LevelUpS2CEvt( const UINT64 &InCurrentTotalExp, const UINT32 &InCurrentLevel );
			// Cmd: Change NickName
			Result SetNickNameRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );
			// Cmd: Create Party
			Result CreatePartyRes( const Result &InResult, const PartyUID &InPartyUID );
			// Cmd: Join party
			Result JoinPartyRes( const Result &InResult, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData );
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
			Result PartyQuickChatMessageS2CEvt( const PlayerID &InSenderID, const UINT32 &InQuickChatID );
			// Cmd: Party chatting
			Result PartyChatMessageRes( const Result &InResult );
			// S2C: Party Chatting message event
			Result PartyChatMessageS2CEvt( const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage );
			// Cmd: Join to a game
			Result JoinGameRes( const Result &InResult, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData );
			// S2C: Player Joined in the game
			Result PlayerJoinedS2CEvt( const GameInsUID &InGameInsUID, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter );
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
			Result GameAdvancedS2CEvt( const GameInsUID &InGameInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay );
			// S2C: Game is ended
			Result GameEndedS2CEvt( const GameInsUID &InGameInsUID, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney );
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
			Result GamePlayAgainRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );
			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			Result GamePlayAgainS2CEvt( const PartyUID &InPartyUID, const PlayerID &InLeadPlayer );
			// Cmd: Player. reveal a player
			Result GameRevealPlayerRes( const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );
			// Cmd: Player. revive himself
			Result GamePlayerReviveRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );
			// S2C: Player is revived
			Result GamePlayerRevivedS2CEvt( const PlayerID &InRevivedPlayerID );
			// Cmd: Player. reset ranking
			Result GamePlayerResetRankRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );
			// Cmd: Request Game match
			Result RequestGameMatchRes( const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );
			// S2C: Game matched
			Result GameMatchedS2CEvt( const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData, const UINT32 &InStamina, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );
			// S2C: Game match failed
			Result GameMatchFailedS2CEvt( const Result &InFailedReason );
			// S2C: Game matching started
			Result GameMatchingStartedS2CEvt(  );
			// Cmd: Cancel Game match
			Result CancelGameMatchRes( const Result &InResult );
			// S2C: game matching canceled
			Result GameMatchingCanceledS2CEvt(  );
			// Cmd: Buy shop item prepare
			Result BuyShopItemPrepareRes( const Result &InResult, const UINT32 &InShopItemID, const char* InPurchaseID );
			// Cmd: Buy shop item
			Result BuyShopItemRes( const Result &InResult, const UINT32 &InShopItemID );
			// Cmd: Give my stamina to other player
			Result GiveStaminaRes( const Result &InResult, const PlayerID &InTargetPlayer, const UINT64 &InTimeStamp );
			// Cmd: For debug, Change configue preset
			Result SetPresetGameConfigIDRes( const Result &InResult );
			// Cmd: For Debug
			Result GainGameResourceRes( const Result &InResult );

		}; // class NetSvrPolicyGame : public ISvrPolicyGame


	}; // namespace Policy
}; // namespace BR


