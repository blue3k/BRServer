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
			private:
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
			public:
				PartyGameMatchedS2CEvt()
					{}

				PartyGameMatchedS2CEvt( MessageData* &pMsg )
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

			}; // class PartyGameMatchedS2CEvt : public MessageBase

			// S2C: 
			class PlayerGameMatchedS2CEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
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
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetDestPlayerID() const	{ return m_DestPlayerID; };
				const GameInsUID& GetGameInsUID() const	{ return m_GameInsUID; };
				const PlayerRole& GetRequestedRole() const	{ return m_RequestedRole; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const PlayerID &InDestPlayerID, const GameInsUID &InGameInsUID, const PlayerRole &InRequestedRole );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class PlayerGameMatchedS2CEvt : public MessageBase





		}; // namespace PartyMatching
	}; // namespace Message
}; // namespace BR


