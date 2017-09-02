////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : ClusterServer Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Protocol/Protocol.h"
#include "String/ToString.h"
#include "Net/NetToString.h"
#include "Container/SFArray.h"
#include "Protocol/ProtocolHelper.h"
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

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListCmd::ParseMessage( MessageData* pIMsg )

			Result GetClusterMemberListCmd::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				GetClusterMemberListCmd parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("TransactionID", parser.GetTransactionID());
				variableBuilder.SetVariable("RouteHopCount", parser.GetRouteHopCount());
				variableBuilder.SetVariable("ClusterID", parser.GetClusterID());


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListCmd::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result GetClusterMemberListCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GetClusterMemberListCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GetClusterMemberListCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* GetClusterMemberListCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t)
					+ sizeof(ClusterID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, ClusterServer::GetClusterMemberListCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GetClusterMemberListCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )

			Result GetClusterMemberListCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
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

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				Assert( iMsgSize >= (INT)sizeof(uint16) );
				*(uint16*)pCur = hopCount;
				pCur += sizeof(uint16); iMsgSize -= sizeof(uint16);


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result GetClusterMemberListCmd::TraceOut(MessageData* pMsg)
			{
 				GetClusterMemberListCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "GetClusterMemberList:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, ClusterID:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetClusterID()); 
				return ResultCode::SUCCESS;
			}; // Result GetClusterMemberListCmd::TraceOut(MessageData* pMsg)

			const MessageID GetClusterMemberListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 0);
			Result GetClusterMemberListRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofMemberList = 0; ServiceInformation* pMemberList = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofMemberList, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pMemberList, pCur, iMsgSize, sizeof(ServiceInformation)*numberofMemberList ) );
				m_MemberList.SetLinkedBuffer(numberofMemberList, numberofMemberList, pMemberList);


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListRes::ParseMessage( MessageData* pIMsg )

			Result GetClusterMemberListRes::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				GetClusterMemberListRes parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("TransactionID", parser.GetTransactionID());
				variableBuilder.SetVariable("Result", parser.GetResult());
				variableBuilder.SetVariable("MemberList", (int)parser.GetMemberList().GetItemCount(), parser.GetMemberList().data());


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListRes::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result GetClusterMemberListRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GetClusterMemberListRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GetClusterMemberListRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* GetClusterMemberListRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(ServiceInformation)*InMemberList.GetItemCount() + sizeof(uint16_t));


				uint16_t numberOfInMemberList = (uint16_t)InMemberList.GetItemCount(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, ClusterServer::GetClusterMemberListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInMemberList, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InMemberList.data(), (INT)(sizeof(ServiceInformation)*InMemberList.GetItemCount())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GetClusterMemberListRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )

			Result GetClusterMemberListRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
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

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				pCur += sizeof(Result); iMsgSize -= sizeof(Result);
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);
				pCur += sizeof(ServiceInformation)*m_MemberList.GetItemCount(); iMsgSize -= (INT)(sizeof(ServiceInformation)*m_MemberList.GetSize());


			Proc_End:

				return hr;

			}; // Result GetClusterMemberListRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result GetClusterMemberListRes::TraceOut(MessageData* pMsg)
			{
 				GetClusterMemberListRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "GetClusterMemberList:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, MemberList:{5,30}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetMemberList()); 
				return ResultCode::SUCCESS;
			}; // Result GetClusterMemberListRes::TraceOut(MessageData* pMsg)

			// Cmd: Join to the cluster, This operation will be manually broadcasted and gathered the result
			const MessageID JoinClusterCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 1);
			Result JoinClusterCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Sender, pCur, iMsgSize, sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_SenderNetClass, pCur, iMsgSize, sizeof(NetClass) ) );
				protocolChk( Protocol::StreamParamCopy( &m_SenderAddress, pCur, iMsgSize, sizeof(NetAddress) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterType, pCur, iMsgSize, sizeof(ClusterType) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterMembership, pCur, iMsgSize, sizeof(ClusterMembership) ) );


			Proc_End:

				return hr;

			}; // Result JoinClusterCmd::ParseMessage( MessageData* pIMsg )

			Result JoinClusterCmd::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				JoinClusterCmd parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("TransactionID", parser.GetTransactionID());
				variableBuilder.SetVariable("RouteHopCount", parser.GetRouteHopCount());
				variableBuilder.SetVariable("Sender", parser.GetSender());
				variableBuilder.SetVariable("SenderNetClass", parser.GetSenderNetClass());
				variableBuilder.SetVariable("SenderAddress", parser.GetSenderAddress());
				variableBuilder.SetVariable("ClusterID", parser.GetClusterID());
				variableBuilder.SetVariable("ClusterType", (int)parser.GetClusterType());
				variableBuilder.SetVariable("ClusterMembership", (int)parser.GetClusterMembership());


			Proc_End:

				return hr;

			}; // Result JoinClusterCmd::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result JoinClusterCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) JoinClusterCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result JoinClusterCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* JoinClusterCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t)
					+ sizeof(EntityUID)
					+ sizeof(NetClass)
					+ sizeof(NetAddress)
					+ sizeof(ClusterID)
					+ sizeof(ClusterType)
					+ sizeof(ClusterMembership));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, ClusterServer::JoinClusterCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InSender, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InSenderNetClass, sizeof(NetClass));
				Protocol::PackParamCopy( pMsgData, &InSenderAddress, sizeof(NetAddress));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InClusterType, sizeof(ClusterType));
				Protocol::PackParamCopy( pMsgData, &InClusterMembership, sizeof(ClusterMembership));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* JoinClusterCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const EntityUID &InSender, const NetClass &InSenderNetClass, const NetAddress &InSenderAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InClusterMembership )

			Result JoinClusterCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result JoinClusterCmd::OverrideRouteContextDestination( EntityUID to )

			Result JoinClusterCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				Assert( iMsgSize >= (INT)sizeof(uint16) );
				*(uint16*)pCur = hopCount;
				pCur += sizeof(uint16); iMsgSize -= sizeof(uint16);


			Proc_End:

				return hr;

			}; // Result JoinClusterCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result JoinClusterCmd::TraceOut(MessageData* pMsg)
			{
 				JoinClusterCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "JoinCluster:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, Sender:{5}, SenderNetClass:{6}, SenderAddress:{7}, ClusterID:{8}, ClusterType:{9}, ClusterMembership:{10}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetSender(), parser.GetSenderNetClass(), parser.GetSenderAddress(), parser.GetClusterID(), (int)parser.GetClusterType(), (int)parser.GetClusterMembership()); 
				return ResultCode::SUCCESS;
			}; // Result JoinClusterCmd::TraceOut(MessageData* pMsg)

			const MessageID JoinClusterRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 1);
			Result JoinClusterRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofMemberList = 0; ServiceInformation* pMemberList = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofMemberList, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pMemberList, pCur, iMsgSize, sizeof(ServiceInformation)*numberofMemberList ) );
				m_MemberList.SetLinkedBuffer(numberofMemberList, numberofMemberList, pMemberList);


			Proc_End:

				return hr;

			}; // Result JoinClusterRes::ParseMessage( MessageData* pIMsg )

			Result JoinClusterRes::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				JoinClusterRes parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("TransactionID", parser.GetTransactionID());
				variableBuilder.SetVariable("Result", parser.GetResult());
				variableBuilder.SetVariable("MemberList", (int)parser.GetMemberList().GetItemCount(), parser.GetMemberList().data());


			Proc_End:

				return hr;

			}; // Result JoinClusterRes::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result JoinClusterRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) JoinClusterRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result JoinClusterRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* JoinClusterRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(ServiceInformation)*InMemberList.GetItemCount() + sizeof(uint16_t));


				uint16_t numberOfInMemberList = (uint16_t)InMemberList.GetItemCount(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, ClusterServer::JoinClusterRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInMemberList, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InMemberList.data(), (INT)(sizeof(ServiceInformation)*InMemberList.GetItemCount())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* JoinClusterRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )

			Result JoinClusterRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result JoinClusterRes::OverrideRouteContextDestination( EntityUID to )

			Result JoinClusterRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				pCur += sizeof(Result); iMsgSize -= sizeof(Result);
				pCur += sizeof(uint16_t); iMsgSize -= sizeof(uint16_t);
				pCur += sizeof(ServiceInformation)*m_MemberList.GetItemCount(); iMsgSize -= (INT)(sizeof(ServiceInformation)*m_MemberList.GetSize());


			Proc_End:

				return hr;

			}; // Result JoinClusterRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result JoinClusterRes::TraceOut(MessageData* pMsg)
			{
 				JoinClusterRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "JoinCluster:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, MemberList:{5,30}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetMemberList()); 
				return ResultCode::SUCCESS;
			}; // Result JoinClusterRes::TraceOut(MessageData* pMsg)

			// C2S: Do not let it broadcasted while it's manual broadcast packet
			const MessageID NewServerServiceJoinedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 2);
			Result NewServerServiceJoinedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedServiceUID, pCur, iMsgSize, sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedServiceNetClass, pCur, iMsgSize, sizeof(NetClass) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedServiceAddress, pCur, iMsgSize, sizeof(NetAddress) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterType, pCur, iMsgSize, sizeof(ClusterType) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedServiceMembership, pCur, iMsgSize, sizeof(ClusterMembership) ) );


			Proc_End:

				return hr;

			}; // Result NewServerServiceJoinedC2SEvt::ParseMessage( MessageData* pIMsg )

			Result NewServerServiceJoinedC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				NewServerServiceJoinedC2SEvt parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("RouteHopCount", parser.GetRouteHopCount());
				variableBuilder.SetVariable("JoinedServiceUID", parser.GetJoinedServiceUID());
				variableBuilder.SetVariable("JoinedServiceNetClass", parser.GetJoinedServiceNetClass());
				variableBuilder.SetVariable("JoinedServiceAddress", parser.GetJoinedServiceAddress());
				variableBuilder.SetVariable("ClusterID", parser.GetClusterID());
				variableBuilder.SetVariable("ClusterType", (int)parser.GetClusterType());
				variableBuilder.SetVariable("JoinedServiceMembership", (int)parser.GetJoinedServiceMembership());


			Proc_End:

				return hr;

			}; // Result NewServerServiceJoinedC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result NewServerServiceJoinedC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) NewServerServiceJoinedC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result NewServerServiceJoinedC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* NewServerServiceJoinedC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(EntityUID)
					+ sizeof(NetClass)
					+ sizeof(NetAddress)
					+ sizeof(ClusterID)
					+ sizeof(ClusterType)
					+ sizeof(ClusterMembership));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, ClusterServer::NewServerServiceJoinedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InJoinedServiceUID, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InJoinedServiceNetClass, sizeof(NetClass));
				Protocol::PackParamCopy( pMsgData, &InJoinedServiceAddress, sizeof(NetAddress));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InClusterType, sizeof(ClusterType));
				Protocol::PackParamCopy( pMsgData, &InJoinedServiceMembership, sizeof(ClusterMembership));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* NewServerServiceJoinedC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InJoinedServiceUID, const NetClass &InJoinedServiceNetClass, const NetAddress &InJoinedServiceAddress, const ClusterID &InClusterID, const ClusterType &InClusterType, const ClusterMembership &InJoinedServiceMembership )

			Result NewServerServiceJoinedC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result NewServerServiceJoinedC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result NewServerServiceJoinedC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16) );
				*(uint16*)pCur = hopCount;
				pCur += sizeof(uint16); iMsgSize -= sizeof(uint16);


			Proc_End:

				return hr;

			}; // Result NewServerServiceJoinedC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result NewServerServiceJoinedC2SEvt::TraceOut(MessageData* pMsg)
			{
 				NewServerServiceJoinedC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "NewServerServiceJoined:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, JoinedServiceUID:{4}, JoinedServiceNetClass:{5}, JoinedServiceAddress:{6}, ClusterID:{7}, ClusterType:{8}, JoinedServiceMembership:{9}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetJoinedServiceUID(), parser.GetJoinedServiceNetClass(), parser.GetJoinedServiceAddress(), parser.GetClusterID(), (int)parser.GetClusterType(), (int)parser.GetJoinedServiceMembership()); 
				return ResultCode::SUCCESS;
			}; // Result NewServerServiceJoinedC2SEvt::TraceOut(MessageData* pMsg)

			// C2S: Sync cluster service informations
			const MessageID SyncClusterServiceC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 3);
			Result SyncClusterServiceC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofMemberList = 0; ServiceInformation* pMemberList = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterType, pCur, iMsgSize, sizeof(ClusterType) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofMemberList, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pMemberList, pCur, iMsgSize, sizeof(ServiceInformation)*numberofMemberList ) );
				m_MemberList.SetLinkedBuffer(numberofMemberList, numberofMemberList, pMemberList);


			Proc_End:

				return hr;

			}; // Result SyncClusterServiceC2SEvt::ParseMessage( MessageData* pIMsg )

			Result SyncClusterServiceC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				SyncClusterServiceC2SEvt parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("RouteHopCount", parser.GetRouteHopCount());
				variableBuilder.SetVariable("ClusterID", parser.GetClusterID());
				variableBuilder.SetVariable("ClusterType", (int)parser.GetClusterType());
				variableBuilder.SetVariable("MemberList", (int)parser.GetMemberList().GetItemCount(), parser.GetMemberList().data());


			Proc_End:

				return hr;

			}; // Result SyncClusterServiceC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result SyncClusterServiceC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) SyncClusterServiceC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result SyncClusterServiceC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* SyncClusterServiceC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(ClusterID)
					+ sizeof(ClusterType)
					+ sizeof(ServiceInformation)*InMemberList.GetItemCount() + sizeof(uint16_t));


				uint16_t numberOfInMemberList = (uint16_t)InMemberList.GetItemCount(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, ClusterServer::SyncClusterServiceC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InClusterType, sizeof(ClusterType));
				Protocol::PackParamCopy( pMsgData, &numberOfInMemberList, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InMemberList.data(), (INT)(sizeof(ServiceInformation)*InMemberList.GetItemCount())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* SyncClusterServiceC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const ClusterType &InClusterType, const Array<ServiceInformation>& InMemberList )

			Result SyncClusterServiceC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result SyncClusterServiceC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result SyncClusterServiceC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16) );
				*(uint16*)pCur = hopCount;
				pCur += sizeof(uint16); iMsgSize -= sizeof(uint16);


			Proc_End:

				return hr;

			}; // Result SyncClusterServiceC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result SyncClusterServiceC2SEvt::TraceOut(MessageData* pMsg)
			{
 				SyncClusterServiceC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "SyncClusterService:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, ClusterID:{4}, ClusterType:{5}, MemberList:{6,30}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetClusterID(), (int)parser.GetClusterType(), parser.GetMemberList()); 
				return ResultCode::SUCCESS;
			}; // Result SyncClusterServiceC2SEvt::TraceOut(MessageData* pMsg)

			// Cmd: Join to the cluster
			const MessageID RequestDataSyncCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 4);
			Result RequestDataSyncCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );


			Proc_End:

				return hr;

			}; // Result RequestDataSyncCmd::ParseMessage( MessageData* pIMsg )

			Result RequestDataSyncCmd::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				RequestDataSyncCmd parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("TransactionID", parser.GetTransactionID());
				variableBuilder.SetVariable("RouteHopCount", parser.GetRouteHopCount());
				variableBuilder.SetVariable("ClusterID", parser.GetClusterID());


			Proc_End:

				return hr;

			}; // Result RequestDataSyncCmd::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result RequestDataSyncCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RequestDataSyncCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RequestDataSyncCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* RequestDataSyncCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t)
					+ sizeof(ClusterID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, ClusterServer::RequestDataSyncCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RequestDataSyncCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )

			Result RequestDataSyncCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
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

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				Assert( iMsgSize >= (INT)sizeof(uint16) );
				*(uint16*)pCur = hopCount;
				pCur += sizeof(uint16); iMsgSize -= sizeof(uint16);


			Proc_End:

				return hr;

			}; // Result RequestDataSyncCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result RequestDataSyncCmd::TraceOut(MessageData* pMsg)
			{
 				RequestDataSyncCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "RequestDataSync:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, ClusterID:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetClusterID()); 
				return ResultCode::SUCCESS;
			}; // Result RequestDataSyncCmd::TraceOut(MessageData* pMsg)

			const MessageID RequestDataSyncRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 4);
			Result RequestDataSyncRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result RequestDataSyncRes::ParseMessage( MessageData* pIMsg )

			Result RequestDataSyncRes::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				RequestDataSyncRes parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("TransactionID", parser.GetTransactionID());
				variableBuilder.SetVariable("Result", parser.GetResult());


			Proc_End:

				return hr;

			}; // Result RequestDataSyncRes::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result RequestDataSyncRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RequestDataSyncRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RequestDataSyncRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* RequestDataSyncRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, ClusterServer::RequestDataSyncRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RequestDataSyncRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result RequestDataSyncRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
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

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				pCur += sizeof(Result); iMsgSize -= sizeof(Result);


			Proc_End:

				return hr;

			}; // Result RequestDataSyncRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result RequestDataSyncRes::TraceOut(MessageData* pMsg)
			{
 				RequestDataSyncRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "RequestDataSync:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result RequestDataSyncRes::TraceOut(MessageData* pMsg)

			// S2C: Master instance of the cluster is assigned
			const MessageID ClusterMasterAssignedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 5);
			Result ClusterMasterAssignedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Sender, pCur, iMsgSize, sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MasterUID, pCur, iMsgSize, sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // Result ClusterMasterAssignedS2CEvt::ParseMessage( MessageData* pIMsg )

			Result ClusterMasterAssignedS2CEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				ClusterMasterAssignedS2CEvt parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("RouteHopCount", parser.GetRouteHopCount());
				variableBuilder.SetVariable("Sender", parser.GetSender());
				variableBuilder.SetVariable("ClusterID", parser.GetClusterID());
				variableBuilder.SetVariable("MasterUID", parser.GetMasterUID());


			Proc_End:

				return hr;

			}; // Result ClusterMasterAssignedS2CEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result ClusterMasterAssignedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) ClusterMasterAssignedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result ClusterMasterAssignedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* ClusterMasterAssignedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const EntityUID &InMasterUID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(EntityUID)
					+ sizeof(ClusterID)
					+ sizeof(EntityUID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, ClusterServer::ClusterMasterAssignedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InSender, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InMasterUID, sizeof(EntityUID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* ClusterMasterAssignedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const EntityUID &InMasterUID )

			Result ClusterMasterAssignedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result ClusterMasterAssignedS2CEvt::OverrideRouteContextDestination( EntityUID to )

			Result ClusterMasterAssignedS2CEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16) );
				*(uint16*)pCur = hopCount;
				pCur += sizeof(uint16); iMsgSize -= sizeof(uint16);


			Proc_End:

				return hr;

			}; // Result ClusterMasterAssignedS2CEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result ClusterMasterAssignedS2CEvt::TraceOut(MessageData* pMsg)
			{
 				ClusterMasterAssignedS2CEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "ClusterMasterAssigned:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, Sender:{4}, ClusterID:{5}, MasterUID:{6}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetSender(), parser.GetClusterID(), parser.GetMasterUID()); 
				return ResultCode::SUCCESS;
			}; // Result ClusterMasterAssignedS2CEvt::TraceOut(MessageData* pMsg)

			// C2S: Master vote
			const MessageID ClusterMasterVoteC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 6);
			Result ClusterMasterVoteC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_VoteToUID, pCur, iMsgSize, sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_VotedUpTime, pCur, iMsgSize, sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result ClusterMasterVoteC2SEvt::ParseMessage( MessageData* pIMsg )

			Result ClusterMasterVoteC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				ClusterMasterVoteC2SEvt parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("RouteHopCount", parser.GetRouteHopCount());
				variableBuilder.SetVariable("ClusterID", parser.GetClusterID());
				variableBuilder.SetVariable("VoteToUID", parser.GetVoteToUID());
				variableBuilder.SetVariable("VotedUpTime", parser.GetVotedUpTime());


			Proc_End:

				return hr;

			}; // Result ClusterMasterVoteC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result ClusterMasterVoteC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) ClusterMasterVoteC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result ClusterMasterVoteC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* ClusterMasterVoteC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const uint64_t &InVotedUpTime )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(ClusterID)
					+ sizeof(EntityUID)
					+ sizeof(uint64_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, ClusterServer::ClusterMasterVoteC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InVoteToUID, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InVotedUpTime, sizeof(uint64_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* ClusterMasterVoteC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const ClusterID &InClusterID, const EntityUID &InVoteToUID, const uint64_t &InVotedUpTime )

			Result ClusterMasterVoteC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result ClusterMasterVoteC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result ClusterMasterVoteC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16) );
				*(uint16*)pCur = hopCount;
				pCur += sizeof(uint16); iMsgSize -= sizeof(uint16);


			Proc_End:

				return hr;

			}; // Result ClusterMasterVoteC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result ClusterMasterVoteC2SEvt::TraceOut(MessageData* pMsg)
			{
 				ClusterMasterVoteC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "ClusterMasterVote:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, ClusterID:{4}, VoteToUID:{5}, VotedUpTime:{6}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetClusterID(), parser.GetVoteToUID(), parser.GetVotedUpTime()); 
				return ResultCode::SUCCESS;
			}; // Result ClusterMasterVoteC2SEvt::TraceOut(MessageData* pMsg)

			// C2S: Update cluster service status
			const MessageID ClusterUpdateStatusC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 7);
			Result ClusterUpdateStatusC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Sender, pCur, iMsgSize, sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MemberStatus, pCur, iMsgSize, sizeof(ServiceStatus) ) );


			Proc_End:

				return hr;

			}; // Result ClusterUpdateStatusC2SEvt::ParseMessage( MessageData* pIMsg )

			Result ClusterUpdateStatusC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				ClusterUpdateStatusC2SEvt parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("RouteHopCount", parser.GetRouteHopCount());
				variableBuilder.SetVariable("Sender", parser.GetSender());
				variableBuilder.SetVariable("ClusterID", parser.GetClusterID());
				variableBuilder.SetVariable("MemberStatus", (int)parser.GetMemberStatus());


			Proc_End:

				return hr;

			}; // Result ClusterUpdateStatusC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result ClusterUpdateStatusC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) ClusterUpdateStatusC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result ClusterUpdateStatusC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* ClusterUpdateStatusC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(EntityUID)
					+ sizeof(ClusterID)
					+ sizeof(ServiceStatus));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, ClusterServer::ClusterUpdateStatusC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InSender, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InMemberStatus, sizeof(ServiceStatus));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* ClusterUpdateStatusC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const ServiceStatus &InMemberStatus )

			Result ClusterUpdateStatusC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result ClusterUpdateStatusC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result ClusterUpdateStatusC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16) );
				*(uint16*)pCur = hopCount;
				pCur += sizeof(uint16); iMsgSize -= sizeof(uint16);


			Proc_End:

				return hr;

			}; // Result ClusterUpdateStatusC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result ClusterUpdateStatusC2SEvt::TraceOut(MessageData* pMsg)
			{
 				ClusterUpdateStatusC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "ClusterUpdateStatus:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, Sender:{4}, ClusterID:{5}, MemberStatus:{6}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetSender(), parser.GetClusterID(), (int)parser.GetMemberStatus()); 
				return ResultCode::SUCCESS;
			}; // Result ClusterUpdateStatusC2SEvt::TraceOut(MessageData* pMsg)

			// C2S: Update cluster service workload
			const MessageID ClusterUpdateWorkloadC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 8);
			Result ClusterUpdateWorkloadC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Sender, pCur, iMsgSize, sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Workload, pCur, iMsgSize, sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result ClusterUpdateWorkloadC2SEvt::ParseMessage( MessageData* pIMsg )

			Result ClusterUpdateWorkloadC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				ClusterUpdateWorkloadC2SEvt parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("RouteHopCount", parser.GetRouteHopCount());
				variableBuilder.SetVariable("Sender", parser.GetSender());
				variableBuilder.SetVariable("ClusterID", parser.GetClusterID());
				variableBuilder.SetVariable("Workload", parser.GetWorkload());


			Proc_End:

				return hr;

			}; // Result ClusterUpdateWorkloadC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result ClusterUpdateWorkloadC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) ClusterUpdateWorkloadC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result ClusterUpdateWorkloadC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* ClusterUpdateWorkloadC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const uint32_t &InWorkload )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(EntityUID)
					+ sizeof(ClusterID)
					+ sizeof(uint32_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, ClusterServer::ClusterUpdateWorkloadC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InSender, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));
				Protocol::PackParamCopy( pMsgData, &InWorkload, sizeof(uint32_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* ClusterUpdateWorkloadC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const EntityUID &InSender, const ClusterID &InClusterID, const uint32_t &InWorkload )

			Result ClusterUpdateWorkloadC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result ClusterUpdateWorkloadC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result ClusterUpdateWorkloadC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16) );
				*(uint16*)pCur = hopCount;
				pCur += sizeof(uint16); iMsgSize -= sizeof(uint16);


			Proc_End:

				return hr;

			}; // Result ClusterUpdateWorkloadC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result ClusterUpdateWorkloadC2SEvt::TraceOut(MessageData* pMsg)
			{
 				ClusterUpdateWorkloadC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "ClusterUpdateWorkload:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, Sender:{4}, ClusterID:{5}, Workload:{6}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetSender(), parser.GetClusterID(), parser.GetWorkload()); 
				return ResultCode::SUCCESS;
			}; // Result ClusterUpdateWorkloadC2SEvt::TraceOut(MessageData* pMsg)

			// Cmd: Get lowest workloaded cluster member
			const MessageID GetLowestWorkloadClusterMemberCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 9);
			Result GetLowestWorkloadClusterMemberCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ClusterID, pCur, iMsgSize, sizeof(ClusterID) ) );


			Proc_End:

				return hr;

			}; // Result GetLowestWorkloadClusterMemberCmd::ParseMessage( MessageData* pIMsg )

			Result GetLowestWorkloadClusterMemberCmd::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				GetLowestWorkloadClusterMemberCmd parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("TransactionID", parser.GetTransactionID());
				variableBuilder.SetVariable("RouteHopCount", parser.GetRouteHopCount());
				variableBuilder.SetVariable("ClusterID", parser.GetClusterID());


			Proc_End:

				return hr;

			}; // Result GetLowestWorkloadClusterMemberCmd::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result GetLowestWorkloadClusterMemberCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GetLowestWorkloadClusterMemberCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GetLowestWorkloadClusterMemberCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* GetLowestWorkloadClusterMemberCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(uint16_t)
					+ sizeof(ClusterID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, ClusterServer::GetLowestWorkloadClusterMemberCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InClusterID, sizeof(ClusterID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GetLowestWorkloadClusterMemberCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )

			Result GetLowestWorkloadClusterMemberCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GetLowestWorkloadClusterMemberCmd::OverrideRouteContextDestination( EntityUID to )

			Result GetLowestWorkloadClusterMemberCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				Assert( iMsgSize >= (INT)sizeof(uint16) );
				*(uint16*)pCur = hopCount;
				pCur += sizeof(uint16); iMsgSize -= sizeof(uint16);


			Proc_End:

				return hr;

			}; // Result GetLowestWorkloadClusterMemberCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result GetLowestWorkloadClusterMemberCmd::TraceOut(MessageData* pMsg)
			{
 				GetLowestWorkloadClusterMemberCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "GetLowestWorkloadClusterMember:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, ClusterID:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetClusterID()); 
				return ResultCode::SUCCESS;
			}; // Result GetLowestWorkloadClusterMemberCmd::TraceOut(MessageData* pMsg)

			const MessageID GetLowestWorkloadClusterMemberRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 9);
			Result GetLowestWorkloadClusterMemberRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Member, pCur, iMsgSize, sizeof(ServiceInformation) ) );


			Proc_End:

				return hr;

			}; // Result GetLowestWorkloadClusterMemberRes::ParseMessage( MessageData* pIMsg )

			Result GetLowestWorkloadClusterMemberRes::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				GetLowestWorkloadClusterMemberRes parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("TransactionID", parser.GetTransactionID());
				variableBuilder.SetVariable("Result", parser.GetResult());
				variableBuilder.SetVariable("Member", parser.GetMember());


			Proc_End:

				return hr;

			}; // Result GetLowestWorkloadClusterMemberRes::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result GetLowestWorkloadClusterMemberRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GetLowestWorkloadClusterMemberRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GetLowestWorkloadClusterMemberRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* GetLowestWorkloadClusterMemberRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const ServiceInformation &InMember )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(ServiceInformation));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, ClusterServer::GetLowestWorkloadClusterMemberRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InMember, sizeof(ServiceInformation));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GetLowestWorkloadClusterMemberRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const ServiceInformation &InMember )

			Result GetLowestWorkloadClusterMemberRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GetLowestWorkloadClusterMemberRes::OverrideRouteContextDestination( EntityUID to )

			Result GetLowestWorkloadClusterMemberRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				pCur += sizeof(TransactionID); iMsgSize -= sizeof(TransactionID);
				pCur += sizeof(Result); iMsgSize -= sizeof(Result);
				pCur += sizeof(ServiceInformation); iMsgSize -= sizeof(ServiceInformation);


			Proc_End:

				return hr;

			}; // Result GetLowestWorkloadClusterMemberRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result GetLowestWorkloadClusterMemberRes::TraceOut(MessageData* pMsg)
			{
 				GetLowestWorkloadClusterMemberRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "GetLowestWorkloadClusterMember:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, Member:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetMember()); 
				return ResultCode::SUCCESS;
			}; // Result GetLowestWorkloadClusterMemberRes::TraceOut(MessageData* pMsg)

			// C2S: Called when a player entity is created
			const MessageID GamePlayerEntityCreatedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 10);
			Result GamePlayerEntityCreatedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerUID, pCur, iMsgSize, sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayerEntityCreatedC2SEvt::ParseMessage( MessageData* pIMsg )

			Result GamePlayerEntityCreatedC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				GamePlayerEntityCreatedC2SEvt parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("RouteHopCount", parser.GetRouteHopCount());
				variableBuilder.SetVariable("PlayerID", parser.GetPlayerID());
				variableBuilder.SetVariable("PlayerUID", parser.GetPlayerUID());


			Proc_End:

				return hr;

			}; // Result GamePlayerEntityCreatedC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result GamePlayerEntityCreatedC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GamePlayerEntityCreatedC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GamePlayerEntityCreatedC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* GamePlayerEntityCreatedC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(PlayerID)
					+ sizeof(EntityUID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, ClusterServer::GamePlayerEntityCreatedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPlayerUID, sizeof(EntityUID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GamePlayerEntityCreatedC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )

			Result GamePlayerEntityCreatedC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GamePlayerEntityCreatedC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result GamePlayerEntityCreatedC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16) );
				*(uint16*)pCur = hopCount;
				pCur += sizeof(uint16); iMsgSize -= sizeof(uint16);


			Proc_End:

				return hr;

			}; // Result GamePlayerEntityCreatedC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result GamePlayerEntityCreatedC2SEvt::TraceOut(MessageData* pMsg)
			{
 				GamePlayerEntityCreatedC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "GamePlayerEntityCreated:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, PlayerID:{4}, PlayerUID:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetPlayerID(), parser.GetPlayerUID()); 
				return ResultCode::SUCCESS;
			}; // Result GamePlayerEntityCreatedC2SEvt::TraceOut(MessageData* pMsg)

			// C2S: Called when a player entity is deleted
			const MessageID GamePlayerEntityDeletedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 11);
			Result GamePlayerEntityDeletedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteHopCount, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerUID, pCur, iMsgSize, sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayerEntityDeletedC2SEvt::ParseMessage( MessageData* pIMsg )

			Result GamePlayerEntityDeletedC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				GamePlayerEntityDeletedC2SEvt parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("RouteContext", parser.GetRouteContext());
				variableBuilder.SetVariable("RouteHopCount", parser.GetRouteHopCount());
				variableBuilder.SetVariable("PlayerID", parser.GetPlayerID());
				variableBuilder.SetVariable("PlayerUID", parser.GetPlayerUID());


			Proc_End:

				return hr;

			}; // Result GamePlayerEntityDeletedC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result GamePlayerEntityDeletedC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GamePlayerEntityDeletedC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GamePlayerEntityDeletedC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* GamePlayerEntityDeletedC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint16_t)
					+ sizeof(PlayerID)
					+ sizeof(EntityUID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, ClusterServer::GamePlayerEntityDeletedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRouteHopCount, sizeof(uint16_t));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPlayerUID, sizeof(EntityUID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GamePlayerEntityDeletedC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const EntityUID &InPlayerUID )

			Result GamePlayerEntityDeletedC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GamePlayerEntityDeletedC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result GamePlayerEntityDeletedC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				pCur += sizeof(RouteContext); iMsgSize -= sizeof(RouteContext);
				Assert( iMsgSize >= (INT)sizeof(uint16) );
				*(uint16*)pCur = hopCount;
				pCur += sizeof(uint16); iMsgSize -= sizeof(uint16);


			Proc_End:

				return hr;

			}; // Result GamePlayerEntityDeletedC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result GamePlayerEntityDeletedC2SEvt::TraceOut(MessageData* pMsg)
			{
 				GamePlayerEntityDeletedC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "GamePlayerEntityDeleted:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, PlayerID:{4}, PlayerUID:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetPlayerID(), parser.GetPlayerUID()); 
				return ResultCode::SUCCESS;
			}; // Result GamePlayerEntityDeletedC2SEvt::TraceOut(MessageData* pMsg)



		}; // namespace ClusterServer
	}; // namespace Message
}; // namespace SF


