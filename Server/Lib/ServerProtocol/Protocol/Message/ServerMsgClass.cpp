////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : Server Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Protocol/SFProtocol.h"
#include "String/SFToString.h"
#include "Net/SFNetToString.h"
#include "Container/SFArray.h"
#include "Protocol/SFProtocolHelper.h"
#include "Protocol/Message/ServerMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace Server
		{
 			// Cmd: Generic failure message
			const MessageID GenericFailureCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_SERVER, 0);
			Result GenericFailureCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );


			Proc_End:

				return hr;

			}; // Result GenericFailureCmd::ParseMessage( MessageData* pIMsg )


			Result GenericFailureCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) GenericFailureCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GenericFailureCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* GenericFailureCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, Server::GenericFailureCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GenericFailureCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID )

			Result GenericFailureCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GenericFailureCmd::OverrideRouteContextDestination( EntityUID to )


			Result GenericFailureCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				GenericFailureCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GenericFailure:{0}:{1} , RouteContext:{2}, TransactionID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID()); 
				return ResultCode::SUCCESS;
			}; // Result GenericFailureCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID GenericFailureRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_SERVER, 0);
			Result GenericFailureRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result GenericFailureRes::ParseMessage( MessageData* pIMsg )


			Result GenericFailureRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) GenericFailureRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GenericFailureRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* GenericFailureRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, Server::GenericFailureRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GenericFailureRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result GenericFailureRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GenericFailureRes::OverrideRouteContextDestination( EntityUID to )


			Result GenericFailureRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				GenericFailureRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GenericFailure:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result GenericFailureRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// C2S: Server Started or Connected
			const MessageID ServerConnectedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_SERVER, 1);
			Result ServerConnectedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_StartUpTime, pCur, iMsgSize, sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PrivateAddress, pCur, iMsgSize, sizeof(NetAddress) ) );


			Proc_End:

				return hr;

			}; // Result ServerConnectedC2SEvt::ParseMessage( MessageData* pIMsg )


			Result ServerConnectedC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) ServerConnectedC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result ServerConnectedC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* ServerConnectedC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint32_t)
					+ sizeof(NetAddress));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, Server::ServerConnectedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InStartUpTime, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InPrivateAddress, sizeof(NetAddress));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* ServerConnectedC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress )

			Result ServerConnectedC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result ServerConnectedC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result ServerConnectedC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				ServerConnectedC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug2, "ServerConnected:{0}:{1} , RouteContext:{2}, StartUpTime:{3}, PrivateAddress:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetStartUpTime(), parser.GetPrivateAddress()); 
				return ResultCode::SUCCESS;
			}; // Result ServerConnectedC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)



		}; // namespace Server
	}; // namespace Message
}; // namespace SF


