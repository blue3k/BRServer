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
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Net/Message.h"
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
		// Cmd: Update my score and Get Ranking list
		Result NetPolicyLogin::UpdateMyScoreCmd( const uint64_t &InRankingScore, const RankingType &InRankingType, const uint16_t &InCount )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::UpdateMyScoreCmd::BuildIMsg(pMsg, InRankingScore, InRankingType, InCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyLogin::UpdateMyScoreCmd( const uint64_t &InRankingScore, const RankingType &InRankingType, const uint16_t &InCount )
		// Cmd: Get Ranking lise
		Result NetPolicyLogin::GetRankingListCmd( const RankingType &InRankingType, const uint8_t &InBaseRanking, const uint8_t &InCount )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::GetRankingListCmd::BuildIMsg(pMsg, InRankingType, InBaseRanking, InCount));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyLogin::GetRankingListCmd( const RankingType &InRankingType, const uint8_t &InBaseRanking, const uint8_t &InCount )
		// Cmd: For network test
		Result NetPolicyLogin::DataTestCmd( const Array<uint8_t>& InTestData )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::DataTestCmd::BuildIMsg(pMsg, InTestData));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyLogin::DataTestCmd( const Array<uint8_t>& InTestData )
		// C2S: Heartbit
		Result NetPolicyLogin::HeartBitC2SEvt(  )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::HeartBitC2SEvt::BuildIMsg(pMsg));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyLogin::HeartBitC2SEvt(  )
		// Cmd: For network test
		Result NetPolicyLogin::DebugPrintALLRankingCmd( const char* InFileName )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::DebugPrintALLRankingCmd::BuildIMsg(pMsg, InFileName));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetPolicyLogin::DebugPrintALLRankingCmd( const char* InFileName )


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
		// Cmd: Update my score and Get Ranking list
		Result NetSvrPolicyLogin::UpdateMyScoreRes( const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::UpdateMyScoreRes::BuildIMsg(pMsg, InResult, InRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyLogin::UpdateMyScoreRes( const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
		// Cmd: Get Ranking lise
		Result NetSvrPolicyLogin::GetRankingListRes( const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::GetRankingListRes::BuildIMsg(pMsg, InResult, InRanking));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyLogin::GetRankingListRes( const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
		// Cmd: For network test
		Result NetSvrPolicyLogin::DataTestRes( const Result &InResult, const Array<uint8_t>& InTestData )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::DataTestRes::BuildIMsg(pMsg, InResult, InTestData));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyLogin::DataTestRes( const Result &InResult, const Array<uint8_t>& InTestData )
		// Cmd: For network test
		Result NetSvrPolicyLogin::DebugPrintALLRankingRes( const Result &InResult )
		{
 			Result hr;

			 Message::MessageData * pMsg = nullptr;

			 protocolChk(Message::Login::DebugPrintALLRankingRes::BuildIMsg(pMsg, InResult));

			 protocolChkPtr(GetConnection());

			 protocolChk(GetConnection()->Send( pMsg ));

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyLogin::DebugPrintALLRankingRes( const Result &InResult )


	}; // namespace Policy
}; // namespace BR


