﻿////////////////////////////////////////////////////////////////////////////////
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
#include "String/ToStringSvr.h"
#include "String/ToStringGame.h"
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
			Result LoginCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfID = 0;
				UINT16 uiSizeOfPassword = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameID, pCur, iMsgSize, (int)sizeof(GameID) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfID, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_ID, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfID ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPassword, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_Password, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfPassword ) );


			Proc_End:

				return hr;

			}; // Result LoginCmd::ParseMessage( MessageData* pIMsg )

			Result LoginCmd::BuildIMsg( OUT MessageData* &pMsg, const GameID &InGameID, const char* InID, const char* InPassword )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInIDLength = InID ? (UINT16)(strlen(InID)+1) : 1;
				UINT16 __uiInPasswordLength = InPassword ? (UINT16)(strlen(InPassword)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInIDLength + sizeof(UINT16) + __uiInPasswordLength 
					+ sizeof(GameID));

				MessageData *pNewMsg = nullptr;

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

			}; // Result LoginCmd::BuildIMsg( OUT MessageData* &pMsg, const GameID &InGameID, const char* InID, const char* InPassword )



			void LoginCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:LoginCmd:{1}:{2} , GameID:{3}, ID:{4}, Password:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameID, m_ID, m_Password); 
			}; // void LoginCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID LoginRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 0);
			Result LoginRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameServerAddr, pCur, iMsgSize, (int)sizeof(NetAddress) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameServerAddrIPV4, pCur, iMsgSize, (int)sizeof(NetAddress) ) );
				protocolChk( Protocol::StreamParamCopy( &m_AccID, pCur, iMsgSize, (int)sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, (int)sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LoginEntityUID, pCur, iMsgSize, (int)sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result LoginRes::ParseMessage( MessageData* pIMsg )

			Result LoginRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(NetAddress)
					+ sizeof(NetAddress)
					+ sizeof(AccountID)
					+ sizeof(AuthTicket)
					+ sizeof(uint64_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Login::LoginRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InGameServerAddr, sizeof(NetAddress));
				Protocol::PackParamCopy( pMsgData, &InGameServerAddrIPV4, sizeof(NetAddress));
				Protocol::PackParamCopy( pMsgData, &InAccID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InLoginEntityUID, sizeof(uint64_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result LoginRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID )



			void LoginRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:LoginRes:{1}:{2} , Result:{3:X8}, GameServerAddr:{4}, GameServerAddrIPV4:{5}, AccID:{6}, Ticket:{7}, LoginEntityUID:{8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_GameServerAddr, m_GameServerAddrIPV4, m_AccID, m_Ticket, m_LoginEntityUID); 
			}; // void LoginRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Login request with Facebook UID
			const MessageID LoginByFacebookCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 1);
			Result LoginByFacebookCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfFaceBookName = 0;
				UINT16 uiSizeOfEMail = 0;
				UINT16 uiSizeOfFacebookToken = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameID, pCur, iMsgSize, (int)sizeof(GameID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_UID, pCur, iMsgSize, (int)sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfFaceBookName, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_FaceBookName, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfFaceBookName ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfEMail, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_EMail, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfEMail ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfFacebookToken, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_FacebookToken, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfFacebookToken ) );


			Proc_End:

				return hr;

			}; // Result LoginByFacebookCmd::ParseMessage( MessageData* pIMsg )

			Result LoginByFacebookCmd::BuildIMsg( OUT MessageData* &pMsg, const GameID &InGameID, const uint64_t &InUID, const char* InFaceBookName, const char* InEMail, const char* InFacebookToken )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInFaceBookNameLength = InFaceBookName ? (UINT16)(strlen(InFaceBookName)+1) : 1;
				UINT16 __uiInEMailLength = InEMail ? (UINT16)(strlen(InEMail)+1) : 1;
				UINT16 __uiInFacebookTokenLength = InFacebookToken ? (UINT16)(strlen(InFacebookToken)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInFaceBookNameLength + sizeof(UINT16) + __uiInEMailLength + sizeof(UINT16) + __uiInFacebookTokenLength 
					+ sizeof(GameID)
					+ sizeof(uint64_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Login::LoginByFacebookCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameID, sizeof(GameID));
				Protocol::PackParamCopy( pMsgData, &InUID, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &__uiInFaceBookNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InFaceBookName ? InFaceBookName : "", __uiInFaceBookNameLength );
				Protocol::PackParamCopy( pMsgData, &__uiInEMailLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InEMail ? InEMail : "", __uiInEMailLength );
				Protocol::PackParamCopy( pMsgData, &__uiInFacebookTokenLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InFacebookToken ? InFacebookToken : "", __uiInFacebookTokenLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result LoginByFacebookCmd::BuildIMsg( OUT MessageData* &pMsg, const GameID &InGameID, const uint64_t &InUID, const char* InFaceBookName, const char* InEMail, const char* InFacebookToken )



			void LoginByFacebookCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:LoginByFacebookCmd:{1}:{2} , GameID:{3}, UID:{4}, FaceBookName:{5}, EMail:{6}, FacebookToken:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameID, m_UID, m_FaceBookName, m_EMail, m_FacebookToken); 
			}; // void LoginByFacebookCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID LoginByFacebookRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 1);
			Result LoginByFacebookRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameServerAddr, pCur, iMsgSize, (int)sizeof(NetAddress) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameServerAddrIPV4, pCur, iMsgSize, (int)sizeof(NetAddress) ) );
				protocolChk( Protocol::StreamParamCopy( &m_AccID, pCur, iMsgSize, (int)sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, (int)sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LoginEntityUID, pCur, iMsgSize, (int)sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result LoginByFacebookRes::ParseMessage( MessageData* pIMsg )

			Result LoginByFacebookRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(NetAddress)
					+ sizeof(NetAddress)
					+ sizeof(AccountID)
					+ sizeof(AuthTicket)
					+ sizeof(uint64_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Login::LoginByFacebookRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InGameServerAddr, sizeof(NetAddress));
				Protocol::PackParamCopy( pMsgData, &InGameServerAddrIPV4, sizeof(NetAddress));
				Protocol::PackParamCopy( pMsgData, &InAccID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InLoginEntityUID, sizeof(uint64_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result LoginByFacebookRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID )



			void LoginByFacebookRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:LoginByFacebookRes:{1}:{2} , Result:{3:X8}, GameServerAddr:{4}, GameServerAddrIPV4:{5}, AccID:{6}, Ticket:{7}, LoginEntityUID:{8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_GameServerAddr, m_GameServerAddrIPV4, m_AccID, m_Ticket, m_LoginEntityUID); 
			}; // void LoginByFacebookRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Login request
			const MessageID CreateRandomUserCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 2);
			Result CreateRandomUserCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfCellPhone = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameID, pCur, iMsgSize, (int)sizeof(GameID) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfCellPhone, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_CellPhone, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfCellPhone ) );


			Proc_End:

				return hr;

			}; // Result CreateRandomUserCmd::ParseMessage( MessageData* pIMsg )

			Result CreateRandomUserCmd::BuildIMsg( OUT MessageData* &pMsg, const GameID &InGameID, const char* InCellPhone )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInCellPhoneLength = InCellPhone ? (UINT16)(strlen(InCellPhone)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInCellPhoneLength 
					+ sizeof(GameID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Login::CreateRandomUserCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameID, sizeof(GameID));
				Protocol::PackParamCopy( pMsgData, &__uiInCellPhoneLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InCellPhone ? InCellPhone : "", __uiInCellPhoneLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result CreateRandomUserCmd::BuildIMsg( OUT MessageData* &pMsg, const GameID &InGameID, const char* InCellPhone )



			void CreateRandomUserCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:CreateRandomUserCmd:{1}:{2} , GameID:{3}, CellPhone:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameID, m_CellPhone); 
			}; // void CreateRandomUserCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID CreateRandomUserRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 2);
			Result CreateRandomUserRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameServerAddr, pCur, iMsgSize, (int)sizeof(NetAddress) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameServerAddrIPV4, pCur, iMsgSize, (int)sizeof(NetAddress) ) );
				protocolChk( Protocol::StreamParamCopy( &m_AccID, pCur, iMsgSize, (int)sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, (int)sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LoginEntityUID, pCur, iMsgSize, (int)sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result CreateRandomUserRes::ParseMessage( MessageData* pIMsg )

			Result CreateRandomUserRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(NetAddress)
					+ sizeof(NetAddress)
					+ sizeof(AccountID)
					+ sizeof(AuthTicket)
					+ sizeof(uint64_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Login::CreateRandomUserRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InGameServerAddr, sizeof(NetAddress));
				Protocol::PackParamCopy( pMsgData, &InGameServerAddrIPV4, sizeof(NetAddress));
				Protocol::PackParamCopy( pMsgData, &InAccID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InLoginEntityUID, sizeof(uint64_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result CreateRandomUserRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID )



			void CreateRandomUserRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:CreateRandomUserRes:{1}:{2} , Result:{3:X8}, GameServerAddr:{4}, GameServerAddrIPV4:{5}, AccID:{6}, Ticket:{7}, LoginEntityUID:{8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_GameServerAddr, m_GameServerAddrIPV4, m_AccID, m_Ticket, m_LoginEntityUID); 
			}; // void CreateRandomUserRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Update my score and Get Ranking list
			const MessageID UpdateMyScoreCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 3);
			Result UpdateMyScoreCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RankingScore, pCur, iMsgSize, (int)sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RankingType, pCur, iMsgSize, (int)sizeof(RankingType) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Count, pCur, iMsgSize, (int)sizeof(uint16_t) ) );


			Proc_End:

				return hr;

			}; // Result UpdateMyScoreCmd::ParseMessage( MessageData* pIMsg )

			Result UpdateMyScoreCmd::BuildIMsg( OUT MessageData* &pMsg, const uint64_t &InRankingScore, const RankingType &InRankingType, const uint16_t &InCount )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(uint64_t)
					+ sizeof(RankingType)
					+ sizeof(uint16_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Login::UpdateMyScoreCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRankingScore, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InRankingType, sizeof(RankingType));
				Protocol::PackParamCopy( pMsgData, &InCount, sizeof(uint16_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result UpdateMyScoreCmd::BuildIMsg( OUT MessageData* &pMsg, const uint64_t &InRankingScore, const RankingType &InRankingType, const uint16_t &InCount )



			void UpdateMyScoreCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:UpdateMyScoreCmd:{1}:{2} , RankingScore:{3}, RankingType:{4}, Count:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RankingScore, m_RankingType, m_Count); 
			}; // void UpdateMyScoreCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID UpdateMyScoreRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 3);
			Result UpdateMyScoreRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofRanking = 0; TotalRankingPlayerInformation* pRanking = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofRanking, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pRanking, pCur, iMsgSize, (int)sizeof(TotalRankingPlayerInformation)*numberofRanking ) );
				m_Ranking.SetLinkedBuffer(numberofRanking, numberofRanking, pRanking);


			Proc_End:

				return hr;

			}; // Result UpdateMyScoreRes::ParseMessage( MessageData* pIMsg )

			Result UpdateMyScoreRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(TotalRankingPlayerInformation)*InRanking.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInRanking = (UINT16)InRanking.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Login::UpdateMyScoreRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInRanking, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InRanking.data(), (INT)(sizeof(TotalRankingPlayerInformation)*InRanking.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result UpdateMyScoreRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )



			void UpdateMyScoreRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:UpdateMyScoreRes:{1}:{2} , Result:{3:X8}, Ranking:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_Ranking); 
			}; // void UpdateMyScoreRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Get Ranking lise
			const MessageID GetRankingListCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 4);
			Result GetRankingListCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RankingType, pCur, iMsgSize, (int)sizeof(RankingType) ) );
				protocolChk( Protocol::StreamParamCopy( &m_BaseRanking, pCur, iMsgSize, (int)sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Count, pCur, iMsgSize, (int)sizeof(uint8_t) ) );


			Proc_End:

				return hr;

			}; // Result GetRankingListCmd::ParseMessage( MessageData* pIMsg )

			Result GetRankingListCmd::BuildIMsg( OUT MessageData* &pMsg, const RankingType &InRankingType, const uint8_t &InBaseRanking, const uint8_t &InCount )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(RankingType)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Login::GetRankingListCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRankingType, sizeof(RankingType));
				Protocol::PackParamCopy( pMsgData, &InBaseRanking, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InCount, sizeof(uint8_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetRankingListCmd::BuildIMsg( OUT MessageData* &pMsg, const RankingType &InRankingType, const uint8_t &InBaseRanking, const uint8_t &InCount )



			void GetRankingListCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetRankingListCmd:{1}:{2} , RankingType:{3}, BaseRanking:{4}, Count:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RankingType, m_BaseRanking, m_Count); 
			}; // void GetRankingListCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetRankingListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 4);
			Result GetRankingListRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofRanking = 0; TotalRankingPlayerInformation* pRanking = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofRanking, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pRanking, pCur, iMsgSize, (int)sizeof(TotalRankingPlayerInformation)*numberofRanking ) );
				m_Ranking.SetLinkedBuffer(numberofRanking, numberofRanking, pRanking);


			Proc_End:

				return hr;

			}; // Result GetRankingListRes::ParseMessage( MessageData* pIMsg )

			Result GetRankingListRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(TotalRankingPlayerInformation)*InRanking.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInRanking = (UINT16)InRanking.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Login::GetRankingListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInRanking, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InRanking.data(), (INT)(sizeof(TotalRankingPlayerInformation)*InRanking.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetRankingListRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )



			void GetRankingListRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetRankingListRes:{1}:{2} , Result:{3:X8}, Ranking:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_Ranking); 
			}; // void GetRankingListRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: For network test
			const MessageID DataTestCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 5);
			Result DataTestCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofTestData = 0; uint8_t* pTestData = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &numberofTestData, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pTestData, pCur, iMsgSize, (int)sizeof(uint8_t)*numberofTestData ) );
				m_TestData.SetLinkedBuffer(numberofTestData, numberofTestData, pTestData);


			Proc_End:

				return hr;

			}; // Result DataTestCmd::ParseMessage( MessageData* pIMsg )

			Result DataTestCmd::BuildIMsg( OUT MessageData* &pMsg, const Array<uint8_t>& InTestData )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(uint8_t)*InTestData.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInTestData = (UINT16)InTestData.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Login::DataTestCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &numberOfInTestData, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InTestData.data(), (INT)(sizeof(uint8_t)*InTestData.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result DataTestCmd::BuildIMsg( OUT MessageData* &pMsg, const Array<uint8_t>& InTestData )



			void DataTestCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:DataTestCmd:{1}:{2} , TestData:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TestData); 
			}; // void DataTestCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID DataTestRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 5);
			Result DataTestRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofTestData = 0; uint8_t* pTestData = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofTestData, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pTestData, pCur, iMsgSize, (int)sizeof(uint8_t)*numberofTestData ) );
				m_TestData.SetLinkedBuffer(numberofTestData, numberofTestData, pTestData);


			Proc_End:

				return hr;

			}; // Result DataTestRes::ParseMessage( MessageData* pIMsg )

			Result DataTestRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const Array<uint8_t>& InTestData )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(uint8_t)*InTestData.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInTestData = (UINT16)InTestData.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Login::DataTestRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInTestData, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InTestData.data(), (INT)(sizeof(uint8_t)*InTestData.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result DataTestRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const Array<uint8_t>& InTestData )



			void DataTestRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:DataTestRes:{1}:{2} , Result:{3:X8}, TestData:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_TestData); 
			}; // void DataTestRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Heartbit
			const MessageID HeartBitC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_LOGIN, 6);
			Result HeartBitC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result HeartBitC2SEvt::ParseMessage( MessageData* pIMsg )

			Result HeartBitC2SEvt::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Login::HeartBitC2SEvt::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result HeartBitC2SEvt::BuildIMsg( OUT MessageData* &pMsg )



			void HeartBitC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:HeartBitC2SEvt:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void HeartBitC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace Login
	}; // namespace Message
}; // namespace BR


