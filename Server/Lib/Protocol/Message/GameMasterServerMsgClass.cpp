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
#include "Common/PolicyID.h"
#include "Common/ToStringSvr.h"
#include "Common/ToStringGame.h"
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
			const MessageID PlayerEnteredC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEMASTERSERVER, 0);
			Result PlayerEnteredC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfPlayerName = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, (int)sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPlayerName, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_PlayerName, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfPlayerName ) );


			Proc_End:

				return hr;

			}; // Result PlayerEnteredC2SEvt::ParseMessage( MessageData* pIMsg )

			Result PlayerEnteredC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const AccountID &InPlayerID, const char* InPlayerName )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInPlayerNameLength = InPlayerName ? (UINT16)(strlen(InPlayerName)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) +  + sizeof(UINT16) + __uiInPlayerNameLength 
					+ sizeof(AccountID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( GameMasterServer::PlayerEnteredC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &__uiInPlayerNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InPlayerName ? InPlayerName : "", __uiInPlayerNameLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PlayerEnteredC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const AccountID &InPlayerID, const char* InPlayerName )



			void PlayerEnteredC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PlayerEnteredC2SEvt:{1}:{2} , PlayerID:{3}, PlayerName:{4,60}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerID, m_PlayerName); 
			}; // void PlayerEnteredC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Player leaved
			const MessageID PlayerLeavedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEMASTERSERVER, 1);
			Result PlayerLeavedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

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

				BYTE *pMsgData = nullptr;

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


