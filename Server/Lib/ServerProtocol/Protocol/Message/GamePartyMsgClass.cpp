////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameParty Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "Protocol/SFProtocol.h"
#include "String/SFToString.h"
#include "Net/SFNetToString.h"
#include "Container/SFArray.h"
#include "Protocol/SFProtocolHelper.h"
#include "Protocol/Message/GamePartyMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace GameParty
		{
 			// Cmd: Join party
			const MessageID JoinPartyCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 0);
			Result JoinPartyCmd::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_InviterID);
				protocolCheck(*input >> m_InvitedPlayer);

				return hr;

			}; // Result JoinPartyCmd::ParseMessage(const MessageData* pIMsg)


			Result JoinPartyCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) JoinPartyCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result JoinPartyCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* JoinPartyCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InInviterID)
					+ SerializedSizeOf(InInvitedPlayer)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::JoinPartyCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InInviterID);
				protocolCheck(*output << InInvitedPlayer);

				return hr;
			}; // MessageData* JoinPartyCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InInviterID, const PlayerInformation &InInvitedPlayer )

			Result JoinPartyCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result JoinPartyCmd::OverrideRouteContextDestination( EntityUID to )


			Result JoinPartyCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				JoinPartyCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "JoinParty:{0}:{1} , RouteContext:{2}, TransactionID:{3}, InviterID:{4}, InvitedPlayer:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetInviterID(), parser.GetInvitedPlayer()); 
				return ResultCode::SUCCESS;
			}; // Result JoinPartyCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID JoinPartyRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 0);
			Result JoinPartyRes::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);
				protocolCheck(*input >> m_PartyLeaderID);
				protocolCheck(input->Read(ArrayLen));
				uint8_t* ChatHistoryDataPtr = nullptr;
				protocolCheck(input->ReadLink(ChatHistoryDataPtr, ArrayLen));
				m_ChatHistoryData.SetLinkedBuffer(ArrayLen, ChatHistoryDataPtr);

				return hr;

			}; // Result JoinPartyRes::ParseMessage(const MessageData* pIMsg)


			Result JoinPartyRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) JoinPartyRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result JoinPartyRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* JoinPartyRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const PlayerID &InPartyLeaderID, const Array<uint8_t>& InChatHistoryData )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				uint16_t numberOfInChatHistoryData = (uint16_t)InChatHistoryData.size(); 
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InResult)
					+ SerializedSizeOf(InPartyLeaderID)
					+ SerializedSizeOf(InChatHistoryData)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::JoinPartyRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);
				protocolCheck(*output << InPartyLeaderID);
				protocolCheck(*output << InChatHistoryData);

				return hr;
			}; // MessageData* JoinPartyRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const PlayerID &InPartyLeaderID, const Array<uint8_t>& InChatHistoryData )

			Result JoinPartyRes::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result JoinPartyRes::OverrideRouteContextDestination( EntityUID to )


			Result JoinPartyRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				JoinPartyRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "JoinParty:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, PartyLeaderID:{5}, ChatHistoryData:{6,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetPartyLeaderID(), parser.GetChatHistoryData()); 
				return ResultCode::SUCCESS;
			}; // Result JoinPartyRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Player joined event.
			const MessageID PlayerJoinedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 1);
			Result PlayerJoinedS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_JoinedPlayer);

				return hr;

			}; // Result PlayerJoinedS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result PlayerJoinedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PlayerJoinedS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PlayerJoinedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* PlayerJoinedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InJoinedPlayer)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::PlayerJoinedS2CEvt::MID, __uiMessageSize ) );
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
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

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

			// S2C: Event for party leader is changed
			const MessageID PartyLeaderChangedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 2);
			Result PartyLeaderChangedS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_NewLeaderID);

				return hr;

			}; // Result PartyLeaderChangedS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result PartyLeaderChangedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PartyLeaderChangedS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PartyLeaderChangedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* PartyLeaderChangedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InNewLeaderID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::PartyLeaderChangedS2CEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InNewLeaderID);

				return hr;
			}; // MessageData* PartyLeaderChangedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InNewLeaderID )

			Result PartyLeaderChangedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result PartyLeaderChangedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PartyLeaderChangedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PartyLeaderChangedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PartyLeaderChanged:{0}:{1} , RouteContext:{2}, NewLeaderID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetNewLeaderID()); 
				return ResultCode::SUCCESS;
			}; // Result PartyLeaderChangedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Event for player left.
			const MessageID LeavePartyCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 3);
			Result LeavePartyCmd::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_PlayerID);

				return hr;

			}; // Result LeavePartyCmd::ParseMessage(const MessageData* pIMsg)


			Result LeavePartyCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) LeavePartyCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result LeavePartyCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* LeavePartyCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
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

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::LeavePartyCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InPlayerID);

				return hr;
			}; // MessageData* LeavePartyCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )

			Result LeavePartyCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result LeavePartyCmd::OverrideRouteContextDestination( EntityUID to )


			Result LeavePartyCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				LeavePartyCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "LeaveParty:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result LeavePartyCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID LeavePartyRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 3);
			Result LeavePartyRes::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);

				return hr;

			}; // Result LeavePartyRes::ParseMessage(const MessageData* pIMsg)


			Result LeavePartyRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) LeavePartyRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result LeavePartyRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* LeavePartyRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
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

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::LeavePartyRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);

				return hr;
			}; // MessageData* LeavePartyRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result LeavePartyRes::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result LeavePartyRes::OverrideRouteContextDestination( EntityUID to )


			Result LeavePartyRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				LeavePartyRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "LeaveParty:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result LeavePartyRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Player left
			const MessageID PlayerLeftS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 4);
			Result PlayerLeftS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_LeftPlayerID);

				return hr;

			}; // Result PlayerLeftS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result PlayerLeftS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PlayerLeftS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PlayerLeftS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* PlayerLeftS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InLeftPlayerID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::PlayerLeftS2CEvt::MID, __uiMessageSize ) );
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
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

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

			// Cmd: Kick player
			const MessageID KickPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 5);
			Result KickPlayerCmd::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_PlayerID);
				protocolCheck(*input >> m_PlayerToKick);

				return hr;

			}; // Result KickPlayerCmd::ParseMessage(const MessageData* pIMsg)


			Result KickPlayerCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) KickPlayerCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result KickPlayerCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* KickPlayerCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
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

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::KickPlayerCmd::MID, __uiMessageSize ) );
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
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

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

			const MessageID KickPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 5);
			Result KickPlayerRes::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);

				return hr;

			}; // Result KickPlayerRes::ParseMessage(const MessageData* pIMsg)


			Result KickPlayerRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) KickPlayerRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result KickPlayerRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* KickPlayerRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
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

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::KickPlayerRes::MID, __uiMessageSize ) );
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
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

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

			// S2C: Event for Player kicked.
			const MessageID PlayerKickedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 6);
			Result PlayerKickedS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_KickedPlayerID);

				return hr;

			}; // Result PlayerKickedS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result PlayerKickedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PlayerKickedS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PlayerKickedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* PlayerKickedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InKickedPlayerID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::PlayerKickedS2CEvt::MID, __uiMessageSize ) );
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
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

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

			// C2S: Send chat message to server.
			const MessageID ChatMessageC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 7);
			Result ChatMessageC2SEvt::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_PlayerID);
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_ChatMessage, ArrayLen));

				return hr;

			}; // Result ChatMessageC2SEvt::ParseMessage(const MessageData* pIMsg)


			Result ChatMessageC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) ChatMessageC2SEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result ChatMessageC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* ChatMessageC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InPlayerID)
					+ SerializedSizeOf(InChatMessage)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::ChatMessageC2SEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InPlayerID);
				protocolCheck(*output << InChatMessage);

				return hr;
			}; // MessageData* ChatMessageC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const char* InChatMessage )

			Result ChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

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
				protocolTrace( Debug1, "ChatMessage:{0}:{1} , RouteContext:{2}, PlayerID:{3}, ChatMessage:{4,60}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetPlayerID(), parser.GetChatMessage()); 
				return ResultCode::SUCCESS;
			}; // Result ChatMessageC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Brocast chatting message
			const MessageID ChatMessageS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 8);
			Result ChatMessageS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_SenderID);
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_SenderName, ArrayLen));
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_ChatMessage, ArrayLen));

				return hr;

			}; // Result ChatMessageS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result ChatMessageS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) ChatMessageS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result ChatMessageS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* ChatMessageS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
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

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::ChatMessageS2CEvt::MID, __uiMessageSize ) );
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
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

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

			// C2S: Quick Chatting message
			const MessageID QuickChatMessageC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 9);
			Result QuickChatMessageC2SEvt::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_PlayerID);
				protocolCheck(*input >> m_QuickChatID);

				return hr;

			}; // Result QuickChatMessageC2SEvt::ParseMessage(const MessageData* pIMsg)


			Result QuickChatMessageC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) QuickChatMessageC2SEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result QuickChatMessageC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* QuickChatMessageC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint32_t &InQuickChatID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InPlayerID)
					+ SerializedSizeOf(InQuickChatID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::QuickChatMessageC2SEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InPlayerID);
				protocolCheck(*output << InQuickChatID);

				return hr;
			}; // MessageData* QuickChatMessageC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint32_t &InQuickChatID )

			Result QuickChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result QuickChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result QuickChatMessageC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				QuickChatMessageC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "QuickChatMessage:{0}:{1} , RouteContext:{2}, PlayerID:{3}, QuickChatID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetPlayerID(), parser.GetQuickChatID()); 
				return ResultCode::SUCCESS;
			}; // Result QuickChatMessageC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Quick Chatting message
			const MessageID QuickChatMessageS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 10);
			Result QuickChatMessageS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_SenderID);
				protocolCheck(*input >> m_QuickChatID);

				return hr;

			}; // Result QuickChatMessageS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result QuickChatMessageS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) QuickChatMessageS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result QuickChatMessageS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* QuickChatMessageS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InSenderID, const uint32_t &InQuickChatID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InSenderID)
					+ SerializedSizeOf(InQuickChatID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::QuickChatMessageS2CEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InSenderID);
				protocolCheck(*output << InQuickChatID);

				return hr;
			}; // MessageData* QuickChatMessageS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InSenderID, const uint32_t &InQuickChatID )

			Result QuickChatMessageS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result QuickChatMessageS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result QuickChatMessageS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				QuickChatMessageS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "QuickChatMessage:{0}:{1} , RouteContext:{2}, SenderID:{3}, QuickChatID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetSenderID(), parser.GetQuickChatID()); 
				return ResultCode::SUCCESS;
			}; // Result QuickChatMessageS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Start party game matching
			const MessageID StartGameMatchCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 11);
			Result StartGameMatchCmd::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_PlayerID);
				protocolCheck(*input >> m_MaxGamePlayers);

				return hr;

			}; // Result StartGameMatchCmd::ParseMessage(const MessageData* pIMsg)


			Result StartGameMatchCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) StartGameMatchCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result StartGameMatchCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* StartGameMatchCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InPlayerID)
					+ SerializedSizeOf(InMaxGamePlayers)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::StartGameMatchCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InPlayerID);
				protocolCheck(*output << InMaxGamePlayers);

				return hr;
			}; // MessageData* StartGameMatchCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const uint32_t &InMaxGamePlayers )

			Result StartGameMatchCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result StartGameMatchCmd::OverrideRouteContextDestination( EntityUID to )


			Result StartGameMatchCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				StartGameMatchCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "StartGameMatch:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, MaxGamePlayers:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetMaxGamePlayers()); 
				return ResultCode::SUCCESS;
			}; // Result StartGameMatchCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID StartGameMatchRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 11);
			Result StartGameMatchRes::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);

				return hr;

			}; // Result StartGameMatchRes::ParseMessage(const MessageData* pIMsg)


			Result StartGameMatchRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) StartGameMatchRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result StartGameMatchRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* StartGameMatchRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
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

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::StartGameMatchRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);

				return hr;
			}; // MessageData* StartGameMatchRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result StartGameMatchRes::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result StartGameMatchRes::OverrideRouteContextDestination( EntityUID to )


			Result StartGameMatchRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				StartGameMatchRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "StartGameMatch:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result StartGameMatchRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Event sent when the party is queued for game matching
			const MessageID QueuedGameMatchingS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 12);
			Result QueuedGameMatchingS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_MatchingQueueTicket);

				return hr;

			}; // Result QueuedGameMatchingS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result QueuedGameMatchingS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) QueuedGameMatchingS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result QueuedGameMatchingS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* QueuedGameMatchingS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InMatchingQueueTicket)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::QueuedGameMatchingS2CEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InMatchingQueueTicket);

				return hr;
			}; // MessageData* QueuedGameMatchingS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )

			Result QueuedGameMatchingS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result QueuedGameMatchingS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result QueuedGameMatchingS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				QueuedGameMatchingS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "QueuedGameMatching:{0}:{1} , RouteContext:{2}, MatchingQueueTicket:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetMatchingQueueTicket()); 
				return ResultCode::SUCCESS;
			}; // Result QueuedGameMatchingS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Cancel game matching
			const MessageID CancelGameMatchCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 13);
			Result CancelGameMatchCmd::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_PlayerID);

				return hr;

			}; // Result CancelGameMatchCmd::ParseMessage(const MessageData* pIMsg)


			Result CancelGameMatchCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) CancelGameMatchCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result CancelGameMatchCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* CancelGameMatchCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
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

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::CancelGameMatchCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InPlayerID);

				return hr;
			}; // MessageData* CancelGameMatchCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )

			Result CancelGameMatchCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result CancelGameMatchCmd::OverrideRouteContextDestination( EntityUID to )


			Result CancelGameMatchCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				CancelGameMatchCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "CancelGameMatch:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result CancelGameMatchCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID CancelGameMatchRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 13);
			Result CancelGameMatchRes::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);

				return hr;

			}; // Result CancelGameMatchRes::ParseMessage(const MessageData* pIMsg)


			Result CancelGameMatchRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) CancelGameMatchRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result CancelGameMatchRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* CancelGameMatchRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
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

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::CancelGameMatchRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);

				return hr;
			}; // MessageData* CancelGameMatchRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result CancelGameMatchRes::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result CancelGameMatchRes::OverrideRouteContextDestination( EntityUID to )


			Result CancelGameMatchRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				CancelGameMatchRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "CancelGameMatch:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result CancelGameMatchRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Event sent when the game matching is canceled
			const MessageID CanceledGameMatchingS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 14);
			Result CanceledGameMatchingS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_MatchingQueueTicket);

				return hr;

			}; // Result CanceledGameMatchingS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result CanceledGameMatchingS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) CanceledGameMatchingS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result CanceledGameMatchingS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* CanceledGameMatchingS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InMatchingQueueTicket)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::CanceledGameMatchingS2CEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InMatchingQueueTicket);

				return hr;
			}; // MessageData* CanceledGameMatchingS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingQueueTicket )

			Result CanceledGameMatchingS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result CanceledGameMatchingS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result CanceledGameMatchingS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				CanceledGameMatchingS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "CanceledGameMatching:{0}:{1} , RouteContext:{2}, MatchingQueueTicket:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetMatchingQueueTicket()); 
				return ResultCode::SUCCESS;
			}; // Result CanceledGameMatchingS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Event sent when the the matching has dequeued.
			const MessageID MatchingItemDequeuedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEPARTY, 15);
			Result MatchingItemDequeuedS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_MatchingTicket);

				return hr;

			}; // Result MatchingItemDequeuedS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result MatchingItemDequeuedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) MatchingItemDequeuedS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result MatchingItemDequeuedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* MatchingItemDequeuedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InMatchingTicket)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameParty::MatchingItemDequeuedS2CEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InMatchingTicket);

				return hr;
			}; // MessageData* MatchingItemDequeuedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const MatchingQueueTicket &InMatchingTicket )

			Result MatchingItemDequeuedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result MatchingItemDequeuedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result MatchingItemDequeuedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				MatchingItemDequeuedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "MatchingItemDequeued:{0}:{1} , RouteContext:{2}, MatchingTicket:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result MatchingItemDequeuedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)



		}; // namespace GameParty
	}; // namespace Message
}; // namespace SF


