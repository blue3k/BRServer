////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Login Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/PolicyID.h"
#include "Common/ToStringSvr.h"
#include "Common/ToStringGame.h"
#include "Common/ArrayUtil.h"
#include "Protocol/ProtocolMessageCPPInc.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/ProtocolHelper.h"
#include "Protocol/Message/LoginMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Message
	{
 		namespace Login
		{
 			// Cmd: Login request
			const MessageID LoginCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 0);
			HRESULT LoginCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameID, pCur, iMsgSize, sizeof(GameID) ) );
				UINT16 uiSizeOfID = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfID, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_ID, pCur, iMsgSize, sizeof(char)*uiSizeOfID ) );
				UINT16 uiSizeOfPassword = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPassword, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_Password, pCur, iMsgSize, sizeof(char)*uiSizeOfPassword ) );


			Proc_End:

				return hr;

			}; // HRESULT LoginCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT LoginCmd::BuildIMsg( OUT MessageData* &pMsg, const GameID &InGameID, const char* InID, const char* InPassword )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInIDLength = InID ? (UINT16)(strlen(InID)+1) : 1;
				UINT16 __uiInPasswordLength = InPassword ? (UINT16)(strlen(InPassword)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInIDLength + sizeof(UINT16) + __uiInPasswordLength 
					+ sizeof(GameID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Login::LoginCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameID, sizeof(GameID));
				Protocol::PackParamCopy( pMsgData, &__uiInIDLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InID ? InID : "", __uiInIDLength );
				Protocol::PackParamCopy( pMsgData, &__uiInPasswordLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InPassword ? InPassword : "", __uiInPasswordLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT LoginCmd::BuildIMsg( OUT MessageData* &pMsg, const GameID &InGameID, const char* InID, const char* InPassword )



			VOID LoginCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:LoginCmd:%1%:%2% , GameID:%3%, ID:%4%, Password:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameID, m_ID, m_Password); 
			}; // VOID LoginCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID LoginRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 0);
			HRESULT LoginRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameServerAddr, pCur, iMsgSize, sizeof(NetAddress) ) );
				protocolChk( Protocol::StreamParamCopy( &m_AccID, pCur, iMsgSize, sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LoginEntityUID, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // HRESULT LoginRes::ParseIMsg( MessageData* pIMsg )

			HRESULT LoginRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const NetAddress &InGameServerAddr, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(NetAddress)
					+ sizeof(AccountID)
					+ sizeof(AuthTicket)
					+ sizeof(UINT64));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Login::LoginRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InGameServerAddr, sizeof(NetAddress));
				Protocol::PackParamCopy( pMsgData, &InAccID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InLoginEntityUID, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT LoginRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const NetAddress &InGameServerAddr, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )



			VOID LoginRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:LoginRes:%1%:%2% , Result:{3}, GameServerAddr:%4%, AccID:%5%, Ticket:%6%, LoginEntityUID:%7%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_GameServerAddr, m_AccID, m_Ticket, m_LoginEntityUID); 
			}; // VOID LoginRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Login request with Facebook UID
			const MessageID LoginByFacebookCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 1);
			HRESULT LoginByFacebookCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameID, pCur, iMsgSize, sizeof(GameID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_UID, pCur, iMsgSize, sizeof(UINT64) ) );
				UINT16 uiSizeOfFaceBookName = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfFaceBookName, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_FaceBookName, pCur, iMsgSize, sizeof(char)*uiSizeOfFaceBookName ) );
				UINT16 uiSizeOfEMail = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfEMail, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_EMail, pCur, iMsgSize, sizeof(char)*uiSizeOfEMail ) );
				UINT16 uiSizeOfFacebookToken = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfFacebookToken, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_FacebookToken, pCur, iMsgSize, sizeof(char)*uiSizeOfFacebookToken ) );


			Proc_End:

				return hr;

			}; // HRESULT LoginByFacebookCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT LoginByFacebookCmd::BuildIMsg( OUT MessageData* &pMsg, const GameID &InGameID, const UINT64 &InUID, const char* InFaceBookName, const char* InEMail, const char* InFacebookToken )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInFaceBookNameLength = InFaceBookName ? (UINT16)(strlen(InFaceBookName)+1) : 1;
				UINT16 __uiInEMailLength = InEMail ? (UINT16)(strlen(InEMail)+1) : 1;
				UINT16 __uiInFacebookTokenLength = InFacebookToken ? (UINT16)(strlen(InFacebookToken)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInFaceBookNameLength + sizeof(UINT16) + __uiInEMailLength + sizeof(UINT16) + __uiInFacebookTokenLength 
					+ sizeof(GameID)
					+ sizeof(UINT64));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Login::LoginByFacebookCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameID, sizeof(GameID));
				Protocol::PackParamCopy( pMsgData, &InUID, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &__uiInFaceBookNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InFaceBookName ? InFaceBookName : "", __uiInFaceBookNameLength );
				Protocol::PackParamCopy( pMsgData, &__uiInEMailLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InEMail ? InEMail : "", __uiInEMailLength );
				Protocol::PackParamCopy( pMsgData, &__uiInFacebookTokenLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InFacebookToken ? InFacebookToken : "", __uiInFacebookTokenLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT LoginByFacebookCmd::BuildIMsg( OUT MessageData* &pMsg, const GameID &InGameID, const UINT64 &InUID, const char* InFaceBookName, const char* InEMail, const char* InFacebookToken )



			VOID LoginByFacebookCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:LoginByFacebookCmd:%1%:%2% , GameID:%3%, UID:%4%, FaceBookName:%5%, EMail:%6%, FacebookToken:%7%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameID, m_UID, m_FaceBookName, m_EMail, m_FacebookToken); 
			}; // VOID LoginByFacebookCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID LoginByFacebookRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 1);
			HRESULT LoginByFacebookRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameServerAddr, pCur, iMsgSize, sizeof(NetAddress) ) );
				protocolChk( Protocol::StreamParamCopy( &m_AccID, pCur, iMsgSize, sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LoginEntityUID, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // HRESULT LoginByFacebookRes::ParseIMsg( MessageData* pIMsg )

			HRESULT LoginByFacebookRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const NetAddress &InGameServerAddr, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(NetAddress)
					+ sizeof(AccountID)
					+ sizeof(AuthTicket)
					+ sizeof(UINT64));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Login::LoginByFacebookRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InGameServerAddr, sizeof(NetAddress));
				Protocol::PackParamCopy( pMsgData, &InAccID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InLoginEntityUID, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT LoginByFacebookRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const NetAddress &InGameServerAddr, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )



			VOID LoginByFacebookRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:LoginByFacebookRes:%1%:%2% , Result:{3}, GameServerAddr:%4%, AccID:%5%, Ticket:%6%, LoginEntityUID:%7%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_GameServerAddr, m_AccID, m_Ticket, m_LoginEntityUID); 
			}; // VOID LoginByFacebookRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Login request
			const MessageID CreateRandomUserCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 2);
			HRESULT CreateRandomUserCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameID, pCur, iMsgSize, sizeof(GameID) ) );
				UINT16 uiSizeOfCellPhone = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfCellPhone, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_CellPhone, pCur, iMsgSize, sizeof(char)*uiSizeOfCellPhone ) );


			Proc_End:

				return hr;

			}; // HRESULT CreateRandomUserCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT CreateRandomUserCmd::BuildIMsg( OUT MessageData* &pMsg, const GameID &InGameID, const char* InCellPhone )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInCellPhoneLength = InCellPhone ? (UINT16)(strlen(InCellPhone)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInCellPhoneLength 
					+ sizeof(GameID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Login::CreateRandomUserCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameID, sizeof(GameID));
				Protocol::PackParamCopy( pMsgData, &__uiInCellPhoneLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InCellPhone ? InCellPhone : "", __uiInCellPhoneLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT CreateRandomUserCmd::BuildIMsg( OUT MessageData* &pMsg, const GameID &InGameID, const char* InCellPhone )



			VOID CreateRandomUserCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:CreateRandomUserCmd:%1%:%2% , GameID:%3%, CellPhone:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameID, m_CellPhone); 
			}; // VOID CreateRandomUserCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID CreateRandomUserRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 2);
			HRESULT CreateRandomUserRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameServerAddr, pCur, iMsgSize, sizeof(NetAddress) ) );
				protocolChk( Protocol::StreamParamCopy( &m_AccID, pCur, iMsgSize, sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LoginEntityUID, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // HRESULT CreateRandomUserRes::ParseIMsg( MessageData* pIMsg )

			HRESULT CreateRandomUserRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const NetAddress &InGameServerAddr, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(NetAddress)
					+ sizeof(AccountID)
					+ sizeof(AuthTicket)
					+ sizeof(UINT64));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Login::CreateRandomUserRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InGameServerAddr, sizeof(NetAddress));
				Protocol::PackParamCopy( pMsgData, &InAccID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InLoginEntityUID, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT CreateRandomUserRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const NetAddress &InGameServerAddr, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )



			VOID CreateRandomUserRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:CreateRandomUserRes:%1%:%2% , Result:{3}, GameServerAddr:%4%, AccID:%5%, Ticket:%6%, LoginEntityUID:%7%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_GameServerAddr, m_AccID, m_Ticket, m_LoginEntityUID); 
			}; // VOID CreateRandomUserRes::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace Login
	}; // namespace Message
}; // namespace BR


