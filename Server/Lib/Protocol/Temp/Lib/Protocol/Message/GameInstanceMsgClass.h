////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameInstance Message parser definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/PolicyID.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"



namespace BR
{
 	namespace Message
	{
 		namespace GameInstance
		{
 			// C2S: Game instance deletion
			class DeleteGameC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
			public:
				DeleteGameC2SEvt()
					{}

				DeleteGameC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class DeleteGameC2SEvt : public MessageBase

			// Cmd: Join Game
			class JoinGameCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerInformation m_Player;
				AuthTicket m_Ticket;
				PlayerRole m_RequestedRole;
			public:
				JoinGameCmd()
					{}

				JoinGameCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerInformation& GetPlayer() const	{ return m_Player; };
				const AuthTicket& GetTicket() const	{ return m_Ticket; };
				const PlayerRole& GetRequestedRole() const	{ return m_RequestedRole; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const PlayerRole &InRequestedRole );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class JoinGameCmd : public MessageBase

			class JoinGameRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
				NetAddress m_GameInsSvr;
				UINT32 m_TimeStamp;
				GameStateID m_GameState;
				UINT8 m_Day;
				UINT8 m_MaxPlayer;
				UINT8 m_PlayerIndex;
				BYTE m_PlayerCharacter;
				PlayerRole m_Role;
				UINT8 m_Dead;
				BYTE m_IsNewJoin;
				LinkedArray<BYTE> m_ChatHistoryData;
				LinkedArray<BYTE> m_GameLogData;
			public:
				JoinGameRes()
					{}

				JoinGameRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const NetAddress& GetGameInsSvr() const	{ return m_GameInsSvr; };
				const UINT32& GetTimeStamp() const	{ return m_TimeStamp; };
				const GameStateID& GetGameState() const	{ return m_GameState; };
				const UINT8& GetDay() const	{ return m_Day; };
				const UINT8& GetMaxPlayer() const	{ return m_MaxPlayer; };
				const UINT8& GetPlayerIndex() const	{ return m_PlayerIndex; };
				const BYTE& GetPlayerCharacter() const	{ return m_PlayerCharacter; };
				const PlayerRole& GetRole() const	{ return m_Role; };
				const UINT8& GetDead() const	{ return m_Dead; };
				const BYTE& GetIsNewJoin() const	{ return m_IsNewJoin; };
				const Array<BYTE>& GetChatHistoryData() const	{ return m_ChatHistoryData; };
				const Array<BYTE>& GetGameLogData() const	{ return m_GameLogData; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const NetAddress &InGameInsSvr, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const BYTE &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const BYTE &InIsNewJoin, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class JoinGameRes : public MessageBase

			// S2C: Player Joined
			class PlayerJoinedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerInformation m_JoinedPlayer;
				PlayerRole m_JoinedPlayerRole;
				UINT8 m_JoinedPlayerDead;
				UINT8 m_JoinedPlayerIndex;
				UINT8 m_JoinedPlayerCharacter;
			public:
				PlayerJoinedS2CEvt()
					{}

				PlayerJoinedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerInformation& GetJoinedPlayer() const	{ return m_JoinedPlayer; };
				const PlayerRole& GetJoinedPlayerRole() const	{ return m_JoinedPlayerRole; };
				const UINT8& GetJoinedPlayerDead() const	{ return m_JoinedPlayerDead; };
				const UINT8& GetJoinedPlayerIndex() const	{ return m_JoinedPlayerIndex; };
				const UINT8& GetJoinedPlayerCharacter() const	{ return m_JoinedPlayerCharacter; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class PlayerJoinedS2CEvt : public MessageBase

			// C2S: Change configue preset
			class SetConfigPresetC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				UINT32 m_PresetID;
			public:
				SetConfigPresetC2SEvt()
					{}

				SetConfigPresetC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT32& GetPresetID() const	{ return m_PresetID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT32 &InPresetID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class SetConfigPresetC2SEvt : public MessageBase

			// Cmd: Leave Game
			class LeaveGameCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
			public:
				LeaveGameCmd()
					{}

				LeaveGameCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class LeaveGameCmd : public MessageBase

			class LeaveGameRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				LeaveGameRes()
					{}

				LeaveGameRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class LeaveGameRes : public MessageBase

			// S2C: Player left
			class PlayerLeftS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerID m_LeftPlayerID;
			public:
				PlayerLeftS2CEvt()
					{}

				PlayerLeftS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetLeftPlayerID() const	{ return m_LeftPlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class PlayerLeftS2CEvt : public MessageBase

			// Cmd: Kick player
			class KickPlayerCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
				PlayerID m_PlayerToKick;
			public:
				KickPlayerCmd()
					{}

				KickPlayerCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const PlayerID& GetPlayerToKick() const	{ return m_PlayerToKick; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class KickPlayerCmd : public MessageBase

			class KickPlayerRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				KickPlayerRes()
					{}

				KickPlayerRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class KickPlayerRes : public MessageBase

			// S2C: Player kicked
			class PlayerKickedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerID m_KickedPlayerID;
			public:
				PlayerKickedS2CEvt()
					{}

				PlayerKickedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetKickedPlayerID() const	{ return m_KickedPlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class PlayerKickedS2CEvt : public MessageBase

			// Cmd: Assign role
			class AssignRoleCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
			public:
				AssignRoleCmd()
					{}

				AssignRoleCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class AssignRoleCmd : public MessageBase

			class AssignRoleRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				AssignRoleRes()
					{}

				AssignRoleRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class AssignRoleRes : public MessageBase

			// S2C: Assign role
			class RoleAssignedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerRole m_Role;
			public:
				RoleAssignedS2CEvt()
					{}

				RoleAssignedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerRole& GetRole() const	{ return m_Role; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerRole &InRole );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class RoleAssignedS2CEvt : public MessageBase

			// C2S: Chatting message
			class ChatMessageC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
				PlayerRole m_Role;
				const char* m_ChatMessage;
			public:
				ChatMessageC2SEvt()
				:m_ChatMessage(nullptr)
					{}

				ChatMessageC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_ChatMessage(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const PlayerRole& GetRole() const	{ return m_Role; };
				const char* GetChatMessage() const	{ return m_ChatMessage; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerRole &InRole, const char* InChatMessage );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class ChatMessageC2SEvt : public MessageBase

			// Cmd: Advance game
			class AdvanceGameCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
			public:
				AdvanceGameCmd()
					{}

				AdvanceGameCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class AdvanceGameCmd : public MessageBase

			class AdvanceGameRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				AdvanceGameRes()
					{}

				AdvanceGameRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class AdvanceGameRes : public MessageBase

			// S2C: The game state is advanced
			class GameAdvancedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				UINT32 m_TimeStamp;
				GameStateID m_GameState;
				UINT8 m_Day;
			public:
				GameAdvancedS2CEvt()
					{}

				GameAdvancedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT32& GetTimeStamp() const	{ return m_TimeStamp; };
				const GameStateID& GetGameState() const	{ return m_GameState; };
				const UINT8& GetDay() const	{ return m_Day; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class GameAdvancedS2CEvt : public MessageBase

			// S2C: Game is ended
			class GameEndedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				GameWinner m_Winner;
				UINT32 m_GainedExp;
				UINT32 m_GainedGameMoney;
				PlayerRole m_PlayedRole;
				BYTE m_IsWon;
			public:
				GameEndedS2CEvt()
					{}

				GameEndedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const GameWinner& GetWinner() const	{ return m_Winner; };
				const UINT32& GetGainedExp() const	{ return m_GainedExp; };
				const UINT32& GetGainedGameMoney() const	{ return m_GainedGameMoney; };
				const PlayerRole& GetPlayedRole() const	{ return m_PlayedRole; };
				const BYTE& GetIsWon() const	{ return m_IsWon; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney, const PlayerRole &InPlayedRole, const BYTE &InIsWon );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class GameEndedS2CEvt : public MessageBase

			// Cmd: *Vote game advance
			class VoteGameAdvanceCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
			public:
				VoteGameAdvanceCmd()
					{}

				VoteGameAdvanceCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class VoteGameAdvanceCmd : public MessageBase

			class VoteGameAdvanceRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				VoteGameAdvanceRes()
					{}

				VoteGameAdvanceRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class VoteGameAdvanceRes : public MessageBase

			// S2C: *GameAdvance is Voted
			class GameAdvanceVotedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerID m_Voter;
			public:
				GameAdvanceVotedS2CEvt()
					{}

				GameAdvanceVotedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetVoter() const	{ return m_Voter; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InVoter );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class GameAdvanceVotedS2CEvt : public MessageBase

			// Cmd: Assign role
			class VoteCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
				PlayerID m_VoteTarget;
				UINT32 m_ActionSerial;
			public:
				VoteCmd()
					{}

				VoteCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const PlayerID& GetVoteTarget() const	{ return m_VoteTarget; };
				const UINT32& GetActionSerial() const	{ return m_ActionSerial; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const UINT32 &InActionSerial );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class VoteCmd : public MessageBase

			class VoteRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				VoteRes()
					{}

				VoteRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class VoteRes : public MessageBase

			// S2C: Player Voted
			class VotedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerID m_Voter;
				PlayerID m_VotedTarget;
			public:
				VotedS2CEvt()
					{}

				VotedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetVoter() const	{ return m_Voter; };
				const PlayerID& GetVotedTarget() const	{ return m_VotedTarget; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class VotedS2CEvt : public MessageBase

			// S2C: Player Voted
			class VoteEndS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				LinkedArray<PlayerID> m_Voted;
			public:
				VoteEndS2CEvt()
					{}

				VoteEndS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const Array<PlayerID>& GetVoted() const	{ return m_Voted; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Array<PlayerID>& InVoted );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class VoteEndS2CEvt : public MessageBase

			// S2C: Player Voted
			class PlayerRevealedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerID m_RevealedPlayerID;
				PlayerRole m_Role;
				PlayerRevealedReason m_Reason;
			public:
				PlayerRevealedS2CEvt()
					{}

				PlayerRevealedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetRevealedPlayerID() const	{ return m_RevealedPlayerID; };
				const PlayerRole& GetRole() const	{ return m_Role; };
				const PlayerRevealedReason& GetReason() const	{ return m_Reason; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class PlayerRevealedS2CEvt : public MessageBase

			// S2C: Player Killed
			class PlayerKilledS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerID m_KilledPlayer;
				PlayerKilledReason m_Reason;
			public:
				PlayerKilledS2CEvt()
					{}

				PlayerKilledS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetKilledPlayer() const	{ return m_KilledPlayer; };
				const PlayerKilledReason& GetReason() const	{ return m_Reason; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class PlayerKilledS2CEvt : public MessageBase

			// Cmd: Play again with the current players
			class GamePlayAgainCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_LeadPlayer;
				PartyUID m_PartyUID;
			public:
				GamePlayAgainCmd()
					{}

				GamePlayAgainCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetLeadPlayer() const	{ return m_LeadPlayer; };
				const PartyUID& GetPartyUID() const	{ return m_PartyUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InLeadPlayer, const PartyUID &InPartyUID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class GamePlayAgainCmd : public MessageBase

			class GamePlayAgainRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
				UINT32 m_ReplayMemberCount;
			public:
				GamePlayAgainRes()
					{}

				GamePlayAgainRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT32& GetReplayMemberCount() const	{ return m_ReplayMemberCount; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InReplayMemberCount );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class GamePlayAgainRes : public MessageBase

			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			class GamePlayAgainS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerID m_TargetPlayer;
				PartyUID m_PartyUID;
				PlayerID m_LeadPlayer;
			public:
				GamePlayAgainS2CEvt()
					{}

				GamePlayAgainS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetTargetPlayer() const	{ return m_TargetPlayer; };
				const PartyUID& GetPartyUID() const	{ return m_PartyUID; };
				const PlayerID& GetLeadPlayer() const	{ return m_LeadPlayer; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const PartyUID &InPartyUID, const PlayerID &InLeadPlayer );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class GamePlayAgainS2CEvt : public MessageBase

			// Cmd: Player. revive himself
			class GameRevealPlayerCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
				LinkedArray<PlayerID> m_TargetPlayerID;
			public:
				GameRevealPlayerCmd()
					{}

				GameRevealPlayerCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const Array<PlayerID>& GetTargetPlayerID() const	{ return m_TargetPlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class GameRevealPlayerCmd : public MessageBase

			class GameRevealPlayerRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
				LinkedArray<PlayerID> m_RevealedPlayerID;
				LinkedArray<PlayerRole> m_RevealedRole;
			public:
				GameRevealPlayerRes()
					{}

				GameRevealPlayerRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const Array<PlayerID>& GetRevealedPlayerID() const	{ return m_RevealedPlayerID; };
				const Array<PlayerRole>& GetRevealedRole() const	{ return m_RevealedRole; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class GameRevealPlayerRes : public MessageBase

			// Cmd: Player. revive himself
			class GamePlayerReviveCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
			public:
				GamePlayerReviveCmd()
					{}

				GamePlayerReviveCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class GamePlayerReviveCmd : public MessageBase

			class GamePlayerReviveRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				GamePlayerReviveRes()
					{}

				GamePlayerReviveRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class GamePlayerReviveRes : public MessageBase

			// S2C: Player is revived
			class GamePlayerRevivedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				PlayerID m_RevivedPlayerID;
			public:
				GamePlayerRevivedS2CEvt()
					{}

				GamePlayerRevivedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetRevivedPlayerID() const	{ return m_RevivedPlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class GamePlayerRevivedS2CEvt : public MessageBase





		}; // namespace GameInstance
	}; // namespace Message
}; // namespace BR


