////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Login Message parser definitions
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
 		namespace Login
		{
 			// Cmd: Login request
			class LoginCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				GameID m_GameID;
				const char* m_ID;
				const char* m_Password;
			public:
				LoginCmd()
				:m_ID(nullptr)
				,m_Password(nullptr)
					{}

				LoginCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_ID(nullptr)
				,m_Password(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameID& GetGameID() const	{ return m_GameID; };
				const char* GetID() const	{ return m_ID; };
				const char* GetPassword() const	{ return m_Password; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameID &InGameID, const char* InID, const char* InPassword );

			}; // class LoginCmd : public MessageBase

			class LoginRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				HRESULT m_Result;
				NetAddress m_GameServerAddr;
				NetAddress m_GameServerAddrIPV4;
				AccountID m_AccID;
				AuthTicket m_Ticket;
				UINT64 m_LoginEntityUID;
			public:
				LoginRes()
					{}

				LoginRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const NetAddress& GetGameServerAddr() const	{ return m_GameServerAddr; };
				const NetAddress& GetGameServerAddrIPV4() const	{ return m_GameServerAddrIPV4; };
				const AccountID& GetAccID() const	{ return m_AccID; };
				const AuthTicket& GetTicket() const	{ return m_Ticket; };
				const UINT64& GetLoginEntityUID() const	{ return m_LoginEntityUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID );

			}; // class LoginRes : public MessageBase

			// Cmd: Login request with Facebook UID
			class LoginByFacebookCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				GameID m_GameID;
				UINT64 m_UID;
				const char* m_FaceBookName;
				const char* m_EMail;
				const char* m_FacebookToken;
			public:
				LoginByFacebookCmd()
				:m_FaceBookName(nullptr)
				,m_EMail(nullptr)
				,m_FacebookToken(nullptr)
					{}

				LoginByFacebookCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_FaceBookName(nullptr)
				,m_EMail(nullptr)
				,m_FacebookToken(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameID& GetGameID() const	{ return m_GameID; };
				const UINT64& GetUID() const	{ return m_UID; };
				const char* GetFaceBookName() const	{ return m_FaceBookName; };
				const char* GetEMail() const	{ return m_EMail; };
				const char* GetFacebookToken() const	{ return m_FacebookToken; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameID &InGameID, const UINT64 &InUID, const char* InFaceBookName, const char* InEMail, const char* InFacebookToken );

			}; // class LoginByFacebookCmd : public MessageBase

			class LoginByFacebookRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				HRESULT m_Result;
				NetAddress m_GameServerAddr;
				NetAddress m_GameServerAddrIPV4;
				AccountID m_AccID;
				AuthTicket m_Ticket;
				UINT64 m_LoginEntityUID;
			public:
				LoginByFacebookRes()
					{}

				LoginByFacebookRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const NetAddress& GetGameServerAddr() const	{ return m_GameServerAddr; };
				const NetAddress& GetGameServerAddrIPV4() const	{ return m_GameServerAddrIPV4; };
				const AccountID& GetAccID() const	{ return m_AccID; };
				const AuthTicket& GetTicket() const	{ return m_Ticket; };
				const UINT64& GetLoginEntityUID() const	{ return m_LoginEntityUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID );

			}; // class LoginByFacebookRes : public MessageBase

			// Cmd: Login request
			class CreateRandomUserCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				GameID m_GameID;
				const char* m_CellPhone;
			public:
				CreateRandomUserCmd()
				:m_CellPhone(nullptr)
					{}

				CreateRandomUserCmd( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_CellPhone(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const GameID& GetGameID() const	{ return m_GameID; };
				const char* GetCellPhone() const	{ return m_CellPhone; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const GameID &InGameID, const char* InCellPhone );

			}; // class CreateRandomUserCmd : public MessageBase

			class CreateRandomUserRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 0,
					HasRouteContext = 0,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				PlayerID GetPlayerID() { return 0; }
				TransactionID GetTransactionID() { return 0; }
				RouteContext GetRouteContext() { return 0; }
				UINT32 GetRouteHopCount() { return 0; }
				PlayerID GetSender() { return 0; }
			private:
				HRESULT m_Result;
				NetAddress m_GameServerAddr;
				NetAddress m_GameServerAddrIPV4;
				AccountID m_AccID;
				AuthTicket m_Ticket;
				UINT64 m_LoginEntityUID;
			public:
				CreateRandomUserRes()
					{}

				CreateRandomUserRes( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const HRESULT& GetResult() const	{ return m_Result; };
				const NetAddress& GetGameServerAddr() const	{ return m_GameServerAddr; };
				const NetAddress& GetGameServerAddrIPV4() const	{ return m_GameServerAddrIPV4; };
				const AccountID& GetAccID() const	{ return m_AccID; };
				const AuthTicket& GetTicket() const	{ return m_Ticket; };
				const UINT64& GetLoginEntityUID() const	{ return m_LoginEntityUID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID );

			}; // class CreateRandomUserRes : public MessageBase





		}; // namespace Login
	}; // namespace Message
}; // namespace BR


