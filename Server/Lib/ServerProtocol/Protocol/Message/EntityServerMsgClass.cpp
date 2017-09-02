////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : EntityServer Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Protocol/Protocol.h"
#include "String/ToString.h"
#include "Net/NetToString.h"
#include "Container/SFArray.h"
#include "Protocol/ProtocolHelper.h"
#include "Protocol/Message/EntityServerMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace EntityServer
		{
 			// Cmd: Register entity
			const MessageID RegisterEntityCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_ENTITYSERVER, 0);
			Result RegisterEntityCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfEntName = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LocalEntID, pCur, iMsgSize, sizeof(EntityID) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfEntName, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_EntName, pCur, iMsgSize, sizeof(char)*uiSizeOfEntName ) );


			Proc_End:

				return hr;

			}; // Result RegisterEntityCmd::ParseMessage( MessageData* pIMsg )


			Result RegisterEntityCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RegisterEntityCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RegisterEntityCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* RegisterEntityCmd::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInEntNameLength = InEntName ? (uint16_t)(strlen(InEntName)+1) : 1;
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) +  + sizeof(uint16_t) + __uiInEntNameLength 
					+ sizeof(TransactionID)
					+ sizeof(EntityID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, EntityServer::RegisterEntityCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InLocalEntID, sizeof(EntityID));
				Protocol::PackParamCopy( pMsgData, &__uiInEntNameLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InEntName ? InEntName : "", __uiInEntNameLength );


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RegisterEntityCmd::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const EntityID &InLocalEntID, const char* InEntName )



			Result RegisterEntityCmd::TraceOut(MessageData* pMsg)
			{
 				RegisterEntityCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "RegisterEntity:{0}:{1} , TransactionID:{2}, LocalEntID:{3}, EntName:{4,60}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetTransactionID(), parser.GetLocalEntID(), parser.GetEntName()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterEntityCmd::TraceOut(MessageData* pMsg)

			const MessageID RegisterEntityRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_ENTITYSERVER, 0);
			Result RegisterEntityRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_EntUID, pCur, iMsgSize, sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result RegisterEntityRes::ParseMessage( MessageData* pIMsg )


			Result RegisterEntityRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RegisterEntityRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RegisterEntityRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* RegisterEntityRes::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InEntUID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(uint64_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, EntityServer::RegisterEntityRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InEntUID, sizeof(uint64_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RegisterEntityRes::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InEntUID )



			Result RegisterEntityRes::TraceOut(MessageData* pMsg)
			{
 				RegisterEntityRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "RegisterEntity:{0}:{1} , TransactionID:{2}, Result:{3:X8}, EntUID:{4}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetTransactionID(), parser.GetResult(), parser.GetEntUID()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterEntityRes::TraceOut(MessageData* pMsg)

			// Cmd: Find Entity
			const MessageID UnregisterEntityCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_ENTITYSERVER, 1);
			Result UnregisterEntityCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_EntUID, pCur, iMsgSize, sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result UnregisterEntityCmd::ParseMessage( MessageData* pIMsg )


			Result UnregisterEntityCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) UnregisterEntityCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result UnregisterEntityCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* UnregisterEntityCmd::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const uint64_t &InEntUID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(uint64_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, EntityServer::UnregisterEntityCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InEntUID, sizeof(uint64_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* UnregisterEntityCmd::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const uint64_t &InEntUID )



			Result UnregisterEntityCmd::TraceOut(MessageData* pMsg)
			{
 				UnregisterEntityCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "UnregisterEntity:{0}:{1} , TransactionID:{2}, EntUID:{3}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetTransactionID(), parser.GetEntUID()); 
				return ResultCode::SUCCESS;
			}; // Result UnregisterEntityCmd::TraceOut(MessageData* pMsg)

			const MessageID UnregisterEntityRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_ENTITYSERVER, 1);
			Result UnregisterEntityRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result UnregisterEntityRes::ParseMessage( MessageData* pIMsg )


			Result UnregisterEntityRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) UnregisterEntityRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result UnregisterEntityRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* UnregisterEntityRes::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, EntityServer::UnregisterEntityRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* UnregisterEntityRes::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const Result &InResult )



			Result UnregisterEntityRes::TraceOut(MessageData* pMsg)
			{
 				UnregisterEntityRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "UnregisterEntity:{0}:{1} , TransactionID:{2}, Result:{3:X8}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result UnregisterEntityRes::TraceOut(MessageData* pMsg)

			// Cmd: Find Entity
			const MessageID FindEntityCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_ENTITYSERVER, 2);
			Result FindEntityCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LocalEntID, pCur, iMsgSize, sizeof(EntityID) ) );


			Proc_End:

				return hr;

			}; // Result FindEntityCmd::ParseMessage( MessageData* pIMsg )


			Result FindEntityCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) FindEntityCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result FindEntityCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* FindEntityCmd::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const EntityID &InLocalEntID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(EntityID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, EntityServer::FindEntityCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InLocalEntID, sizeof(EntityID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* FindEntityCmd::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const EntityID &InLocalEntID )



			Result FindEntityCmd::TraceOut(MessageData* pMsg)
			{
 				FindEntityCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "FindEntity:{0}:{1} , TransactionID:{2}, LocalEntID:{3}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetTransactionID(), parser.GetLocalEntID()); 
				return ResultCode::SUCCESS;
			}; // Result FindEntityCmd::TraceOut(MessageData* pMsg)

			const MessageID FindEntityRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_ENTITYSERVER, 2);
			Result FindEntityRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_EntUID, pCur, iMsgSize, sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result FindEntityRes::ParseMessage( MessageData* pIMsg )


			Result FindEntityRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) FindEntityRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result FindEntityRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* FindEntityRes::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InEntUID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(uint64_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, EntityServer::FindEntityRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InEntUID, sizeof(uint64_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* FindEntityRes::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InEntUID )



			Result FindEntityRes::TraceOut(MessageData* pMsg)
			{
 				FindEntityRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "FindEntity:{0}:{1} , TransactionID:{2}, Result:{3:X8}, EntUID:{4}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetTransactionID(), parser.GetResult(), parser.GetEntUID()); 
				return ResultCode::SUCCESS;
			}; // Result FindEntityRes::TraceOut(MessageData* pMsg)



		}; // namespace EntityServer
	}; // namespace Message
}; // namespace SF


