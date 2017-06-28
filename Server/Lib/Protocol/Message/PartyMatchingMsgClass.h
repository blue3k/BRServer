////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : PartyMatching Message parser definitions
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
 		namespace PartyMatching
		{
 			// S2C: 
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
				PlayerID GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint16_t m_RouteHopCount;
			public:
				PartyGameMatchedS2CEvt()
					{}

				PartyGameMatchedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class PartyGameMatchedS2CEvt : public MessageBase

			// S2C: 
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
				PlayerID GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint16_t m_RouteHopCount;
				PlayerID m_DestPlayerID;
				GameInsUID m_GameInsUID;
				PlayerRole m_RequestedRole;
			public:
				PlayerGameMatchedS2CEvt()
					{}

				PlayerGameMatchedS2CEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetDestPlayerID() const	{ return m_DestPlayerID; };
				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const PlayerRole& GetRequestedRole() const	{ return m_RequestedRole; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageTo( MessageData* pIMsg, class VariableMapBuilder& variableBuilder );

				static Result BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const GameInsUID &InGameInsUID, const PlayerRole &InRequestedRole );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class PlayerGameMatchedS2CEvt : public MessageBase





		}; // namespace PartyMatching
	}; // namespace Message
}; // namespace BR


