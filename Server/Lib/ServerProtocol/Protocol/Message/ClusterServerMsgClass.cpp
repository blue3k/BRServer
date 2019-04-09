////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : ClusterServer Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "Protocol/SFProtocol.h"
#include "String/SFToString.h"
#include "Net/SFNetToString.h"
#include "Container/SFArray.h"
#include "Protocol/SFProtocolHelper.h"
#include "Protocol/Message/ClusterServerMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace ClusterServer
		{
 			// Cmd: Cluster member list query
			const MessageID GetClusterMemberListCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 0);
			Result GetClusterMemberListCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListCmd::ParseMessage( MessageData* pIMsg )


			Result GetClusterMemberListCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) GetClusterMemberListCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GetClusterMemberListCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* GetClusterMemberListCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint64_t)
					+ sizeof(uint16_t)
					+ sizeof(ClusterID));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, ClusterServer::GetClusterMemberListCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GetClusterMemberListCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )

			Result GetClusterMemberListCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result GetClusterMemberListCmd::OverrideRouteContextDestination( EntityUID to )

			Result GetClusterMemberListCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(uint64_t); iMsgSize -= sizeof(uint64_t);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result GetClusterMemberListCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				GetClusterMemberListCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug2, "GetClusterMemberList:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, ClusterID:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetClusterID()); 
				return ResultCode::SUCCESS;
			}; // Result GetClusterMemberListCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID GetClusterMemberListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 0);
			Result GetClusterMemberListRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofMemberList = 0; ServiceInformation* pMemberList = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofMemberList, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pMemberList, pCur, iMsgSize, sizeof(ServiceInformation)*numberofMemberList ) );
				m_MemberList.SetLinkedBuffer(numberofMemberList, numberofMemberList, pMemberList);


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListRes::ParseMessage( MessageData* pIMsg )


			Result GetClusterMemberListRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) GetClusterMemberListRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GetClusterMemberListRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* GetClusterMemberListRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint64_t)
					+ sizeof(Result)
					+ sizeof(ServiceInformation)*InMemberList.size() + sizeof(uint16_t));


				uint16_t numberOfInMemberList = (uint16_t)InMemberList.size(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, ClusterServer::GetClusterMemberListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInMemberList, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InMemberList.data(), (INT)(sizeof(ServiceInformation)*InMemberList.size())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GetClusterMemberListRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )

			Result GetClusterMemberListRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result GetClusterMemberListRes::OverrideRouteContextDestination( EntityUID to )

			Result GetClusterMemberListRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(uint64_t); iMsgSize -= sizeof(uint64_t);
				pCur += sizeof(Result); iMsgSize -= sizeof(Result);
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);
				pCur += sizeof(ServiceInformation)*m_MemberList.size(); iMsgSize -= (INT)(sizeof(ServiceInformation)*m_MemberList.size());


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result GetClusterMemberListRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				GetClusterMemberListRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug2, "GetClusterMemberList:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, MemberList:{5,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetMemberList()); 
				return ResultCode::SUCCESS;
			}; // Result GetClusterMemberListRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// Cmd: Join to the cluster
			const MessageID RequestDataSyncCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 1);
			Result RequestDataSyncCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );


			Proc_End:

				return hr;

			}; // Result RequestDataSyncCmd::ParseMessage( MessageData* pIMsg )


			Result RequestDataSyncCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) RequestDataSyncCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RequestDataSyncCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* RequestDataSyncCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint64_t)
					+ sizeof(uint16_t)
					+ sizeof(ClusterID));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, ClusterServer::RequestDataSyncCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RequestDataSyncCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )

			Result RequestDataSyncCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RequestDataSyncCmd::OverrideRouteContextDestination( EntityUID to )

			Result RequestDataSyncCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(uint64_t); iMsgSize -= sizeof(uint64_t);
				Assert( iMsgSize >= (INT)sizeof(uint16_t) );
				*(uint16_t*)pCur = hopCount;
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);


			Proc_End:

				return hr;

			}; // Result RequestDataSyncCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result RequestDataSyncCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				RequestDataSyncCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug2, "RequestDataSync:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, ClusterID:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetClusterID()); 
				return ResultCode::SUCCESS;
			}; // Result RequestDataSyncCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID RequestDataSyncRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 1);
			Result RequestDataSyncRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result RequestDataSyncRes::ParseMessage( MessageData* pIMsg )


			Result RequestDataSyncRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) RequestDataSyncRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RequestDataSyncRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* RequestDataSyncRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint64_t)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, ClusterServer::RequestDataSyncRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RequestDataSyncRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result RequestDataSyncRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RequestDataSyncRes::OverrideRouteContextDestination( EntityUID to )

			Result RequestDataSyncRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(uint64_t); iMsgSize -= sizeof(uint64_t);
				pCur += sizeof(Result); iMsgSize -= sizeof(Result);


			Proc_End:

				return hr;

			}; // Result RequestDataSyncRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result RequestDataSyncRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				RequestDataSyncRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug2, "RequestDataSync:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result RequestDataSyncRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)



		}; // namespace ClusterServer
	}; // namespace Message
}; // namespace SF


