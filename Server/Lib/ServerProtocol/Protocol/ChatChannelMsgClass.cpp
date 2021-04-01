////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) StormForge
// 
// Author : Generated
// 
// Description : ChatChannel Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "Protocol/SFProtocol.h"
#include "Util/SFToString.h"
#include "Net/SFNetToString.h"
#include "Container/SFArray.h"
#include "Protocol/SFProtocolHelper.h"
#include "Protocol/ChatChannelMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace ChatChannel
		{
 			// Cmd: Request to join chat channel
			const MessageID JoinCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHATCHANNEL, 0);
			Result JoinCmd::ParseMessage(const MessageData* pIMsg)
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
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_Passcode, ArrayLen));
				protocolCheck(*input >> m_JoiningPlayer);

				return hr;

			}; // Result JoinCmd::ParseMessage(const MessageData* pIMsg)


			Result JoinCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) JoinCmd(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result JoinCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* JoinCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer )
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
					+ SerializedSizeOf(InPasscode)
					+ SerializedSizeOf(InJoiningPlayer)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ChatChannel::JoinCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InPasscode);
				protocolCheck(*output << InJoiningPlayer);

				return hr;
			}; // MessageData* JoinCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const char* InPasscode, const PlayerInformation &InJoiningPlayer )

			Result JoinCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result JoinCmd::OverrideRouteContextDestination( EntityUID to )


			Result JoinCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				JoinCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "Join:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Passcode:{4,60}, JoiningPlayer:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPasscode(), parser.GetJoiningPlayer()); 
				return ResultCode::SUCCESS;
			}; // Result JoinCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID JoinRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHATCHANNEL, 0);
			Result JoinRes::ParseMessage(const MessageData* pIMsg)
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
				protocolCheck(*input >> m_ChatChannelLeaderID);

				return hr;

			}; // Result JoinRes::ParseMessage(const MessageData* pIMsg)


			Result JoinRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) JoinRes(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result JoinRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* JoinRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const PlayerID &InChatChannelLeaderID )
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
					+ SerializedSizeOf(InChatChannelLeaderID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ChatChannel::JoinRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);
				protocolCheck(*output << InChatChannelLeaderID);

				return hr;
			}; // MessageData* JoinRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const PlayerID &InChatChannelLeaderID )

			Result JoinRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result JoinRes::OverrideRouteContextDestination( EntityUID to )


			Result JoinRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				JoinRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "Join:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, ChatChannelLeaderID:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetChatChannelLeaderID()); 
				return ResultCode::SUCCESS;
			}; // Result JoinRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Server envent to notify joined player information
			const MessageID PlayerJoinedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHATCHANNEL, 1);
			Result PlayerJoinedS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_JoinedPlayer);

				return hr;

			}; // Result PlayerJoinedS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result PlayerJoinedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PlayerJoinedS2CEvt(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PlayerJoinedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* PlayerJoinedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )
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
					+ SerializedSizeOf(InJoinedPlayer)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ChatChannel::PlayerJoinedS2CEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InJoinedPlayer);

				return hr;
			}; // MessageData* PlayerJoinedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )

			Result PlayerJoinedS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result PlayerJoinedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PlayerJoinedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PlayerJoinedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PlayerJoined:{0}:{1} , RouteContext:{2}, JoinedPlayer:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetJoinedPlayer()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerJoinedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Chat channel leader changed
			const MessageID LeaderChangedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHATCHANNEL, 2);
			Result LeaderChangedS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_NewLeaderID);

				return hr;

			}; // Result LeaderChangedS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result LeaderChangedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) LeaderChangedS2CEvt(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result LeaderChangedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* LeaderChangedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )
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
					+ SerializedSizeOf(InNewLeaderID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ChatChannel::LeaderChangedS2CEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InNewLeaderID);

				return hr;
			}; // MessageData* LeaderChangedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )

			Result LeaderChangedS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result LeaderChangedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result LeaderChangedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				LeaderChangedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "LeaderChanged:{0}:{1} , RouteContext:{2}, NewLeaderID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetNewLeaderID()); 
				return ResultCode::SUCCESS;
			}; // Result LeaderChangedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Leave chat channel
			const MessageID LeaveCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHATCHANNEL, 3);
			Result LeaveCmd::ParseMessage(const MessageData* pIMsg)
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
				protocolCheck(*input >> m_PlayerID);

				return hr;

			}; // Result LeaveCmd::ParseMessage(const MessageData* pIMsg)


			Result LeaveCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) LeaveCmd(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result LeaveCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* LeaveCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
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
					+ SerializedSizeOf(InPlayerID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ChatChannel::LeaveCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InPlayerID);

				return hr;
			}; // MessageData* LeaveCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )

			Result LeaveCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result LeaveCmd::OverrideRouteContextDestination( EntityUID to )


			Result LeaveCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				LeaveCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "Leave:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result LeaveCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID LeaveRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHATCHANNEL, 3);
			Result LeaveRes::ParseMessage(const MessageData* pIMsg)
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

			}; // Result LeaveRes::ParseMessage(const MessageData* pIMsg)


			Result LeaveRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) LeaveRes(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result LeaveRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* LeaveRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
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

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ChatChannel::LeaveRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);

				return hr;
			}; // MessageData* LeaveRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result LeaveRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result LeaveRes::OverrideRouteContextDestination( EntityUID to )


			Result LeaveRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				LeaveRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "Leave:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result LeaveRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Notification event when a player left
			const MessageID PlayerLeftS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHATCHANNEL, 4);
			Result PlayerLeftS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_LeftPlayerID);

				return hr;

			}; // Result PlayerLeftS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result PlayerLeftS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PlayerLeftS2CEvt(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PlayerLeftS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* PlayerLeftS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
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
					+ SerializedSizeOf(InLeftPlayerID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ChatChannel::PlayerLeftS2CEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InLeftPlayerID);

				return hr;
			}; // MessageData* PlayerLeftS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )

			Result PlayerLeftS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result PlayerLeftS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PlayerLeftS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PlayerLeftS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PlayerLeft:{0}:{1} , RouteContext:{2}, LeftPlayerID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetLeftPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerLeftS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Kick a player
			const MessageID KickPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHATCHANNEL, 5);
			Result KickPlayerCmd::ParseMessage(const MessageData* pIMsg)
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
				protocolCheck(*input >> m_PlayerID);
				protocolCheck(*input >> m_PlayerToKick);

				return hr;

			}; // Result KickPlayerCmd::ParseMessage(const MessageData* pIMsg)


			Result KickPlayerCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) KickPlayerCmd(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result KickPlayerCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* KickPlayerCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
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
					+ SerializedSizeOf(InPlayerID)
					+ SerializedSizeOf(InPlayerToKick)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ChatChannel::KickPlayerCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InPlayerID);
				protocolCheck(*output << InPlayerToKick);

				return hr;
			}; // MessageData* KickPlayerCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )

			Result KickPlayerCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result KickPlayerCmd::OverrideRouteContextDestination( EntityUID to )


			Result KickPlayerCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				KickPlayerCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "KickPlayer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, PlayerToKick:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetPlayerToKick()); 
				return ResultCode::SUCCESS;
			}; // Result KickPlayerCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID KickPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHATCHANNEL, 5);
			Result KickPlayerRes::ParseMessage(const MessageData* pIMsg)
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

			}; // Result KickPlayerRes::ParseMessage(const MessageData* pIMsg)


			Result KickPlayerRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) KickPlayerRes(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result KickPlayerRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* KickPlayerRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
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

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ChatChannel::KickPlayerRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);

				return hr;
			}; // MessageData* KickPlayerRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result KickPlayerRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result KickPlayerRes::OverrideRouteContextDestination( EntityUID to )


			Result KickPlayerRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				KickPlayerRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "KickPlayer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result KickPlayerRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Notification event when a player kicked
			const MessageID PlayerKickedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHATCHANNEL, 6);
			Result PlayerKickedS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_KickedPlayerID);

				return hr;

			}; // Result PlayerKickedS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result PlayerKickedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PlayerKickedS2CEvt(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PlayerKickedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* PlayerKickedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
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
					+ SerializedSizeOf(InKickedPlayerID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ChatChannel::PlayerKickedS2CEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InKickedPlayerID);

				return hr;
			}; // MessageData* PlayerKickedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )

			Result PlayerKickedS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result PlayerKickedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PlayerKickedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PlayerKickedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PlayerKicked:{0}:{1} , RouteContext:{2}, KickedPlayerID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetKickedPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerKickedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// C2S: Sending a chatting message
			const MessageID ChatMessageC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHATCHANNEL, 7);
			Result ChatMessageC2SEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_SenderID);
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_ChatMessage, ArrayLen));

				return hr;

			}; // Result ChatMessageC2SEvt::ParseMessage(const MessageData* pIMsg)


			Result ChatMessageC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) ChatMessageC2SEvt(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result ChatMessageC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* ChatMessageC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InChatMessage )
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
					+ SerializedSizeOf(InSenderID)
					+ SerializedSizeOf(InChatMessage)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ChatChannel::ChatMessageC2SEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InSenderID);
				protocolCheck(*output << InChatMessage);

				return hr;
			}; // MessageData* ChatMessageC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InChatMessage )

			Result ChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result ChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result ChatMessageC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				ChatMessageC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "ChatMessage:{0}:{1} , RouteContext:{2}, SenderID:{3}, ChatMessage:{4,60}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetSenderID(), parser.GetChatMessage()); 
				return ResultCode::SUCCESS;
			}; // Result ChatMessageC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: brocasting event for a chatting message
			const MessageID ChatMessageS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHATCHANNEL, 8);
			Result ChatMessageS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_SenderID);
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_SenderName, ArrayLen));
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_ChatMessage, ArrayLen));

				return hr;

			}; // Result ChatMessageS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result ChatMessageS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) ChatMessageS2CEvt(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result ChatMessageS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* ChatMessageS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
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
					+ SerializedSizeOf(InSenderID)
					+ SerializedSizeOf(InSenderName)
					+ SerializedSizeOf(InChatMessage)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, ChatChannel::ChatMessageS2CEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InSenderID);
				protocolCheck(*output << InSenderName);
				protocolCheck(*output << InChatMessage);

				return hr;
			}; // MessageData* ChatMessageS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )

			Result ChatMessageS2CEvt::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result ChatMessageS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result ChatMessageS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				ChatMessageS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "ChatMessage:{0}:{1} , RouteContext:{2}, SenderID:{3}, SenderName:{4,60}, ChatMessage:{5,60}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetSenderID(), parser.GetSenderName(), parser.GetChatMessage()); 
				return ResultCode::SUCCESS;
			}; // Result ChatMessageS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)



		}; // namespace ChatChannel
	}; // namespace Message
}; // namespace SF


