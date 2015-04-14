////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Login Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"
#include "Protocol/Policy/LoginIPolicy.h"
#include "Protocol/Policy/LoginNetPolicy.h"
#include "Protocol/Message/LoginMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Policy
	{
 		// Cmd: Login request
		HRESULT NetPolicyLogin::LoginCmd( const GameID &InGameID, const char* InID, const char* InPassword )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::LoginCmd::BuildIMsg(pMsg, InGameID, InID, InPassword));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyLogin::LoginCmd( const GameID &InGameID, const char* InID, const char* InPassword )
		// Cmd: Login request with Facebook UID
		HRESULT NetPolicyLogin::LoginByFacebookCmd( const GameID &InGameID, const UINT64 &InUID, const char* InFaceBookName, const char* InEMail, const char* InFacebookToken )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::LoginByFacebookCmd::BuildIMsg(pMsg, InGameID, InUID, InFaceBookName, InEMail, InFacebookToken));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetPolicyLogin::LoginByFacebookCmd( const GameID &InGameID, const UINT64 &InUID, const char* InFaceBookName, const char* InEMail, const char* InFacebookToken )


		// Cmd: Login request
		HRESULT NetSvrPolicyLogin::LoginRes( const HRESULT &InResult, const NetAddress &InGameServerAddr, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::LoginRes::BuildIMsg(pMsg, InResult, InGameServerAddr, InAccID, InTicket, InLoginEntityUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyLogin::LoginRes( const HRESULT &InResult, const NetAddress &InGameServerAddr, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )
		// Cmd: Login request with Facebook UID
		HRESULT NetSvrPolicyLogin::LoginByFacebookRes( const HRESULT &InResult, const NetAddress &InGameServerAddr, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )
		{
 			HRESULT hr = S_OK;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::LoginByFacebookRes::BuildIMsg(pMsg, InResult, InGameServerAddr, InAccID, InTicket, InLoginEntityUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // HRESULT NetSvrPolicyLogin::LoginByFacebookRes( const HRESULT &InResult, const NetAddress &InGameServerAddr, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )


	}; // namespace Policy
}; // namespace BR


