////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Ranking Message parser definitions
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
 		namespace Ranking
		{
 			// Cmd: Add a player to ranking
			class AddPlayerCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
				UINT64 m_RankingScore;
				LinkedArray<BYTE> m_PlayerInfo;
			public:
				AddPlayerCmd()
					{}

				AddPlayerCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const UINT64& GetRankingScore() const	{ return m_RankingScore; };
				const Array<BYTE>& GetPlayerInfo() const	{ return m_PlayerInfo; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class AddPlayerCmd : public MessageBase

			class AddPlayerRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
				UINT32 m_Ranking;
			public:
				AddPlayerRes()
					{}

				AddPlayerRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT32& GetRanking() const	{ return m_Ranking; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InRanking );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class AddPlayerRes : public MessageBase

			// Cmd: Remove a player to ranking
			class RemovePlayerCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
			public:
				RemovePlayerCmd()
					{}

				RemovePlayerCmd( MessageData* &pMsg )
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

			}; // class RemovePlayerCmd : public MessageBase

			class RemovePlayerRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				RemovePlayerRes()
					{}

				RemovePlayerRes( MessageData* &pMsg )
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

			}; // class RemovePlayerRes : public MessageBase

			// Cmd: Remove a player to ranking
			class GetPlayerRankingCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
			public:
				GetPlayerRankingCmd()
					{}

				GetPlayerRankingCmd( MessageData* &pMsg )
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

			}; // class GetPlayerRankingCmd : public MessageBase

			class GetPlayerRankingRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
				UINT32 m_Ranking;
			public:
				GetPlayerRankingRes()
					{}

				GetPlayerRankingRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT32& GetRanking() const	{ return m_Ranking; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InRanking );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class GetPlayerRankingRes : public MessageBase

			// Cmd: Update a player to ranking
			class UpdatePlayerCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
				UINT64 m_RankingScore;
				LinkedArray<BYTE> m_PlayerInfo;
			public:
				UpdatePlayerCmd()
					{}

				UpdatePlayerCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const UINT64& GetRankingScore() const	{ return m_RankingScore; };
				const Array<BYTE>& GetPlayerInfo() const	{ return m_PlayerInfo; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const UINT64 &InRankingScore, const Array<BYTE>& InPlayerInfo );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class UpdatePlayerCmd : public MessageBase

			class UpdatePlayerRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
				UINT32 m_Ranking;
			public:
				UpdatePlayerRes()
					{}

				UpdatePlayerRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT32& GetRanking() const	{ return m_Ranking; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InRanking );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class UpdatePlayerRes : public MessageBase

			// Cmd: Remove a player to ranking
			class GetRankingCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				UINT32 m_From;
				UINT32 m_Count;
			public:
				GetRankingCmd()
					{}

				GetRankingCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT32& GetFrom() const	{ return m_From; };
				const UINT32& GetCount() const	{ return m_Count; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT32 &InFrom, const UINT32 &InCount );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class GetRankingCmd : public MessageBase

			class GetRankingRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
				UINT32 m_PlayerRanking;
			public:
				GetRankingRes()
					{}

				GetRankingRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT32& GetPlayerRanking() const	{ return m_PlayerRanking; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InPlayerRanking );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class GetRankingRes : public MessageBase





		}; // namespace Ranking
	}; // namespace Message
}; // namespace BR


