////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : EntityServer Message parser definitions
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
 		namespace EntityServer
		{
 			// Cmd: Register entity
			class RegisterEntityCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				TransactionID m_TransactionID;
				EntityID m_LocalEntID;
				const char* m_EntName;
			public:
				RegisterEntityCmd()
				:m_EntName(nullptr)
					{}

				RegisterEntityCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_EntName(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const EntityID& GetLocalEntID() const	{ return m_LocalEntID; };
				const char* GetEntName() const	{ return m_EntName; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseIMsg( IN MessageData* pIMsg );

				static Result BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName );

			}; // class RegisterEntityCmd : public MessageBase

			class RegisterEntityRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				TransactionID m_TransactionID;
				Result m_Result;
				EntityUID m_EntUID;
			public:
				RegisterEntityRes()
					{}

				RegisterEntityRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const EntityUID& GetEntUID() const	{ return m_EntUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseIMsg( IN MessageData* pIMsg );

				static Result BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InEntUID );

			}; // class RegisterEntityRes : public MessageBase

			// Cmd: Find Entity
			class UnregisterEntityCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				TransactionID m_TransactionID;
				EntityUID m_EntUID;
			public:
				UnregisterEntityCmd()
					{}

				UnregisterEntityCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const EntityUID& GetEntUID() const	{ return m_EntUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseIMsg( IN MessageData* pIMsg );

				static Result BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const EntityUID &InEntUID );

			}; // class UnregisterEntityCmd : public MessageBase

			class UnregisterEntityRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				TransactionID m_TransactionID;
				Result m_Result;
			public:
				UnregisterEntityRes()
					{}

				UnregisterEntityRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseIMsg( IN MessageData* pIMsg );

				static Result BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const Result &InResult );

			}; // class UnregisterEntityRes : public MessageBase

			// Cmd: Find Entity
			class FindEntityCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				TransactionID m_TransactionID;
				EntityID m_LocalEntID;
			public:
				FindEntityCmd()
					{}

				FindEntityCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const EntityID& GetLocalEntID() const	{ return m_LocalEntID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseIMsg( IN MessageData* pIMsg );

				static Result BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const EntityID &InLocalEntID );

			}; // class FindEntityCmd : public MessageBase

			class FindEntityRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				TransactionID m_TransactionID;
				Result m_Result;
				EntityUID m_EntUID;
			public:
				FindEntityRes()
					{}

				FindEntityRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const TransactionID& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const EntityUID& GetEntUID() const	{ return m_EntUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual Result ParseIMsg( IN MessageData* pIMsg );

				static Result BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InEntUID );

			}; // class FindEntityRes : public MessageBase





		}; // namespace EntityServer
	}; // namespace Message
}; // namespace BR


