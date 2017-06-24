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
#include "Net/NetDef.h"
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
		Result NetPolicyLogin::LoginCmd( const GameID &InGameID, const char* InID, const char* InPassword )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::LoginCmd::BuildIMsg(pMsg, InGameID, InID, InPassword));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyLogin::LoginCmd( const GameID &InGameID, const char* InID, const char* InPassword )
		// Cmd: Login request with Facebook UID
		Result NetPolicyLogin::LoginByFacebookCmd( const GameID &InGameID, const uint64_t &InUID, const char* InFaceBookName, const char* InEMail, const char* InFacebookToken )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::LoginByFacebookCmd::BuildIMsg(pMsg, InGameID, InUID, InFaceBookName, InEMail, InFacebookToken));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyLogin::LoginByFacebookCmd( const GameID &InGameID, const uint64_t &InUID, const char* InFaceBookName, const char* InEMail, const char* InFacebookToken )
		// Cmd: Login request
		Result NetPolicyLogin::CreateRandomUserCmd( const GameID &InGameID, const char* InCellPhone )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::CreateRandomUserCmd::BuildIMsg(pMsg, InGameID, InCellPhone));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyLogin::CreateRandomUserCmd( const GameID &InGameID, const char* InCellPhone )


		// Cmd: Login request
		Result NetSvrPolicyLogin::LoginRes( const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::LoginRes::BuildIMsg(pMsg, InResult, InGameServerAddr, InGameServerAddrIPV4, InAccID, InTicket, InLoginEntityUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyLogin::LoginRes( const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID )
		// Cmd: Login request with Facebook UID
		Result NetSvrPolicyLogin::LoginByFacebookRes( const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::LoginByFacebookRes::BuildIMsg(pMsg, InResult, InGameServerAddr, InGameServerAddrIPV4, InAccID, InTicket, InLoginEntityUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyLogin::LoginByFacebookRes( const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID )
		// Cmd: Login request
		Result NetSvrPolicyLogin::CreateRandomUserRes( const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::CreateRandomUserRes::BuildIMsg(pMsg, InResult, InGameServerAddr, InGameServerAddrIPV4, InAccID, InTicket, InLoginEntityUID));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyLogin::CreateRandomUserRes( const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID )


	}; // namespace Policy
}; // namespace BR


