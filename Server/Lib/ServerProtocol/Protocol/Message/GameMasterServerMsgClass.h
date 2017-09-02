////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GameMasterServer Message parser definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol/Protocol.h"
#include "Net/Message.h"
#include "Container/VariableMapBuilder.h"



namespace SF
{
 	namespace Message
	{
 		namespace GameMasterServer
		{
 			// C2S: Player entered
			class PlayerEnteredC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 0,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				TransactionID GetTransactionID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				AccountID m_PlayerID;
				const char* m_PlayerName;
			public:
				PlayerEnteredC2SEvt()
				:m_PlayerName(nullptr)
					{}

				PlayerEnteredC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_PlayerName(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const AccountID& GetPlayerID() const	{ return m_PlayerID; };
				const char* GetPlayerName() const	{ return m_PlayerName; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const AccountID &InPlayerID, const char* InPlayerName );

			}; // class PlayerEnteredC2SEvt : public MessageBase

			// C2S: Player leaved
			class PlayerLeavedC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 0,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				TransactionID GetTransactionID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				AccountID m_PlayerID;
			public:
				PlayerLeavedC2SEvt()
					{}

				PlayerLeavedC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const AccountID& GetPlayerID() const	{ return m_PlayerID; };

				static Result TraceOut(MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder );
				static Result ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IMemoryManager& memoryManager, const AccountID &InPlayerID );

			}; // class PlayerLeavedC2SEvt : public MessageBase





		}; // namespace GameMasterServer
	}; // namespace Message
}; // namespace SF


