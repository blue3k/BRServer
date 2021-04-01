////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) StormForge
// 
// Author : Generated
// 
// Description : GameInstanceManager Message parser definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol/SFProtocol.h"
#include "Net/SFMessage.h"
#include "Types/SFEngineTypedefs.h"
#include "Variable/SFVariableTable.h"
#include "Container/SFArray.h"
#include "Actor/Movement/SFActorMovement.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace GameInstanceManager
		{
 			// Cmd: Create a game instance
			class CreateGameInstanceCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext{};
				uint64_t m_TransactionID{};
				uint16_t m_RouteHopCount{};
				uint32_t m_ZoneTableId{};
				ArrayView<uint8_t> m_AttributesRaw;
				mutable bool m_AttributesHasParsed = false;
				mutable VariableTable m_Attributes;
			public:
				CreateGameInstanceCmd()
					{}

				CreateGameInstanceCmd( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const uint32_t& GetZoneTableId() const	{ return m_ZoneTableId; };
				const Array<uint8_t>& GetAttributesRaw() const	{ return m_AttributesRaw; };
				const VariableTable& GetAttributes() const;

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InZoneTableId, const Array<uint8_t>& InAttributes );
				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InZoneTableId, const VariableTable &InAttributes );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class CreateGameInstanceCmd : public MessageBase

			class CreateGameInstanceRes : public MessageBase
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
				RouteContext m_RouteContext{};
				uint64_t m_TransactionID{};
				Result m_Result{};
			public:
				CreateGameInstanceRes()
					{}

				CreateGameInstanceRes( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class CreateGameInstanceRes : public MessageBase





		}; // namespace GameInstanceManager
	}; // namespace Message
}; // namespace SF


