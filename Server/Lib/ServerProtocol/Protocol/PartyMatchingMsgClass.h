////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) StormForge
// 
// Author : Generated
// 
// Description : PartyMatching Message parser definitions
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
 		namespace PartyMatching
		{
 			// S2C: Event message will be sent from matching srvice when the pary game mathcing has done.
			class PartyGameMatchedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetTransactionID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext{};
				uint16_t m_RouteHopCount{};
			public:
				PartyGameMatchedS2CEvt()
					{}

				PartyGameMatchedS2CEvt( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class PartyGameMatchedS2CEvt : public MessageBase

			// S2C: Event message will be sent when a player game matching has successfully finished.
			class PlayerGameMatchedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint64_t GetTransactionID() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext{};
				uint16_t m_RouteHopCount{};
				PlayerID m_DestPlayerID{};
				uint64_t m_GameInsUID{};
				uint8_t m_RequestedRole{};
			public:
				PlayerGameMatchedS2CEvt()
					{}

				PlayerGameMatchedS2CEvt( const MessageDataPtr &pMsg )
					: MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetDestPlayerID() const	{ return m_DestPlayerID; };
				const uint64_t& GetGameInsUID() const	{ return m_GameInsUID; };
				const uint8_t& GetRequestedRole() const	{ return m_RequestedRole; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage(const MessageData* pIMsg);
				static Result ParseMessageToMessageBase(IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMsgBase);

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const uint64_t &InGameInsUID, const uint8_t &InRequestedRole );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class PlayerGameMatchedS2CEvt : public MessageBase





		}; // namespace PartyMatching
	}; // namespace Message
}; // namespace SF


