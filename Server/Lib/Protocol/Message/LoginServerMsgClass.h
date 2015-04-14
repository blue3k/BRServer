////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : LoginServer Message parser definitions
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
 		namespace LoginServer
		{
 			// Cmd: Notify user joind and see it's valid authticket instance
			class PlayerJoinedToGameServerCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_PlayerID;
				AuthTicket m_AuthTicket;
			public:
				PlayerJoinedToGameServerCmd()
					{}

				PlayerJoinedToGameServerCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const AuthTicket& GetAuthTicket() const	{ return m_AuthTicket; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const AuthTicket &InAuthTicket );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class PlayerJoinedToGameServerCmd : public MessageBase

			class PlayerJoinedToGameServerRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				PlayerJoinedToGameServerRes()
					{}

				PlayerJoinedToGameServerRes( MessageData* &pMsg )
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

			}; // class PlayerJoinedToGameServerRes : public MessageBase

			// Cmd: Kick logged in player
			class KickPlayerCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				PlayerID m_KickedPlayerID;
			public:
				KickPlayerCmd()
					{}

				KickPlayerCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const PlayerID& GetKickedPlayerID() const	{ return m_KickedPlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID );

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





		}; // namespace LoginServer
	}; // namespace Message
}; // namespace BR


