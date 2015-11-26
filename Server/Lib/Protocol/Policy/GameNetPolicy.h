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
			HRESULT HeartBitC2SEvt(  );
			// Cmd: Player connected from a login server and moved to game server
			HRESULT JoinGameServerCmd( const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID );
			// Cmd: player complition statues
			HRESULT GetComplitionStateCmd(  );
			// Cmd: Player complition state
			HRESULT SetComplitionStateCmd( const char* InComplitionState );
			// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
			HRESULT RegisterGCMCmd( const char* InGCMRegisteredID );
			// Cmd: Unregister Google notification service ID
			HRESULT UnregisterGCMCmd( const char* InGCMRegisteredID );
			// Cmd: Invite friend
			HRESULT InviteFriendCmd( const PlayerID &InFriendID );
			// Cmd: Accept friend request
			HRESULT AcceptFriendRequestCmd( const PlayerID &InInviterID, const FacebookUID &InInviterFacebookUID );
			// Cmd: Remove friden form the friend list
			HRESULT RemoveFriendCmd( const PlayerID &InFriendID );
			// Cmd: Get friend list
			HRESULT GetFriendListCmd( const UINT16 &InStartIndex, const UINT16 &InCount );
			// Cmd: Query notification list
			HRESULT GetNotificationListCmd(  );
			// Cmd: Delete notification
			HRESULT DeleteNotificationCmd( const UINT32 &InNotificationID );
			// Cmd: Set notification is read
			HRESULT SetNotificationReadCmd( const UINT32 &InNotificationID );
			// Cmd: Accept notification
			HRESULT AcceptNotificationCmd( const UINT32 &InNotificationID );
			// Cmd: Query playerID list
			HRESULT FindPlayerByEMailCmd( const char* InPlayerEMail );
			// Cmd: Query playerID list
			HRESULT FindPlayerByPlayerIDCmd( const PlayerID &InPlayerID );
			// Cmd: *Request Player Status Update
			HRESULT RequestPlayerStatusUpdateCmd( const Array<PlayerID>& InTargetPlayerID );
			// Cmd: Get Ranking lise
			HRESULT GetRankingListCmd( const RankingType &InRankingType, const BYTE &InBaseRanking, const BYTE &InCount );
			// Cmd: Game user game play information
			HRESULT GetUserGamePlayerInfoCmd(  );
			// Cmd: Game game play information
			HRESULT GetGamePlayerInfoCmd( const PlayerID &InPlayerID );
			// Cmd: Change NickName
			HRESULT SetNickNameCmd( const char* InNickName, const BYTE &InIsCostFree );
			// Cmd: Create Party
			HRESULT CreatePartyCmd(  );
			// Cmd: Join party
			HRESULT JoinPartyCmd( const PartyUID &InPartyUID, const PlayerID &InInviterID );
			// Cmd: Leave party command
			HRESULT LeavePartyCmd( const PartyUID &InPartyUID, const PlayerID &InPlayerID );
			// Cmd: Kick player from the party
			HRESULT PartyKickPlayerCmd( const PartyUID &InPartyUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
			// Cmd: Invite a player to the party
			HRESULT PartyInviteCmd( const PlayerID &InInviteTargetID );
			// Cmd: Send Party quick chat message
			HRESULT PartyQuickChatMessageCmd( const UINT32 &InQuickChatID );
			// Cmd: Party chatting
			HRESULT PartyChatMessageCmd( const char* InChatMessage );
			// Cmd: Join to a game
			HRESULT JoinGameCmd( const PlayerID &InPlayerID, const AuthTicket &InTicket, const GameInsUID &InInsUID );
			// Cmd: Leave Game
			HRESULT LeaveGameCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket );
			// Cmd: Kick player
			HRESULT KickPlayerCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );
			// Cmd: Assign role + Game state reset
			HRESULT AssignRoleCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket );
			// Cmd: Send chatting message to the game
			HRESULT ChatMessageCmd( const char* InChatMessage, const PlayerRole &InRole );
			// Cmd: Advance game
			HRESULT AdvanceGameCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket );
			// Cmd: Vote game advance
			HRESULT VoteGameAdvanceCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket );
			// Cmd: Vote
			HRESULT VoteCmd( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const PlayerID &InVoteTarget, const UINT32 &InActionSerial );
			// Cmd: Play again with the current players
			HRESULT GamePlayAgainCmd(  );
			// Cmd: Player. reveal a player
			HRESULT GameRevealPlayerCmd( const Array<PlayerID>& InTargetPlayerID );
			// Cmd: Player. revive himself
			HRESULT GamePlayerReviveCmd(  );
			// Cmd: Player. reset ranking
			HRESULT GamePlayerResetRankCmd(  );
			// Cmd: Request Game match
			HRESULT RequestGameMatchCmd( const BYTE &InNumPlayer, const PlayerRole &InRequestRole );
			// Cmd: Cancel Game match
			HRESULT CancelGameMatchCmd(  );
			// Cmd: Buy shop item prepare
			HRESULT BuyShopItemPrepareCmd( const UINT32 &InShopItemID );
			// Cmd: Buy shop item
			HRESULT BuyShopItemCmd( const UINT32 &InShopItemID, const char* InPlatform, const char* InPackageName, const char* InPurchaseTransactionID, const Array<BYTE>& InPurchaseToken );
			// Cmd: Give my stamina to other player
			HRESULT GiveStaminaCmd( const PlayerID &InTargetPlayer );
			// Cmd: For debug, Change configue preset
			HRESULT SetPresetGameConfigIDCmd( const UINT32 &InPresetID );
			// Cmd: For Debug
			HRESULT GainGameResourceCmd( const INT32 &InResource, const INT32 &InValue );

		}; // class NetPolicyGame : public IPolicyGame


		class NetSvrPolicyGame : public ISvrPolicyGame
		{
 			public:
			// Constructor
			NetSvrPolicyGame ( Net::IConnection* pConn ) : ISvrPolicyGame(pConn)
			{}

			// Cmd: Player connected from a login server and moved to game server
			HRESULT JoinGameServerRes( const HRESULT &InResult, const char* InNickName, const GameInsUID &InGameUID, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const MatchingQueueTicket &InMatchingTicket );
			// Cmd: player complition statues
			HRESULT GetComplitionStateRes( const HRESULT &InResult, const char* InComplitionState );
			// Cmd: Player complition state
			HRESULT SetComplitionStateRes( const HRESULT &InResult );
			// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
			HRESULT RegisterGCMRes( const HRESULT &InResult );
			// Cmd: Unregister Google notification service ID
			HRESULT UnregisterGCMRes( const HRESULT &InResult );
			// Cmd: Invite friend
			HRESULT InviteFriendRes( const HRESULT &InResult );
			// Cmd: Accept friend request
			HRESULT AcceptFriendRequestRes( const HRESULT &InResult, const FriendInformation &InNewFriend );
			// S2C: Notification for friend request is accepted
			HRESULT FriendRequestAcceptedS2CEvt( const FriendInformation &InAccepter );
			// Cmd: Remove friden form the friend list
			HRESULT RemoveFriendRes( const HRESULT &InResult, const PlayerID &InFriendID );
			// S2C: Friend removed
			HRESULT FriendRemovedS2CEvt( const PlayerID &InFriendID );
			// Cmd: Get friend list
			HRESULT GetFriendListRes( const HRESULT &InResult, const UINT16 &InMaxFriendSlot, const UINT16 &InTotalNumberOfFriends, const UINT16 &InStartIndex, const Array<FriendInformation>& InFriendList );
			// Cmd: Query notification list
			HRESULT GetNotificationListRes( const HRESULT &InResult );
			// Cmd: Delete notification
			HRESULT DeleteNotificationRes( const HRESULT &InResult, const UINT32 &InNotificationID );
			// Cmd: Set notification is read
			HRESULT SetNotificationReadRes( const HRESULT &InResult, const UINT32 &InNotificationID );
			// Cmd: Accept notification
			HRESULT AcceptNotificationRes( const HRESULT &InResult, const UINT32 &InNotificationID );
			// S2C: Notify new notification
			HRESULT NotifyS2CEvt( const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const BYTE &InIsRead, const UINT64 &InTimeStamp );
			// Cmd: Query playerID list
			HRESULT FindPlayerByEMailRes( const HRESULT &InResult, const PlayerInformation &InPlayer );
			// Cmd: Query playerID list
			HRESULT FindPlayerByPlayerIDRes( const HRESULT &InResult, const PlayerInformation &InPlayer );
			// Cmd: *Request Player Status Update
			HRESULT RequestPlayerStatusUpdateRes( const HRESULT &InResult );
			// S2C: *Notify Player Status Updated
			HRESULT NotifyPlayerStatusUpdatedS2CEvt( const PlayerID &InPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame );
			// Cmd: Get Ranking lise
			HRESULT GetRankingListRes( const HRESULT &InResult, const Array<TotalRankingPlayerInformation>& InRanking );
			// Cmd: Game user game play information
			HRESULT GetUserGamePlayerInfoRes( const HRESULT &InResult, const INT16 &InLevel, const INT64 &InExp, const INT64 &InGameMoney, const INT64 &InGem, const INT16 &InStamina, const UINT32 &InLastUpdateTime, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose );
			// Cmd: Game game play information
			HRESULT GetGamePlayerInfoRes( const HRESULT &InResult, const PlayerID &InPlayerID, const INT16 &InLevel, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose );
			// S2C: Player level up event
			HRESULT LevelUpS2CEvt( const UINT64 &InCurrentTotalExp, const UINT32 &InCurrentLevel );
			// Cmd: Change NickName
			HRESULT SetNickNameRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );
			// Cmd: Create Party
			HRESULT CreatePartyRes( const HRESULT &InResult, const PartyUID &InPartyUID );
			// Cmd: Join party
			HRESULT JoinPartyRes( const HRESULT &InResult, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData );
			// S2C: Player Joined event
			HRESULT PartyPlayerJoinedS2CEvt( const PartyUID &InPartyUID, const PlayerInformation &InJoinedPlayer );
			// S2C: Party leader changed event
			HRESULT PartyLeaderChangedS2CEvt( const PartyUID &InPartyUID, const PlayerID &InNewLeaderID );
			// Cmd: Leave party command
			HRESULT LeavePartyRes( const HRESULT &InResult );
			// S2C: Party Player left event
			HRESULT PartyPlayerLeftS2CEvt( const PartyUID &InPartyUID, const PlayerID &InLeftPlayerID );
			// Cmd: Kick player from the party
			HRESULT PartyKickPlayerRes( const HRESULT &InResult );
			// S2C: Party Player kicked message
			HRESULT PartyPlayerKickedS2CEvt( const PartyUID &InPartyUID, const PlayerID &InKickedPlayerID );
			// Cmd: Invite a player to the party
			HRESULT PartyInviteRes( const HRESULT &InResult );
			// S2C: Party invite requested
			HRESULT PartyInviteRequestedS2CEvt( const PlayerID &InInviterID, const char* InInviterName, const PartyUID &InPartyToJoinUID );
			// Cmd: Send Party quick chat message
			HRESULT PartyQuickChatMessageRes( const HRESULT &InResult );
			// S2C: Party Chatting message event
			HRESULT PartyQuickChatMessageS2CEvt( const PlayerID &InSenderID, const UINT32 &InQuickChatID );
			// Cmd: Party chatting
			HRESULT PartyChatMessageRes( const HRESULT &InResult );
			// S2C: Party Chatting message event
			HRESULT PartyChatMessageS2CEvt( const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage );
			// Cmd: Join to a game
			HRESULT JoinGameRes( const HRESULT &InResult, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData );
			// S2C: Player Joined in the game
			HRESULT PlayerJoinedS2CEvt( const GameInsUID &InGameInsUID, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter );
			// Cmd: Leave Game
			HRESULT LeaveGameRes( const HRESULT &InResult );
			// S2C: Player left event
			HRESULT PlayerLeftS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InLeftPlayerID );
			// Cmd: Kick player
			HRESULT KickPlayerRes( const HRESULT &InResult );
			// S2C: Player kicked
			HRESULT PlayerKickedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InKickedPlayerID );
			// Cmd: Assign role + Game state reset
			HRESULT AssignRoleRes( const HRESULT &InResult );
			// S2C: Role assigned event
			HRESULT RoleAssignedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerRole &InRole );
			// Cmd: Send chatting message to the game
			HRESULT ChatMessageRes( const HRESULT &InResult );
			// S2C: Chatting message event 
			HRESULT ChatMessageS2CEvt( const PlayerID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage );
			// Cmd: Advance game
			HRESULT AdvanceGameRes( const HRESULT &InResult );
			// S2C: The game state is advanced
			HRESULT GameAdvancedS2CEvt( const GameInsUID &InGameInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay );
			// S2C: Game is ended
			HRESULT GameEndedS2CEvt( const GameInsUID &InGameInsUID, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney );
			// Cmd: Vote game advance
			HRESULT VoteGameAdvanceRes( const HRESULT &InResult );
			// S2C: GameAdvance is Voted
			HRESULT GameAdvanceVotedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InVoter );
			// Cmd: Vote
			HRESULT VoteRes( const HRESULT &InResult );
			// S2C: Player Voted
			HRESULT VotedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InVoter, const PlayerID &InVotedTarget );
			// S2C: Vote is ended
			HRESULT VoteEndS2CEvt( const GameInsUID &InGameInsUID, const Array<PlayerID>& InVoted );
			// S2C: Player Killed
			HRESULT PlayerKilledS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason );
			// S2C: Player Voted
			HRESULT PlayerRevealedS2CEvt( const GameInsUID &InGameInsUID, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason );
			// Cmd: Play again with the current players
			HRESULT GamePlayAgainRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );
			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			HRESULT GamePlayAgainS2CEvt( const PartyUID &InPartyUID, const PlayerID &InLeadPlayer );
			// Cmd: Player. reveal a player
			HRESULT GameRevealPlayerRes( const HRESULT &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );
			// Cmd: Player. revive himself
			HRESULT GamePlayerReviveRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );
			// S2C: Player is revived
			HRESULT GamePlayerRevivedS2CEvt( const PlayerID &InRevivedPlayerID );
			// Cmd: Player. reset ranking
			HRESULT GamePlayerResetRankRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );
			// Cmd: Request Game match
			HRESULT RequestGameMatchRes( const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );
			// S2C: Game matched
			HRESULT GameMatchedS2CEvt( const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData, const UINT32 &InStamina, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney );
			// S2C: Game match failed
			HRESULT GameMatchFailedS2CEvt( const HRESULT &InFailedReason );
			// S2C: Game matching started
			HRESULT GameMatchingStartedS2CEvt(  );
			// Cmd: Cancel Game match
			HRESULT CancelGameMatchRes( const HRESULT &InResult );
			// S2C: game matching canceled
			HRESULT GameMatchingCanceledS2CEvt(  );
			// Cmd: Buy shop item prepare
			HRESULT BuyShopItemPrepareRes( const HRESULT &InResult, const UINT32 &InShopItemID, const char* InPurchaseID );
			// Cmd: Buy shop item
			HRESULT BuyShopItemRes( const HRESULT &InResult, const UINT32 &InShopItemID );
			// Cmd: Give my stamina to other player
			HRESULT GiveStaminaRes( const HRESULT &InResult, const PlayerID &InTargetPlayer, const UINT64 &InTimeStamp );
			// Cmd: For debug, Change configue preset
			HRESULT SetPresetGameConfigIDRes( const HRESULT &InResult );
			// Cmd: For Debug
			HRESULT GainGameResourceRes( const HRESULT &InResult );

		}; // class NetSvrPolicyGame : public ISvrPolicyGame


	}; // namespace Policy
}; // namespace BR


