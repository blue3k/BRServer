////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameMasterServer Message parser implementations
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
#include "Protocol/Message/GameMasterServerMsgClass.h"
#include "Protocol/ProtocolTrace.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Message
	{
 		namespace GameMasterServer
		{
 			// C2S: Player entered
			const MessageID PlayerEnteredC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEMASTERSERVER, 0);
			Result PlayerEnteredC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfPlayerName = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPlayerName, pCur, iMsgSize, (int)sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_PlayerName, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfPlayerName ) );


			Proc_End:

				return hr;

			}; // Result PlayerEnteredC2SEvt::ParseMessage( MessageData* pIMsg )

			Result PlayerEnteredC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const AccountID &InPlayerID, const char* InPlayerName )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInPlayerNameLength = InPlayerName ? (uint16_t)(strlen(InPlayerName)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) +  + sizeof(uint16_t) + __uiInPlayerNameLength 
					+ sizeof(AccountID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( GameMasterServer::PlayerEnteredC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &__uiInPlayerNameLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InPlayerName ? InPlayerName : "", __uiInPlayerNameLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PlayerEnteredC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const AccountID &InPlayerID, const char* InPlayerName )



			void PlayerEnteredC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PlayerEnteredC2SEvt:{1}:{2} , PlayerID:{3}, PlayerName:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerID, m_PlayerName); 
			}; // void PlayerEnteredC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Player leaved
			const MessageID PlayerLeavedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEMASTERSERVER, 1);
			Result PlayerLeavedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(AccountID) ) );


			Proc_End:

				return hr;

			}; // Result PlayerLeavedC2SEvt::ParseMessage( MessageData* pIMsg )

			Result PlayerLeavedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const AccountID &InPlayerID )
			{
 				Result hr;

				uint8_t *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(AccountID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( GameMasterServer::PlayerLeavedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(AccountID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PlayerLeavedC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const AccountID &InPlayerID )



			void PlayerLeavedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PlayerLeavedC2SEvt:{1}:{2} , PlayerID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerID); 
			}; // void PlayerLeavedC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace GameMasterServer
	}; // namespace Message
}; // namespace BR


