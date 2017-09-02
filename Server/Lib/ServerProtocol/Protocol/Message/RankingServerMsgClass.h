////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : RankingServer Message parser definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol/Protocol.h"
#include "Net/Message.h"
#include "Types/SFEngineTypedefs.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace RankingServer
		{
 			// Cmd: Add a player to ranking
			class AddPlayerCmd : public MessageBase
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
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				PlayerInformation m_PlayerInfo;
				uint64_t m_RankingScore;
			public:
				AddPlayerCmd()
					{}

				AddPlayerCmd( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const PlayerInformation& GetPlayerInfo() const	{ return m_PlayerInfo; };
				const uint64_t& GetRankingScore() const	{ return m_RankingScore; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerInformation &InPlayerInfo, const uint64_t &InRankingScore );

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
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
				uint32_t m_Ranking;
			public:
				AddPlayerRes()
					{}

				AddPlayerRes( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const uint32_t& GetRanking() const	{ return m_Ranking; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking );

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
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				PlayerID m_PlayerID;
			public:
				RemovePlayerCmd()
					{}

				RemovePlayerCmd( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );

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
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
			public:
				RemovePlayerRes()
					{}

				RemovePlayerRes( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

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
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				PlayerID m_PlayerID;
			public:
				GetPlayerRankingCmd()
					{}

				GetPlayerRankingCmd( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID );

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
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
				uint32_t m_Ranking;
			public:
				GetPlayerRankingRes()
					{}

				GetPlayerRankingRes( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const uint32_t& GetRanking() const	{ return m_Ranking; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InRanking );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class GetPlayerRankingRes : public MessageBase

			// Cmd: Update a player to ranking
			class UpdatePlayerScoreCmd : public MessageBase
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
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				uint64_t m_RankingScore;
				PlayerInformation m_PlayerInfo;
				uint16_t m_Count;
			public:
				UpdatePlayerScoreCmd()
					{}

				UpdatePlayerScoreCmd( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const uint64_t& GetRankingScore() const	{ return m_RankingScore; };
				const PlayerInformation& GetPlayerInfo() const	{ return m_PlayerInfo; };
				const uint16_t& GetCount() const	{ return m_Count; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint64_t &InRankingScore, const PlayerInformation &InPlayerInfo, const uint16_t &InCount );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class UpdatePlayerScoreCmd : public MessageBase

			class UpdatePlayerScoreRes : public MessageBase
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
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
				LinkedArray<TotalRankingPlayerInformation> m_Ranking;
			public:
				UpdatePlayerScoreRes()
					{}

				UpdatePlayerScoreRes( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const Array<TotalRankingPlayerInformation>& GetRanking() const	{ return m_Ranking; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class UpdatePlayerScoreRes : public MessageBase

			// Cmd: Get ranking list
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
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				RankingType m_RankingType;
				uint16_t m_BaseRanking;
				uint16_t m_Count;
			public:
				GetRankingCmd()
					{}

				GetRankingCmd( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const RankingType& GetRankingType() const	{ return m_RankingType; };
				const uint16_t& GetBaseRanking() const	{ return m_BaseRanking; };
				const uint16_t& GetCount() const	{ return m_Count; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const RankingType &InRankingType, const uint16_t &InBaseRanking, const uint16_t &InCount );

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
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
				LinkedArray<TotalRankingPlayerInformation> m_Ranking;
			public:
				GetRankingRes()
					{}

				GetRankingRes( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const Array<TotalRankingPlayerInformation>& GetRanking() const	{ return m_Ranking; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class GetRankingRes : public MessageBase

			// Cmd: Debug test
			class DebugPrintALLRankingCmd : public MessageBase
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
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				const char* m_FileName;
			public:
				DebugPrintALLRankingCmd()
				:m_FileName(nullptr)
					{}

				DebugPrintALLRankingCmd( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
				,m_FileName(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const char* GetFileName() const	{ return m_FileName; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const char* InFileName );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class DebugPrintALLRankingCmd : public MessageBase

			class DebugPrintALLRankingRes : public MessageBase
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
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				Result m_Result;
			public:
				DebugPrintALLRankingRes()
					{}

				DebugPrintALLRankingRes( MessageDataPtr &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult );

				Result OverrideRouteContextDestination( EntityUID to );

			}; // class DebugPrintALLRankingRes : public MessageBase





		}; // namespace RankingServer
	}; // namespace Message
}; // namespace SF


