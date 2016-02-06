////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GamePartyManager Message parser definitions
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
 		namespace GamePartyManager
		{
 			// Cmd: Create a party instance
			class CreatePartyCmd : public MessageBase
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
				PlayerID GetPlayerID() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				UINT16 m_RouteHopCount;
				PlayerInformation m_Creator;
			public:
				CreatePartyCmd()
					{}

				CreatePartyCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerInformation& GetCreator() const	{ return m_Creator; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const UINT16 &InRouteHopCount, const PlayerInformation &InCreator );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class CreatePartyCmd : public MessageBase

			class CreatePartyRes : public MessageBase
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
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				TransactionID m_TransactionID;
				HRESULT m_Result;
			public:
				CreatePartyRes()
					{}

				CreatePartyRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const HRESULT &InResult );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class CreatePartyRes : public MessageBase

			// C2S: Party instance notify of deletion
			class PartyDeletedC2SEvt : public MessageBase
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
				UINT16 m_RouteHopCount;
			public:
				PartyDeletedC2SEvt()
					{}

				PartyDeletedC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class PartyDeletedC2SEvt : public MessageBase





		}; // namespace GamePartyManager
	}; // namespace Message
}; // namespace BR


