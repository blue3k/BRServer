////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameInstanceManager Message parser definitions
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
 		namespace GameInstanceManager
		{
 			// Cmd: Create a game instance
			class CreateGameCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
				UINT16 m_NumberOfBotPlayer;
				UINT16 m_MaxPlayer;
			public:
				CreateGameCmd()
					{}

				CreateGameCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const UINT16& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const UINT16& GetNumberOfBotPlayer() const	{ return m_NumberOfBotPlayer; };
				const UINT16& GetMaxPlayer() const	{ return m_MaxPlayer; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const UINT16 &InRouteHopCount, const UINT16 &InNumberOfBotPlayer, const UINT16 &InMaxPlayer );

				HRESULT OverrideRouteContextDestination( EntityUID to );

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class CreateGameCmd : public MessageBase

			class CreateGameRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				RouteContext m_RouteContext;
			public:
				CreateGameRes()
					{}

				CreateGameRes( MessageData* &pMsg )
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

				HRESULT OverrideRouteInformation( EntityUID to, UINT hopCount );

			}; // class CreateGameRes : public MessageBase

			// C2S: Game instance notify of deletion
			class GameDeletedC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				RouteContext m_RouteContext;
				UINT16 m_RouteHopCount;
			public:
				GameDeletedC2SEvt()
					{}

				GameDeletedC2SEvt( MessageData* &pMsg )
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

			}; // class GameDeletedC2SEvt : public MessageBase





		}; // namespace GameInstanceManager
	}; // namespace Message
}; // namespace BR


