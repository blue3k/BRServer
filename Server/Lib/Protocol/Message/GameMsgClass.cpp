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
#include "Common/PolicyID.h"
#include "Common/ToStringSvr.h"
#include "Common/ToStringGame.h"
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
			const MessageID HeartBitC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 0);
			HRESULT HeartBitC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();



			Proc_End:

				return hr;

			}; // HRESULT HeartBitC2SEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT HeartBitC2SEvt::BuildIMsg( OUT MessageData* &pMsg )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::HeartBitC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT HeartBitC2SEvt::BuildIMsg( OUT MessageData* &pMsg )



			VOID HeartBitC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:HeartBitC2SEvt:%1%:%2% ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // VOID HeartBitC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Player connected from a login server and moved to game server
			const MessageID JoinGameServerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 1);
			HRESULT JoinGameServerCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_AccID, pCur, iMsgSize, sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LoginEntityUID, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // HRESULT JoinGameServerCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT JoinGameServerCmd::BuildIMsg( OUT MessageData* &pMsg, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(AccountID)
					+ sizeof(AuthTicket)
					+ sizeof(UINT64));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinGameServerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InAccID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InLoginEntityUID, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT JoinGameServerCmd::BuildIMsg( OUT MessageData* &pMsg, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )



			VOID JoinGameServerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:JoinGameServerCmd:%1%:%2% , AccID:%3%, Ticket:%4%, LoginEntityUID:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_AccID, m_Ticket, m_LoginEntityUID); 
			}; // VOID JoinGameServerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID JoinGameServerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 1);
			HRESULT JoinGameServerRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				UINT16 uiSizeOfNickName = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfNickName, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_NickName, pCur, iMsgSize, sizeof(char)*uiSizeOfNickName ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyLeaderID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // HRESULT JoinGameServerRes::ParseIMsg( MessageData* pIMsg )

			HRESULT JoinGameServerRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const char* InNickName, const GameInsUID &InGameUID, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const MatchingQueueTicket &InMatchingTicket )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInNickNameLength = InNickName ? (UINT16)(strlen(InNickName)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInNickNameLength 
					+ sizeof(HRESULT)
					+ sizeof(GameInsUID)
					+ sizeof(PartyUID)
					+ sizeof(PlayerID)
					+ sizeof(MatchingQueueTicket));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinGameServerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &__uiInNickNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InNickName ? InNickName : "", __uiInNickNameLength );
				Protocol::PackParamCopy( pMsgData, &InGameUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InPartyLeaderID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT JoinGameServerRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const char* InNickName, const GameInsUID &InGameUID, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const MatchingQueueTicket &InMatchingTicket )



			VOID JoinGameServerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:JoinGameServerRes:%1%:%2% , Result:%3%, NickName:%4%, GameUID:%5%, PartyUID:%6%, PartyLeaderID:%7%, MatchingTicket:%8%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_NickName, m_GameUID, m_PartyUID, m_PartyLeaderID, m_MatchingTicket); 
			}; // VOID JoinGameServerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: player complition statues
			const MessageID GetComplitionStateCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 2);
			HRESULT GetComplitionStateCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();



			Proc_End:

				return hr;

			}; // HRESULT GetComplitionStateCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GetComplitionStateCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetComplitionStateCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetComplitionStateCmd::BuildIMsg( OUT MessageData* &pMsg )



			VOID GetComplitionStateCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GetComplitionStateCmd:%1%:%2% ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // VOID GetComplitionStateCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetComplitionStateRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 2);
			HRESULT GetComplitionStateRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				UINT16 uiSizeOfComplitionState = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfComplitionState, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_ComplitionState, pCur, iMsgSize, sizeof(char)*uiSizeOfComplitionState ) );


			Proc_End:

				return hr;

			}; // HRESULT GetComplitionStateRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GetComplitionStateRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const char* InComplitionState )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInComplitionStateLength = InComplitionState ? (UINT16)(strlen(InComplitionState)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInComplitionStateLength 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetComplitionStateRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &__uiInComplitionStateLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InComplitionState ? InComplitionState : "", __uiInComplitionStateLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetComplitionStateRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const char* InComplitionState )



			VOID GetComplitionStateRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GetComplitionStateRes:%1%:%2% , Result:%3%, ComplitionState:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_ComplitionState); 
			}; // VOID GetComplitionStateRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Player complition state
			const MessageID SetComplitionStateCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 3);
			HRESULT SetComplitionStateCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				UINT16 uiSizeOfComplitionState = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfComplitionState, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_ComplitionState, pCur, iMsgSize, sizeof(char)*uiSizeOfComplitionState ) );


			Proc_End:

				return hr;

			}; // HRESULT SetComplitionStateCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT SetComplitionStateCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InComplitionState )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInComplitionStateLength = InComplitionState ? (UINT16)(strlen(InComplitionState)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInComplitionStateLength );

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetComplitionStateCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInComplitionStateLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InComplitionState ? InComplitionState : "", __uiInComplitionStateLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT SetComplitionStateCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InComplitionState )



			VOID SetComplitionStateCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:SetComplitionStateCmd:%1%:%2% , ComplitionState:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_ComplitionState); 
			}; // VOID SetComplitionStateCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID SetComplitionStateRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 3);
			HRESULT SetComplitionStateRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT SetComplitionStateRes::ParseIMsg( MessageData* pIMsg )

			HRESULT SetComplitionStateRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetComplitionStateRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT SetComplitionStateRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID SetComplitionStateRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:SetComplitionStateRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID SetComplitionStateRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
			const MessageID RegisterGCMCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 4);
			HRESULT RegisterGCMCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				UINT16 uiSizeOfGCMRegisteredID = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfGCMRegisteredID, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_GCMRegisteredID, pCur, iMsgSize, sizeof(char)*uiSizeOfGCMRegisteredID ) );


			Proc_End:

				return hr;

			}; // HRESULT RegisterGCMCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT RegisterGCMCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InGCMRegisteredID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInGCMRegisteredIDLength = InGCMRegisteredID ? (UINT16)(strlen(InGCMRegisteredID)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInGCMRegisteredIDLength );

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RegisterGCMCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInGCMRegisteredIDLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InGCMRegisteredID ? InGCMRegisteredID : "", __uiInGCMRegisteredIDLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RegisterGCMCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InGCMRegisteredID )



			VOID RegisterGCMCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:RegisterGCMCmd:%1%:%2% , GCMRegisteredID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GCMRegisteredID); 
			}; // VOID RegisterGCMCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RegisterGCMRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 4);
			HRESULT RegisterGCMRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT RegisterGCMRes::ParseIMsg( MessageData* pIMsg )

			HRESULT RegisterGCMRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RegisterGCMRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RegisterGCMRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID RegisterGCMRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:RegisterGCMRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID RegisterGCMRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Unregister Google notification service ID
			const MessageID UnregisterGCMCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 5);
			HRESULT UnregisterGCMCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				UINT16 uiSizeOfGCMRegisteredID = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfGCMRegisteredID, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_GCMRegisteredID, pCur, iMsgSize, sizeof(char)*uiSizeOfGCMRegisteredID ) );


			Proc_End:

				return hr;

			}; // HRESULT UnregisterGCMCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT UnregisterGCMCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InGCMRegisteredID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInGCMRegisteredIDLength = InGCMRegisteredID ? (UINT16)(strlen(InGCMRegisteredID)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInGCMRegisteredIDLength );

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::UnregisterGCMCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInGCMRegisteredIDLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InGCMRegisteredID ? InGCMRegisteredID : "", __uiInGCMRegisteredIDLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT UnregisterGCMCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InGCMRegisteredID )



			VOID UnregisterGCMCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:UnregisterGCMCmd:%1%:%2% , GCMRegisteredID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GCMRegisteredID); 
			}; // VOID UnregisterGCMCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID UnregisterGCMRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 5);
			HRESULT UnregisterGCMRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT UnregisterGCMRes::ParseIMsg( MessageData* pIMsg )

			HRESULT UnregisterGCMRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::UnregisterGCMRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT UnregisterGCMRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID UnregisterGCMRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:UnregisterGCMRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID UnregisterGCMRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Invite friend
			const MessageID InviteFriendCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 6);
			HRESULT InviteFriendCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_FriendID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT InviteFriendCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT InviteFriendCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::InviteFriendCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InFriendID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT InviteFriendCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )



			VOID InviteFriendCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:InviteFriendCmd:%1%:%2% , FriendID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_FriendID); 
			}; // VOID InviteFriendCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID InviteFriendRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 6);
			HRESULT InviteFriendRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT InviteFriendRes::ParseIMsg( MessageData* pIMsg )

			HRESULT InviteFriendRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::InviteFriendRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT InviteFriendRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID InviteFriendRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:InviteFriendRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID InviteFriendRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Accept friend request
			const MessageID AcceptFriendRequestCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 7);
			HRESULT AcceptFriendRequestCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InviterID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InviterFacebookUID, pCur, iMsgSize, sizeof(FacebookUID) ) );


			Proc_End:

				return hr;

			}; // HRESULT AcceptFriendRequestCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT AcceptFriendRequestCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviterID, const FacebookUID &InInviterFacebookUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)
					+ sizeof(FacebookUID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AcceptFriendRequestCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInviterID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InInviterFacebookUID, sizeof(FacebookUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT AcceptFriendRequestCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviterID, const FacebookUID &InInviterFacebookUID )



			VOID AcceptFriendRequestCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:AcceptFriendRequestCmd:%1%:%2% , InviterID:%3%, InviterFacebookUID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InviterID, m_InviterFacebookUID); 
			}; // VOID AcceptFriendRequestCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID AcceptFriendRequestRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 7);
			HRESULT AcceptFriendRequestRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NewFriend, pCur, iMsgSize, sizeof(FriendInformation) ) );


			Proc_End:

				return hr;

			}; // HRESULT AcceptFriendRequestRes::ParseIMsg( MessageData* pIMsg )

			HRESULT AcceptFriendRequestRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const FriendInformation &InNewFriend )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(FriendInformation));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AcceptFriendRequestRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InNewFriend, sizeof(FriendInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT AcceptFriendRequestRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const FriendInformation &InNewFriend )



			VOID AcceptFriendRequestRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:AcceptFriendRequestRes:%1%:%2% , Result:%3%, NewFriend:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_NewFriend); 
			}; // VOID AcceptFriendRequestRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Notification for friend request is accepted
			const MessageID FriendRequestAcceptedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 8);
			HRESULT FriendRequestAcceptedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Accepter, pCur, iMsgSize, sizeof(FriendInformation) ) );


			Proc_End:

				return hr;

			}; // HRESULT FriendRequestAcceptedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT FriendRequestAcceptedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const FriendInformation &InAccepter )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(FriendInformation));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FriendRequestAcceptedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InAccepter, sizeof(FriendInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT FriendRequestAcceptedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const FriendInformation &InAccepter )



			VOID FriendRequestAcceptedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:FriendRequestAcceptedS2CEvt:%1%:%2% , Accepter:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Accepter); 
			}; // VOID FriendRequestAcceptedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Remove friden form the friend list
			const MessageID RemoveFriendCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 9);
			HRESULT RemoveFriendCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_FriendID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT RemoveFriendCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT RemoveFriendCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RemoveFriendCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InFriendID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RemoveFriendCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )



			VOID RemoveFriendCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:RemoveFriendCmd:%1%:%2% , FriendID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_FriendID); 
			}; // VOID RemoveFriendCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RemoveFriendRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 9);
			HRESULT RemoveFriendRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_FriendID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT RemoveFriendRes::ParseIMsg( MessageData* pIMsg )

			HRESULT RemoveFriendRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PlayerID &InFriendID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RemoveFriendRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InFriendID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RemoveFriendRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PlayerID &InFriendID )



			VOID RemoveFriendRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:RemoveFriendRes:%1%:%2% , Result:%3%, FriendID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_FriendID); 
			}; // VOID RemoveFriendRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Friend removed
			const MessageID FriendRemovedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 10);
			HRESULT FriendRemovedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_FriendID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT FriendRemovedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT FriendRemovedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FriendRemovedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InFriendID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT FriendRemovedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )



			VOID FriendRemovedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:FriendRemovedS2CEvt:%1%:%2% , FriendID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_FriendID); 
			}; // VOID FriendRemovedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Get friend list
			const MessageID GetFriendListCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 11);
			HRESULT GetFriendListCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_StartIndex, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Count, pCur, iMsgSize, sizeof(UINT16) ) );


			Proc_End:

				return hr;

			}; // HRESULT GetFriendListCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GetFriendListCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT16 &InStartIndex, const UINT16 &InCount )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(UINT16)
					+ sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetFriendListCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InStartIndex, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InCount, sizeof(UINT16));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetFriendListCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT16 &InStartIndex, const UINT16 &InCount )



			VOID GetFriendListCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GetFriendListCmd:%1%:%2% , StartIndex:%3%, Count:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_StartIndex, m_Count); 
			}; // VOID GetFriendListCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetFriendListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 11);
			HRESULT GetFriendListRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MaxFriendSlot, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalNumberOfFriends, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_StartIndex, pCur, iMsgSize, sizeof(UINT16) ) );
				UINT16 numberofFriendList = 0; FriendInformation* pFriendList = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofFriendList, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pFriendList, pCur, iMsgSize, sizeof(FriendInformation)*numberofFriendList ) );
				m_FriendList.SetLinkedBuffer(numberofFriendList, numberofFriendList, pFriendList);


			Proc_End:

				return hr;

			}; // HRESULT GetFriendListRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GetFriendListRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT16 &InMaxFriendSlot, const UINT16 &InTotalNumberOfFriends, const UINT16 &InStartIndex, const Array<FriendInformation>& InFriendList )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(UINT16)
					+ sizeof(UINT16)
					+ sizeof(UINT16)
					+ sizeof(FriendInformation)*InFriendList.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetFriendListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InMaxFriendSlot, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InTotalNumberOfFriends, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InStartIndex, sizeof(UINT16));
				UINT16 numberOfInFriendList = (UINT16)InFriendList.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInFriendList, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InFriendList.data(), (INT)(sizeof(FriendInformation)*InFriendList.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetFriendListRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT16 &InMaxFriendSlot, const UINT16 &InTotalNumberOfFriends, const UINT16 &InStartIndex, const Array<FriendInformation>& InFriendList )



			VOID GetFriendListRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GetFriendListRes:%1%:%2% , Result:%3%, MaxFriendSlot:%4%, TotalNumberOfFriends:%5%, StartIndex:%6%, FriendList:%7%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_MaxFriendSlot, m_TotalNumberOfFriends, m_StartIndex, ArgArray<FriendInformation>(m_FriendList)); 
			}; // VOID GetFriendListRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Query notification list
			const MessageID GetNotificationListCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 12);
			HRESULT GetNotificationListCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();



			Proc_End:

				return hr;

			}; // HRESULT GetNotificationListCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GetNotificationListCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetNotificationListCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetNotificationListCmd::BuildIMsg( OUT MessageData* &pMsg )



			VOID GetNotificationListCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GetNotificationListCmd:%1%:%2% ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // VOID GetNotificationListCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetNotificationListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 12);
			HRESULT GetNotificationListRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT GetNotificationListRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GetNotificationListRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetNotificationListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetNotificationListRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID GetNotificationListRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GetNotificationListRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID GetNotificationListRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Delete notification
			const MessageID DeleteNotificationCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 13);
			HRESULT DeleteNotificationCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT DeleteNotificationCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT DeleteNotificationCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::DeleteNotificationCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT DeleteNotificationCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID )



			VOID DeleteNotificationCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:DeleteNotificationCmd:%1%:%2% , NotificationID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NotificationID); 
			}; // VOID DeleteNotificationCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID DeleteNotificationRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 13);
			HRESULT DeleteNotificationRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT DeleteNotificationRes::ParseIMsg( MessageData* pIMsg )

			HRESULT DeleteNotificationRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT32 &InNotificationID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::DeleteNotificationRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT DeleteNotificationRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT32 &InNotificationID )



			VOID DeleteNotificationRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:DeleteNotificationRes:%1%:%2% , Result:%3%, NotificationID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_NotificationID); 
			}; // VOID DeleteNotificationRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Set notification is read
			const MessageID SetNotificationReadCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 14);
			HRESULT SetNotificationReadCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT SetNotificationReadCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT SetNotificationReadCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetNotificationReadCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT SetNotificationReadCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID )



			VOID SetNotificationReadCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:SetNotificationReadCmd:%1%:%2% , NotificationID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NotificationID); 
			}; // VOID SetNotificationReadCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID SetNotificationReadRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 14);
			HRESULT SetNotificationReadRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT SetNotificationReadRes::ParseIMsg( MessageData* pIMsg )

			HRESULT SetNotificationReadRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT32 &InNotificationID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetNotificationReadRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT SetNotificationReadRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT32 &InNotificationID )



			VOID SetNotificationReadRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:SetNotificationReadRes:%1%:%2% , Result:%3%, NotificationID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_NotificationID); 
			}; // VOID SetNotificationReadRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Accept notification
			const MessageID AcceptNotificationCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 15);
			HRESULT AcceptNotificationCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT AcceptNotificationCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT AcceptNotificationCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AcceptNotificationCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT AcceptNotificationCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID )



			VOID AcceptNotificationCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:AcceptNotificationCmd:%1%:%2% , NotificationID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NotificationID); 
			}; // VOID AcceptNotificationCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID AcceptNotificationRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 15);
			HRESULT AcceptNotificationRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT AcceptNotificationRes::ParseIMsg( MessageData* pIMsg )

			HRESULT AcceptNotificationRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT32 &InNotificationID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AcceptNotificationRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT AcceptNotificationRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT32 &InNotificationID )



			VOID AcceptNotificationRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:AcceptNotificationRes:%1%:%2% , Result:%3%, NotificationID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_NotificationID); 
			}; // VOID AcceptNotificationRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Notify new notification
			const MessageID NotifyS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 16);
			HRESULT NotifyS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MessageID, pCur, iMsgSize, sizeof(NotificationType) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MessageParam0, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MessageParam1, pCur, iMsgSize, sizeof(UINT64) ) );
				UINT16 uiSizeOfMessageText = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfMessageText, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_MessageText, pCur, iMsgSize, sizeof(char)*uiSizeOfMessageText ) );
				protocolChk( Protocol::StreamParamCopy( &m_IsRead, pCur, iMsgSize, sizeof(BYTE) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // HRESULT NotifyS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT NotifyS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const BYTE &InIsRead, const UINT64 &InTimeStamp )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInMessageTextLength = InMessageText ? (UINT16)(strlen(InMessageText)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInMessageTextLength 
					+ sizeof(UINT32)
					+ sizeof(NotificationType)
					+ sizeof(UINT64)
					+ sizeof(UINT64)
					+ sizeof(BYTE)
					+ sizeof(UINT64));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::NotifyS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InMessageID, sizeof(NotificationType));
				Protocol::PackParamCopy( pMsgData, &InMessageParam0, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InMessageParam1, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &__uiInMessageTextLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InMessageText ? InMessageText : "", __uiInMessageTextLength );
				Protocol::PackParamCopy( pMsgData, &InIsRead, sizeof(BYTE));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT NotifyS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const BYTE &InIsRead, const UINT64 &InTimeStamp )



			VOID NotifyS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:NotifyS2CEvt:%1%:%2% , NotificationID:%3%, MessageID:%4%, MessageParam0:%5%, MessageParam1:%6%, MessageText:%7%, IsRead:%8%, TimeStamp:%9%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NotificationID, m_MessageID, m_MessageParam0, m_MessageParam1, m_MessageText, m_IsRead, m_TimeStamp); 
			}; // VOID NotifyS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Query playerID list
			const MessageID FindPlayerByEMailCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 17);
			HRESULT FindPlayerByEMailCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				UINT16 uiSizeOfPlayerEMail = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPlayerEMail, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_PlayerEMail, pCur, iMsgSize, sizeof(char)*uiSizeOfPlayerEMail ) );


			Proc_End:

				return hr;

			}; // HRESULT FindPlayerByEMailCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT FindPlayerByEMailCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InPlayerEMail )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInPlayerEMailLength = InPlayerEMail ? (UINT16)(strlen(InPlayerEMail)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInPlayerEMailLength );

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FindPlayerByEMailCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInPlayerEMailLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InPlayerEMail ? InPlayerEMail : "", __uiInPlayerEMailLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT FindPlayerByEMailCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InPlayerEMail )



			VOID FindPlayerByEMailCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:FindPlayerByEMailCmd:%1%:%2% , PlayerEMail:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerEMail); 
			}; // VOID FindPlayerByEMailCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID FindPlayerByEMailRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 17);
			HRESULT FindPlayerByEMailRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Player, pCur, iMsgSize, sizeof(PlayerInformation) ) );


			Proc_End:

				return hr;

			}; // HRESULT FindPlayerByEMailRes::ParseIMsg( MessageData* pIMsg )

			HRESULT FindPlayerByEMailRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PlayerInformation &InPlayer )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(PlayerInformation));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FindPlayerByEMailRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InPlayer, sizeof(PlayerInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT FindPlayerByEMailRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PlayerInformation &InPlayer )



			VOID FindPlayerByEMailRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:FindPlayerByEMailRes:%1%:%2% , Result:%3%, Player:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_Player); 
			}; // VOID FindPlayerByEMailRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Query playerID list
			const MessageID FindPlayerByPlayerIDCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 18);
			HRESULT FindPlayerByPlayerIDCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT FindPlayerByPlayerIDCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT FindPlayerByPlayerIDCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FindPlayerByPlayerIDCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT FindPlayerByPlayerIDCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID )



			VOID FindPlayerByPlayerIDCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:FindPlayerByPlayerIDCmd:%1%:%2% , PlayerID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerID); 
			}; // VOID FindPlayerByPlayerIDCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID FindPlayerByPlayerIDRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 18);
			HRESULT FindPlayerByPlayerIDRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Player, pCur, iMsgSize, sizeof(PlayerInformation) ) );


			Proc_End:

				return hr;

			}; // HRESULT FindPlayerByPlayerIDRes::ParseIMsg( MessageData* pIMsg )

			HRESULT FindPlayerByPlayerIDRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PlayerInformation &InPlayer )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(PlayerInformation));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FindPlayerByPlayerIDRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InPlayer, sizeof(PlayerInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT FindPlayerByPlayerIDRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PlayerInformation &InPlayer )



			VOID FindPlayerByPlayerIDRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:FindPlayerByPlayerIDRes:%1%:%2% , Result:%3%, Player:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_Player); 
			}; // VOID FindPlayerByPlayerIDRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: *Request Player Status Update
			const MessageID RequestPlayerStatusUpdateCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 19);
			HRESULT RequestPlayerStatusUpdateCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				UINT16 numberofTargetPlayerID = 0; PlayerID* pTargetPlayerID = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofTargetPlayerID, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pTargetPlayerID, pCur, iMsgSize, sizeof(PlayerID)*numberofTargetPlayerID ) );
				m_TargetPlayerID.SetLinkedBuffer(numberofTargetPlayerID, numberofTargetPlayerID, pTargetPlayerID);


			Proc_End:

				return hr;

			}; // HRESULT RequestPlayerStatusUpdateCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT RequestPlayerStatusUpdateCmd::BuildIMsg( OUT MessageData* &pMsg, const Array<PlayerID>& InTargetPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)*InTargetPlayerID.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RequestPlayerStatusUpdateCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				UINT16 numberOfInTargetPlayerID = (UINT16)InTargetPlayerID.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInTargetPlayerID, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InTargetPlayerID.data(), (INT)(sizeof(PlayerID)*InTargetPlayerID.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RequestPlayerStatusUpdateCmd::BuildIMsg( OUT MessageData* &pMsg, const Array<PlayerID>& InTargetPlayerID )



			VOID RequestPlayerStatusUpdateCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:RequestPlayerStatusUpdateCmd:%1%:%2% , TargetPlayerID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgArray<PlayerID>(m_TargetPlayerID)); 
			}; // VOID RequestPlayerStatusUpdateCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RequestPlayerStatusUpdateRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 19);
			HRESULT RequestPlayerStatusUpdateRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT RequestPlayerStatusUpdateRes::ParseIMsg( MessageData* pIMsg )

			HRESULT RequestPlayerStatusUpdateRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RequestPlayerStatusUpdateRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RequestPlayerStatusUpdateRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID RequestPlayerStatusUpdateRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:RequestPlayerStatusUpdateRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID RequestPlayerStatusUpdateRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: *Notify Player Status Updated
			const MessageID NotifyPlayerStatusUpdatedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 20);
			HRESULT NotifyPlayerStatusUpdatedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LatestActiveTime, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_IsInGame, pCur, iMsgSize, sizeof(BYTE) ) );


			Proc_End:

				return hr;

			}; // HRESULT NotifyPlayerStatusUpdatedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT NotifyPlayerStatusUpdatedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)
					+ sizeof(UINT32)
					+ sizeof(BYTE));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::NotifyPlayerStatusUpdatedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InLatestActiveTime, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InIsInGame, sizeof(BYTE));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT NotifyPlayerStatusUpdatedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame )



			VOID NotifyPlayerStatusUpdatedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:NotifyPlayerStatusUpdatedS2CEvt:%1%:%2% , PlayerID:%3%, LatestActiveTime:%4%, IsInGame:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerID, m_LatestActiveTime, m_IsInGame); 
			}; // VOID NotifyPlayerStatusUpdatedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Get Ranking lise
			const MessageID GetRankingListCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 21);
			HRESULT GetRankingListCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RankingType, pCur, iMsgSize, sizeof(RankingType) ) );
				protocolChk( Protocol::StreamParamCopy( &m_BaseRanking, pCur, iMsgSize, sizeof(BYTE) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Count, pCur, iMsgSize, sizeof(BYTE) ) );


			Proc_End:

				return hr;

			}; // HRESULT GetRankingListCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GetRankingListCmd::BuildIMsg( OUT MessageData* &pMsg, const RankingType &InRankingType, const BYTE &InBaseRanking, const BYTE &InCount )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(RankingType)
					+ sizeof(BYTE)
					+ sizeof(BYTE));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetRankingListCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRankingType, sizeof(RankingType));
				Protocol::PackParamCopy( pMsgData, &InBaseRanking, sizeof(BYTE));
				Protocol::PackParamCopy( pMsgData, &InCount, sizeof(BYTE));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetRankingListCmd::BuildIMsg( OUT MessageData* &pMsg, const RankingType &InRankingType, const BYTE &InBaseRanking, const BYTE &InCount )



			VOID GetRankingListCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GetRankingListCmd:%1%:%2% , RankingType:%3%, BaseRanking:%4%, Count:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RankingType, m_BaseRanking, m_Count); 
			}; // VOID GetRankingListCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetRankingListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 21);
			HRESULT GetRankingListRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				UINT16 numberofRanking = 0; TotalRankingPlayerInformation* pRanking = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofRanking, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pRanking, pCur, iMsgSize, sizeof(TotalRankingPlayerInformation)*numberofRanking ) );
				m_Ranking.SetLinkedBuffer(numberofRanking, numberofRanking, pRanking);


			Proc_End:

				return hr;

			}; // HRESULT GetRankingListRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GetRankingListRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(TotalRankingPlayerInformation)*InRanking.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetRankingListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				UINT16 numberOfInRanking = (UINT16)InRanking.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInRanking, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InRanking.data(), (INT)(sizeof(TotalRankingPlayerInformation)*InRanking.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetRankingListRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const Array<TotalRankingPlayerInformation>& InRanking )



			VOID GetRankingListRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GetRankingListRes:%1%:%2% , Result:%3%, Ranking:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), ArgArray<TotalRankingPlayerInformation>(m_Ranking)); 
			}; // VOID GetRankingListRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Game user game play information
			const MessageID GetUserGamePlayerInfoCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 22);
			HRESULT GetUserGamePlayerInfoCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();



			Proc_End:

				return hr;

			}; // HRESULT GetUserGamePlayerInfoCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GetUserGamePlayerInfoCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetUserGamePlayerInfoCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetUserGamePlayerInfoCmd::BuildIMsg( OUT MessageData* &pMsg )



			VOID GetUserGamePlayerInfoCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GetUserGamePlayerInfoCmd:%1%:%2% ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // VOID GetUserGamePlayerInfoCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetUserGamePlayerInfoRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 22);
			HRESULT GetUserGamePlayerInfoRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Level, pCur, iMsgSize, sizeof(INT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Exp, pCur, iMsgSize, sizeof(INT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameMoney, pCur, iMsgSize, sizeof(INT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Gem, pCur, iMsgSize, sizeof(INT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Stamina, pCur, iMsgSize, sizeof(INT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LastUpdateTime, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalPlayed, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySC, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySM, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySS, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySC, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySM, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySS, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNC, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNM, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNS, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNC, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNM, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNS, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WeeklyWin, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WeeklyLose, pCur, iMsgSize, sizeof(INT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT GetUserGamePlayerInfoRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GetUserGamePlayerInfoRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const INT16 &InLevel, const INT64 &InExp, const INT64 &InGameMoney, const INT64 &InGem, const INT16 &InStamina, const UINT32 &InLastUpdateTime, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(INT16)
					+ sizeof(INT64)
					+ sizeof(INT64)
					+ sizeof(INT64)
					+ sizeof(INT16)
					+ sizeof(UINT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetUserGamePlayerInfoRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InLevel, sizeof(INT16));
				Protocol::PackParamCopy( pMsgData, &InExp, sizeof(INT64));
				Protocol::PackParamCopy( pMsgData, &InGameMoney, sizeof(INT64));
				Protocol::PackParamCopy( pMsgData, &InGem, sizeof(INT64));
				Protocol::PackParamCopy( pMsgData, &InStamina, sizeof(INT16));
				Protocol::PackParamCopy( pMsgData, &InLastUpdateTime, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InTotalPlayed, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySC, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySM, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySS, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySC, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySM, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySS, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNC, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNM, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNS, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNC, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNM, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNS, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWeeklyWin, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWeeklyLose, sizeof(INT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetUserGamePlayerInfoRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const INT16 &InLevel, const INT64 &InExp, const INT64 &InGameMoney, const INT64 &InGem, const INT16 &InStamina, const UINT32 &InLastUpdateTime, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose )



			VOID GetUserGamePlayerInfoRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GetUserGamePlayerInfoRes:%1%:%2% , Result:%3%, Level:%4%, Exp:%5%, GameMoney:%6%, Gem:%7%, Stamina:%8%, LastUpdateTime:%9%, TotalPlayed:%10%, WinPlaySC:%11%, WinPlaySM:%12%, WinPlaySS:%13%, LosePlaySC:%14%, LosePlaySM:%15%, LosePlaySS:%16%, WinPlayNC:%17%, WinPlayNM:%18%, WinPlayNS:%19%, LosePlayNC:%20%, LosePlayNM:%21%, LosePlayNS:%22%, WeeklyWin:%23%, WeeklyLose:%24%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_Level, m_Exp, m_GameMoney, m_Gem, m_Stamina, m_LastUpdateTime, m_TotalPlayed, m_WinPlaySC, m_WinPlaySM, m_WinPlaySS, m_LosePlaySC, m_LosePlaySM, m_LosePlaySS, m_WinPlayNC, m_WinPlayNM, m_WinPlayNS, m_LosePlayNC, m_LosePlayNM, m_LosePlayNS, m_WeeklyWin, m_WeeklyLose); 
			}; // VOID GetUserGamePlayerInfoRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Game game play information
			const MessageID GetGamePlayerInfoCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 23);
			HRESULT GetGamePlayerInfoCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT GetGamePlayerInfoCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GetGamePlayerInfoCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetGamePlayerInfoCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetGamePlayerInfoCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID )



			VOID GetGamePlayerInfoCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GetGamePlayerInfoCmd:%1%:%2% , PlayerID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerID); 
			}; // VOID GetGamePlayerInfoCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetGamePlayerInfoRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 23);
			HRESULT GetGamePlayerInfoRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Level, pCur, iMsgSize, sizeof(INT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalPlayed, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySC, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySM, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySS, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySC, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySM, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySS, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNC, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNM, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNS, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNC, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNM, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNS, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WeeklyWin, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WeeklyLose, pCur, iMsgSize, sizeof(INT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT GetGamePlayerInfoRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GetGamePlayerInfoRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PlayerID &InPlayerID, const INT16 &InLevel, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(PlayerID)
					+ sizeof(INT16)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetGamePlayerInfoRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InLevel, sizeof(INT16));
				Protocol::PackParamCopy( pMsgData, &InTotalPlayed, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySC, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySM, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySS, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySC, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySM, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySS, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNC, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNM, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNS, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNC, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNM, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNS, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWeeklyWin, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWeeklyLose, sizeof(INT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetGamePlayerInfoRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PlayerID &InPlayerID, const INT16 &InLevel, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose )



			VOID GetGamePlayerInfoRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GetGamePlayerInfoRes:%1%:%2% , Result:%3%, PlayerID:%4%, Level:%5%, TotalPlayed:%6%, WinPlaySC:%7%, WinPlaySM:%8%, WinPlaySS:%9%, LosePlaySC:%10%, LosePlaySM:%11%, LosePlaySS:%12%, WinPlayNC:%13%, WinPlayNM:%14%, WinPlayNS:%15%, LosePlayNC:%16%, LosePlayNM:%17%, LosePlayNS:%18%, WeeklyWin:%19%, WeeklyLose:%20%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_PlayerID, m_Level, m_TotalPlayed, m_WinPlaySC, m_WinPlaySM, m_WinPlaySS, m_LosePlaySC, m_LosePlaySM, m_LosePlaySS, m_WinPlayNC, m_WinPlayNM, m_WinPlayNS, m_LosePlayNC, m_LosePlayNM, m_LosePlayNS, m_WeeklyWin, m_WeeklyLose); 
			}; // VOID GetGamePlayerInfoRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player level up event
			const MessageID LevelUpS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 24);
			HRESULT LevelUpS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_CurrentTotalExp, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_CurrentLevel, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT LevelUpS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT LevelUpS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const UINT64 &InCurrentTotalExp, const UINT32 &InCurrentLevel )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(UINT64)
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::LevelUpS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InCurrentTotalExp, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InCurrentLevel, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT LevelUpS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const UINT64 &InCurrentTotalExp, const UINT32 &InCurrentLevel )



			VOID LevelUpS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:LevelUpS2CEvt:%1%:%2% , CurrentTotalExp:%3%, CurrentLevel:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_CurrentTotalExp, m_CurrentLevel); 
			}; // VOID LevelUpS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Change NickName
			const MessageID SetNickNameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 25);
			HRESULT SetNickNameCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				UINT16 uiSizeOfNickName = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfNickName, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_NickName, pCur, iMsgSize, sizeof(char)*uiSizeOfNickName ) );
				protocolChk( Protocol::StreamParamCopy( &m_IsCostFree, pCur, iMsgSize, sizeof(BYTE) ) );


			Proc_End:

				return hr;

			}; // HRESULT SetNickNameCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT SetNickNameCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InNickName, const BYTE &InIsCostFree )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInNickNameLength = InNickName ? (UINT16)(strlen(InNickName)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInNickNameLength 
					+ sizeof(BYTE));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetNickNameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInNickNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InNickName ? InNickName : "", __uiInNickNameLength );
				Protocol::PackParamCopy( pMsgData, &InIsCostFree, sizeof(BYTE));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT SetNickNameCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InNickName, const BYTE &InIsCostFree )



			VOID SetNickNameCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:SetNickNameCmd:%1%:%2% , NickName:%3%, IsCostFree:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NickName, m_IsCostFree); 
			}; // VOID SetNickNameCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID SetNickNameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 25);
			HRESULT SetNickNameRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // HRESULT SetNickNameRes::ParseIMsg( MessageData* pIMsg )

			HRESULT SetNickNameRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(UINT64)
					+ sizeof(UINT64));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetNickNameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT SetNickNameRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )



			VOID SetNickNameRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:SetNickNameRes:%1%:%2% , Result:%3%, TotalGem:%4%, TotalGameMoney:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_TotalGem, m_TotalGameMoney); 
			}; // VOID SetNickNameRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Create Party
			const MessageID CreatePartyCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 26);
			HRESULT CreatePartyCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();



			Proc_End:

				return hr;

			}; // HRESULT CreatePartyCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT CreatePartyCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::CreatePartyCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT CreatePartyCmd::BuildIMsg( OUT MessageData* &pMsg )



			VOID CreatePartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:CreatePartyCmd:%1%:%2% ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // VOID CreatePartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID CreatePartyRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 26);
			HRESULT CreatePartyRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );


			Proc_End:

				return hr;

			}; // HRESULT CreatePartyRes::ParseIMsg( MessageData* pIMsg )

			HRESULT CreatePartyRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PartyUID &InPartyUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(PartyUID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::CreatePartyRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT CreatePartyRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PartyUID &InPartyUID )



			VOID CreatePartyRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:CreatePartyRes:%1%:%2% , Result:%3%, PartyUID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_PartyUID); 
			}; // VOID CreatePartyRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Join party
			const MessageID JoinPartyCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 27);
			HRESULT JoinPartyCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InviterID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT JoinPartyCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT JoinPartyCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InInviterID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinPartyCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InInviterID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT JoinPartyCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InInviterID )



			VOID JoinPartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:JoinPartyCmd:%1%:%2% , PartyUID:%3%, InviterID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_InviterID); 
			}; // VOID JoinPartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID JoinPartyRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 27);
			HRESULT JoinPartyRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyLeaderID, pCur, iMsgSize, sizeof(PlayerID) ) );
				UINT16 numberofChatHistoryData = 0; BYTE* pChatHistoryData = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofChatHistoryData, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pChatHistoryData, pCur, iMsgSize, sizeof(BYTE)*numberofChatHistoryData ) );
				m_ChatHistoryData.SetLinkedBuffer(numberofChatHistoryData, numberofChatHistoryData, pChatHistoryData);


			Proc_End:

				return hr;

			}; // HRESULT JoinPartyRes::ParseIMsg( MessageData* pIMsg )

			HRESULT JoinPartyRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(PartyUID)
					+ sizeof(PlayerID)
					+ sizeof(BYTE)*InChatHistoryData.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinPartyRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InPartyLeaderID, sizeof(PlayerID));
				UINT16 numberOfInChatHistoryData = (UINT16)InChatHistoryData.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInChatHistoryData, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InChatHistoryData.data(), (INT)(sizeof(BYTE)*InChatHistoryData.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT JoinPartyRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData )



			VOID JoinPartyRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:JoinPartyRes:%1%:%2% , Result:%3%, PartyUID:%4%, PartyLeaderID:%5%, ChatHistoryData:%6%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_PartyUID, m_PartyLeaderID, ArgArray<BYTE>(m_ChatHistoryData)); 
			}; // VOID JoinPartyRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Joined event
			const MessageID PartyPlayerJoinedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 28);
			HRESULT PartyPlayerJoinedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayer, pCur, iMsgSize, sizeof(PlayerInformation) ) );


			Proc_End:

				return hr;

			}; // HRESULT PartyPlayerJoinedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyPlayerJoinedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerInformation &InJoinedPlayer )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerInformation));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyPlayerJoinedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayer, sizeof(PlayerInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyPlayerJoinedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerInformation &InJoinedPlayer )



			VOID PartyPlayerJoinedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PartyPlayerJoinedS2CEvt:%1%:%2% , PartyUID:%3%, JoinedPlayer:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_JoinedPlayer); 
			}; // VOID PartyPlayerJoinedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party leader changed event
			const MessageID PartyLeaderChangedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 29);
			HRESULT PartyLeaderChangedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NewLeaderID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT PartyLeaderChangedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyLeaderChangedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InNewLeaderID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyLeaderChangedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InNewLeaderID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyLeaderChangedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InNewLeaderID )



			VOID PartyLeaderChangedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PartyLeaderChangedS2CEvt:%1%:%2% , PartyUID:%3%, NewLeaderID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_NewLeaderID); 
			}; // VOID PartyLeaderChangedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Leave party command
			const MessageID LeavePartyCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 30);
			HRESULT LeavePartyCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT LeavePartyCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT LeavePartyCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::LeavePartyCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT LeavePartyCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InPlayerID )



			VOID LeavePartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:LeavePartyCmd:%1%:%2% , PartyUID:%3%, PlayerID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_PlayerID); 
			}; // VOID LeavePartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID LeavePartyRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 30);
			HRESULT LeavePartyRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT LeavePartyRes::ParseIMsg( MessageData* pIMsg )

			HRESULT LeavePartyRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::LeavePartyRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT LeavePartyRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID LeavePartyRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:LeavePartyRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID LeavePartyRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party Player left event
			const MessageID PartyPlayerLeftS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 31);
			HRESULT PartyPlayerLeftS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LeftPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT PartyPlayerLeftS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyPlayerLeftS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InLeftPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyPlayerLeftS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InLeftPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyPlayerLeftS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InLeftPlayerID )



			VOID PartyPlayerLeftS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PartyPlayerLeftS2CEvt:%1%:%2% , PartyUID:%3%, LeftPlayerID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_LeftPlayerID); 
			}; // VOID PartyPlayerLeftS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Kick player from the party
			const MessageID PartyKickPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 32);
			HRESULT PartyKickPlayerCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerToKick, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT PartyKickPlayerCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyKickPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyKickPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPlayerToKick, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyKickPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )



			VOID PartyKickPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PartyKickPlayerCmd:%1%:%2% , PartyUID:%3%, PlayerID:%4%, PlayerToKick:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_PlayerID, m_PlayerToKick); 
			}; // VOID PartyKickPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID PartyKickPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 32);
			HRESULT PartyKickPlayerRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT PartyKickPlayerRes::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyKickPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyKickPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyKickPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID PartyKickPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PartyKickPlayerRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID PartyKickPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party Player kicked message
			const MessageID PartyPlayerKickedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 33);
			HRESULT PartyPlayerKickedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_KickedPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT PartyPlayerKickedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyPlayerKickedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InKickedPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyPlayerKickedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InKickedPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyPlayerKickedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InKickedPlayerID )



			VOID PartyPlayerKickedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PartyPlayerKickedS2CEvt:%1%:%2% , PartyUID:%3%, KickedPlayerID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_KickedPlayerID); 
			}; // VOID PartyPlayerKickedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Invite a player to the party
			const MessageID PartyInviteCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 34);
			HRESULT PartyInviteCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InviteTargetID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT PartyInviteCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyInviteCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviteTargetID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyInviteCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInviteTargetID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyInviteCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviteTargetID )



			VOID PartyInviteCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PartyInviteCmd:%1%:%2% , InviteTargetID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InviteTargetID); 
			}; // VOID PartyInviteCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID PartyInviteRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 34);
			HRESULT PartyInviteRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT PartyInviteRes::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyInviteRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyInviteRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyInviteRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID PartyInviteRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PartyInviteRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID PartyInviteRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party invite requested
			const MessageID PartyInviteRequestedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 35);
			HRESULT PartyInviteRequestedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InviterID, pCur, iMsgSize, sizeof(PlayerID) ) );
				UINT16 uiSizeOfInviterName = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfInviterName, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_InviterName, pCur, iMsgSize, sizeof(char)*uiSizeOfInviterName ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyToJoinUID, pCur, iMsgSize, sizeof(PartyUID) ) );


			Proc_End:

				return hr;

			}; // HRESULT PartyInviteRequestedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyInviteRequestedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviterID, const char* InInviterName, const PartyUID &InPartyToJoinUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInInviterNameLength = InInviterName ? (UINT16)(strlen(InInviterName)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInInviterNameLength 
					+ sizeof(PlayerID)
					+ sizeof(PartyUID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyInviteRequestedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInviterID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &__uiInInviterNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InInviterName ? InInviterName : "", __uiInInviterNameLength );
				Protocol::PackParamCopy( pMsgData, &InPartyToJoinUID, sizeof(PartyUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyInviteRequestedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviterID, const char* InInviterName, const PartyUID &InPartyToJoinUID )



			VOID PartyInviteRequestedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PartyInviteRequestedS2CEvt:%1%:%2% , InviterID:%3%, InviterName:%4%, PartyToJoinUID:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InviterID, m_InviterName, m_PartyToJoinUID); 
			}; // VOID PartyInviteRequestedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Send Party quick chat message
			const MessageID PartyQuickChatMessageCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 36);
			HRESULT PartyQuickChatMessageCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_QuickChatID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT PartyQuickChatMessageCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyQuickChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InQuickChatID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyQuickChatMessageCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InQuickChatID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyQuickChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InQuickChatID )



			VOID PartyQuickChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PartyQuickChatMessageCmd:%1%:%2% , QuickChatID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_QuickChatID); 
			}; // VOID PartyQuickChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID PartyQuickChatMessageRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 36);
			HRESULT PartyQuickChatMessageRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT PartyQuickChatMessageRes::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyQuickChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyQuickChatMessageRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyQuickChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID PartyQuickChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PartyQuickChatMessageRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID PartyQuickChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party Chatting message event
			const MessageID PartyQuickChatMessageS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 37);
			HRESULT PartyQuickChatMessageS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_SenderID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_QuickChatID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT PartyQuickChatMessageS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyQuickChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const UINT32 &InQuickChatID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyQuickChatMessageS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InSenderID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InQuickChatID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyQuickChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const UINT32 &InQuickChatID )



			VOID PartyQuickChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PartyQuickChatMessageS2CEvt:%1%:%2% , SenderID:%3%, QuickChatID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_SenderID, m_QuickChatID); 
			}; // VOID PartyQuickChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Party chatting
			const MessageID PartyChatMessageCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 38);
			HRESULT PartyChatMessageCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				UINT16 uiSizeOfChatMessage = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, sizeof(char)*uiSizeOfChatMessage ) );


			Proc_End:

				return hr;

			}; // HRESULT PartyChatMessageCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InChatMessage )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInChatMessageLength = InChatMessage ? (UINT16)(strlen(InChatMessage)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInChatMessageLength );

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyChatMessageCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InChatMessage )



			VOID PartyChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PartyChatMessageCmd:%1%:%2% , ChatMessage:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_ChatMessage); 
			}; // VOID PartyChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID PartyChatMessageRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 38);
			HRESULT PartyChatMessageRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT PartyChatMessageRes::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyChatMessageRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID PartyChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PartyChatMessageRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID PartyChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party Chatting message event
			const MessageID PartyChatMessageS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 39);
			HRESULT PartyChatMessageS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_SenderID, pCur, iMsgSize, sizeof(PlayerID) ) );
				UINT16 uiSizeOfSenderName = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfSenderName, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_SenderName, pCur, iMsgSize, sizeof(char)*uiSizeOfSenderName ) );
				UINT16 uiSizeOfChatMessage = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, sizeof(char)*uiSizeOfChatMessage ) );


			Proc_End:

				return hr;

			}; // HRESULT PartyChatMessageS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PartyChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInSenderNameLength = InSenderName ? (UINT16)(strlen(InSenderName)+1) : 1;
				UINT16 __uiInChatMessageLength = InChatMessage ? (UINT16)(strlen(InChatMessage)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInSenderNameLength + sizeof(UINT16) + __uiInChatMessageLength 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyChatMessageS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InSenderID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &__uiInSenderNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InSenderName ? InSenderName : "", __uiInSenderNameLength );
				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PartyChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )



			VOID PartyChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PartyChatMessageS2CEvt:%1%:%2% , SenderID:%3%, SenderName:%4%, ChatMessage:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_SenderID, m_SenderName, m_ChatMessage); 
			}; // VOID PartyChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Join to a game
			const MessageID JoinGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 40);
			HRESULT JoinGameCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );


			Proc_End:

				return hr;

			}; // HRESULT JoinGameCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT JoinGameCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID, const AuthTicket &InTicket, const GameInsUID &InInsUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket)
					+ sizeof(GameInsUID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InInsUID, sizeof(GameInsUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT JoinGameCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID, const AuthTicket &InTicket, const GameInsUID &InInsUID )



			VOID JoinGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:JoinGameCmd:%1%:%2% , PlayerID:%3%, Ticket:%4%, InsUID:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerID, m_Ticket, m_InsUID); 
			}; // VOID JoinGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID JoinGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 40);
			HRESULT JoinGameRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameState, pCur, iMsgSize, sizeof(GameStateID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Day, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MaxPlayer, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerIndex, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerCharacter, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Dead, pCur, iMsgSize, sizeof(UINT8) ) );
				UINT16 numberofChatHistoryData = 0; BYTE* pChatHistoryData = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofChatHistoryData, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pChatHistoryData, pCur, iMsgSize, sizeof(BYTE)*numberofChatHistoryData ) );
				m_ChatHistoryData.SetLinkedBuffer(numberofChatHistoryData, numberofChatHistoryData, pChatHistoryData);
				UINT16 numberofGameLogData = 0; BYTE* pGameLogData = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofGameLogData, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pGameLogData, pCur, iMsgSize, sizeof(BYTE)*numberofGameLogData ) );
				m_GameLogData.SetLinkedBuffer(numberofGameLogData, numberofGameLogData, pGameLogData);


			Proc_End:

				return hr;

			}; // HRESULT JoinGameRes::ParseIMsg( MessageData* pIMsg )

			HRESULT JoinGameRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(GameInsUID)
					+ sizeof(UINT32)
					+ sizeof(GameStateID)
					+ sizeof(UINT8)
					+ sizeof(UINT8)
					+ sizeof(UINT8)
					+ sizeof(UINT8)
					+ sizeof(PlayerRole)
					+ sizeof(UINT8)
					+ sizeof(BYTE)*InChatHistoryData.GetSize() + sizeof(UINT16)
					+ sizeof(BYTE)*InGameLogData.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinGameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InGameState, sizeof(GameStateID));
				Protocol::PackParamCopy( pMsgData, &InDay, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InMaxPlayer, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InPlayerIndex, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InPlayerCharacter, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &InDead, sizeof(UINT8));
				UINT16 numberOfInChatHistoryData = (UINT16)InChatHistoryData.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInChatHistoryData, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InChatHistoryData.data(), (INT)(sizeof(BYTE)*InChatHistoryData.GetSize())); 
				UINT16 numberOfInGameLogData = (UINT16)InGameLogData.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInGameLogData, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InGameLogData.data(), (INT)(sizeof(BYTE)*InGameLogData.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT JoinGameRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData )



			VOID JoinGameRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:JoinGameRes:%1%:%2% , Result:%3%, InsUID:%4%, TimeStamp:%5%, GameState:%6%, Day:%7%, MaxPlayer:%8%, PlayerIndex:%9%, PlayerCharacter:%10%, Role:%11%, Dead:%12%, ChatHistoryData:%13%, GameLogData:%14%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_InsUID, m_TimeStamp, m_GameState, m_Day, m_MaxPlayer, m_PlayerIndex, m_PlayerCharacter, m_Role, m_Dead, ArgArray<BYTE>(m_ChatHistoryData), ArgArray<BYTE>(m_GameLogData)); 
			}; // VOID JoinGameRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Joined in the game
			const MessageID PlayerJoinedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 41);
			HRESULT PlayerJoinedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayer, pCur, iMsgSize, sizeof(PlayerInformation) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerRole, pCur, iMsgSize, sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerDead, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerIndex, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerCharacter, pCur, iMsgSize, sizeof(UINT8) ) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerJoinedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PlayerJoinedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerInformation)
					+ sizeof(PlayerRole)
					+ sizeof(UINT8)
					+ sizeof(UINT8)
					+ sizeof(UINT8));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PlayerJoinedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayer, sizeof(PlayerInformation));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerDead, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerIndex, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerCharacter, sizeof(UINT8));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PlayerJoinedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter )



			VOID PlayerJoinedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PlayerJoinedS2CEvt:%1%:%2% , GameInsUID:%3%, JoinedPlayer:%4%, JoinedPlayerRole:%5%, JoinedPlayerDead:%6%, JoinedPlayerIndex:%7%, JoinedPlayerCharacter:%8%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_JoinedPlayer, m_JoinedPlayerRole, m_JoinedPlayerDead, m_JoinedPlayerIndex, m_JoinedPlayerCharacter); 
			}; // VOID PlayerJoinedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Leave Game
			const MessageID LeaveGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 42);
			HRESULT LeaveGameCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );


			Proc_End:

				return hr;

			}; // HRESULT LeaveGameCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT LeaveGameCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::LeaveGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT LeaveGameCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )



			VOID LeaveGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:LeaveGameCmd:%1%:%2% , GameInsUID:%3%, PlayerID:%4%, Ticket:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Ticket); 
			}; // VOID LeaveGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID LeaveGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 42);
			HRESULT LeaveGameRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT LeaveGameRes::ParseIMsg( MessageData* pIMsg )

			HRESULT LeaveGameRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::LeaveGameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT LeaveGameRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID LeaveGameRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:LeaveGameRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID LeaveGameRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player left event
			const MessageID PlayerLeftS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 43);
			HRESULT PlayerLeftS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LeftPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerLeftS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PlayerLeftS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InLeftPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PlayerLeftS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InLeftPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PlayerLeftS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InLeftPlayerID )



			VOID PlayerLeftS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PlayerLeftS2CEvt:%1%:%2% , GameInsUID:%3%, LeftPlayerID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_LeftPlayerID); 
			}; // VOID PlayerLeftS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Kick player
			const MessageID KickPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 44);
			HRESULT KickPlayerCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerToKick, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT KickPlayerCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT KickPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::KickPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPlayerToKick, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT KickPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )



			VOID KickPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:KickPlayerCmd:%1%:%2% , GameInsUID:%3%, PlayerID:%4%, PlayerToKick:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_PlayerToKick); 
			}; // VOID KickPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID KickPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 44);
			HRESULT KickPlayerRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT KickPlayerRes::ParseIMsg( MessageData* pIMsg )

			HRESULT KickPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::KickPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT KickPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID KickPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:KickPlayerRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID KickPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player kicked
			const MessageID PlayerKickedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 45);
			HRESULT PlayerKickedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_KickedPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerKickedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PlayerKickedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InKickedPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PlayerKickedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InKickedPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PlayerKickedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InKickedPlayerID )



			VOID PlayerKickedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PlayerKickedS2CEvt:%1%:%2% , GameInsUID:%3%, KickedPlayerID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_KickedPlayerID); 
			}; // VOID PlayerKickedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Assign role + Game state reset
			const MessageID AssignRoleCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 46);
			HRESULT AssignRoleCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );


			Proc_End:

				return hr;

			}; // HRESULT AssignRoleCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT AssignRoleCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AssignRoleCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT AssignRoleCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )



			VOID AssignRoleCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:AssignRoleCmd:%1%:%2% , GameInsUID:%3%, PlayerID:%4%, Ticket:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Ticket); 
			}; // VOID AssignRoleCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID AssignRoleRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 46);
			HRESULT AssignRoleRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT AssignRoleRes::ParseIMsg( MessageData* pIMsg )

			HRESULT AssignRoleRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AssignRoleRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT AssignRoleRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID AssignRoleRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:AssignRoleRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID AssignRoleRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Role assigned event
			const MessageID RoleAssignedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 47);
			HRESULT RoleAssignedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(PlayerRole) ) );


			Proc_End:

				return hr;

			}; // HRESULT RoleAssignedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT RoleAssignedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerRole &InRole )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerRole));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RoleAssignedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RoleAssignedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerRole &InRole )



			VOID RoleAssignedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:RoleAssignedS2CEvt:%1%:%2% , GameInsUID:%3%, PlayerID:%4%, Role:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Role); 
			}; // VOID RoleAssignedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Send chatting message to the game
			const MessageID ChatMessageCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 48);
			HRESULT ChatMessageCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				UINT16 uiSizeOfChatMessage = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, sizeof(char)*uiSizeOfChatMessage ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(PlayerRole) ) );


			Proc_End:

				return hr;

			}; // HRESULT ChatMessageCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT ChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InChatMessage, const PlayerRole &InRole )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInChatMessageLength = InChatMessage ? (UINT16)(strlen(InChatMessage)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInChatMessageLength 
					+ sizeof(PlayerRole));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::ChatMessageCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT ChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InChatMessage, const PlayerRole &InRole )



			VOID ChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:ChatMessageCmd:%1%:%2% , ChatMessage:%3%, Role:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_ChatMessage, m_Role); 
			}; // VOID ChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID ChatMessageRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 48);
			HRESULT ChatMessageRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT ChatMessageRes::ParseIMsg( MessageData* pIMsg )

			HRESULT ChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::ChatMessageRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT ChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID ChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:ChatMessageRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID ChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Chatting message event 
			const MessageID ChatMessageS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 49);
			HRESULT ChatMessageS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_SenderID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(PlayerRole) ) );
				UINT16 uiSizeOfSenderName = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfSenderName, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_SenderName, pCur, iMsgSize, sizeof(char)*uiSizeOfSenderName ) );
				UINT16 uiSizeOfChatMessage = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, sizeof(char)*uiSizeOfChatMessage ) );


			Proc_End:

				return hr;

			}; // HRESULT ChatMessageS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT ChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInSenderNameLength = InSenderName ? (UINT16)(strlen(InSenderName)+1) : 1;
				UINT16 __uiInChatMessageLength = InChatMessage ? (UINT16)(strlen(InChatMessage)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInSenderNameLength + sizeof(UINT16) + __uiInChatMessageLength 
					+ sizeof(PlayerID)
					+ sizeof(PlayerRole));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::ChatMessageS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InSenderID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &__uiInSenderNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InSenderName ? InSenderName : "", __uiInSenderNameLength );
				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT ChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )



			VOID ChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:ChatMessageS2CEvt:%1%:%2% , SenderID:%3%, Role:%4%, SenderName:%5%, ChatMessage:%6%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_SenderID, m_Role, m_SenderName, m_ChatMessage); 
			}; // VOID ChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Advance game
			const MessageID AdvanceGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 50);
			HRESULT AdvanceGameCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );


			Proc_End:

				return hr;

			}; // HRESULT AdvanceGameCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT AdvanceGameCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AdvanceGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT AdvanceGameCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )



			VOID AdvanceGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:AdvanceGameCmd:%1%:%2% , GameInsUID:%3%, PlayerID:%4%, Ticket:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Ticket); 
			}; // VOID AdvanceGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID AdvanceGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 50);
			HRESULT AdvanceGameRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT AdvanceGameRes::ParseIMsg( MessageData* pIMsg )

			HRESULT AdvanceGameRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AdvanceGameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT AdvanceGameRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID AdvanceGameRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:AdvanceGameRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID AdvanceGameRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: The game state is advanced
			const MessageID GameAdvancedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 51);
			HRESULT GameAdvancedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameState, pCur, iMsgSize, sizeof(GameStateID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Day, pCur, iMsgSize, sizeof(UINT8) ) );


			Proc_End:

				return hr;

			}; // HRESULT GameAdvancedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT GameAdvancedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(UINT32)
					+ sizeof(GameStateID)
					+ sizeof(UINT8));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameAdvancedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InGameState, sizeof(GameStateID));
				Protocol::PackParamCopy( pMsgData, &InDay, sizeof(UINT8));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GameAdvancedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay )



			VOID GameAdvancedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GameAdvancedS2CEvt:%1%:%2% , GameInsUID:%3%, TimeStamp:%4%, GameState:%5%, Day:%6%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_TimeStamp, m_GameState, m_Day); 
			}; // VOID GameAdvancedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Game is ended
			const MessageID GameEndedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 52);
			HRESULT GameEndedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Winner, pCur, iMsgSize, sizeof(GameWinner) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GainedExp, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GainedGameMoney, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT GameEndedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT GameEndedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(GameWinner)
					+ sizeof(UINT32)
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameEndedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InWinner, sizeof(GameWinner));
				Protocol::PackParamCopy( pMsgData, &InGainedExp, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InGainedGameMoney, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GameEndedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney )



			VOID GameEndedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GameEndedS2CEvt:%1%:%2% , GameInsUID:%3%, Winner:%4%, GainedExp:%5%, GainedGameMoney:%6%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_Winner, m_GainedExp, m_GainedGameMoney); 
			}; // VOID GameEndedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Vote game advance
			const MessageID VoteGameAdvanceCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 53);
			HRESULT VoteGameAdvanceCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );


			Proc_End:

				return hr;

			}; // HRESULT VoteGameAdvanceCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT VoteGameAdvanceCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::VoteGameAdvanceCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT VoteGameAdvanceCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )



			VOID VoteGameAdvanceCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:VoteGameAdvanceCmd:%1%:%2% , GameInsUID:%3%, PlayerID:%4%, Ticket:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Ticket); 
			}; // VOID VoteGameAdvanceCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID VoteGameAdvanceRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 53);
			HRESULT VoteGameAdvanceRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT VoteGameAdvanceRes::ParseIMsg( MessageData* pIMsg )

			HRESULT VoteGameAdvanceRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::VoteGameAdvanceRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT VoteGameAdvanceRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID VoteGameAdvanceRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:VoteGameAdvanceRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID VoteGameAdvanceRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: GameAdvance is Voted
			const MessageID GameAdvanceVotedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 54);
			HRESULT GameAdvanceVotedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Voter, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT GameAdvanceVotedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT GameAdvanceVotedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InVoter )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameAdvanceVotedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InVoter, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GameAdvanceVotedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InVoter )



			VOID GameAdvanceVotedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GameAdvanceVotedS2CEvt:%1%:%2% , GameInsUID:%3%, Voter:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_Voter); 
			}; // VOID GameAdvanceVotedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Vote
			const MessageID VoteCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 55);
			HRESULT VoteCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_VoteTarget, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ActionSerial, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT VoteCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT VoteCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const PlayerID &InVoteTarget, const UINT32 &InActionSerial )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket)
					+ sizeof(PlayerID)
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::VoteCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InVoteTarget, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InActionSerial, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT VoteCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const PlayerID &InVoteTarget, const UINT32 &InActionSerial )



			VOID VoteCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:VoteCmd:%1%:%2% , GameInsUID:%3%, PlayerID:%4%, Ticket:%5%, VoteTarget:%6%, ActionSerial:%7%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Ticket, m_VoteTarget, m_ActionSerial); 
			}; // VOID VoteCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID VoteRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 55);
			HRESULT VoteRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT VoteRes::ParseIMsg( MessageData* pIMsg )

			HRESULT VoteRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::VoteRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT VoteRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID VoteRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:VoteRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID VoteRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Voted
			const MessageID VotedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 56);
			HRESULT VotedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Voter, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_VotedTarget, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT VotedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT VotedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InVoter, const PlayerID &InVotedTarget )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::VotedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InVoter, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InVotedTarget, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT VotedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InVoter, const PlayerID &InVotedTarget )



			VOID VotedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:VotedS2CEvt:%1%:%2% , GameInsUID:%3%, Voter:%4%, VotedTarget:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_Voter, m_VotedTarget); 
			}; // VOID VotedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Vote is ended
			const MessageID VoteEndS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 57);
			HRESULT VoteEndS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				UINT16 numberofVoted = 0; PlayerID* pVoted = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofVoted, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pVoted, pCur, iMsgSize, sizeof(PlayerID)*numberofVoted ) );
				m_Voted.SetLinkedBuffer(numberofVoted, numberofVoted, pVoted);


			Proc_End:

				return hr;

			}; // HRESULT VoteEndS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT VoteEndS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const Array<PlayerID>& InVoted )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)*InVoted.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::VoteEndS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				UINT16 numberOfInVoted = (UINT16)InVoted.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInVoted, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InVoted.data(), (INT)(sizeof(PlayerID)*InVoted.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT VoteEndS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const Array<PlayerID>& InVoted )



			VOID VoteEndS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:VoteEndS2CEvt:%1%:%2% , GameInsUID:%3%, Voted:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, ArgArray<PlayerID>(m_Voted)); 
			}; // VOID VoteEndS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Killed
			const MessageID PlayerKilledS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 58);
			HRESULT PlayerKilledS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_KilledPlayer, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Reason, pCur, iMsgSize, sizeof(PlayerKilledReason) ) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerKilledS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PlayerKilledS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerKilledReason));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PlayerKilledS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InKilledPlayer, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InReason, sizeof(PlayerKilledReason));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PlayerKilledS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason )



			VOID PlayerKilledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PlayerKilledS2CEvt:%1%:%2% , GameInsUID:%3%, KilledPlayer:%4%, Reason:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_KilledPlayer, m_Reason); 
			}; // VOID PlayerKilledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Voted
			const MessageID PlayerRevealedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 59);
			HRESULT PlayerRevealedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RevealedPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Reason, pCur, iMsgSize, sizeof(PlayerRevealedReason) ) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerRevealedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PlayerRevealedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerRole)
					+ sizeof(PlayerRevealedReason));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PlayerRevealedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InRevealedPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &InReason, sizeof(PlayerRevealedReason));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PlayerRevealedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason )



			VOID PlayerRevealedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PlayerRevealedS2CEvt:%1%:%2% , GameInsUID:%3%, RevealedPlayerID:%4%, Role:%5%, Reason:%6%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_RevealedPlayerID, m_Role, m_Reason); 
			}; // VOID PlayerRevealedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Play again with the current players
			const MessageID GamePlayAgainCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 60);
			HRESULT GamePlayAgainCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();



			Proc_End:

				return hr;

			}; // HRESULT GamePlayAgainCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GamePlayAgainCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayAgainCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GamePlayAgainCmd::BuildIMsg( OUT MessageData* &pMsg )



			VOID GamePlayAgainCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GamePlayAgainCmd:%1%:%2% ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // VOID GamePlayAgainCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GamePlayAgainRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 60);
			HRESULT GamePlayAgainRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayAgainRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GamePlayAgainRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(UINT64)
					+ sizeof(UINT64));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayAgainRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GamePlayAgainRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )



			VOID GamePlayAgainRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GamePlayAgainRes:%1%:%2% , Result:%3%, TotalGem:%4%, TotalGameMoney:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_TotalGem, m_TotalGameMoney); 
			}; // VOID GamePlayAgainRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			const MessageID GamePlayAgainS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 61);
			HRESULT GamePlayAgainS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LeadPlayer, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayAgainS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT GamePlayAgainS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InLeadPlayer )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayAgainS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InLeadPlayer, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GamePlayAgainS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InLeadPlayer )



			VOID GamePlayAgainS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GamePlayAgainS2CEvt:%1%:%2% , PartyUID:%3%, LeadPlayer:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_LeadPlayer); 
			}; // VOID GamePlayAgainS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Player. reveal a player
			const MessageID GameRevealPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 62);
			HRESULT GameRevealPlayerCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				UINT16 numberofTargetPlayerID = 0; PlayerID* pTargetPlayerID = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofTargetPlayerID, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pTargetPlayerID, pCur, iMsgSize, sizeof(PlayerID)*numberofTargetPlayerID ) );
				m_TargetPlayerID.SetLinkedBuffer(numberofTargetPlayerID, numberofTargetPlayerID, pTargetPlayerID);


			Proc_End:

				return hr;

			}; // HRESULT GameRevealPlayerCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GameRevealPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const Array<PlayerID>& InTargetPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)*InTargetPlayerID.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameRevealPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				UINT16 numberOfInTargetPlayerID = (UINT16)InTargetPlayerID.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInTargetPlayerID, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InTargetPlayerID.data(), (INT)(sizeof(PlayerID)*InTargetPlayerID.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GameRevealPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const Array<PlayerID>& InTargetPlayerID )



			VOID GameRevealPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GameRevealPlayerCmd:%1%:%2% , TargetPlayerID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgArray<PlayerID>(m_TargetPlayerID)); 
			}; // VOID GameRevealPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GameRevealPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 62);
			HRESULT GameRevealPlayerRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				UINT16 numberofRevealedPlayerID = 0; PlayerID* pRevealedPlayerID = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofRevealedPlayerID, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pRevealedPlayerID, pCur, iMsgSize, sizeof(PlayerID)*numberofRevealedPlayerID ) );
				m_RevealedPlayerID.SetLinkedBuffer(numberofRevealedPlayerID, numberofRevealedPlayerID, pRevealedPlayerID);
				UINT16 numberofRevealedRole = 0; PlayerRole* pRevealedRole = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofRevealedRole, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pRevealedRole, pCur, iMsgSize, sizeof(PlayerRole)*numberofRevealedRole ) );
				m_RevealedRole.SetLinkedBuffer(numberofRevealedRole, numberofRevealedRole, pRevealedRole);
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // HRESULT GameRevealPlayerRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GameRevealPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(PlayerID)*InRevealedPlayerID.GetSize() + sizeof(UINT16)
					+ sizeof(PlayerRole)*InRevealedRole.GetSize() + sizeof(UINT16)
					+ sizeof(UINT64)
					+ sizeof(UINT64));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameRevealPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				UINT16 numberOfInRevealedPlayerID = (UINT16)InRevealedPlayerID.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInRevealedPlayerID, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InRevealedPlayerID.data(), (INT)(sizeof(PlayerID)*InRevealedPlayerID.GetSize())); 
				UINT16 numberOfInRevealedRole = (UINT16)InRevealedRole.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInRevealedRole, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InRevealedRole.data(), (INT)(sizeof(PlayerRole)*InRevealedRole.GetSize())); 
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GameRevealPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )



			VOID GameRevealPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GameRevealPlayerRes:%1%:%2% , Result:%3%, RevealedPlayerID:%4%, RevealedRole:%5%, TotalGem:%6%, TotalGameMoney:%7%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), ArgArray<PlayerID>(m_RevealedPlayerID), ArgArray<PlayerRole>(m_RevealedRole), m_TotalGem, m_TotalGameMoney); 
			}; // VOID GameRevealPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Player. revive himself
			const MessageID GamePlayerReviveCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 63);
			HRESULT GamePlayerReviveCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();



			Proc_End:

				return hr;

			}; // HRESULT GamePlayerReviveCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GamePlayerReviveCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayerReviveCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerReviveCmd::BuildIMsg( OUT MessageData* &pMsg )



			VOID GamePlayerReviveCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GamePlayerReviveCmd:%1%:%2% ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // VOID GamePlayerReviveCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GamePlayerReviveRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 63);
			HRESULT GamePlayerReviveRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerReviveRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GamePlayerReviveRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(UINT64)
					+ sizeof(UINT64));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayerReviveRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerReviveRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )



			VOID GamePlayerReviveRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GamePlayerReviveRes:%1%:%2% , Result:%3%, TotalGem:%4%, TotalGameMoney:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_TotalGem, m_TotalGameMoney); 
			}; // VOID GamePlayerReviveRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player is revived
			const MessageID GamePlayerRevivedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 64);
			HRESULT GamePlayerRevivedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RevivedPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerRevivedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT GamePlayerRevivedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InRevivedPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayerRevivedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRevivedPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerRevivedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InRevivedPlayerID )



			VOID GamePlayerRevivedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GamePlayerRevivedS2CEvt:%1%:%2% , RevivedPlayerID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RevivedPlayerID); 
			}; // VOID GamePlayerRevivedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Player. reset ranking
			const MessageID GamePlayerResetRankCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 65);
			HRESULT GamePlayerResetRankCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();



			Proc_End:

				return hr;

			}; // HRESULT GamePlayerResetRankCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GamePlayerResetRankCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayerResetRankCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerResetRankCmd::BuildIMsg( OUT MessageData* &pMsg )



			VOID GamePlayerResetRankCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GamePlayerResetRankCmd:%1%:%2% ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // VOID GamePlayerResetRankCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GamePlayerResetRankRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 65);
			HRESULT GamePlayerResetRankRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerResetRankRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GamePlayerResetRankRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(UINT64)
					+ sizeof(UINT64));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayerResetRankRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerResetRankRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )



			VOID GamePlayerResetRankRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GamePlayerResetRankRes:%1%:%2% , Result:%3%, TotalGem:%4%, TotalGameMoney:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_TotalGem, m_TotalGameMoney); 
			}; // VOID GamePlayerResetRankRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Request Game match
			const MessageID RequestGameMatchCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 66);
			HRESULT RequestGameMatchCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_NumPlayer, pCur, iMsgSize, sizeof(BYTE) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RequestRole, pCur, iMsgSize, sizeof(PlayerRole) ) );


			Proc_End:

				return hr;

			}; // HRESULT RequestGameMatchCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT RequestGameMatchCmd::BuildIMsg( OUT MessageData* &pMsg, const BYTE &InNumPlayer, const PlayerRole &InRequestRole )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(BYTE)
					+ sizeof(PlayerRole));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RequestGameMatchCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InNumPlayer, sizeof(BYTE));
				Protocol::PackParamCopy( pMsgData, &InRequestRole, sizeof(PlayerRole));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RequestGameMatchCmd::BuildIMsg( OUT MessageData* &pMsg, const BYTE &InNumPlayer, const PlayerRole &InRequestRole )



			VOID RequestGameMatchCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:RequestGameMatchCmd:%1%:%2% , NumPlayer:%3%, RequestRole:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NumPlayer, m_RequestRole); 
			}; // VOID RequestGameMatchCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RequestGameMatchRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 66);
			HRESULT RequestGameMatchRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // HRESULT RequestGameMatchRes::ParseIMsg( MessageData* pIMsg )

			HRESULT RequestGameMatchRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(UINT64)
					+ sizeof(UINT64));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RequestGameMatchRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RequestGameMatchRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )



			VOID RequestGameMatchRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:RequestGameMatchRes:%1%:%2% , Result:%3%, TotalGem:%4%, TotalGameMoney:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_TotalGem, m_TotalGameMoney); 
			}; // VOID RequestGameMatchRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Game matched
			const MessageID GameMatchedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 67);
			HRESULT GameMatchedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameState, pCur, iMsgSize, sizeof(GameStateID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Day, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MaxPlayer, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerIndex, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerCharacter, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Dead, pCur, iMsgSize, sizeof(UINT8) ) );
				UINT16 numberofChatHistoryData = 0; BYTE* pChatHistoryData = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofChatHistoryData, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pChatHistoryData, pCur, iMsgSize, sizeof(BYTE)*numberofChatHistoryData ) );
				m_ChatHistoryData.SetLinkedBuffer(numberofChatHistoryData, numberofChatHistoryData, pChatHistoryData);
				UINT16 numberofGameLogData = 0; BYTE* pGameLogData = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofGameLogData, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pGameLogData, pCur, iMsgSize, sizeof(BYTE)*numberofGameLogData ) );
				m_GameLogData.SetLinkedBuffer(numberofGameLogData, numberofGameLogData, pGameLogData);
				protocolChk( Protocol::StreamParamCopy( &m_Stamina, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // HRESULT GameMatchedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT GameMatchedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData, const UINT32 &InStamina, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(UINT32)
					+ sizeof(GameStateID)
					+ sizeof(UINT8)
					+ sizeof(UINT8)
					+ sizeof(UINT8)
					+ sizeof(UINT8)
					+ sizeof(PlayerRole)
					+ sizeof(UINT8)
					+ sizeof(BYTE)*InChatHistoryData.GetSize() + sizeof(UINT16)
					+ sizeof(BYTE)*InGameLogData.GetSize() + sizeof(UINT16)
					+ sizeof(UINT32)
					+ sizeof(UINT64)
					+ sizeof(UINT64));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameMatchedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InGameState, sizeof(GameStateID));
				Protocol::PackParamCopy( pMsgData, &InDay, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InMaxPlayer, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InPlayerIndex, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InPlayerCharacter, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &InDead, sizeof(UINT8));
				UINT16 numberOfInChatHistoryData = (UINT16)InChatHistoryData.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInChatHistoryData, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InChatHistoryData.data(), (INT)(sizeof(BYTE)*InChatHistoryData.GetSize())); 
				UINT16 numberOfInGameLogData = (UINT16)InGameLogData.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInGameLogData, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InGameLogData.data(), (INT)(sizeof(BYTE)*InGameLogData.GetSize())); 
				Protocol::PackParamCopy( pMsgData, &InStamina, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GameMatchedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData, const UINT32 &InStamina, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )



			VOID GameMatchedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GameMatchedS2CEvt:%1%:%2% , InsUID:%3%, TimeStamp:%4%, GameState:%5%, Day:%6%, MaxPlayer:%7%, PlayerIndex:%8%, PlayerCharacter:%9%, Role:%10%, Dead:%11%, ChatHistoryData:%12%, GameLogData:%13%, Stamina:%14%, TotalGem:%15%, TotalGameMoney:%16%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InsUID, m_TimeStamp, m_GameState, m_Day, m_MaxPlayer, m_PlayerIndex, m_PlayerCharacter, m_Role, m_Dead, ArgArray<BYTE>(m_ChatHistoryData), ArgArray<BYTE>(m_GameLogData), m_Stamina, m_TotalGem, m_TotalGameMoney); 
			}; // VOID GameMatchedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Game match failed
			const MessageID GameMatchFailedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 68);
			HRESULT GameMatchFailedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_FailedReason, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT GameMatchFailedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT GameMatchFailedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InFailedReason )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameMatchFailedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InFailedReason, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GameMatchFailedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InFailedReason )



			VOID GameMatchFailedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GameMatchFailedS2CEvt:%1%:%2% , FailedReason:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_FailedReason)); 
			}; // VOID GameMatchFailedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Game matching started
			const MessageID GameMatchingStartedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 69);
			HRESULT GameMatchingStartedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();



			Proc_End:

				return hr;

			}; // HRESULT GameMatchingStartedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT GameMatchingStartedS2CEvt::BuildIMsg( OUT MessageData* &pMsg )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameMatchingStartedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GameMatchingStartedS2CEvt::BuildIMsg( OUT MessageData* &pMsg )



			VOID GameMatchingStartedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GameMatchingStartedS2CEvt:%1%:%2% ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // VOID GameMatchingStartedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Cancel Game match
			const MessageID CancelGameMatchCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 70);
			HRESULT CancelGameMatchCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();



			Proc_End:

				return hr;

			}; // HRESULT CancelGameMatchCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT CancelGameMatchCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::CancelGameMatchCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT CancelGameMatchCmd::BuildIMsg( OUT MessageData* &pMsg )



			VOID CancelGameMatchCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:CancelGameMatchCmd:%1%:%2% ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // VOID CancelGameMatchCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID CancelGameMatchRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 70);
			HRESULT CancelGameMatchRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT CancelGameMatchRes::ParseIMsg( MessageData* pIMsg )

			HRESULT CancelGameMatchRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::CancelGameMatchRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT CancelGameMatchRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID CancelGameMatchRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:CancelGameMatchRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID CancelGameMatchRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: game matching canceled
			const MessageID GameMatchingCanceledS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 71);
			HRESULT GameMatchingCanceledS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();



			Proc_End:

				return hr;

			}; // HRESULT GameMatchingCanceledS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT GameMatchingCanceledS2CEvt::BuildIMsg( OUT MessageData* &pMsg )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameMatchingCanceledS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GameMatchingCanceledS2CEvt::BuildIMsg( OUT MessageData* &pMsg )



			VOID GameMatchingCanceledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GameMatchingCanceledS2CEvt:%1%:%2% ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // VOID GameMatchingCanceledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Buy shop item prepare
			const MessageID BuyShopItemPrepareCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 72);
			HRESULT BuyShopItemPrepareCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_ShopItemID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT BuyShopItemPrepareCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT BuyShopItemPrepareCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InShopItemID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::BuyShopItemPrepareCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InShopItemID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT BuyShopItemPrepareCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InShopItemID )



			VOID BuyShopItemPrepareCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:BuyShopItemPrepareCmd:%1%:%2% , ShopItemID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_ShopItemID); 
			}; // VOID BuyShopItemPrepareCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID BuyShopItemPrepareRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 72);
			HRESULT BuyShopItemPrepareRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ShopItemID, pCur, iMsgSize, sizeof(UINT32) ) );
				UINT16 uiSizeOfPurchaseID = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPurchaseID, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_PurchaseID, pCur, iMsgSize, sizeof(char)*uiSizeOfPurchaseID ) );


			Proc_End:

				return hr;

			}; // HRESULT BuyShopItemPrepareRes::ParseIMsg( MessageData* pIMsg )

			HRESULT BuyShopItemPrepareRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT32 &InShopItemID, const char* InPurchaseID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInPurchaseIDLength = InPurchaseID ? (UINT16)(strlen(InPurchaseID)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInPurchaseIDLength 
					+ sizeof(HRESULT)
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::BuyShopItemPrepareRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InShopItemID, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &__uiInPurchaseIDLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InPurchaseID ? InPurchaseID : "", __uiInPurchaseIDLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT BuyShopItemPrepareRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT32 &InShopItemID, const char* InPurchaseID )



			VOID BuyShopItemPrepareRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:BuyShopItemPrepareRes:%1%:%2% , Result:%3%, ShopItemID:%4%, PurchaseID:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_ShopItemID, m_PurchaseID); 
			}; // VOID BuyShopItemPrepareRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Buy shop item
			const MessageID BuyShopItemCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 73);
			HRESULT BuyShopItemCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_ShopItemID, pCur, iMsgSize, sizeof(UINT32) ) );
				UINT16 uiSizeOfPlatform = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPlatform, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_Platform, pCur, iMsgSize, sizeof(char)*uiSizeOfPlatform ) );
				UINT16 uiSizeOfPackageName = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPackageName, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_PackageName, pCur, iMsgSize, sizeof(char)*uiSizeOfPackageName ) );
				UINT16 uiSizeOfPurchaseTransactionID = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPurchaseTransactionID, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_PurchaseTransactionID, pCur, iMsgSize, sizeof(char)*uiSizeOfPurchaseTransactionID ) );
				UINT16 numberofPurchaseToken = 0; BYTE* pPurchaseToken = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofPurchaseToken, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pPurchaseToken, pCur, iMsgSize, sizeof(BYTE)*numberofPurchaseToken ) );
				m_PurchaseToken.SetLinkedBuffer(numberofPurchaseToken, numberofPurchaseToken, pPurchaseToken);


			Proc_End:

				return hr;

			}; // HRESULT BuyShopItemCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT BuyShopItemCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InShopItemID, const char* InPlatform, const char* InPackageName, const char* InPurchaseTransactionID, const Array<BYTE>& InPurchaseToken )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInPlatformLength = InPlatform ? (UINT16)(strlen(InPlatform)+1) : 1;
				UINT16 __uiInPackageNameLength = InPackageName ? (UINT16)(strlen(InPackageName)+1) : 1;
				UINT16 __uiInPurchaseTransactionIDLength = InPurchaseTransactionID ? (UINT16)(strlen(InPurchaseTransactionID)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInPlatformLength + sizeof(UINT16) + __uiInPackageNameLength + sizeof(UINT16) + __uiInPurchaseTransactionIDLength 
					+ sizeof(UINT32)
					+ sizeof(BYTE)*InPurchaseToken.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::BuyShopItemCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InShopItemID, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &__uiInPlatformLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InPlatform ? InPlatform : "", __uiInPlatformLength );
				Protocol::PackParamCopy( pMsgData, &__uiInPackageNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InPackageName ? InPackageName : "", __uiInPackageNameLength );
				Protocol::PackParamCopy( pMsgData, &__uiInPurchaseTransactionIDLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InPurchaseTransactionID ? InPurchaseTransactionID : "", __uiInPurchaseTransactionIDLength );
				UINT16 numberOfInPurchaseToken = (UINT16)InPurchaseToken.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInPurchaseToken, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InPurchaseToken.data(), (INT)(sizeof(BYTE)*InPurchaseToken.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT BuyShopItemCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InShopItemID, const char* InPlatform, const char* InPackageName, const char* InPurchaseTransactionID, const Array<BYTE>& InPurchaseToken )



			VOID BuyShopItemCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:BuyShopItemCmd:%1%:%2% , ShopItemID:%3%, Platform:%4%, PackageName:%5%, PurchaseTransactionID:%6%, PurchaseToken:%7%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_ShopItemID, m_Platform, m_PackageName, m_PurchaseTransactionID, ArgArray<BYTE>(m_PurchaseToken)); 
			}; // VOID BuyShopItemCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID BuyShopItemRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 73);
			HRESULT BuyShopItemRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ShopItemID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT BuyShopItemRes::ParseIMsg( MessageData* pIMsg )

			HRESULT BuyShopItemRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT32 &InShopItemID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::BuyShopItemRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InShopItemID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT BuyShopItemRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const UINT32 &InShopItemID )



			VOID BuyShopItemRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:BuyShopItemRes:%1%:%2% , Result:%3%, ShopItemID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_ShopItemID); 
			}; // VOID BuyShopItemRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Give my stamina to other player
			const MessageID GiveStaminaCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 74);
			HRESULT GiveStaminaCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TargetPlayer, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT GiveStaminaCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GiveStaminaCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InTargetPlayer )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GiveStaminaCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTargetPlayer, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GiveStaminaCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InTargetPlayer )



			VOID GiveStaminaCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GiveStaminaCmd:%1%:%2% , TargetPlayer:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TargetPlayer); 
			}; // VOID GiveStaminaCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GiveStaminaRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 74);
			HRESULT GiveStaminaRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TargetPlayer, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // HRESULT GiveStaminaRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GiveStaminaRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PlayerID &InTargetPlayer, const UINT64 &InTimeStamp )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT)
					+ sizeof(PlayerID)
					+ sizeof(UINT64));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GiveStaminaRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InTargetPlayer, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GiveStaminaRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult, const PlayerID &InTargetPlayer, const UINT64 &InTimeStamp )



			VOID GiveStaminaRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GiveStaminaRes:%1%:%2% , Result:%3%, TargetPlayer:%4%, TimeStamp:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result), m_TargetPlayer, m_TimeStamp); 
			}; // VOID GiveStaminaRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: For debug, Change configue preset
			const MessageID SetPresetGameConfigIDCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 75);
			HRESULT SetPresetGameConfigIDCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PresetID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT SetPresetGameConfigIDCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT SetPresetGameConfigIDCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InPresetID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetPresetGameConfigIDCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPresetID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT SetPresetGameConfigIDCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InPresetID )



			VOID SetPresetGameConfigIDCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:SetPresetGameConfigIDCmd:%1%:%2% , PresetID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PresetID); 
			}; // VOID SetPresetGameConfigIDCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID SetPresetGameConfigIDRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 75);
			HRESULT SetPresetGameConfigIDRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT SetPresetGameConfigIDRes::ParseIMsg( MessageData* pIMsg )

			HRESULT SetPresetGameConfigIDRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetPresetGameConfigIDRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT SetPresetGameConfigIDRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID SetPresetGameConfigIDRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:SetPresetGameConfigIDRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID SetPresetGameConfigIDRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: For Debug
			const MessageID GainGameResourceCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 76);
			HRESULT GainGameResourceCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Resource, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Value, pCur, iMsgSize, sizeof(INT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT GainGameResourceCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GainGameResourceCmd::BuildIMsg( OUT MessageData* &pMsg, const INT32 &InResource, const INT32 &InValue )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(INT32)
					+ sizeof(INT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GainGameResourceCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResource, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InValue, sizeof(INT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GainGameResourceCmd::BuildIMsg( OUT MessageData* &pMsg, const INT32 &InResource, const INT32 &InValue )



			VOID GainGameResourceCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GainGameResourceCmd:%1%:%2% , Resource:%3%, Value:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Resource, m_Value); 
			}; // VOID GainGameResourceCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GainGameResourceRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 76);
			HRESULT GainGameResourceRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT GainGameResourceRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GainGameResourceRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GainGameResourceRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GainGameResourceRes::BuildIMsg( OUT MessageData* &pMsg, const HRESULT &InResult )



			VOID GainGameResourceRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GainGameResourceRes:%1%:%2% , Result:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, ArgHex32(m_Result)); 
			}; // VOID GainGameResourceRes::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace Game
	}; // namespace Message
}; // namespace BR

