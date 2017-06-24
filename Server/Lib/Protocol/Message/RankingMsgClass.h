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
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				PlayerID m_PlayerID;
				uint64_t m_RankingScore;
				LinkedArray<uint8_t> m_PlayerInfo;
			public:
				AddPlayerCmd()
					{}

				AddPlayerCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const uint64_t& GetRankingScore() const	{ return m_RankingScore; };
				const Array<uint8_t>& GetPlayerInfo() const	{ return m_PlayerInfo; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class AddPlayerCmd : public MessageBase

			class AddPlayerRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
				uint32_t m_Ranking;
			public:
				AddPlayerRes()
					{}

				AddPlayerRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const uint32_t& GetRanking() const	{ return m_Ranking; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class AddPlayerRes : public MessageBase

			// Cmd: Remove a player to ranking
			class RemovePlayerCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				PlayerID m_PlayerID;
			public:
				RemovePlayerCmd()
					{}

				RemovePlayerCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class RemovePlayerCmd : public MessageBase

			class RemovePlayerRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
			public:
				RemovePlayerRes()
					{}

				RemovePlayerRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class RemovePlayerRes : public MessageBase

			// Cmd: Remove a player to ranking
			class GetPlayerRankingCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				PlayerID m_PlayerID;
			public:
				GetPlayerRankingCmd()
					{}

				GetPlayerRankingCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class GetPlayerRankingCmd : public MessageBase

			class GetPlayerRankingRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
				uint32_t m_Ranking;
			public:
				GetPlayerRankingRes()
					{}

				GetPlayerRankingRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const uint32_t& GetRanking() const	{ return m_Ranking; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class GetPlayerRankingRes : public MessageBase

			// Cmd: Update a player to ranking
			class UpdatePlayerCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				PlayerID m_PlayerID;
				uint64_t m_RankingScore;
				LinkedArray<uint8_t> m_PlayerInfo;
			public:
				UpdatePlayerCmd()
					{}

				UpdatePlayerCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const uint64_t& GetRankingScore() const	{ return m_RankingScore; };
				const Array<uint8_t>& GetPlayerInfo() const	{ return m_PlayerInfo; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const uint64_t &InRankingScore, const Array<uint8_t>& InPlayerInfo );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class UpdatePlayerCmd : public MessageBase

			class UpdatePlayerRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
				uint32_t m_Ranking;
			public:
				UpdatePlayerRes()
					{}

				UpdatePlayerRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const uint32_t& GetRanking() const	{ return m_Ranking; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class UpdatePlayerRes : public MessageBase

			// Cmd: Remove a player to ranking
			class GetRankingCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				uint32_t m_From;
				uint32_t m_Count;
			public:
				GetRankingCmd()
					{}

				GetRankingCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint32_t& GetFrom() const	{ return m_From; };
				const uint32_t& GetCount() const	{ return m_Count; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint32_t &InFrom, const uint32_t &InCount );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class GetRankingCmd : public MessageBase

			class GetRankingRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
				uint32_t m_PlayerRanking;
			public:
				GetRankingRes()
					{}

				GetRankingRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const uint32_t& GetPlayerRanking() const	{ return m_PlayerRanking; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InPlayerRanking );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class GetRankingRes : public MessageBase





		}; // namespace Ranking
	}; // namespace Message
}; // namespace BR


