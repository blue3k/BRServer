////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) StormForge
// 
// Author : Generated
// 
// Description : ChatChannelManager Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "Protocol/SFProtocol.h"
#include "Util/SFToString.h"
#include "Net/SFNetToString.h"
#include "Container/SFArray.h"
#include "Protocol/SFProtocolHelper.h"
#include "Protocol/ChatChannelManagerMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace ChatChannelManager
		{
 			// Cmd: Create a channel instance
			const MessageID CreateChannelCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHATCHANNELMANAGER, 0);
			Result CreateChannelCmd::ParseMessage(const MessageData* pIMsg)
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
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_ChannelName, ArrayLen));
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_Passcode, ArrayLen));
				protocolCheck(*input >> m_Creator);

				return hr;

			}; // Result CreateChannelCmd::ParseMessage(const MessageData* pIMsg)


			Result CreateChannelCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) CreateChannelCmd(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result CreateChannelCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* CreateChannelCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName, const char* InPasscode, const PlayerInformation &InCreator )
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
					+ SerializedSizeOf(InChannelName)
					+ SerializedSizeOf(InPasscode)
					+ SerializedSizeOf(InCreator)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ChatChannelManager::CreateChannelCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InChannelName);
				protocolCheck(*output << InPasscode);
				protocolCheck(*output << InCreator);

				return hr;
			}; // MessageData* CreateChannelCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName, const char* InPasscode, const PlayerInformation &InCreator )

			Result CreateChannelCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result CreateChannelCmd::OverrideRouteContextDestination( EntityUID to )

			Result CreateChannelCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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

			}; // Result CreateChannelCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result CreateChannelCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				CreateChannelCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "CreateChannel:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, ChannelName:{5,60}, Passcode:{6,60}, Creator:{7}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetChannelName(), parser.GetPasscode(), parser.GetCreator()); 
				return ResultCode::SUCCESS;
			}; // Result CreateChannelCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID CreateChannelRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHATCHANNELMANAGER, 0);
			Result CreateChannelRes::ParseMessage(const MessageData* pIMsg)
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
				protocolCheck(*input >> m_ChannelUID);

				return hr;

			}; // Result CreateChannelRes::ParseMessage(const MessageData* pIMsg)


			Result CreateChannelRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) CreateChannelRes(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result CreateChannelRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* CreateChannelRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint64_t &InChannelUID )
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
					+ SerializedSizeOf(InChannelUID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ChatChannelManager::CreateChannelRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);
				protocolCheck(*output << InChannelUID);

				return hr;
			}; // MessageData* CreateChannelRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint64_t &InChannelUID )

			Result CreateChannelRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result CreateChannelRes::OverrideRouteContextDestination( EntityUID to )

			Result CreateChannelRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				protocolCheck(input->Skip(sizeof(uint64_t)));

				return hr;

			}; // Result CreateChannelRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result CreateChannelRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				CreateChannelRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "CreateChannel:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, ChannelUID:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetChannelUID()); 
				return ResultCode::SUCCESS;
			}; // Result CreateChannelRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Find a channel instance with name
			const MessageID FindChannelCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHATCHANNELMANAGER, 1);
			Result FindChannelCmd::ParseMessage(const MessageData* pIMsg)
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
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_ChannelName, ArrayLen));

				return hr;

			}; // Result FindChannelCmd::ParseMessage(const MessageData* pIMsg)


			Result FindChannelCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) FindChannelCmd(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result FindChannelCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* FindChannelCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName )
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
					+ SerializedSizeOf(InChannelName)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ChatChannelManager::FindChannelCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InChannelName);

				return hr;
			}; // MessageData* FindChannelCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName )

			Result FindChannelCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result FindChannelCmd::OverrideRouteContextDestination( EntityUID to )

			Result FindChannelCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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

			}; // Result FindChannelCmd::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result FindChannelCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				FindChannelCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "FindChannel:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, ChannelName:{5,60}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetChannelName()); 
				return ResultCode::SUCCESS;
			}; // Result FindChannelCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID FindChannelRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHATCHANNELMANAGER, 1);
			Result FindChannelRes::ParseMessage(const MessageData* pIMsg)
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
				protocolCheck(*input >> m_ChannelUID);

				return hr;

			}; // Result FindChannelRes::ParseMessage(const MessageData* pIMsg)


			Result FindChannelRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) FindChannelRes(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result FindChannelRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* FindChannelRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint64_t &InChannelUID )
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
					+ SerializedSizeOf(InChannelUID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ChatChannelManager::FindChannelRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);
				protocolCheck(*output << InChannelUID);

				return hr;
			}; // MessageData* FindChannelRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint64_t &InChannelUID )

			Result FindChannelRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result FindChannelRes::OverrideRouteContextDestination( EntityUID to )

			Result FindChannelRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				protocolCheck(input->Skip(sizeof(uint64_t)));

				return hr;

			}; // Result FindChannelRes::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result FindChannelRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				FindChannelRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "FindChannel:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, ChannelUID:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetChannelUID()); 
				return ResultCode::SUCCESS;
			}; // Result FindChannelRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// C2S: Notification that a chat channel instance has deleted
			const MessageID ChatChannelDeletedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHATCHANNELMANAGER, 2);
			Result ChatChannelDeletedC2SEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_RouteHopCount);

				return hr;

			}; // Result ChatChannelDeletedC2SEvt::ParseMessage(const MessageData* pIMsg)


			Result ChatChannelDeletedC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) ChatChannelDeletedC2SEvt(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result ChatChannelDeletedC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* ChatChannelDeletedC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
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
					+ SerializedSizeOf(InRouteHopCount)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ChatChannelManager::ChatChannelDeletedC2SEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InRouteHopCount);

				return hr;
			}; // MessageData* ChatChannelDeletedC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )

			Result ChatChannelDeletedC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result ChatChannelDeletedC2SEvt::OverrideRouteContextDestination( EntityUID to )

			Result ChatChannelDeletedC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )
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
				Assert(input->GetRemainSize() >= sizeof(uint16_t));
				pCur = input->GetBufferPtr() + input->GetPosition();
				*(uint16_t*)pCur = hopCount;
				protocolCheck(input->Skip(sizeof(uint16_t)));

				return hr;

			}; // Result ChatChannelDeletedC2SEvt::OverrideRouteInformation( EntityUID to, unsigned hopCount )

			Result ChatChannelDeletedC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				ChatChannelDeletedC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "ChatChannelDeleted:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetRouteHopCount()); 
				return ResultCode::SUCCESS;
			}; // Result ChatChannelDeletedC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)



		}; // namespace ChatChannelManager
	}; // namespace Message
}; // namespace SF


