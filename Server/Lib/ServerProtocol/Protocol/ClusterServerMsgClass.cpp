////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) StormForge
// 
// Author : Generated
// 
// Description : ClusterServer Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "Protocol/SFProtocol.h"
#include "Util/SFToString.h"
#include "Net/SFNetToString.h"
#include "Container/SFArray.h"
#include "Protocol/SFProtocolHelper.h"
#include "Protocol/ClusterServerMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace ClusterServer
		{
 			// Cmd: Cluster member list query
			const MessageID GetClusterMemberListCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 0);
			Result GetClusterMemberListCmd::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_RouteHopCount);
				protocolCheck(*input >> m_ClusterID);

				return hr;

			}; // Result GetClusterMemberListCmd::ParseMessage(const MessageData* pIMsg)


			Result GetClusterMemberListCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) GetClusterMemberListCmd(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result GetClusterMemberListCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* GetClusterMemberListCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
			{
 				MessageData *pNewMsg = nullptr;
				ScopeContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						IHeap::Delete(pNewMsg);
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InRouteHopCount)
					+ SerializedSizeOf(InClusterID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ClusterServer::GetClusterMemberListCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InClusterID);

				return hr;
			}; // MessageData* GetClusterMemberListCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )

			Result GetClusterMemberListCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				ScopeContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);
				uint8_t* pCur = nullptr;(void)(pCur);

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result GetClusterMemberListCmd::OverrideRouteContextDestination( EntityUID to )

			Result GetClusterMemberListCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				ScopeContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);
				uint8_t* pCur = nullptr;(void)(pCur);

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				protocolCheck(input->Skip(sizeof(RouteContext)));
				protocolCheck(input->Skip(sizeof(uint64_t)));
				Assert(input->GetRemainSize() >= sizeof(uint16_t));
				pCur = input->GetBufferPtr() + input->GetPosition();
				*(uint16_t*)pCur = hopCount;
				protocolCheck(input->Skip(sizeof(uint16_t)));

				return hr;

			}; // Result GetClusterMemberListCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result GetClusterMemberListCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				GetClusterMemberListCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug2, "GetClusterMemberList:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, ClusterID:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetClusterID()); 
				return ResultCode::SUCCESS;
			}; // Result GetClusterMemberListCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID GetClusterMemberListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 0);
			Result GetClusterMemberListRes::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);
				protocolCheck(input->Read(ArrayLen));
				ServiceInformation* MemberListPtr = nullptr;
				protocolCheck(input->ReadLink(MemberListPtr, ArrayLen));
				m_MemberList.SetLinkedBuffer(ArrayLen, MemberListPtr);

				return hr;

			}; // Result GetClusterMemberListRes::ParseMessage(const MessageData* pIMsg)


			Result GetClusterMemberListRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) GetClusterMemberListRes(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result GetClusterMemberListRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* GetClusterMemberListRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )
			{
 				MessageData *pNewMsg = nullptr;
				ScopeContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						IHeap::Delete(pNewMsg);
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InResult)
					+ SerializedSizeOf(InMemberList)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ClusterServer::GetClusterMemberListRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);
				protocolCheck(*output << InMemberList);

				return hr;
			}; // MessageData* GetClusterMemberListRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<ServiceInformation>& InMemberList )

			Result GetClusterMemberListRes::OverrideRouteContextDestination( EntityUID to )
			{
 				ScopeContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);
				uint8_t* pCur = nullptr;(void)(pCur);

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result GetClusterMemberListRes::OverrideRouteContextDestination( EntityUID to )

			Result GetClusterMemberListRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				ScopeContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);
				uint8_t* pCur = nullptr;(void)(pCur);

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				protocolCheck(input->Skip(sizeof(RouteContext)));
				protocolCheck(input->Skip(sizeof(uint64_t)));
				protocolCheck(input->Skip(sizeof(Result)));
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->Skip(ArrayLen * sizeof(ServiceInformation)));

				return hr;

			}; // Result GetClusterMemberListRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result GetClusterMemberListRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				GetClusterMemberListRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug2, "GetClusterMemberList:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, MemberList:{5,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetMemberList()); 
				return ResultCode::SUCCESS;
			}; // Result GetClusterMemberListRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Join to the cluster
			const MessageID RequestDataSyncCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 1);
			Result RequestDataSyncCmd::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_RouteHopCount);
				protocolCheck(*input >> m_ClusterID);

				return hr;

			}; // Result RequestDataSyncCmd::ParseMessage(const MessageData* pIMsg)


			Result RequestDataSyncCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RequestDataSyncCmd(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RequestDataSyncCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* RequestDataSyncCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )
			{
 				MessageData *pNewMsg = nullptr;
				ScopeContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						IHeap::Delete(pNewMsg);
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InRouteHopCount)
					+ SerializedSizeOf(InClusterID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ClusterServer::RequestDataSyncCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InClusterID);

				return hr;
			}; // MessageData* RequestDataSyncCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID )

			Result RequestDataSyncCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				ScopeContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);
				uint8_t* pCur = nullptr;(void)(pCur);

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result RequestDataSyncCmd::OverrideRouteContextDestination( EntityUID to )

			Result RequestDataSyncCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				ScopeContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);
				uint8_t* pCur = nullptr;(void)(pCur);

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				protocolCheck(input->Skip(sizeof(RouteContext)));
				protocolCheck(input->Skip(sizeof(uint64_t)));
				Assert(input->GetRemainSize() >= sizeof(uint16_t));
				pCur = input->GetBufferPtr() + input->GetPosition();
				*(uint16_t*)pCur = hopCount;
				protocolCheck(input->Skip(sizeof(uint16_t)));

				return hr;

			}; // Result RequestDataSyncCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result RequestDataSyncCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RequestDataSyncCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug2, "RequestDataSync:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, ClusterID:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetClusterID()); 
				return ResultCode::SUCCESS;
			}; // Result RequestDataSyncCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID RequestDataSyncRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CLUSTERSERVER, 1);
			Result RequestDataSyncRes::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);

				return hr;

			}; // Result RequestDataSyncRes::ParseMessage(const MessageData* pIMsg)


			Result RequestDataSyncRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RequestDataSyncRes(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RequestDataSyncRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* RequestDataSyncRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				ScopeContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						IHeap::Delete(pNewMsg);
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InResult)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ClusterServer::RequestDataSyncRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);

				return hr;
			}; // MessageData* RequestDataSyncRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result RequestDataSyncRes::OverrideRouteContextDestination( EntityUID to )
			{
 				ScopeContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);
				uint8_t* pCur = nullptr;(void)(pCur);

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result RequestDataSyncRes::OverrideRouteContextDestination( EntityUID to )

			Result RequestDataSyncRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
			{
 				ScopeContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);
				uint8_t* pCur = nullptr;(void)(pCur);

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );
				protocolCheck(input->Skip(sizeof(RouteContext)));
				protocolCheck(input->Skip(sizeof(uint64_t)));
				protocolCheck(input->Skip(sizeof(Result)));

				return hr;

			}; // Result RequestDataSyncRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result RequestDataSyncRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RequestDataSyncRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug2, "RequestDataSync:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result RequestDataSyncRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)



		}; // namespace ClusterServer
	}; // namespace Message
}; // namespace SF


