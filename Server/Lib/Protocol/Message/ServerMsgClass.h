////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Server Message parser definitions
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
 		namespace Server
		{
 			// Cmd: 
			class GenericFailureCmd : public MessageBase
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
			public:
				GenericFailureCmd()
					{}

				GenericFailureCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const TransactionID &InTransactionID );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class GenericFailureCmd : public MessageBase

			class GenericFailureRes : public MessageBase
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
				GenericFailureRes()
					{}

				GenericFailureRes( MessageData* &pMsg )
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

			}; // class GenericFailureRes : public MessageBase

			// C2S: Server Started or Connected
			class ServerConnectedC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				ServiceInformation m_ClusterManagerServiceInformation;
				UINT32 m_StartUpTime;
				NetAddress m_PrivateAddress;
			public:
				ServerConnectedC2SEvt()
					{}

				ServerConnectedC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const ServiceInformation& GetClusterManagerServiceInformation() const	{ return m_ClusterManagerServiceInformation; };
				const UINT32& GetStartUpTime() const	{ return m_StartUpTime; };
				const NetAddress& GetPrivateAddress() const	{ return m_PrivateAddress; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const ServiceInformation &InClusterManagerServiceInformation, const UINT32 &InStartUpTime, const NetAddress &InPrivateAddress );

				HRESULT OverrideRouteContextDestination( EntityUID to );

			}; // class ServerConnectedC2SEvt : public MessageBase





		}; // namespace Server
	}; // namespace Message
}; // namespace BR


