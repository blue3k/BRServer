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
#include "Common/ToStringSvr.h"
#include "Common/ToStringGame.h"
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
			HRESULT RegisterEntityCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfEntName = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LocalEntID, pCur, iMsgSize, sizeof(EntityID) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfEntName, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_EntName, pCur, iMsgSize, sizeof(char)*uiSizeOfEntName ) );


			Proc_End:

				return hr;

			}; // HRESULT RegisterEntityCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT RegisterEntityCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const EntityID &InLocalEntID, const char* InEntName )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInEntNameLength = InEntName ? (UINT16)(strlen(InEntName)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) +  + sizeof(UINT16) + __uiInEntNameLength 
					+ sizeof(Context)
					+ sizeof(EntityID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( EntityServer::RegisterEntityCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InLocalEntID, sizeof(EntityID));
				Protocol::PackParamCopy( pMsgData, &__uiInEntNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InEntName ? InEntName : "", __uiInEntNameLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RegisterEntityCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const EntityID &InLocalEntID, const char* InEntName )



			void RegisterEntityCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:RegisterEntityCmd:{1}:{2} , Context:{3}, LocalEntID:{4}, EntName:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_LocalEntID, m_EntName); 
			}; // void RegisterEntityCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RegisterEntityRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_ENTITYSERVER, 0);
			HRESULT RegisterEntityRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_EntUID, pCur, iMsgSize, sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // HRESULT RegisterEntityRes::ParseIMsg( MessageData* pIMsg )

			HRESULT RegisterEntityRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const EntityUID &InEntUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(EntityUID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( EntityServer::RegisterEntityRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InEntUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RegisterEntityRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const EntityUID &InEntUID )



			void RegisterEntityRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:RegisterEntityRes:{1}:{2} , Context:{3}, Result:{4:X8}, EntUID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_Result, m_EntUID); 
			}; // void RegisterEntityRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Find Entity
			const MessageID UnregisterEntityCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_ENTITYSERVER, 1);
			HRESULT UnregisterEntityCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_EntUID, pCur, iMsgSize, sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // HRESULT UnregisterEntityCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT UnregisterEntityCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const EntityUID &InEntUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(EntityUID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( EntityServer::UnregisterEntityCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InEntUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT UnregisterEntityCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const EntityUID &InEntUID )



			void UnregisterEntityCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:UnregisterEntityCmd:{1}:{2} , Context:{3}, EntUID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_EntUID); 
			}; // void UnregisterEntityCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID UnregisterEntityRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_ENTITYSERVER, 1);
			HRESULT UnregisterEntityRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );


			Proc_End:

				return hr;

			}; // HRESULT UnregisterEntityRes::ParseIMsg( MessageData* pIMsg )

			HRESULT UnregisterEntityRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( EntityServer::UnregisterEntityRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT UnregisterEntityRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult )



			void UnregisterEntityRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:UnregisterEntityRes:{1}:{2} , Context:{3}, Result:{4:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_Result); 
			}; // void UnregisterEntityRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Find Entity
			const MessageID FindEntityCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_ENTITYSERVER, 2);
			HRESULT FindEntityCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LocalEntID, pCur, iMsgSize, sizeof(EntityID) ) );


			Proc_End:

				return hr;

			}; // HRESULT FindEntityCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT FindEntityCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const EntityID &InLocalEntID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(EntityID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( EntityServer::FindEntityCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InLocalEntID, sizeof(EntityID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT FindEntityCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const EntityID &InLocalEntID )



			void FindEntityCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:FindEntityCmd:{1}:{2} , Context:{3}, LocalEntID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_LocalEntID); 
			}; // void FindEntityCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID FindEntityRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_ENTITYSERVER, 2);
			HRESULT FindEntityRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_EntUID, pCur, iMsgSize, sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // HRESULT FindEntityRes::ParseIMsg( MessageData* pIMsg )

			HRESULT FindEntityRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const EntityUID &InEntUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(EntityUID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( EntityServer::FindEntityRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InEntUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT FindEntityRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const EntityUID &InEntUID )



			void FindEntityRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:FindEntityRes:{1}:{2} , Context:{3}, Result:{4:X8}, EntUID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_Result, m_EntUID); 
			}; // void FindEntityRes::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace EntityServer
	}; // namespace Message
}; // namespace BR


