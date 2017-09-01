﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Game Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Protocol/Protocol.h"
#include "String/ToStringSvr.h"
#include "String/ToStringGame.h"
#include "Common/ArrayUtil.h"
#include "Protocol/ProtocolMessageCPPInc.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/ProtocolHelper.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Message
	{
 		namespace Game
		{
 			// C2S: Client heart bit
			const MessageID HeartBitC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 0);
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

				protocolMem( pNewMsg = MessageData::NewMessage( Game::HeartBitC2SEvt::MID, __uiMessageSize ) );


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

			// Cmd: Player connected from a login server and moved to game server
			const MessageID JoinGameServerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 1);
			Result JoinGameServerCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_AccID, pCur, iMsgSize, (int)sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, (int)sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LoginEntityUID, pCur, iMsgSize, (int)sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result JoinGameServerCmd::ParseMessage( MessageData* pIMsg )

			Result JoinGameServerCmd::BuildIMsg( OUT MessageData* &pMsg, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(AccountID)
					+ sizeof(AuthTicket)
					+ sizeof(uint64_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinGameServerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InAccID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InLoginEntityUID, sizeof(uint64_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result JoinGameServerCmd::BuildIMsg( OUT MessageData* &pMsg, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID )



			void JoinGameServerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:JoinGameServerCmd:{1}:{2} , AccID:{3}, Ticket:{4}, LoginEntityUID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_AccID, m_Ticket, m_LoginEntityUID); 
			}; // void JoinGameServerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID JoinGameServerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 1);
			Result JoinGameServerRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfNickName = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfNickName, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_NickName, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfNickName ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, (int)sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyLeaderID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, (int)sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // Result JoinGameServerRes::ParseMessage( MessageData* pIMsg )

			Result JoinGameServerRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const char* InNickName, const GameInsUID &InGameUID, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const MatchingQueueTicket &InMatchingTicket )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInNickNameLength = InNickName ? (uint16_t)(strlen(InNickName)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(uint16_t) + __uiInNickNameLength 
					+ sizeof(Result)
					+ sizeof(GameInsUID)
					+ sizeof(PartyUID)
					+ sizeof(PlayerID)
					+ sizeof(MatchingQueueTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinGameServerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &__uiInNickNameLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InNickName ? InNickName : "", __uiInNickNameLength );
				Protocol::PackParamCopy( pMsgData, &InGameUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InPartyLeaderID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result JoinGameServerRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const char* InNickName, const GameInsUID &InGameUID, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const MatchingQueueTicket &InMatchingTicket )



			void JoinGameServerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:JoinGameServerRes:{1}:{2} , Result:{3:X8}, NickName:{4,60}, GameUID:{5}, PartyUID:{6}, PartyLeaderID:{7}, MatchingTicket:{8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_NickName, m_GameUID, m_PartyUID, m_PartyLeaderID, m_MatchingTicket); 
			}; // void JoinGameServerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: player complition statues
			const MessageID GetComplitionStateCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 2);
			Result GetComplitionStateCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result GetComplitionStateCmd::ParseMessage( MessageData* pIMsg )

			Result GetComplitionStateCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetComplitionStateCmd::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetComplitionStateCmd::BuildIMsg( OUT MessageData* &pMsg )



			void GetComplitionStateCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetComplitionStateCmd:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void GetComplitionStateCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetComplitionStateRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 2);
			Result GetComplitionStateRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfComplitionState = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfComplitionState, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_ComplitionState, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfComplitionState ) );


			Proc_End:

				return hr;

			}; // Result GetComplitionStateRes::ParseMessage( MessageData* pIMsg )

			Result GetComplitionStateRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const char* InComplitionState )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInComplitionStateLength = InComplitionState ? (uint16_t)(strlen(InComplitionState)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(uint16_t) + __uiInComplitionStateLength 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetComplitionStateRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &__uiInComplitionStateLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InComplitionState ? InComplitionState : "", __uiInComplitionStateLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetComplitionStateRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const char* InComplitionState )



			void GetComplitionStateRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetComplitionStateRes:{1}:{2} , Result:{3:X8}, ComplitionState:{4,60}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_ComplitionState); 
			}; // void GetComplitionStateRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Player complition state
			const MessageID SetComplitionStateCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 3);
			Result SetComplitionStateCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfComplitionState = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &uiSizeOfComplitionState, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_ComplitionState, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfComplitionState ) );


			Proc_End:

				return hr;

			}; // Result SetComplitionStateCmd::ParseMessage( MessageData* pIMsg )

			Result SetComplitionStateCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InComplitionState )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInComplitionStateLength = InComplitionState ? (uint16_t)(strlen(InComplitionState)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(uint16_t) + __uiInComplitionStateLength );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetComplitionStateCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInComplitionStateLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InComplitionState ? InComplitionState : "", __uiInComplitionStateLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result SetComplitionStateCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InComplitionState )



			void SetComplitionStateCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:SetComplitionStateCmd:{1}:{2} , ComplitionState:{3,60}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_ComplitionState); 
			}; // void SetComplitionStateCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID SetComplitionStateRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 3);
			Result SetComplitionStateRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result SetComplitionStateRes::ParseMessage( MessageData* pIMsg )

			Result SetComplitionStateRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetComplitionStateRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result SetComplitionStateRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void SetComplitionStateRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:SetComplitionStateRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void SetComplitionStateRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
			const MessageID RegisterGCMCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 4);
			Result RegisterGCMCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfGCMRegisteredID = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &uiSizeOfGCMRegisteredID, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_GCMRegisteredID, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfGCMRegisteredID ) );


			Proc_End:

				return hr;

			}; // Result RegisterGCMCmd::ParseMessage( MessageData* pIMsg )

			Result RegisterGCMCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InGCMRegisteredID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInGCMRegisteredIDLength = InGCMRegisteredID ? (uint16_t)(strlen(InGCMRegisteredID)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(uint16_t) + __uiInGCMRegisteredIDLength );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RegisterGCMCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInGCMRegisteredIDLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InGCMRegisteredID ? InGCMRegisteredID : "", __uiInGCMRegisteredIDLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RegisterGCMCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InGCMRegisteredID )



			void RegisterGCMCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RegisterGCMCmd:{1}:{2} , GCMRegisteredID:{3,60}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GCMRegisteredID); 
			}; // void RegisterGCMCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RegisterGCMRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 4);
			Result RegisterGCMRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result RegisterGCMRes::ParseMessage( MessageData* pIMsg )

			Result RegisterGCMRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RegisterGCMRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RegisterGCMRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void RegisterGCMRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RegisterGCMRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void RegisterGCMRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Unregister Google notification service ID
			const MessageID UnregisterGCMCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 5);
			Result UnregisterGCMCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfGCMRegisteredID = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &uiSizeOfGCMRegisteredID, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_GCMRegisteredID, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfGCMRegisteredID ) );


			Proc_End:

				return hr;

			}; // Result UnregisterGCMCmd::ParseMessage( MessageData* pIMsg )

			Result UnregisterGCMCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InGCMRegisteredID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInGCMRegisteredIDLength = InGCMRegisteredID ? (uint16_t)(strlen(InGCMRegisteredID)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(uint16_t) + __uiInGCMRegisteredIDLength );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::UnregisterGCMCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInGCMRegisteredIDLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InGCMRegisteredID ? InGCMRegisteredID : "", __uiInGCMRegisteredIDLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result UnregisterGCMCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InGCMRegisteredID )



			void UnregisterGCMCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:UnregisterGCMCmd:{1}:{2} , GCMRegisteredID:{3,60}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GCMRegisteredID); 
			}; // void UnregisterGCMCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID UnregisterGCMRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 5);
			Result UnregisterGCMRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result UnregisterGCMRes::ParseMessage( MessageData* pIMsg )

			Result UnregisterGCMRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::UnregisterGCMRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result UnregisterGCMRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void UnregisterGCMRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:UnregisterGCMRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void UnregisterGCMRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Invite friend
			const MessageID InviteFriendCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 6);
			Result InviteFriendCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_FriendID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result InviteFriendCmd::ParseMessage( MessageData* pIMsg )

			Result InviteFriendCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::InviteFriendCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InFriendID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result InviteFriendCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )



			void InviteFriendCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:InviteFriendCmd:{1}:{2} , FriendID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_FriendID); 
			}; // void InviteFriendCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID InviteFriendRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 6);
			Result InviteFriendRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result InviteFriendRes::ParseMessage( MessageData* pIMsg )

			Result InviteFriendRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::InviteFriendRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result InviteFriendRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void InviteFriendRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:InviteFriendRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void InviteFriendRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Accept friend request
			const MessageID AcceptFriendRequestCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 7);
			Result AcceptFriendRequestCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InviterID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InviterFacebookUID, pCur, iMsgSize, (int)sizeof(FacebookUID) ) );


			Proc_End:

				return hr;

			}; // Result AcceptFriendRequestCmd::ParseMessage( MessageData* pIMsg )

			Result AcceptFriendRequestCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviterID, const FacebookUID &InInviterFacebookUID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)
					+ sizeof(FacebookUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AcceptFriendRequestCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInviterID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InInviterFacebookUID, sizeof(FacebookUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AcceptFriendRequestCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviterID, const FacebookUID &InInviterFacebookUID )



			void AcceptFriendRequestCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AcceptFriendRequestCmd:{1}:{2} , InviterID:{3}, InviterFacebookUID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InviterID, m_InviterFacebookUID); 
			}; // void AcceptFriendRequestCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID AcceptFriendRequestRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 7);
			Result AcceptFriendRequestRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NewFriend, pCur, iMsgSize, (int)sizeof(FriendInformation) ) );


			Proc_End:

				return hr;

			}; // Result AcceptFriendRequestRes::ParseMessage( MessageData* pIMsg )

			Result AcceptFriendRequestRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const FriendInformation &InNewFriend )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(FriendInformation));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AcceptFriendRequestRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InNewFriend, sizeof(FriendInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AcceptFriendRequestRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const FriendInformation &InNewFriend )



			void AcceptFriendRequestRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AcceptFriendRequestRes:{1}:{2} , Result:{3:X8}, NewFriend:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_NewFriend); 
			}; // void AcceptFriendRequestRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Notification for friend request is accepted
			const MessageID FriendRequestAcceptedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 8);
			Result FriendRequestAcceptedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Accepter, pCur, iMsgSize, (int)sizeof(FriendInformation) ) );


			Proc_End:

				return hr;

			}; // Result FriendRequestAcceptedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result FriendRequestAcceptedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const FriendInformation &InAccepter )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(FriendInformation));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FriendRequestAcceptedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InAccepter, sizeof(FriendInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result FriendRequestAcceptedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const FriendInformation &InAccepter )



			void FriendRequestAcceptedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:FriendRequestAcceptedS2CEvt:{1}:{2} , Accepter:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Accepter); 
			}; // void FriendRequestAcceptedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Remove friden form the friend list
			const MessageID RemoveFriendCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 9);
			Result RemoveFriendCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_FriendID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result RemoveFriendCmd::ParseMessage( MessageData* pIMsg )

			Result RemoveFriendCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RemoveFriendCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InFriendID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RemoveFriendCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )



			void RemoveFriendCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RemoveFriendCmd:{1}:{2} , FriendID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_FriendID); 
			}; // void RemoveFriendCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RemoveFriendRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 9);
			Result RemoveFriendRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_FriendID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result RemoveFriendRes::ParseMessage( MessageData* pIMsg )

			Result RemoveFriendRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerID &InFriendID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RemoveFriendRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InFriendID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RemoveFriendRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerID &InFriendID )



			void RemoveFriendRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RemoveFriendRes:{1}:{2} , Result:{3:X8}, FriendID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_FriendID); 
			}; // void RemoveFriendRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Friend removed
			const MessageID FriendRemovedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 10);
			Result FriendRemovedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_FriendID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result FriendRemovedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result FriendRemovedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FriendRemovedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InFriendID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result FriendRemovedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )



			void FriendRemovedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:FriendRemovedS2CEvt:{1}:{2} , FriendID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_FriendID); 
			}; // void FriendRemovedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Get friend list
			const MessageID GetFriendListCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 11);
			Result GetFriendListCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_StartIndex, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Count, pCur, iMsgSize, (int)sizeof(uint16_t) ) );


			Proc_End:

				return hr;

			}; // Result GetFriendListCmd::ParseMessage( MessageData* pIMsg )

			Result GetFriendListCmd::BuildIMsg( OUT MessageData* &pMsg, const uint16_t &InStartIndex, const uint16_t &InCount )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(uint16_t)
					+ sizeof(uint16_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetFriendListCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InStartIndex, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InCount, sizeof(uint16_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetFriendListCmd::BuildIMsg( OUT MessageData* &pMsg, const uint16_t &InStartIndex, const uint16_t &InCount )



			void GetFriendListCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetFriendListCmd:{1}:{2} , StartIndex:{3}, Count:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_StartIndex, m_Count); 
			}; // void GetFriendListCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetFriendListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 11);
			Result GetFriendListRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t numberofFriendList = 0; FriendInformation* pFriendList = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MaxFriendSlot, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalNumberOfFriends, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_StartIndex, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofFriendList, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pFriendList, pCur, iMsgSize, (int)sizeof(FriendInformation)*numberofFriendList ) );
				m_FriendList.SetLinkedBuffer(numberofFriendList, numberofFriendList, pFriendList);


			Proc_End:

				return hr;

			}; // Result GetFriendListRes::ParseMessage( MessageData* pIMsg )

			Result GetFriendListRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint16_t &InMaxFriendSlot, const uint16_t &InTotalNumberOfFriends, const uint16_t &InStartIndex, const Array<FriendInformation>& InFriendList )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(uint16_t)
					+ sizeof(uint16_t)
					+ sizeof(uint16_t)
					+ sizeof(FriendInformation)*InFriendList.GetSize() + sizeof(uint16_t));

				MessageData *pNewMsg = nullptr;

				uint16_t numberOfInFriendList = (uint16_t)InFriendList.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetFriendListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InMaxFriendSlot, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InTotalNumberOfFriends, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InStartIndex, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &numberOfInFriendList, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InFriendList.data(), (INT)(sizeof(FriendInformation)*InFriendList.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetFriendListRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint16_t &InMaxFriendSlot, const uint16_t &InTotalNumberOfFriends, const uint16_t &InStartIndex, const Array<FriendInformation>& InFriendList )



			void GetFriendListRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetFriendListRes:{1}:{2} , Result:{3:X8}, MaxFriendSlot:{4}, TotalNumberOfFriends:{5}, StartIndex:{6}, FriendList:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_MaxFriendSlot, m_TotalNumberOfFriends, m_StartIndex, m_FriendList); 
			}; // void GetFriendListRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Query notification list
			const MessageID GetNotificationListCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 12);
			Result GetNotificationListCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result GetNotificationListCmd::ParseMessage( MessageData* pIMsg )

			Result GetNotificationListCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetNotificationListCmd::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetNotificationListCmd::BuildIMsg( OUT MessageData* &pMsg )



			void GetNotificationListCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetNotificationListCmd:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void GetNotificationListCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetNotificationListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 12);
			Result GetNotificationListRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result GetNotificationListRes::ParseMessage( MessageData* pIMsg )

			Result GetNotificationListRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetNotificationListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetNotificationListRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void GetNotificationListRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetNotificationListRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void GetNotificationListRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Delete notification
			const MessageID DeleteNotificationCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 13);
			Result DeleteNotificationCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result DeleteNotificationCmd::ParseMessage( MessageData* pIMsg )

			Result DeleteNotificationCmd::BuildIMsg( OUT MessageData* &pMsg, const uint32_t &InNotificationID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::DeleteNotificationCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result DeleteNotificationCmd::BuildIMsg( OUT MessageData* &pMsg, const uint32_t &InNotificationID )



			void DeleteNotificationCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:DeleteNotificationCmd:{1}:{2} , NotificationID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NotificationID); 
			}; // void DeleteNotificationCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID DeleteNotificationRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 13);
			Result DeleteNotificationRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result DeleteNotificationRes::ParseMessage( MessageData* pIMsg )

			Result DeleteNotificationRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint32_t &InNotificationID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::DeleteNotificationRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result DeleteNotificationRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint32_t &InNotificationID )



			void DeleteNotificationRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:DeleteNotificationRes:{1}:{2} , Result:{3:X8}, NotificationID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_NotificationID); 
			}; // void DeleteNotificationRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Set notification is read
			const MessageID SetNotificationReadCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 14);
			Result SetNotificationReadCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result SetNotificationReadCmd::ParseMessage( MessageData* pIMsg )

			Result SetNotificationReadCmd::BuildIMsg( OUT MessageData* &pMsg, const uint32_t &InNotificationID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetNotificationReadCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result SetNotificationReadCmd::BuildIMsg( OUT MessageData* &pMsg, const uint32_t &InNotificationID )



			void SetNotificationReadCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:SetNotificationReadCmd:{1}:{2} , NotificationID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NotificationID); 
			}; // void SetNotificationReadCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID SetNotificationReadRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 14);
			Result SetNotificationReadRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result SetNotificationReadRes::ParseMessage( MessageData* pIMsg )

			Result SetNotificationReadRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint32_t &InNotificationID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetNotificationReadRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result SetNotificationReadRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint32_t &InNotificationID )



			void SetNotificationReadRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:SetNotificationReadRes:{1}:{2} , Result:{3:X8}, NotificationID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_NotificationID); 
			}; // void SetNotificationReadRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Accept notification
			const MessageID AcceptNotificationCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 15);
			Result AcceptNotificationCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result AcceptNotificationCmd::ParseMessage( MessageData* pIMsg )

			Result AcceptNotificationCmd::BuildIMsg( OUT MessageData* &pMsg, const uint32_t &InNotificationID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AcceptNotificationCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AcceptNotificationCmd::BuildIMsg( OUT MessageData* &pMsg, const uint32_t &InNotificationID )



			void AcceptNotificationCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AcceptNotificationCmd:{1}:{2} , NotificationID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NotificationID); 
			}; // void AcceptNotificationCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID AcceptNotificationRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 15);
			Result AcceptNotificationRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result AcceptNotificationRes::ParseMessage( MessageData* pIMsg )

			Result AcceptNotificationRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint32_t &InNotificationID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AcceptNotificationRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AcceptNotificationRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint32_t &InNotificationID )



			void AcceptNotificationRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AcceptNotificationRes:{1}:{2} , Result:{3:X8}, NotificationID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_NotificationID); 
			}; // void AcceptNotificationRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Notify new notification
			const MessageID NotifyS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 16);
			Result NotifyS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfMessageText = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, (int)sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MessageID, pCur, iMsgSize, (int)sizeof(NotificationType) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MessageParam0, pCur, iMsgSize, (int)sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MessageParam1, pCur, iMsgSize, (int)sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfMessageText, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_MessageText, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfMessageText ) );
				protocolChk( Protocol::StreamParamCopy( &m_IsRead, pCur, iMsgSize, (int)sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, (int)sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result NotifyS2CEvt::ParseMessage( MessageData* pIMsg )

			Result NotifyS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint8_t &InIsRead, const uint64_t &InTimeStamp )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInMessageTextLength = InMessageText ? (uint16_t)(strlen(InMessageText)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(uint16_t) + __uiInMessageTextLength 
					+ sizeof(uint32_t)
					+ sizeof(NotificationType)
					+ sizeof(uint64_t)
					+ sizeof(uint64_t)
					+ sizeof(uint8_t)
					+ sizeof(uint64_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::NotifyS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InMessageID, sizeof(NotificationType));
				Protocol::PackParamCopy( pMsgData, &InMessageParam0, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InMessageParam1, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &__uiInMessageTextLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InMessageText ? InMessageText : "", __uiInMessageTextLength );
				Protocol::PackParamCopy( pMsgData, &InIsRead, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(uint64_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result NotifyS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const uint32_t &InNotificationID, const NotificationType &InMessageID, const uint64_t &InMessageParam0, const uint64_t &InMessageParam1, const char* InMessageText, const uint8_t &InIsRead, const uint64_t &InTimeStamp )



			void NotifyS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:NotifyS2CEvt:{1}:{2} , NotificationID:{3}, MessageID:{4}, MessageParam0:{5}, MessageParam1:{6}, MessageText:{7,60}, IsRead:{8}, TimeStamp:{9}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NotificationID, m_MessageID, m_MessageParam0, m_MessageParam1, m_MessageText, m_IsRead, m_TimeStamp); 
			}; // void NotifyS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Query playerID list
			const MessageID FindPlayerByEMailCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 17);
			Result FindPlayerByEMailCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfPlayerEMail = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPlayerEMail, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_PlayerEMail, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfPlayerEMail ) );


			Proc_End:

				return hr;

			}; // Result FindPlayerByEMailCmd::ParseMessage( MessageData* pIMsg )

			Result FindPlayerByEMailCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InPlayerEMail )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInPlayerEMailLength = InPlayerEMail ? (uint16_t)(strlen(InPlayerEMail)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(uint16_t) + __uiInPlayerEMailLength );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FindPlayerByEMailCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInPlayerEMailLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InPlayerEMail ? InPlayerEMail : "", __uiInPlayerEMailLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result FindPlayerByEMailCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InPlayerEMail )



			void FindPlayerByEMailCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:FindPlayerByEMailCmd:{1}:{2} , PlayerEMail:{3,60}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerEMail); 
			}; // void FindPlayerByEMailCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID FindPlayerByEMailRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 17);
			Result FindPlayerByEMailRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Player, pCur, iMsgSize, (int)sizeof(PlayerInformation) ) );


			Proc_End:

				return hr;

			}; // Result FindPlayerByEMailRes::ParseMessage( MessageData* pIMsg )

			Result FindPlayerByEMailRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerInformation &InPlayer )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(PlayerInformation));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FindPlayerByEMailRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InPlayer, sizeof(PlayerInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result FindPlayerByEMailRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerInformation &InPlayer )



			void FindPlayerByEMailRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:FindPlayerByEMailRes:{1}:{2} , Result:{3:X8}, Player:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_Player); 
			}; // void FindPlayerByEMailRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Query playerID list
			const MessageID FindPlayerByPlayerIDCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 18);
			Result FindPlayerByPlayerIDCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result FindPlayerByPlayerIDCmd::ParseMessage( MessageData* pIMsg )

			Result FindPlayerByPlayerIDCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FindPlayerByPlayerIDCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result FindPlayerByPlayerIDCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID )



			void FindPlayerByPlayerIDCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:FindPlayerByPlayerIDCmd:{1}:{2} , PlayerID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerID); 
			}; // void FindPlayerByPlayerIDCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID FindPlayerByPlayerIDRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 18);
			Result FindPlayerByPlayerIDRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Player, pCur, iMsgSize, (int)sizeof(PlayerInformation) ) );


			Proc_End:

				return hr;

			}; // Result FindPlayerByPlayerIDRes::ParseMessage( MessageData* pIMsg )

			Result FindPlayerByPlayerIDRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerInformation &InPlayer )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(PlayerInformation));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FindPlayerByPlayerIDRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InPlayer, sizeof(PlayerInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result FindPlayerByPlayerIDRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerInformation &InPlayer )



			void FindPlayerByPlayerIDRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:FindPlayerByPlayerIDRes:{1}:{2} , Result:{3:X8}, Player:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_Player); 
			}; // void FindPlayerByPlayerIDRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: *Request Player Status Update
			const MessageID RequestPlayerStatusUpdateCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 19);
			Result RequestPlayerStatusUpdateCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t numberofTargetPlayerID = 0; PlayerID* pTargetPlayerID = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &numberofTargetPlayerID, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pTargetPlayerID, pCur, iMsgSize, (int)sizeof(PlayerID)*numberofTargetPlayerID ) );
				m_TargetPlayerID.SetLinkedBuffer(numberofTargetPlayerID, numberofTargetPlayerID, pTargetPlayerID);


			Proc_End:

				return hr;

			}; // Result RequestPlayerStatusUpdateCmd::ParseMessage( MessageData* pIMsg )

			Result RequestPlayerStatusUpdateCmd::BuildIMsg( OUT MessageData* &pMsg, const Array<PlayerID>& InTargetPlayerID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)*InTargetPlayerID.GetSize() + sizeof(uint16_t));

				MessageData *pNewMsg = nullptr;

				uint16_t numberOfInTargetPlayerID = (uint16_t)InTargetPlayerID.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::RequestPlayerStatusUpdateCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &numberOfInTargetPlayerID, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InTargetPlayerID.data(), (INT)(sizeof(PlayerID)*InTargetPlayerID.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RequestPlayerStatusUpdateCmd::BuildIMsg( OUT MessageData* &pMsg, const Array<PlayerID>& InTargetPlayerID )



			void RequestPlayerStatusUpdateCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RequestPlayerStatusUpdateCmd:{1}:{2} , TargetPlayerID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TargetPlayerID); 
			}; // void RequestPlayerStatusUpdateCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RequestPlayerStatusUpdateRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 19);
			Result RequestPlayerStatusUpdateRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result RequestPlayerStatusUpdateRes::ParseMessage( MessageData* pIMsg )

			Result RequestPlayerStatusUpdateRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RequestPlayerStatusUpdateRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RequestPlayerStatusUpdateRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void RequestPlayerStatusUpdateRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RequestPlayerStatusUpdateRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void RequestPlayerStatusUpdateRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: *Notify Player Status Updated
			const MessageID NotifyPlayerStatusUpdatedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 20);
			Result NotifyPlayerStatusUpdatedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LatestActiveTime, pCur, iMsgSize, (int)sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_IsInGame, pCur, iMsgSize, (int)sizeof(uint8_t) ) );


			Proc_End:

				return hr;

			}; // Result NotifyPlayerStatusUpdatedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result NotifyPlayerStatusUpdatedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)
					+ sizeof(uint32_t)
					+ sizeof(uint8_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::NotifyPlayerStatusUpdatedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InLatestActiveTime, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InIsInGame, sizeof(uint8_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result NotifyPlayerStatusUpdatedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID, const uint32_t &InLatestActiveTime, const uint8_t &InIsInGame )



			void NotifyPlayerStatusUpdatedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:NotifyPlayerStatusUpdatedS2CEvt:{1}:{2} , PlayerID:{3}, LatestActiveTime:{4}, IsInGame:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerID, m_LatestActiveTime, m_IsInGame); 
			}; // void NotifyPlayerStatusUpdatedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Get Ranking lise
			const MessageID GetRankingListCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 21);
			Result GetRankingListCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

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

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(RankingType)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetRankingListCmd::MID, __uiMessageSize ) );

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

			const MessageID GetRankingListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 21);
			Result GetRankingListRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t numberofRanking = 0; TotalRankingPlayerInformation* pRanking = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofRanking, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pRanking, pCur, iMsgSize, (int)sizeof(TotalRankingPlayerInformation)*numberofRanking ) );
				m_Ranking.SetLinkedBuffer(numberofRanking, numberofRanking, pRanking);


			Proc_End:

				return hr;

			}; // Result GetRankingListRes::ParseMessage( MessageData* pIMsg )

			Result GetRankingListRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(TotalRankingPlayerInformation)*InRanking.GetSize() + sizeof(uint16_t));

				MessageData *pNewMsg = nullptr;

				uint16_t numberOfInRanking = (uint16_t)InRanking.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetRankingListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInRanking, sizeof(uint16_t)); 
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

			// Cmd: Game user game play information
			const MessageID GetUserGamePlayerInfoCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 22);
			Result GetUserGamePlayerInfoCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result GetUserGamePlayerInfoCmd::ParseMessage( MessageData* pIMsg )

			Result GetUserGamePlayerInfoCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetUserGamePlayerInfoCmd::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetUserGamePlayerInfoCmd::BuildIMsg( OUT MessageData* &pMsg )



			void GetUserGamePlayerInfoCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetUserGamePlayerInfoCmd:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void GetUserGamePlayerInfoCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetUserGamePlayerInfoRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 22);
			Result GetUserGamePlayerInfoRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Level, pCur, iMsgSize, (int)sizeof(int16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Exp, pCur, iMsgSize, (int)sizeof(int64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameMoney, pCur, iMsgSize, (int)sizeof(int64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Gem, pCur, iMsgSize, (int)sizeof(int64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Stamina, pCur, iMsgSize, (int)sizeof(int16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LastUpdateTime, pCur, iMsgSize, (int)sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalPlayed, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySC, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySM, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySS, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySC, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySM, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySS, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNC, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNM, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNS, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNC, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNM, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNS, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WeeklyWin, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WeeklyLose, pCur, iMsgSize, (int)sizeof(int32_t) ) );


			Proc_End:

				return hr;

			}; // Result GetUserGamePlayerInfoRes::ParseMessage( MessageData* pIMsg )

			Result GetUserGamePlayerInfoRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const int16_t &InLevel, const int64_t &InExp, const int64_t &InGameMoney, const int64_t &InGem, const int16_t &InStamina, const uint32_t &InLastUpdateTime, const int32_t &InTotalPlayed, const int32_t &InWinPlaySC, const int32_t &InWinPlaySM, const int32_t &InWinPlaySS, const int32_t &InLosePlaySC, const int32_t &InLosePlaySM, const int32_t &InLosePlaySS, const int32_t &InWinPlayNC, const int32_t &InWinPlayNM, const int32_t &InWinPlayNS, const int32_t &InLosePlayNC, const int32_t &InLosePlayNM, const int32_t &InLosePlayNS, const int32_t &InWeeklyWin, const int32_t &InWeeklyLose )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(int16_t)
					+ sizeof(int64_t)
					+ sizeof(int64_t)
					+ sizeof(int64_t)
					+ sizeof(int16_t)
					+ sizeof(uint32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetUserGamePlayerInfoRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InLevel, sizeof(int16_t));
				Protocol::PackParamCopy( pMsgData, &InExp, sizeof(int64_t));
				Protocol::PackParamCopy( pMsgData, &InGameMoney, sizeof(int64_t));
				Protocol::PackParamCopy( pMsgData, &InGem, sizeof(int64_t));
				Protocol::PackParamCopy( pMsgData, &InStamina, sizeof(int16_t));
				Protocol::PackParamCopy( pMsgData, &InLastUpdateTime, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InTotalPlayed, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySC, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySM, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySS, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySC, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySM, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySS, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNC, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNM, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNS, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNC, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNM, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNS, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InWeeklyWin, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InWeeklyLose, sizeof(int32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetUserGamePlayerInfoRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const int16_t &InLevel, const int64_t &InExp, const int64_t &InGameMoney, const int64_t &InGem, const int16_t &InStamina, const uint32_t &InLastUpdateTime, const int32_t &InTotalPlayed, const int32_t &InWinPlaySC, const int32_t &InWinPlaySM, const int32_t &InWinPlaySS, const int32_t &InLosePlaySC, const int32_t &InLosePlaySM, const int32_t &InLosePlaySS, const int32_t &InWinPlayNC, const int32_t &InWinPlayNM, const int32_t &InWinPlayNS, const int32_t &InLosePlayNC, const int32_t &InLosePlayNM, const int32_t &InLosePlayNS, const int32_t &InWeeklyWin, const int32_t &InWeeklyLose )



			void GetUserGamePlayerInfoRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetUserGamePlayerInfoRes:{1}:{2} , Result:{3:X8}, Level:{4}, Exp:{5}, GameMoney:{6}, Gem:{7}, Stamina:{8}, LastUpdateTime:{9}, TotalPlayed:{10}, WinPlaySC:{11}, WinPlaySM:{12}, WinPlaySS:{13}, LosePlaySC:{14}, LosePlaySM:{15}, LosePlaySS:{16}, WinPlayNC:{17}, WinPlayNM:{18}, WinPlayNS:{19}, LosePlayNC:{20}, LosePlayNM:{21}, LosePlayNS:{22}, WeeklyWin:{23}, WeeklyLose:{24}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_Level, m_Exp, m_GameMoney, m_Gem, m_Stamina, m_LastUpdateTime, m_TotalPlayed, m_WinPlaySC, m_WinPlaySM, m_WinPlaySS, m_LosePlaySC, m_LosePlaySM, m_LosePlaySS, m_WinPlayNC, m_WinPlayNM, m_WinPlayNS, m_LosePlayNC, m_LosePlayNM, m_LosePlayNS, m_WeeklyWin, m_WeeklyLose); 
			}; // void GetUserGamePlayerInfoRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Game game play information
			const MessageID GetGamePlayerInfoCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 23);
			Result GetGamePlayerInfoCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result GetGamePlayerInfoCmd::ParseMessage( MessageData* pIMsg )

			Result GetGamePlayerInfoCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetGamePlayerInfoCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetGamePlayerInfoCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID )



			void GetGamePlayerInfoCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetGamePlayerInfoCmd:{1}:{2} , PlayerID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerID); 
			}; // void GetGamePlayerInfoCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetGamePlayerInfoRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 23);
			Result GetGamePlayerInfoRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Level, pCur, iMsgSize, (int)sizeof(int16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalPlayed, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySC, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySM, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySS, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySC, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySM, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySS, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNC, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNM, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNS, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNC, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNM, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNS, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WeeklyWin, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WeeklyLose, pCur, iMsgSize, (int)sizeof(int32_t) ) );


			Proc_End:

				return hr;

			}; // Result GetGamePlayerInfoRes::ParseMessage( MessageData* pIMsg )

			Result GetGamePlayerInfoRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerID &InPlayerID, const int16_t &InLevel, const int32_t &InTotalPlayed, const int32_t &InWinPlaySC, const int32_t &InWinPlaySM, const int32_t &InWinPlaySS, const int32_t &InLosePlaySC, const int32_t &InLosePlaySM, const int32_t &InLosePlaySS, const int32_t &InWinPlayNC, const int32_t &InWinPlayNM, const int32_t &InWinPlayNS, const int32_t &InLosePlayNC, const int32_t &InLosePlayNM, const int32_t &InLosePlayNS, const int32_t &InWeeklyWin, const int32_t &InWeeklyLose )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(PlayerID)
					+ sizeof(int16_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t)
					+ sizeof(int32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetGamePlayerInfoRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InLevel, sizeof(int16_t));
				Protocol::PackParamCopy( pMsgData, &InTotalPlayed, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySC, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySM, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySS, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySC, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySM, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySS, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNC, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNM, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNS, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNC, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNM, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNS, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InWeeklyWin, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InWeeklyLose, sizeof(int32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetGamePlayerInfoRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerID &InPlayerID, const int16_t &InLevel, const int32_t &InTotalPlayed, const int32_t &InWinPlaySC, const int32_t &InWinPlaySM, const int32_t &InWinPlaySS, const int32_t &InLosePlaySC, const int32_t &InLosePlaySM, const int32_t &InLosePlaySS, const int32_t &InWinPlayNC, const int32_t &InWinPlayNM, const int32_t &InWinPlayNS, const int32_t &InLosePlayNC, const int32_t &InLosePlayNM, const int32_t &InLosePlayNS, const int32_t &InWeeklyWin, const int32_t &InWeeklyLose )



			void GetGamePlayerInfoRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetGamePlayerInfoRes:{1}:{2} , Result:{3:X8}, PlayerID:{4}, Level:{5}, TotalPlayed:{6}, WinPlaySC:{7}, WinPlaySM:{8}, WinPlaySS:{9}, LosePlaySC:{10}, LosePlaySM:{11}, LosePlaySS:{12}, WinPlayNC:{13}, WinPlayNM:{14}, WinPlayNS:{15}, LosePlayNC:{16}, LosePlayNM:{17}, LosePlayNS:{18}, WeeklyWin:{19}, WeeklyLose:{20}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_PlayerID, m_Level, m_TotalPlayed, m_WinPlaySC, m_WinPlaySM, m_WinPlaySS, m_LosePlaySC, m_LosePlaySM, m_LosePlaySS, m_WinPlayNC, m_WinPlayNM, m_WinPlayNS, m_LosePlayNC, m_LosePlayNM, m_LosePlayNS, m_WeeklyWin, m_WeeklyLose); 
			}; // void GetGamePlayerInfoRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player level up event
			const MessageID LevelUpS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 24);
			Result LevelUpS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_CurrentTotalExp, pCur, iMsgSize, (int)sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_CurrentLevel, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result LevelUpS2CEvt::ParseMessage( MessageData* pIMsg )

			Result LevelUpS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const uint64_t &InCurrentTotalExp, const uint32_t &InCurrentLevel )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(uint64_t)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::LevelUpS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InCurrentTotalExp, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InCurrentLevel, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result LevelUpS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const uint64_t &InCurrentTotalExp, const uint32_t &InCurrentLevel )



			void LevelUpS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:LevelUpS2CEvt:{1}:{2} , CurrentTotalExp:{3}, CurrentLevel:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_CurrentTotalExp, m_CurrentLevel); 
			}; // void LevelUpS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Change NickName
			const MessageID SetNickNameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 25);
			Result SetNickNameCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfNickName = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &uiSizeOfNickName, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_NickName, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfNickName ) );
				protocolChk( Protocol::StreamParamCopy( &m_IsCostFree, pCur, iMsgSize, (int)sizeof(uint8_t) ) );


			Proc_End:

				return hr;

			}; // Result SetNickNameCmd::ParseMessage( MessageData* pIMsg )

			Result SetNickNameCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InNickName, const uint8_t &InIsCostFree )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInNickNameLength = InNickName ? (uint16_t)(strlen(InNickName)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(uint16_t) + __uiInNickNameLength 
					+ sizeof(uint8_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetNickNameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInNickNameLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InNickName ? InNickName : "", __uiInNickNameLength );
				Protocol::PackParamCopy( pMsgData, &InIsCostFree, sizeof(uint8_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result SetNickNameCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InNickName, const uint8_t &InIsCostFree )



			void SetNickNameCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:SetNickNameCmd:{1}:{2} , NickName:{3,60}, IsCostFree:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NickName, m_IsCostFree); 
			}; // void SetNickNameCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID SetNickNameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 25);
			Result SetNickNameRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, (int)sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, (int)sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result SetNickNameRes::ParseMessage( MessageData* pIMsg )

			Result SetNickNameRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(uint64_t)
					+ sizeof(uint64_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetNickNameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(uint64_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result SetNickNameRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney )



			void SetNickNameRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:SetNickNameRes:{1}:{2} , Result:{3:X8}, TotalGem:{4}, TotalGameMoney:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_TotalGem, m_TotalGameMoney); 
			}; // void SetNickNameRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Create Party
			const MessageID CreatePartyCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 26);
			Result CreatePartyCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result CreatePartyCmd::ParseMessage( MessageData* pIMsg )

			Result CreatePartyCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::CreatePartyCmd::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result CreatePartyCmd::BuildIMsg( OUT MessageData* &pMsg )



			void CreatePartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:CreatePartyCmd:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void CreatePartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID CreatePartyRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 26);
			Result CreatePartyRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, (int)sizeof(PartyUID) ) );


			Proc_End:

				return hr;

			}; // Result CreatePartyRes::ParseMessage( MessageData* pIMsg )

			Result CreatePartyRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PartyUID &InPartyUID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(PartyUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::CreatePartyRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result CreatePartyRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PartyUID &InPartyUID )



			void CreatePartyRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:CreatePartyRes:{1}:{2} , Result:{3:X8}, PartyUID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_PartyUID); 
			}; // void CreatePartyRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Join party
			const MessageID JoinPartyCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 27);
			Result JoinPartyCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, (int)sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InviterID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result JoinPartyCmd::ParseMessage( MessageData* pIMsg )

			Result JoinPartyCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InInviterID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinPartyCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InInviterID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result JoinPartyCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InInviterID )



			void JoinPartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:JoinPartyCmd:{1}:{2} , PartyUID:{3}, InviterID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_InviterID); 
			}; // void JoinPartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID JoinPartyRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 27);
			Result JoinPartyRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t numberofChatHistoryData = 0; uint8_t* pChatHistoryData = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, (int)sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyLeaderID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofChatHistoryData, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pChatHistoryData, pCur, iMsgSize, (int)sizeof(uint8_t)*numberofChatHistoryData ) );
				m_ChatHistoryData.SetLinkedBuffer(numberofChatHistoryData, numberofChatHistoryData, pChatHistoryData);


			Proc_End:

				return hr;

			}; // Result JoinPartyRes::ParseMessage( MessageData* pIMsg )

			Result JoinPartyRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const Array<uint8_t>& InChatHistoryData )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(PartyUID)
					+ sizeof(PlayerID)
					+ sizeof(uint8_t)*InChatHistoryData.GetSize() + sizeof(uint16_t));

				MessageData *pNewMsg = nullptr;

				uint16_t numberOfInChatHistoryData = (uint16_t)InChatHistoryData.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinPartyRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InPartyLeaderID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &numberOfInChatHistoryData, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InChatHistoryData.data(), (INT)(sizeof(uint8_t)*InChatHistoryData.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result JoinPartyRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const Array<uint8_t>& InChatHistoryData )



			void JoinPartyRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:JoinPartyRes:{1}:{2} , Result:{3:X8}, PartyUID:{4}, PartyLeaderID:{5}, ChatHistoryData:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_PartyUID, m_PartyLeaderID, m_ChatHistoryData); 
			}; // void JoinPartyRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Joined event
			const MessageID PartyPlayerJoinedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 28);
			Result PartyPlayerJoinedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, (int)sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayer, pCur, iMsgSize, (int)sizeof(PlayerInformation) ) );


			Proc_End:

				return hr;

			}; // Result PartyPlayerJoinedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result PartyPlayerJoinedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerInformation &InJoinedPlayer )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerInformation));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyPlayerJoinedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayer, sizeof(PlayerInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyPlayerJoinedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerInformation &InJoinedPlayer )



			void PartyPlayerJoinedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyPlayerJoinedS2CEvt:{1}:{2} , PartyUID:{3}, JoinedPlayer:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_JoinedPlayer); 
			}; // void PartyPlayerJoinedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party leader changed event
			const MessageID PartyLeaderChangedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 29);
			Result PartyLeaderChangedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, (int)sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NewLeaderID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PartyLeaderChangedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result PartyLeaderChangedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InNewLeaderID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyLeaderChangedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InNewLeaderID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyLeaderChangedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InNewLeaderID )



			void PartyLeaderChangedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyLeaderChangedS2CEvt:{1}:{2} , PartyUID:{3}, NewLeaderID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_NewLeaderID); 
			}; // void PartyLeaderChangedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Leave party command
			const MessageID LeavePartyCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 30);
			Result LeavePartyCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, (int)sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result LeavePartyCmd::ParseMessage( MessageData* pIMsg )

			Result LeavePartyCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InPlayerID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::LeavePartyCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result LeavePartyCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InPlayerID )



			void LeavePartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:LeavePartyCmd:{1}:{2} , PartyUID:{3}, PlayerID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_PlayerID); 
			}; // void LeavePartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID LeavePartyRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 30);
			Result LeavePartyRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result LeavePartyRes::ParseMessage( MessageData* pIMsg )

			Result LeavePartyRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::LeavePartyRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result LeavePartyRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void LeavePartyRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:LeavePartyRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void LeavePartyRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party Player left event
			const MessageID PartyPlayerLeftS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 31);
			Result PartyPlayerLeftS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, (int)sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LeftPlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PartyPlayerLeftS2CEvt::ParseMessage( MessageData* pIMsg )

			Result PartyPlayerLeftS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InLeftPlayerID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyPlayerLeftS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InLeftPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyPlayerLeftS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InLeftPlayerID )



			void PartyPlayerLeftS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyPlayerLeftS2CEvt:{1}:{2} , PartyUID:{3}, LeftPlayerID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_LeftPlayerID); 
			}; // void PartyPlayerLeftS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Kick player from the party
			const MessageID PartyKickPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 32);
			Result PartyKickPlayerCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, (int)sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerToKick, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PartyKickPlayerCmd::ParseMessage( MessageData* pIMsg )

			Result PartyKickPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyKickPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPlayerToKick, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyKickPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )



			void PartyKickPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyKickPlayerCmd:{1}:{2} , PartyUID:{3}, PlayerID:{4}, PlayerToKick:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_PlayerID, m_PlayerToKick); 
			}; // void PartyKickPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID PartyKickPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 32);
			Result PartyKickPlayerRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result PartyKickPlayerRes::ParseMessage( MessageData* pIMsg )

			Result PartyKickPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyKickPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyKickPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void PartyKickPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyKickPlayerRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void PartyKickPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party Player kicked message
			const MessageID PartyPlayerKickedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 33);
			Result PartyPlayerKickedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, (int)sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_KickedPlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PartyPlayerKickedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result PartyPlayerKickedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InKickedPlayerID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyPlayerKickedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InKickedPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyPlayerKickedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InKickedPlayerID )



			void PartyPlayerKickedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyPlayerKickedS2CEvt:{1}:{2} , PartyUID:{3}, KickedPlayerID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_KickedPlayerID); 
			}; // void PartyPlayerKickedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Invite a player to the party
			const MessageID PartyInviteCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 34);
			Result PartyInviteCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InviteTargetID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PartyInviteCmd::ParseMessage( MessageData* pIMsg )

			Result PartyInviteCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviteTargetID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyInviteCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInviteTargetID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyInviteCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviteTargetID )



			void PartyInviteCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyInviteCmd:{1}:{2} , InviteTargetID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InviteTargetID); 
			}; // void PartyInviteCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID PartyInviteRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 34);
			Result PartyInviteRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result PartyInviteRes::ParseMessage( MessageData* pIMsg )

			Result PartyInviteRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyInviteRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyInviteRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void PartyInviteRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyInviteRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void PartyInviteRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party invite requested
			const MessageID PartyInviteRequestedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 35);
			Result PartyInviteRequestedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfInviterName = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InviterID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfInviterName, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_InviterName, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfInviterName ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyToJoinUID, pCur, iMsgSize, (int)sizeof(PartyUID) ) );


			Proc_End:

				return hr;

			}; // Result PartyInviteRequestedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result PartyInviteRequestedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviterID, const char* InInviterName, const PartyUID &InPartyToJoinUID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInInviterNameLength = InInviterName ? (uint16_t)(strlen(InInviterName)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(uint16_t) + __uiInInviterNameLength 
					+ sizeof(PlayerID)
					+ sizeof(PartyUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyInviteRequestedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInviterID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &__uiInInviterNameLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InInviterName ? InInviterName : "", __uiInInviterNameLength );
				Protocol::PackParamCopy( pMsgData, &InPartyToJoinUID, sizeof(PartyUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyInviteRequestedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviterID, const char* InInviterName, const PartyUID &InPartyToJoinUID )



			void PartyInviteRequestedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyInviteRequestedS2CEvt:{1}:{2} , InviterID:{3}, InviterName:{4,60}, PartyToJoinUID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InviterID, m_InviterName, m_PartyToJoinUID); 
			}; // void PartyInviteRequestedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Send Party quick chat message
			const MessageID PartyQuickChatMessageCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 36);
			Result PartyQuickChatMessageCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_QuickChatID, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result PartyQuickChatMessageCmd::ParseMessage( MessageData* pIMsg )

			Result PartyQuickChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const uint32_t &InQuickChatID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyQuickChatMessageCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InQuickChatID, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyQuickChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const uint32_t &InQuickChatID )



			void PartyQuickChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyQuickChatMessageCmd:{1}:{2} , QuickChatID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_QuickChatID); 
			}; // void PartyQuickChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID PartyQuickChatMessageRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 36);
			Result PartyQuickChatMessageRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result PartyQuickChatMessageRes::ParseMessage( MessageData* pIMsg )

			Result PartyQuickChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyQuickChatMessageRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyQuickChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void PartyQuickChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyQuickChatMessageRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void PartyQuickChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party Chatting message event
			const MessageID PartyQuickChatMessageS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 37);
			Result PartyQuickChatMessageS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_SenderID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_QuickChatID, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result PartyQuickChatMessageS2CEvt::ParseMessage( MessageData* pIMsg )

			Result PartyQuickChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const uint32_t &InQuickChatID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyQuickChatMessageS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InSenderID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InQuickChatID, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyQuickChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const uint32_t &InQuickChatID )



			void PartyQuickChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyQuickChatMessageS2CEvt:{1}:{2} , SenderID:{3}, QuickChatID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_SenderID, m_QuickChatID); 
			}; // void PartyQuickChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Party chatting
			const MessageID PartyChatMessageCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 38);
			Result PartyChatMessageCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfChatMessage = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfChatMessage ) );


			Proc_End:

				return hr;

			}; // Result PartyChatMessageCmd::ParseMessage( MessageData* pIMsg )

			Result PartyChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InChatMessage )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInChatMessageLength = InChatMessage ? (uint16_t)(strlen(InChatMessage)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(uint16_t) + __uiInChatMessageLength );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyChatMessageCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InChatMessage )



			void PartyChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyChatMessageCmd:{1}:{2} , ChatMessage:{3,60}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_ChatMessage); 
			}; // void PartyChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID PartyChatMessageRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 38);
			Result PartyChatMessageRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result PartyChatMessageRes::ParseMessage( MessageData* pIMsg )

			Result PartyChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyChatMessageRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void PartyChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyChatMessageRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void PartyChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party Chatting message event
			const MessageID PartyChatMessageS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 39);
			Result PartyChatMessageS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfSenderName = 0;
				uint16_t uiSizeOfChatMessage = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_SenderID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfSenderName, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_SenderName, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfSenderName ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfChatMessage ) );


			Proc_End:

				return hr;

			}; // Result PartyChatMessageS2CEvt::ParseMessage( MessageData* pIMsg )

			Result PartyChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInSenderNameLength = InSenderName ? (uint16_t)(strlen(InSenderName)+1) : 1;
				uint16_t __uiInChatMessageLength = InChatMessage ? (uint16_t)(strlen(InChatMessage)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(uint16_t) + __uiInSenderNameLength + sizeof(uint16_t) + __uiInChatMessageLength 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyChatMessageS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InSenderID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &__uiInSenderNameLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InSenderName ? InSenderName : "", __uiInSenderNameLength );
				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )



			void PartyChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyChatMessageS2CEvt:{1}:{2} , SenderID:{3}, SenderName:{4,60}, ChatMessage:{5,60}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_SenderID, m_SenderName, m_ChatMessage); 
			}; // void PartyChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Join to a game
			const MessageID JoinGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 40);
			Result JoinGameCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, (int)sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );


			Proc_End:

				return hr;

			}; // Result JoinGameCmd::ParseMessage( MessageData* pIMsg )

			Result JoinGameCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID, const AuthTicket &InTicket, const GameInsUID &InInsUID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket)
					+ sizeof(GameInsUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InInsUID, sizeof(GameInsUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result JoinGameCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID, const AuthTicket &InTicket, const GameInsUID &InInsUID )



			void JoinGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:JoinGameCmd:{1}:{2} , PlayerID:{3}, Ticket:{4}, InsUID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerID, m_Ticket, m_InsUID); 
			}; // void JoinGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID JoinGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 40);
			Result JoinGameRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t numberofChatHistoryData = 0; uint8_t* pChatHistoryData = nullptr;
				uint16_t numberofGameLogData = 0; uint8_t* pGameLogData = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, (int)sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameState, pCur, iMsgSize, (int)sizeof(GameStateID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Day, pCur, iMsgSize, (int)sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MaxPlayer, pCur, iMsgSize, (int)sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerIndex, pCur, iMsgSize, (int)sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerCharacter, pCur, iMsgSize, (int)sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, (int)sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Dead, pCur, iMsgSize, (int)sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofChatHistoryData, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pChatHistoryData, pCur, iMsgSize, (int)sizeof(uint8_t)*numberofChatHistoryData ) );
				m_ChatHistoryData.SetLinkedBuffer(numberofChatHistoryData, numberofChatHistoryData, pChatHistoryData);
				protocolChk( Protocol::StreamParamCopy( &numberofGameLogData, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pGameLogData, pCur, iMsgSize, (int)sizeof(uint8_t)*numberofGameLogData ) );
				m_GameLogData.SetLinkedBuffer(numberofGameLogData, numberofGameLogData, pGameLogData);


			Proc_End:

				return hr;

			}; // Result JoinGameRes::ParseMessage( MessageData* pIMsg )

			Result JoinGameRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const GameInsUID &InInsUID, const uint32_t &InTimeStamp, const GameStateID &InGameState, const uint8_t &InDay, const uint8_t &InMaxPlayer, const uint8_t &InPlayerIndex, const uint8_t &InPlayerCharacter, const PlayerRole &InRole, const uint8_t &InDead, const Array<uint8_t>& InChatHistoryData, const Array<uint8_t>& InGameLogData )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(GameInsUID)
					+ sizeof(uint32_t)
					+ sizeof(GameStateID)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)
					+ sizeof(PlayerRole)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)*InChatHistoryData.GetSize() + sizeof(uint16_t)
					+ sizeof(uint8_t)*InGameLogData.GetSize() + sizeof(uint16_t));

				MessageData *pNewMsg = nullptr;

				uint16_t numberOfInChatHistoryData = (uint16_t)InChatHistoryData.GetSize(); 
				uint16_t numberOfInGameLogData = (uint16_t)InGameLogData.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinGameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InGameState, sizeof(GameStateID));
				Protocol::PackParamCopy( pMsgData, &InDay, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InMaxPlayer, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InPlayerIndex, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InPlayerCharacter, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &InDead, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &numberOfInChatHistoryData, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InChatHistoryData.data(), (INT)(sizeof(uint8_t)*InChatHistoryData.GetSize())); 
				Protocol::PackParamCopy( pMsgData, &numberOfInGameLogData, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InGameLogData.data(), (INT)(sizeof(uint8_t)*InGameLogData.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result JoinGameRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const GameInsUID &InInsUID, const uint32_t &InTimeStamp, const GameStateID &InGameState, const uint8_t &InDay, const uint8_t &InMaxPlayer, const uint8_t &InPlayerIndex, const uint8_t &InPlayerCharacter, const PlayerRole &InRole, const uint8_t &InDead, const Array<uint8_t>& InChatHistoryData, const Array<uint8_t>& InGameLogData )



			void JoinGameRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:JoinGameRes:{1}:{2} , Result:{3:X8}, InsUID:{4}, TimeStamp:{5}, GameState:{6}, Day:{7}, MaxPlayer:{8}, PlayerIndex:{9}, PlayerCharacter:{10}, Role:{11}, Dead:{12}, ChatHistoryData:{13}, GameLogData:{14}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_InsUID, m_TimeStamp, m_GameState, m_Day, m_MaxPlayer, m_PlayerIndex, m_PlayerCharacter, m_Role, m_Dead, m_ChatHistoryData, m_GameLogData); 
			}; // void JoinGameRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Joined in the game
			const MessageID PlayerJoinedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 41);
			Result PlayerJoinedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayer, pCur, iMsgSize, (int)sizeof(PlayerInformation) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerRole, pCur, iMsgSize, (int)sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerDead, pCur, iMsgSize, (int)sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerIndex, pCur, iMsgSize, (int)sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerCharacter, pCur, iMsgSize, (int)sizeof(uint8_t) ) );


			Proc_End:

				return hr;

			}; // Result PlayerJoinedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result PlayerJoinedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const uint8_t &InJoinedPlayerDead, const uint8_t &InJoinedPlayerIndex, const uint8_t &InJoinedPlayerCharacter )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerInformation)
					+ sizeof(PlayerRole)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PlayerJoinedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayer, sizeof(PlayerInformation));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerDead, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerIndex, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerCharacter, sizeof(uint8_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PlayerJoinedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const uint8_t &InJoinedPlayerDead, const uint8_t &InJoinedPlayerIndex, const uint8_t &InJoinedPlayerCharacter )



			void PlayerJoinedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PlayerJoinedS2CEvt:{1}:{2} , GameInsUID:{3}, JoinedPlayer:{4}, JoinedPlayerRole:{5}, JoinedPlayerDead:{6}, JoinedPlayerIndex:{7}, JoinedPlayerCharacter:{8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_JoinedPlayer, m_JoinedPlayerRole, m_JoinedPlayerDead, m_JoinedPlayerIndex, m_JoinedPlayerCharacter); 
			}; // void PlayerJoinedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Leave Game
			const MessageID LeaveGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 42);
			Result LeaveGameCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, (int)sizeof(AuthTicket) ) );


			Proc_End:

				return hr;

			}; // Result LeaveGameCmd::ParseMessage( MessageData* pIMsg )

			Result LeaveGameCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::LeaveGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result LeaveGameCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )



			void LeaveGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:LeaveGameCmd:{1}:{2} , GameInsUID:{3}, PlayerID:{4}, Ticket:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Ticket); 
			}; // void LeaveGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID LeaveGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 42);
			Result LeaveGameRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result LeaveGameRes::ParseMessage( MessageData* pIMsg )

			Result LeaveGameRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::LeaveGameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result LeaveGameRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void LeaveGameRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:LeaveGameRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void LeaveGameRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player left event
			const MessageID PlayerLeftS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 43);
			Result PlayerLeftS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LeftPlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PlayerLeftS2CEvt::ParseMessage( MessageData* pIMsg )

			Result PlayerLeftS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InLeftPlayerID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PlayerLeftS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InLeftPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PlayerLeftS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InLeftPlayerID )



			void PlayerLeftS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PlayerLeftS2CEvt:{1}:{2} , GameInsUID:{3}, LeftPlayerID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_LeftPlayerID); 
			}; // void PlayerLeftS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Kick player
			const MessageID KickPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 44);
			Result KickPlayerCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerToKick, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result KickPlayerCmd::ParseMessage( MessageData* pIMsg )

			Result KickPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::KickPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPlayerToKick, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result KickPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )



			void KickPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:KickPlayerCmd:{1}:{2} , GameInsUID:{3}, PlayerID:{4}, PlayerToKick:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_PlayerToKick); 
			}; // void KickPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID KickPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 44);
			Result KickPlayerRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result KickPlayerRes::ParseMessage( MessageData* pIMsg )

			Result KickPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::KickPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result KickPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void KickPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:KickPlayerRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void KickPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player kicked
			const MessageID PlayerKickedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 45);
			Result PlayerKickedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_KickedPlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PlayerKickedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result PlayerKickedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InKickedPlayerID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PlayerKickedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InKickedPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PlayerKickedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InKickedPlayerID )



			void PlayerKickedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PlayerKickedS2CEvt:{1}:{2} , GameInsUID:{3}, KickedPlayerID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_KickedPlayerID); 
			}; // void PlayerKickedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Assign role + Game state reset
			const MessageID AssignRoleCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 46);
			Result AssignRoleCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, (int)sizeof(AuthTicket) ) );


			Proc_End:

				return hr;

			}; // Result AssignRoleCmd::ParseMessage( MessageData* pIMsg )

			Result AssignRoleCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AssignRoleCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AssignRoleCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )



			void AssignRoleCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AssignRoleCmd:{1}:{2} , GameInsUID:{3}, PlayerID:{4}, Ticket:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Ticket); 
			}; // void AssignRoleCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID AssignRoleRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 46);
			Result AssignRoleRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result AssignRoleRes::ParseMessage( MessageData* pIMsg )

			Result AssignRoleRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AssignRoleRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AssignRoleRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void AssignRoleRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AssignRoleRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void AssignRoleRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Role assigned event
			const MessageID RoleAssignedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 47);
			Result RoleAssignedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, (int)sizeof(PlayerRole) ) );


			Proc_End:

				return hr;

			}; // Result RoleAssignedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result RoleAssignedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerRole &InRole )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerRole));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RoleAssignedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RoleAssignedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerRole &InRole )



			void RoleAssignedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RoleAssignedS2CEvt:{1}:{2} , GameInsUID:{3}, PlayerID:{4}, Role:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Role); 
			}; // void RoleAssignedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Send chatting message to the game
			const MessageID ChatMessageCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 48);
			Result ChatMessageCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfChatMessage = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfChatMessage ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, (int)sizeof(PlayerRole) ) );


			Proc_End:

				return hr;

			}; // Result ChatMessageCmd::ParseMessage( MessageData* pIMsg )

			Result ChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InChatMessage, const PlayerRole &InRole )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInChatMessageLength = InChatMessage ? (uint16_t)(strlen(InChatMessage)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(uint16_t) + __uiInChatMessageLength 
					+ sizeof(PlayerRole));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::ChatMessageCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result ChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InChatMessage, const PlayerRole &InRole )



			void ChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:ChatMessageCmd:{1}:{2} , ChatMessage:{3,60}, Role:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_ChatMessage, m_Role); 
			}; // void ChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID ChatMessageRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 48);
			Result ChatMessageRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result ChatMessageRes::ParseMessage( MessageData* pIMsg )

			Result ChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::ChatMessageRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result ChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void ChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:ChatMessageRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void ChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Chatting message event 
			const MessageID ChatMessageS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 49);
			Result ChatMessageS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfSenderName = 0;
				uint16_t uiSizeOfChatMessage = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_SenderID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, (int)sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfSenderName, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_SenderName, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfSenderName ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfChatMessage ) );


			Proc_End:

				return hr;

			}; // Result ChatMessageS2CEvt::ParseMessage( MessageData* pIMsg )

			Result ChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInSenderNameLength = InSenderName ? (uint16_t)(strlen(InSenderName)+1) : 1;
				uint16_t __uiInChatMessageLength = InChatMessage ? (uint16_t)(strlen(InChatMessage)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(uint16_t) + __uiInSenderNameLength + sizeof(uint16_t) + __uiInChatMessageLength 
					+ sizeof(PlayerID)
					+ sizeof(PlayerRole));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::ChatMessageS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InSenderID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &__uiInSenderNameLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InSenderName ? InSenderName : "", __uiInSenderNameLength );
				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result ChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )



			void ChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:ChatMessageS2CEvt:{1}:{2} , SenderID:{3}, Role:{4}, SenderName:{5,60}, ChatMessage:{6,60}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_SenderID, m_Role, m_SenderName, m_ChatMessage); 
			}; // void ChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Advance game
			const MessageID AdvanceGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 50);
			Result AdvanceGameCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, (int)sizeof(AuthTicket) ) );


			Proc_End:

				return hr;

			}; // Result AdvanceGameCmd::ParseMessage( MessageData* pIMsg )

			Result AdvanceGameCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AdvanceGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AdvanceGameCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )



			void AdvanceGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AdvanceGameCmd:{1}:{2} , GameInsUID:{3}, PlayerID:{4}, Ticket:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Ticket); 
			}; // void AdvanceGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID AdvanceGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 50);
			Result AdvanceGameRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result AdvanceGameRes::ParseMessage( MessageData* pIMsg )

			Result AdvanceGameRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AdvanceGameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AdvanceGameRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void AdvanceGameRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AdvanceGameRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void AdvanceGameRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: The game state is advanced
			const MessageID GameAdvancedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 51);
			Result GameAdvancedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, (int)sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameState, pCur, iMsgSize, (int)sizeof(GameStateID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Day, pCur, iMsgSize, (int)sizeof(uint8_t) ) );


			Proc_End:

				return hr;

			}; // Result GameAdvancedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result GameAdvancedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const uint32_t &InTimeStamp, const GameStateID &InGameState, const uint8_t &InDay )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(uint32_t)
					+ sizeof(GameStateID)
					+ sizeof(uint8_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameAdvancedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InGameState, sizeof(GameStateID));
				Protocol::PackParamCopy( pMsgData, &InDay, sizeof(uint8_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameAdvancedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const uint32_t &InTimeStamp, const GameStateID &InGameState, const uint8_t &InDay )



			void GameAdvancedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameAdvancedS2CEvt:{1}:{2} , GameInsUID:{3}, TimeStamp:{4}, GameState:{5}, Day:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_TimeStamp, m_GameState, m_Day); 
			}; // void GameAdvancedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Game is ended
			const MessageID GameEndedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 52);
			Result GameEndedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Winner, pCur, iMsgSize, (int)sizeof(GameWinner) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GainedExp, pCur, iMsgSize, (int)sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GainedGameMoney, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result GameEndedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result GameEndedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const GameWinner &InWinner, const uint32_t &InGainedExp, const uint32_t &InGainedGameMoney )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(GameWinner)
					+ sizeof(uint32_t)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameEndedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InWinner, sizeof(GameWinner));
				Protocol::PackParamCopy( pMsgData, &InGainedExp, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InGainedGameMoney, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameEndedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const GameWinner &InWinner, const uint32_t &InGainedExp, const uint32_t &InGainedGameMoney )



			void GameEndedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameEndedS2CEvt:{1}:{2} , GameInsUID:{3}, Winner:{4}, GainedExp:{5}, GainedGameMoney:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_Winner, m_GainedExp, m_GainedGameMoney); 
			}; // void GameEndedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Vote game advance
			const MessageID VoteGameAdvanceCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 53);
			Result VoteGameAdvanceCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, (int)sizeof(AuthTicket) ) );


			Proc_End:

				return hr;

			}; // Result VoteGameAdvanceCmd::ParseMessage( MessageData* pIMsg )

			Result VoteGameAdvanceCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::VoteGameAdvanceCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result VoteGameAdvanceCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )



			void VoteGameAdvanceCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:VoteGameAdvanceCmd:{1}:{2} , GameInsUID:{3}, PlayerID:{4}, Ticket:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Ticket); 
			}; // void VoteGameAdvanceCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID VoteGameAdvanceRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 53);
			Result VoteGameAdvanceRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result VoteGameAdvanceRes::ParseMessage( MessageData* pIMsg )

			Result VoteGameAdvanceRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::VoteGameAdvanceRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result VoteGameAdvanceRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void VoteGameAdvanceRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:VoteGameAdvanceRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void VoteGameAdvanceRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: GameAdvance is Voted
			const MessageID GameAdvanceVotedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 54);
			Result GameAdvanceVotedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Voter, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result GameAdvanceVotedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result GameAdvanceVotedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InVoter )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameAdvanceVotedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InVoter, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameAdvanceVotedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InVoter )



			void GameAdvanceVotedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameAdvanceVotedS2CEvt:{1}:{2} , GameInsUID:{3}, Voter:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_Voter); 
			}; // void GameAdvanceVotedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Vote
			const MessageID VoteCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 55);
			Result VoteCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, (int)sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_VoteTarget, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ActionSerial, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result VoteCmd::ParseMessage( MessageData* pIMsg )

			Result VoteCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const PlayerID &InVoteTarget, const uint32_t &InActionSerial )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket)
					+ sizeof(PlayerID)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::VoteCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InVoteTarget, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InActionSerial, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result VoteCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const PlayerID &InVoteTarget, const uint32_t &InActionSerial )



			void VoteCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:VoteCmd:{1}:{2} , GameInsUID:{3}, PlayerID:{4}, Ticket:{5}, VoteTarget:{6}, ActionSerial:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Ticket, m_VoteTarget, m_ActionSerial); 
			}; // void VoteCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID VoteRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 55);
			Result VoteRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result VoteRes::ParseMessage( MessageData* pIMsg )

			Result VoteRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::VoteRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result VoteRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void VoteRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:VoteRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void VoteRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Voted
			const MessageID VotedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 56);
			Result VotedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Voter, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_VotedTarget, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result VotedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result VotedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InVoter, const PlayerID &InVotedTarget )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::VotedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InVoter, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InVotedTarget, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result VotedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InVoter, const PlayerID &InVotedTarget )



			void VotedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:VotedS2CEvt:{1}:{2} , GameInsUID:{3}, Voter:{4}, VotedTarget:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_Voter, m_VotedTarget); 
			}; // void VotedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Vote is ended
			const MessageID VoteEndS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 57);
			Result VoteEndS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t numberofVoted = 0; PlayerID* pVoted = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofVoted, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pVoted, pCur, iMsgSize, (int)sizeof(PlayerID)*numberofVoted ) );
				m_Voted.SetLinkedBuffer(numberofVoted, numberofVoted, pVoted);


			Proc_End:

				return hr;

			}; // Result VoteEndS2CEvt::ParseMessage( MessageData* pIMsg )

			Result VoteEndS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const Array<PlayerID>& InVoted )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)*InVoted.GetSize() + sizeof(uint16_t));

				MessageData *pNewMsg = nullptr;

				uint16_t numberOfInVoted = (uint16_t)InVoted.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::VoteEndS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &numberOfInVoted, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InVoted.data(), (INT)(sizeof(PlayerID)*InVoted.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result VoteEndS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const Array<PlayerID>& InVoted )



			void VoteEndS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:VoteEndS2CEvt:{1}:{2} , GameInsUID:{3}, Voted:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_Voted); 
			}; // void VoteEndS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Killed
			const MessageID PlayerKilledS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 58);
			Result PlayerKilledS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_KilledPlayer, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Reason, pCur, iMsgSize, (int)sizeof(PlayerKilledReason) ) );


			Proc_End:

				return hr;

			}; // Result PlayerKilledS2CEvt::ParseMessage( MessageData* pIMsg )

			Result PlayerKilledS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerKilledReason));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PlayerKilledS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InKilledPlayer, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InReason, sizeof(PlayerKilledReason));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PlayerKilledS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason )



			void PlayerKilledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PlayerKilledS2CEvt:{1}:{2} , GameInsUID:{3}, KilledPlayer:{4}, Reason:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_KilledPlayer, m_Reason); 
			}; // void PlayerKilledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Voted
			const MessageID PlayerRevealedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 59);
			Result PlayerRevealedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RevealedPlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, (int)sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Reason, pCur, iMsgSize, (int)sizeof(PlayerRevealedReason) ) );


			Proc_End:

				return hr;

			}; // Result PlayerRevealedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result PlayerRevealedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerRole)
					+ sizeof(PlayerRevealedReason));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PlayerRevealedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InRevealedPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &InReason, sizeof(PlayerRevealedReason));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PlayerRevealedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason )



			void PlayerRevealedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PlayerRevealedS2CEvt:{1}:{2} , GameInsUID:{3}, RevealedPlayerID:{4}, Role:{5}, Reason:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_RevealedPlayerID, m_Role, m_Reason); 
			}; // void PlayerRevealedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Play again with the current players
			const MessageID GamePlayAgainCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 60);
			Result GamePlayAgainCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result GamePlayAgainCmd::ParseMessage( MessageData* pIMsg )

			Result GamePlayAgainCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayAgainCmd::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayAgainCmd::BuildIMsg( OUT MessageData* &pMsg )



			void GamePlayAgainCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayAgainCmd:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void GamePlayAgainCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GamePlayAgainRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 60);
			Result GamePlayAgainRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, (int)sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, (int)sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayAgainRes::ParseMessage( MessageData* pIMsg )

			Result GamePlayAgainRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(uint64_t)
					+ sizeof(uint64_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayAgainRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(uint64_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayAgainRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney )



			void GamePlayAgainRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayAgainRes:{1}:{2} , Result:{3:X8}, TotalGem:{4}, TotalGameMoney:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_TotalGem, m_TotalGameMoney); 
			}; // void GamePlayAgainRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			const MessageID GamePlayAgainS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 61);
			Result GamePlayAgainS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, (int)sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LeadPlayer, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayAgainS2CEvt::ParseMessage( MessageData* pIMsg )

			Result GamePlayAgainS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InLeadPlayer )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayAgainS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InLeadPlayer, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayAgainS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InLeadPlayer )



			void GamePlayAgainS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayAgainS2CEvt:{1}:{2} , PartyUID:{3}, LeadPlayer:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_LeadPlayer); 
			}; // void GamePlayAgainS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Player. reveal a player
			const MessageID GameRevealPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 62);
			Result GameRevealPlayerCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t numberofTargetPlayerID = 0; PlayerID* pTargetPlayerID = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &numberofTargetPlayerID, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pTargetPlayerID, pCur, iMsgSize, (int)sizeof(PlayerID)*numberofTargetPlayerID ) );
				m_TargetPlayerID.SetLinkedBuffer(numberofTargetPlayerID, numberofTargetPlayerID, pTargetPlayerID);


			Proc_End:

				return hr;

			}; // Result GameRevealPlayerCmd::ParseMessage( MessageData* pIMsg )

			Result GameRevealPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const Array<PlayerID>& InTargetPlayerID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)*InTargetPlayerID.GetSize() + sizeof(uint16_t));

				MessageData *pNewMsg = nullptr;

				uint16_t numberOfInTargetPlayerID = (uint16_t)InTargetPlayerID.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameRevealPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &numberOfInTargetPlayerID, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InTargetPlayerID.data(), (INT)(sizeof(PlayerID)*InTargetPlayerID.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameRevealPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const Array<PlayerID>& InTargetPlayerID )



			void GameRevealPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameRevealPlayerCmd:{1}:{2} , TargetPlayerID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TargetPlayerID); 
			}; // void GameRevealPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GameRevealPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 62);
			Result GameRevealPlayerRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t numberofRevealedPlayerID = 0; PlayerID* pRevealedPlayerID = nullptr;
				uint16_t numberofRevealedRole = 0; PlayerRole* pRevealedRole = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofRevealedPlayerID, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pRevealedPlayerID, pCur, iMsgSize, (int)sizeof(PlayerID)*numberofRevealedPlayerID ) );
				m_RevealedPlayerID.SetLinkedBuffer(numberofRevealedPlayerID, numberofRevealedPlayerID, pRevealedPlayerID);
				protocolChk( Protocol::StreamParamCopy( &numberofRevealedRole, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pRevealedRole, pCur, iMsgSize, (int)sizeof(PlayerRole)*numberofRevealedRole ) );
				m_RevealedRole.SetLinkedBuffer(numberofRevealedRole, numberofRevealedRole, pRevealedRole);
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, (int)sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, (int)sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result GameRevealPlayerRes::ParseMessage( MessageData* pIMsg )

			Result GameRevealPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(PlayerID)*InRevealedPlayerID.GetSize() + sizeof(uint16_t)
					+ sizeof(PlayerRole)*InRevealedRole.GetSize() + sizeof(uint16_t)
					+ sizeof(uint64_t)
					+ sizeof(uint64_t));

				MessageData *pNewMsg = nullptr;

				uint16_t numberOfInRevealedPlayerID = (uint16_t)InRevealedPlayerID.GetSize(); 
				uint16_t numberOfInRevealedRole = (uint16_t)InRevealedRole.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameRevealPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInRevealedPlayerID, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InRevealedPlayerID.data(), (INT)(sizeof(PlayerID)*InRevealedPlayerID.GetSize())); 
				Protocol::PackParamCopy( pMsgData, &numberOfInRevealedRole, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InRevealedRole.data(), (INT)(sizeof(PlayerRole)*InRevealedRole.GetSize())); 
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(uint64_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameRevealPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney )



			void GameRevealPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameRevealPlayerRes:{1}:{2} , Result:{3:X8}, RevealedPlayerID:{4}, RevealedRole:{5}, TotalGem:{6}, TotalGameMoney:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_RevealedPlayerID, m_RevealedRole, m_TotalGem, m_TotalGameMoney); 
			}; // void GameRevealPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Player. revive himself
			const MessageID GamePlayerReviveCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 63);
			Result GamePlayerReviveCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result GamePlayerReviveCmd::ParseMessage( MessageData* pIMsg )

			Result GamePlayerReviveCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayerReviveCmd::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayerReviveCmd::BuildIMsg( OUT MessageData* &pMsg )



			void GamePlayerReviveCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayerReviveCmd:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void GamePlayerReviveCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GamePlayerReviveRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 63);
			Result GamePlayerReviveRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, (int)sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, (int)sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayerReviveRes::ParseMessage( MessageData* pIMsg )

			Result GamePlayerReviveRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(uint64_t)
					+ sizeof(uint64_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayerReviveRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(uint64_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayerReviveRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney )



			void GamePlayerReviveRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayerReviveRes:{1}:{2} , Result:{3:X8}, TotalGem:{4}, TotalGameMoney:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_TotalGem, m_TotalGameMoney); 
			}; // void GamePlayerReviveRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player is revived
			const MessageID GamePlayerRevivedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 64);
			Result GamePlayerRevivedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RevivedPlayerID, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayerRevivedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result GamePlayerRevivedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InRevivedPlayerID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayerRevivedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRevivedPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayerRevivedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InRevivedPlayerID )



			void GamePlayerRevivedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayerRevivedS2CEvt:{1}:{2} , RevivedPlayerID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RevivedPlayerID); 
			}; // void GamePlayerRevivedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Player. reset ranking
			const MessageID GamePlayerResetRankCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 65);
			Result GamePlayerResetRankCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result GamePlayerResetRankCmd::ParseMessage( MessageData* pIMsg )

			Result GamePlayerResetRankCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayerResetRankCmd::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayerResetRankCmd::BuildIMsg( OUT MessageData* &pMsg )



			void GamePlayerResetRankCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayerResetRankCmd:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void GamePlayerResetRankCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GamePlayerResetRankRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 65);
			Result GamePlayerResetRankRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, (int)sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, (int)sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayerResetRankRes::ParseMessage( MessageData* pIMsg )

			Result GamePlayerResetRankRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(uint64_t)
					+ sizeof(uint64_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayerResetRankRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(uint64_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayerResetRankRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney )



			void GamePlayerResetRankRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayerResetRankRes:{1}:{2} , Result:{3:X8}, TotalGem:{4}, TotalGameMoney:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_TotalGem, m_TotalGameMoney); 
			}; // void GamePlayerResetRankRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Request Game match
			const MessageID RequestGameMatchCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 66);
			Result RequestGameMatchCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_NumPlayer, pCur, iMsgSize, (int)sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RequestRole, pCur, iMsgSize, (int)sizeof(PlayerRole) ) );


			Proc_End:

				return hr;

			}; // Result RequestGameMatchCmd::ParseMessage( MessageData* pIMsg )

			Result RequestGameMatchCmd::BuildIMsg( OUT MessageData* &pMsg, const uint8_t &InNumPlayer, const PlayerRole &InRequestRole )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(uint8_t)
					+ sizeof(PlayerRole));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RequestGameMatchCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InNumPlayer, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InRequestRole, sizeof(PlayerRole));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RequestGameMatchCmd::BuildIMsg( OUT MessageData* &pMsg, const uint8_t &InNumPlayer, const PlayerRole &InRequestRole )



			void RequestGameMatchCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RequestGameMatchCmd:{1}:{2} , NumPlayer:{3}, RequestRole:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NumPlayer, m_RequestRole); 
			}; // void RequestGameMatchCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RequestGameMatchRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 66);
			Result RequestGameMatchRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, (int)sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, (int)sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result RequestGameMatchRes::ParseMessage( MessageData* pIMsg )

			Result RequestGameMatchRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(uint64_t)
					+ sizeof(uint64_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RequestGameMatchRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(uint64_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RequestGameMatchRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney )



			void RequestGameMatchRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RequestGameMatchRes:{1}:{2} , Result:{3:X8}, TotalGem:{4}, TotalGameMoney:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_TotalGem, m_TotalGameMoney); 
			}; // void RequestGameMatchRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Game matched
			const MessageID GameMatchedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 67);
			Result GameMatchedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t numberofChatHistoryData = 0; uint8_t* pChatHistoryData = nullptr;
				uint16_t numberofGameLogData = 0; uint8_t* pGameLogData = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InsUID, pCur, iMsgSize, (int)sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, (int)sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameState, pCur, iMsgSize, (int)sizeof(GameStateID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Day, pCur, iMsgSize, (int)sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MaxPlayer, pCur, iMsgSize, (int)sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerIndex, pCur, iMsgSize, (int)sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerCharacter, pCur, iMsgSize, (int)sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, (int)sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Dead, pCur, iMsgSize, (int)sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofChatHistoryData, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pChatHistoryData, pCur, iMsgSize, (int)sizeof(uint8_t)*numberofChatHistoryData ) );
				m_ChatHistoryData.SetLinkedBuffer(numberofChatHistoryData, numberofChatHistoryData, pChatHistoryData);
				protocolChk( Protocol::StreamParamCopy( &numberofGameLogData, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pGameLogData, pCur, iMsgSize, (int)sizeof(uint8_t)*numberofGameLogData ) );
				m_GameLogData.SetLinkedBuffer(numberofGameLogData, numberofGameLogData, pGameLogData);
				protocolChk( Protocol::StreamParamCopy( &m_Stamina, pCur, iMsgSize, (int)sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, (int)sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, (int)sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result GameMatchedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result GameMatchedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InInsUID, const uint32_t &InTimeStamp, const GameStateID &InGameState, const uint8_t &InDay, const uint8_t &InMaxPlayer, const uint8_t &InPlayerIndex, const uint8_t &InPlayerCharacter, const PlayerRole &InRole, const uint8_t &InDead, const Array<uint8_t>& InChatHistoryData, const Array<uint8_t>& InGameLogData, const uint32_t &InStamina, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(uint32_t)
					+ sizeof(GameStateID)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)
					+ sizeof(PlayerRole)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)*InChatHistoryData.GetSize() + sizeof(uint16_t)
					+ sizeof(uint8_t)*InGameLogData.GetSize() + sizeof(uint16_t)
					+ sizeof(uint32_t)
					+ sizeof(uint64_t)
					+ sizeof(uint64_t));

				MessageData *pNewMsg = nullptr;

				uint16_t numberOfInChatHistoryData = (uint16_t)InChatHistoryData.GetSize(); 
				uint16_t numberOfInGameLogData = (uint16_t)InGameLogData.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameMatchedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InGameState, sizeof(GameStateID));
				Protocol::PackParamCopy( pMsgData, &InDay, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InMaxPlayer, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InPlayerIndex, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InPlayerCharacter, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &InDead, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &numberOfInChatHistoryData, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InChatHistoryData.data(), (INT)(sizeof(uint8_t)*InChatHistoryData.GetSize())); 
				Protocol::PackParamCopy( pMsgData, &numberOfInGameLogData, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InGameLogData.data(), (INT)(sizeof(uint8_t)*InGameLogData.GetSize())); 
				Protocol::PackParamCopy( pMsgData, &InStamina, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(uint64_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameMatchedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InInsUID, const uint32_t &InTimeStamp, const GameStateID &InGameState, const uint8_t &InDay, const uint8_t &InMaxPlayer, const uint8_t &InPlayerIndex, const uint8_t &InPlayerCharacter, const PlayerRole &InRole, const uint8_t &InDead, const Array<uint8_t>& InChatHistoryData, const Array<uint8_t>& InGameLogData, const uint32_t &InStamina, const uint64_t &InTotalGem, const uint64_t &InTotalGameMoney )



			void GameMatchedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameMatchedS2CEvt:{1}:{2} , InsUID:{3}, TimeStamp:{4}, GameState:{5}, Day:{6}, MaxPlayer:{7}, PlayerIndex:{8}, PlayerCharacter:{9}, Role:{10}, Dead:{11}, ChatHistoryData:{12}, GameLogData:{13}, Stamina:{14}, TotalGem:{15}, TotalGameMoney:{16}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InsUID, m_TimeStamp, m_GameState, m_Day, m_MaxPlayer, m_PlayerIndex, m_PlayerCharacter, m_Role, m_Dead, m_ChatHistoryData, m_GameLogData, m_Stamina, m_TotalGem, m_TotalGameMoney); 
			}; // void GameMatchedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Game match failed
			const MessageID GameMatchFailedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 68);
			Result GameMatchFailedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_FailedReason, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result GameMatchFailedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result GameMatchFailedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const Result &InFailedReason )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameMatchFailedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InFailedReason, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameMatchFailedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const Result &InFailedReason )



			void GameMatchFailedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameMatchFailedS2CEvt:{1}:{2} , FailedReason:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_FailedReason); 
			}; // void GameMatchFailedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Game matching started
			const MessageID GameMatchingStartedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 69);
			Result GameMatchingStartedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result GameMatchingStartedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result GameMatchingStartedS2CEvt::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameMatchingStartedS2CEvt::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameMatchingStartedS2CEvt::BuildIMsg( OUT MessageData* &pMsg )



			void GameMatchingStartedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameMatchingStartedS2CEvt:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void GameMatchingStartedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Cancel Game match
			const MessageID CancelGameMatchCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 70);
			Result CancelGameMatchCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result CancelGameMatchCmd::ParseMessage( MessageData* pIMsg )

			Result CancelGameMatchCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::CancelGameMatchCmd::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result CancelGameMatchCmd::BuildIMsg( OUT MessageData* &pMsg )



			void CancelGameMatchCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:CancelGameMatchCmd:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void CancelGameMatchCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID CancelGameMatchRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 70);
			Result CancelGameMatchRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result CancelGameMatchRes::ParseMessage( MessageData* pIMsg )

			Result CancelGameMatchRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::CancelGameMatchRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result CancelGameMatchRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void CancelGameMatchRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:CancelGameMatchRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void CancelGameMatchRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: game matching canceled
			const MessageID GameMatchingCanceledS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 71);
			Result GameMatchingCanceledS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result GameMatchingCanceledS2CEvt::ParseMessage( MessageData* pIMsg )

			Result GameMatchingCanceledS2CEvt::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameMatchingCanceledS2CEvt::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameMatchingCanceledS2CEvt::BuildIMsg( OUT MessageData* &pMsg )



			void GameMatchingCanceledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameMatchingCanceledS2CEvt:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void GameMatchingCanceledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Buy shop item prepare
			const MessageID BuyShopItemPrepareCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 72);
			Result BuyShopItemPrepareCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_ShopItemID, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result BuyShopItemPrepareCmd::ParseMessage( MessageData* pIMsg )

			Result BuyShopItemPrepareCmd::BuildIMsg( OUT MessageData* &pMsg, const uint32_t &InShopItemID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::BuyShopItemPrepareCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InShopItemID, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result BuyShopItemPrepareCmd::BuildIMsg( OUT MessageData* &pMsg, const uint32_t &InShopItemID )



			void BuyShopItemPrepareCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:BuyShopItemPrepareCmd:{1}:{2} , ShopItemID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_ShopItemID); 
			}; // void BuyShopItemPrepareCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID BuyShopItemPrepareRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 72);
			Result BuyShopItemPrepareRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfPurchaseID = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ShopItemID, pCur, iMsgSize, (int)sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPurchaseID, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_PurchaseID, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfPurchaseID ) );


			Proc_End:

				return hr;

			}; // Result BuyShopItemPrepareRes::ParseMessage( MessageData* pIMsg )

			Result BuyShopItemPrepareRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint32_t &InShopItemID, const char* InPurchaseID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInPurchaseIDLength = InPurchaseID ? (uint16_t)(strlen(InPurchaseID)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(uint16_t) + __uiInPurchaseIDLength 
					+ sizeof(Result)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::BuyShopItemPrepareRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InShopItemID, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &__uiInPurchaseIDLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InPurchaseID ? InPurchaseID : "", __uiInPurchaseIDLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result BuyShopItemPrepareRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint32_t &InShopItemID, const char* InPurchaseID )



			void BuyShopItemPrepareRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:BuyShopItemPrepareRes:{1}:{2} , Result:{3:X8}, ShopItemID:{4}, PurchaseID:{5,60}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_ShopItemID, m_PurchaseID); 
			}; // void BuyShopItemPrepareRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Buy shop item
			const MessageID BuyShopItemCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 73);
			Result BuyShopItemCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfPlatform = 0;
				uint16_t uiSizeOfPackageName = 0;
				uint16_t uiSizeOfPurchaseTransactionID = 0;
				uint16_t numberofPurchaseToken = 0; uint8_t* pPurchaseToken = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_ShopItemID, pCur, iMsgSize, (int)sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPlatform, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_Platform, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfPlatform ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPackageName, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_PackageName, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfPackageName ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPurchaseTransactionID, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_PurchaseTransactionID, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfPurchaseTransactionID ) );
				protocolChk( Protocol::StreamParamCopy( &numberofPurchaseToken, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pPurchaseToken, pCur, iMsgSize, (int)sizeof(uint8_t)*numberofPurchaseToken ) );
				m_PurchaseToken.SetLinkedBuffer(numberofPurchaseToken, numberofPurchaseToken, pPurchaseToken);


			Proc_End:

				return hr;

			}; // Result BuyShopItemCmd::ParseMessage( MessageData* pIMsg )

			Result BuyShopItemCmd::BuildIMsg( OUT MessageData* &pMsg, const uint32_t &InShopItemID, const char* InPlatform, const char* InPackageName, const char* InPurchaseTransactionID, const Array<uint8_t>& InPurchaseToken )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInPlatformLength = InPlatform ? (uint16_t)(strlen(InPlatform)+1) : 1;
				uint16_t __uiInPackageNameLength = InPackageName ? (uint16_t)(strlen(InPackageName)+1) : 1;
				uint16_t __uiInPurchaseTransactionIDLength = InPurchaseTransactionID ? (uint16_t)(strlen(InPurchaseTransactionID)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(uint16_t) + __uiInPlatformLength + sizeof(uint16_t) + __uiInPackageNameLength + sizeof(uint16_t) + __uiInPurchaseTransactionIDLength 
					+ sizeof(uint32_t)
					+ sizeof(uint8_t)*InPurchaseToken.GetSize() + sizeof(uint16_t));

				MessageData *pNewMsg = nullptr;

				uint16_t numberOfInPurchaseToken = (uint16_t)InPurchaseToken.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::BuyShopItemCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InShopItemID, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &__uiInPlatformLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InPlatform ? InPlatform : "", __uiInPlatformLength );
				Protocol::PackParamCopy( pMsgData, &__uiInPackageNameLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InPackageName ? InPackageName : "", __uiInPackageNameLength );
				Protocol::PackParamCopy( pMsgData, &__uiInPurchaseTransactionIDLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InPurchaseTransactionID ? InPurchaseTransactionID : "", __uiInPurchaseTransactionIDLength );
				Protocol::PackParamCopy( pMsgData, &numberOfInPurchaseToken, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InPurchaseToken.data(), (INT)(sizeof(uint8_t)*InPurchaseToken.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result BuyShopItemCmd::BuildIMsg( OUT MessageData* &pMsg, const uint32_t &InShopItemID, const char* InPlatform, const char* InPackageName, const char* InPurchaseTransactionID, const Array<uint8_t>& InPurchaseToken )



			void BuyShopItemCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:BuyShopItemCmd:{1}:{2} , ShopItemID:{3}, Platform:{4,60}, PackageName:{5,60}, PurchaseTransactionID:{6,60}, PurchaseToken:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_ShopItemID, m_Platform, m_PackageName, m_PurchaseTransactionID, m_PurchaseToken); 
			}; // void BuyShopItemCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID BuyShopItemRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 73);
			Result BuyShopItemRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ShopItemID, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result BuyShopItemRes::ParseMessage( MessageData* pIMsg )

			Result BuyShopItemRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint32_t &InShopItemID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::BuyShopItemRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InShopItemID, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result BuyShopItemRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const uint32_t &InShopItemID )



			void BuyShopItemRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:BuyShopItemRes:{1}:{2} , Result:{3:X8}, ShopItemID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_ShopItemID); 
			}; // void BuyShopItemRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Give my stamina to other player
			const MessageID GiveStaminaCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 74);
			Result GiveStaminaCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TargetPlayer, pCur, iMsgSize, (int)sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result GiveStaminaCmd::ParseMessage( MessageData* pIMsg )

			Result GiveStaminaCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InTargetPlayer )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GiveStaminaCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTargetPlayer, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GiveStaminaCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InTargetPlayer )



			void GiveStaminaCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GiveStaminaCmd:{1}:{2} , TargetPlayer:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TargetPlayer); 
			}; // void GiveStaminaCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GiveStaminaRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 74);
			Result GiveStaminaRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TargetPlayer, pCur, iMsgSize, (int)sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, (int)sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result GiveStaminaRes::ParseMessage( MessageData* pIMsg )

			Result GiveStaminaRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerID &InTargetPlayer, const uint64_t &InTimeStamp )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(PlayerID)
					+ sizeof(uint64_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GiveStaminaRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InTargetPlayer, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(uint64_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GiveStaminaRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerID &InTargetPlayer, const uint64_t &InTimeStamp )



			void GiveStaminaRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GiveStaminaRes:{1}:{2} , Result:{3:X8}, TargetPlayer:{4}, TimeStamp:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_TargetPlayer, m_TimeStamp); 
			}; // void GiveStaminaRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: For debug, Change configue preset
			const MessageID SetPresetGameConfigIDCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 75);
			Result SetPresetGameConfigIDCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PresetID, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result SetPresetGameConfigIDCmd::ParseMessage( MessageData* pIMsg )

			Result SetPresetGameConfigIDCmd::BuildIMsg( OUT MessageData* &pMsg, const uint32_t &InPresetID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetPresetGameConfigIDCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPresetID, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result SetPresetGameConfigIDCmd::BuildIMsg( OUT MessageData* &pMsg, const uint32_t &InPresetID )



			void SetPresetGameConfigIDCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:SetPresetGameConfigIDCmd:{1}:{2} , PresetID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PresetID); 
			}; // void SetPresetGameConfigIDCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID SetPresetGameConfigIDRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 75);
			Result SetPresetGameConfigIDRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result SetPresetGameConfigIDRes::ParseMessage( MessageData* pIMsg )

			Result SetPresetGameConfigIDRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetPresetGameConfigIDRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result SetPresetGameConfigIDRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void SetPresetGameConfigIDRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:SetPresetGameConfigIDRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void SetPresetGameConfigIDRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: For Debug
			const MessageID GainGameResourceCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 76);
			Result GainGameResourceCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Resource, pCur, iMsgSize, (int)sizeof(int32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Value, pCur, iMsgSize, (int)sizeof(int32_t) ) );


			Proc_End:

				return hr;

			}; // Result GainGameResourceCmd::ParseMessage( MessageData* pIMsg )

			Result GainGameResourceCmd::BuildIMsg( OUT MessageData* &pMsg, const int32_t &InResource, const int32_t &InValue )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(int32_t)
					+ sizeof(int32_t));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GainGameResourceCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResource, sizeof(int32_t));
				Protocol::PackParamCopy( pMsgData, &InValue, sizeof(int32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GainGameResourceCmd::BuildIMsg( OUT MessageData* &pMsg, const int32_t &InResource, const int32_t &InValue )



			void GainGameResourceCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GainGameResourceCmd:{1}:{2} , Resource:{3}, Value:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Resource, m_Value); 
			}; // void GainGameResourceCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GainGameResourceRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, PROTOCOLID_GAME, 76);
			Result GainGameResourceRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result GainGameResourceRes::ParseMessage( MessageData* pIMsg )

			Result GainGameResourceRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GainGameResourceRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GainGameResourceRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void GainGameResourceRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GainGameResourceRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void GainGameResourceRes::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace Game
	}; // namespace Message
}; // namespace BR


