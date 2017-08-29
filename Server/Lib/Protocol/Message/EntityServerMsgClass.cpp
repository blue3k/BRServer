////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : EntityServer Message parser implementations
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
#include "Protocol/Message/EntityServerMsgClass.h"
#include "Protocol/ProtocolTrace.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Message
	{
 		namespace EntityServer
		{
 			// Cmd: Register entity
			const MessageID RegisterEntityCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_ENTITYSERVER, 0);
			Result RegisterEntityCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfEntName = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LocalEntID, pCur, iMsgSize, (int)sizeof(EntityID) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfEntName, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_EntName, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfEntName ) );


			Proc_End:

				return hr;

			}; // Result RegisterEntityCmd::ParseMessage( MessageData* pIMsg )

			Result RegisterEntityCmd::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInEntNameLength = InEntName ? (UINT16)(strlen(InEntName)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) +  + sizeof(UINT16) + __uiInEntNameLength 
					+ sizeof(TransactionID)
					+ sizeof(EntityID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( EntityServer::RegisterEntityCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InLocalEntID, sizeof(EntityID));
				Protocol::PackParamCopy( pMsgData, &__uiInEntNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InEntName ? InEntName : "", __uiInEntNameLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RegisterEntityCmd::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName )



			void RegisterEntityCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:RegisterEntityCmd:{1}:{2} , TransactionID:{3}, LocalEntID:{4}, EntName:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TransactionID, m_LocalEntID, m_EntName); 
			}; // void RegisterEntityCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RegisterEntityRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_ENTITYSERVER, 0);
			Result RegisterEntityRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_EntUID, pCur, iMsgSize, (int)sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // Result RegisterEntityRes::ParseMessage( MessageData* pIMsg )

			Result RegisterEntityRes::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InEntUID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(EntityUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( EntityServer::RegisterEntityRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InEntUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RegisterEntityRes::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InEntUID )



			void RegisterEntityRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:RegisterEntityRes:{1}:{2} , TransactionID:{3}, Result:{4:X8}, EntUID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TransactionID, m_Result, m_EntUID); 
			}; // void RegisterEntityRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Find Entity
			const MessageID UnregisterEntityCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_ENTITYSERVER, 1);
			Result UnregisterEntityCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_EntUID, pCur, iMsgSize, (int)sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // Result UnregisterEntityCmd::ParseMessage( MessageData* pIMsg )

			Result UnregisterEntityCmd::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const EntityUID &InEntUID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(EntityUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( EntityServer::UnregisterEntityCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InEntUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result UnregisterEntityCmd::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const EntityUID &InEntUID )



			void UnregisterEntityCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:UnregisterEntityCmd:{1}:{2} , TransactionID:{3}, EntUID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TransactionID, m_EntUID); 
			}; // void UnregisterEntityCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID UnregisterEntityRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_ENTITYSERVER, 1);
			Result UnregisterEntityRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result UnregisterEntityRes::ParseMessage( MessageData* pIMsg )

			Result UnregisterEntityRes::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( EntityServer::UnregisterEntityRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result UnregisterEntityRes::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const Result &InResult )



			void UnregisterEntityRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:UnregisterEntityRes:{1}:{2} , TransactionID:{3}, Result:{4:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TransactionID, m_Result); 
			}; // void UnregisterEntityRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Find Entity
			const MessageID FindEntityCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_ENTITYSERVER, 2);
			Result FindEntityCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LocalEntID, pCur, iMsgSize, (int)sizeof(EntityID) ) );


			Proc_End:

				return hr;

			}; // Result FindEntityCmd::ParseMessage( MessageData* pIMsg )

			Result FindEntityCmd::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const EntityID &InLocalEntID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(EntityID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( EntityServer::FindEntityCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InLocalEntID, sizeof(EntityID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result FindEntityCmd::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const EntityID &InLocalEntID )



			void FindEntityCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:FindEntityCmd:{1}:{2} , TransactionID:{3}, LocalEntID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TransactionID, m_LocalEntID); 
			}; // void FindEntityCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID FindEntityRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_ENTITYSERVER, 2);
			Result FindEntityRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_EntUID, pCur, iMsgSize, (int)sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // Result FindEntityRes::ParseMessage( MessageData* pIMsg )

			Result FindEntityRes::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InEntUID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(EntityUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( EntityServer::FindEntityRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InEntUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result FindEntityRes::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InEntUID )



			void FindEntityRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:FindEntityRes:{1}:{2} , TransactionID:{3}, Result:{4:X8}, EntUID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TransactionID, m_Result, m_EntUID); 
			}; // void FindEntityRes::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace EntityServer
	}; // namespace Message
}; // namespace BR


