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
			private:
				Context m_Context;
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

				const Context& GetContext() const	{ return m_Context; };
				const EntityID& GetLocalEntID() const	{ return m_LocalEntID; };
				const char* GetEntName() const	{ return m_EntName; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const EntityID &InLocalEntID, const char* InEntName );

			}; // class RegisterEntityCmd : public MessageBase

			class RegisterEntityRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				EntityUID m_EntUID;
			public:
				RegisterEntityRes()
					{}

				RegisterEntityRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const EntityUID& GetEntUID() const	{ return m_EntUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const EntityUID &InEntUID );

			}; // class RegisterEntityRes : public MessageBase

			// Cmd: Find Entity
			class UnregisterEntityCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				EntityUID m_EntUID;
			public:
				UnregisterEntityCmd()
					{}

				UnregisterEntityCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const EntityUID& GetEntUID() const	{ return m_EntUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const EntityUID &InEntUID );

			}; // class UnregisterEntityCmd : public MessageBase

			class UnregisterEntityRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
			public:
				UnregisterEntityRes()
					{}

				UnregisterEntityRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult );

			}; // class UnregisterEntityRes : public MessageBase

			// Cmd: Find Entity
			class FindEntityCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				EntityID m_LocalEntID;
			public:
				FindEntityCmd()
					{}

				FindEntityCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const EntityID& GetLocalEntID() const	{ return m_LocalEntID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const EntityID &InLocalEntID );

			}; // class FindEntityCmd : public MessageBase

			class FindEntityRes : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				Context m_Context;
				HRESULT m_Result;
				EntityUID m_EntUID;
			public:
				FindEntityRes()
					{}

				FindEntityRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const Context& GetContext() const	{ return m_Context; };
				const HRESULT& GetResult() const	{ return m_Result; };
				const EntityUID& GetEntUID() const	{ return m_EntUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const EntityUID &InEntUID );

			}; // class FindEntityRes : public MessageBase





		}; // namespace EntityServer
	}; // namespace Message
}; // namespace BR


