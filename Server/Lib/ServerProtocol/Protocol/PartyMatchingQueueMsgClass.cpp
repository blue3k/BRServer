////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) StormForge
// 
// Author : Generated
// 
// Description : PartyMatchingQueue Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "Protocol/SFProtocol.h"
#include "Util/SFToString.h"
#include "Net/SFNetToString.h"
#include "Container/SFArray.h"
#include "Protocol/SFProtocolHelper.h"
#include "Protocol/PartyMatchingQueueMsgClass.h"



namespace SF
{
 	namespace Message
	{
 		namespace PartyMatchingQueue
		{
 			// Cmd: Register a party for matching
			const MessageID RegisterPartyMatchingCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 0);
			Result RegisterPartyMatchingCmd::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_RouteHopCount);
				protocolCheck(input->Read(ArrayLen));
				MatchingPlayerInformation* PlayersPtr = nullptr;
				protocolCheck(input->ReadLink(PlayersPtr, ArrayLen));
				m_Players.SetLinkedBuffer(ArrayLen, PlayersPtr);

				return hr;

			}; // Result RegisterPartyMatchingCmd::ParseMessage(const MessageData* pIMsg)


			Result RegisterPartyMatchingCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RegisterPartyMatchingCmd(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RegisterPartyMatchingCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* RegisterPartyMatchingCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )
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
					+ SerializedSizeOf(InPlayers)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::RegisterPartyMatchingCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InPlayers);

				return hr;
			}; // MessageData* RegisterPartyMatchingCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingPlayerInformation>& InPlayers )

			Result RegisterPartyMatchingCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RegisterPartyMatchingCmd parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug1, "RegisterPartyMatching:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, Players:{5,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetPlayers()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterPartyMatchingCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID RegisterPartyMatchingRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 0);
			Result RegisterPartyMatchingRes::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);
				protocolCheck(*input >> m_MatchingTicket);

				return hr;

			}; // Result RegisterPartyMatchingRes::ParseMessage(const MessageData* pIMsg)


			Result RegisterPartyMatchingRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RegisterPartyMatchingRes(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RegisterPartyMatchingRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* RegisterPartyMatchingRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket )
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
					+ SerializedSizeOf(InMatchingTicket)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::RegisterPartyMatchingRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);
				protocolCheck(*output << InMatchingTicket);

				return hr;
			}; // MessageData* RegisterPartyMatchingRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket )

			Result RegisterPartyMatchingRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RegisterPartyMatchingRes parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug1, "RegisterPartyMatching:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, MatchingTicket:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterPartyMatchingRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Register a player for matching
			const MessageID RegisterPlayerMatchingCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 1);
			Result RegisterPlayerMatchingCmd::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_RouteHopCount);
				protocolCheck(*input >> m_PlayerID);

				return hr;

			}; // Result RegisterPlayerMatchingCmd::ParseMessage(const MessageData* pIMsg)


			Result RegisterPlayerMatchingCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RegisterPlayerMatchingCmd(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RegisterPlayerMatchingCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* RegisterPlayerMatchingCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID )
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
					+ SerializedSizeOf(InPlayerID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::RegisterPlayerMatchingCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InPlayerID);

				return hr;
			}; // MessageData* RegisterPlayerMatchingCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID )

			Result RegisterPlayerMatchingCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RegisterPlayerMatchingCmd parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug1, "RegisterPlayerMatching:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, PlayerID:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterPlayerMatchingCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID RegisterPlayerMatchingRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 1);
			Result RegisterPlayerMatchingRes::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);
				protocolCheck(*input >> m_MatchingTicket);

				return hr;

			}; // Result RegisterPlayerMatchingRes::ParseMessage(const MessageData* pIMsg)


			Result RegisterPlayerMatchingRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RegisterPlayerMatchingRes(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RegisterPlayerMatchingRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* RegisterPlayerMatchingRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket )
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
					+ SerializedSizeOf(InMatchingTicket)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::RegisterPlayerMatchingRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);
				protocolCheck(*output << InMatchingTicket);

				return hr;
			}; // MessageData* RegisterPlayerMatchingRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket )

			Result RegisterPlayerMatchingRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RegisterPlayerMatchingRes parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug1, "RegisterPlayerMatching:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, MatchingTicket:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result RegisterPlayerMatchingRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: [Deprecated] update registration information
			const MessageID UpdateMatchingEntityUIDCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 2);
			Result UpdateMatchingEntityUIDCmd::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_RouteHopCount);
				protocolCheck(*input >> m_MatchingTicket);
				protocolCheck(*input >> m_PreviousUID);

				return hr;

			}; // Result UpdateMatchingEntityUIDCmd::ParseMessage(const MessageData* pIMsg)


			Result UpdateMatchingEntityUIDCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) UpdateMatchingEntityUIDCmd(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result UpdateMatchingEntityUIDCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* UpdateMatchingEntityUIDCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InPreviousUID )
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
					+ SerializedSizeOf(InMatchingTicket)
					+ SerializedSizeOf(InPreviousUID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::UpdateMatchingEntityUIDCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InMatchingTicket);
				protocolCheck(*output << InPreviousUID);

				return hr;
			}; // MessageData* UpdateMatchingEntityUIDCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InPreviousUID )

			Result UpdateMatchingEntityUIDCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				UpdateMatchingEntityUIDCmd parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug1, "UpdateMatchingEntityUID:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, MatchingTicket:{5}, PreviousUID:{6}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetMatchingTicket(), parser.GetPreviousUID()); 
				return ResultCode::SUCCESS;
			}; // Result UpdateMatchingEntityUIDCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID UpdateMatchingEntityUIDRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 2);
			Result UpdateMatchingEntityUIDRes::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);

				return hr;

			}; // Result UpdateMatchingEntityUIDRes::ParseMessage(const MessageData* pIMsg)


			Result UpdateMatchingEntityUIDRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) UpdateMatchingEntityUIDRes(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result UpdateMatchingEntityUIDRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* UpdateMatchingEntityUIDRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
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

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::UpdateMatchingEntityUIDRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);

				return hr;
			}; // MessageData* UpdateMatchingEntityUIDRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result UpdateMatchingEntityUIDRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				UpdateMatchingEntityUIDRes parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug1, "UpdateMatchingEntityUID:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result UpdateMatchingEntityUIDRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: calcel registration
			const MessageID UnregisterMatchingCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 3);
			Result UnregisterMatchingCmd::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_RouteHopCount);
				protocolCheck(*input >> m_MatchingTicket);

				return hr;

			}; // Result UnregisterMatchingCmd::ParseMessage(const MessageData* pIMsg)


			Result UnregisterMatchingCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) UnregisterMatchingCmd(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result UnregisterMatchingCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* UnregisterMatchingCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
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
					+ SerializedSizeOf(InMatchingTicket)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::UnregisterMatchingCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InMatchingTicket);

				return hr;
			}; // MessageData* UnregisterMatchingCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )

			Result UnregisterMatchingCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				UnregisterMatchingCmd parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug1, "UnregisterMatching:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, MatchingTicket:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result UnregisterMatchingCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID UnregisterMatchingRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 3);
			Result UnregisterMatchingRes::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);

				return hr;

			}; // Result UnregisterMatchingRes::ParseMessage(const MessageData* pIMsg)


			Result UnregisterMatchingRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) UnregisterMatchingRes(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result UnregisterMatchingRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* UnregisterMatchingRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
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

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::UnregisterMatchingRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);

				return hr;
			}; // MessageData* UnregisterMatchingRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result UnregisterMatchingRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				UnregisterMatchingRes parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug1, "UnregisterMatching:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result UnregisterMatchingRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Event when the matching is canceled, for party
			const MessageID PartyMatchingCanceledS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 4);
			Result PartyMatchingCanceledS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_RouteHopCount);
				protocolCheck(*input >> m_DestPartyUID);
				protocolCheck(*input >> m_MatchingTicket);

				return hr;

			}; // Result PartyMatchingCanceledS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result PartyMatchingCanceledS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PartyMatchingCanceledS2CEvt(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PartyMatchingCanceledS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* PartyMatchingCanceledS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket )
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
					+ SerializedSizeOf(InDestPartyUID)
					+ SerializedSizeOf(InMatchingTicket)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::PartyMatchingCanceledS2CEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InDestPartyUID);
				protocolCheck(*output << InMatchingTicket);

				return hr;
			}; // MessageData* PartyMatchingCanceledS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const uint64_t &InDestPartyUID, const MatchingQueueTicket &InMatchingTicket )

			Result PartyMatchingCanceledS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PartyMatchingCanceledS2CEvt parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug1, "PartyMatchingCanceled:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, DestPartyUID:{4}, MatchingTicket:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetDestPartyUID(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result PartyMatchingCanceledS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Event whan the matching is canceled, for a player
			const MessageID PlayerMatchingCanceledS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 5);
			Result PlayerMatchingCanceledS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_RouteHopCount);
				protocolCheck(*input >> m_DestPlayerID);
				protocolCheck(*input >> m_MatchingTicket);

				return hr;

			}; // Result PlayerMatchingCanceledS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result PlayerMatchingCanceledS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PlayerMatchingCanceledS2CEvt(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PlayerMatchingCanceledS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* PlayerMatchingCanceledS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket )
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
					+ SerializedSizeOf(InDestPlayerID)
					+ SerializedSizeOf(InMatchingTicket)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::PlayerMatchingCanceledS2CEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InDestPlayerID);
				protocolCheck(*output << InMatchingTicket);

				return hr;
			}; // MessageData* PlayerMatchingCanceledS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const MatchingQueueTicket &InMatchingTicket )

			Result PlayerMatchingCanceledS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PlayerMatchingCanceledS2CEvt parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug1, "PlayerMatchingCanceled:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, DestPlayerID:{4}, MatchingTicket:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetDestPlayerID(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerMatchingCanceledS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Matching item has been dequeued for matching
			const MessageID PartyMatchingItemDequeuedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 6);
			Result PartyMatchingItemDequeuedS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_RouteHopCount);
				protocolCheck(*input >> m_MatchingTicket);

				return hr;

			}; // Result PartyMatchingItemDequeuedS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result PartyMatchingItemDequeuedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PartyMatchingItemDequeuedS2CEvt(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PartyMatchingItemDequeuedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* PartyMatchingItemDequeuedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
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
					+ SerializedSizeOf(InMatchingTicket)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::PartyMatchingItemDequeuedS2CEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InMatchingTicket);

				return hr;
			}; // MessageData* PartyMatchingItemDequeuedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )

			Result PartyMatchingItemDequeuedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PartyMatchingItemDequeuedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug1, "PartyMatchingItemDequeued:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, MatchingTicket:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result PartyMatchingItemDequeuedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Matching item has been dequeued for matching
			const MessageID PlayerMatchingItemDequeuedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 7);
			Result PlayerMatchingItemDequeuedS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_RouteHopCount);
				protocolCheck(*input >> m_MatchingTicket);

				return hr;

			}; // Result PlayerMatchingItemDequeuedS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result PlayerMatchingItemDequeuedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PlayerMatchingItemDequeuedS2CEvt(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PlayerMatchingItemDequeuedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* PlayerMatchingItemDequeuedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
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
					+ SerializedSizeOf(InMatchingTicket)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::PlayerMatchingItemDequeuedS2CEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InMatchingTicket);

				return hr;
			}; // MessageData* PlayerMatchingItemDequeuedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )

			Result PlayerMatchingItemDequeuedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PlayerMatchingItemDequeuedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug1, "PlayerMatchingItemDequeued:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, MatchingTicket:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerMatchingItemDequeuedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Reserve a item
			const MessageID ReserveItemCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 8);
			Result ReserveItemCmd::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_RouteHopCount);

				return hr;

			}; // Result ReserveItemCmd::ParseMessage(const MessageData* pIMsg)


			Result ReserveItemCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) ReserveItemCmd(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result ReserveItemCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* ReserveItemCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount )
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
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::ReserveItemCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InRouteHopCount);

				return hr;
			}; // MessageData* ReserveItemCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount )

			Result ReserveItemCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				ReserveItemCmd parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug4, "ReserveItem:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount()); 
				return ResultCode::SUCCESS;
			}; // Result ReserveItemCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID ReserveItemRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 8);
			Result ReserveItemRes::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);
				protocolCheck(*input >> m_NumberOfPlayersInTheItem);
				protocolCheck(*input >> m_MatchingTicket);

				return hr;

			}; // Result ReserveItemRes::ParseMessage(const MessageData* pIMsg)


			Result ReserveItemRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) ReserveItemRes(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result ReserveItemRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* ReserveItemRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket )
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
					+ SerializedSizeOf(InNumberOfPlayersInTheItem)
					+ SerializedSizeOf(InMatchingTicket)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::ReserveItemRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);
				protocolCheck(*output << InNumberOfPlayersInTheItem);
				protocolCheck(*output << InMatchingTicket);

				return hr;
			}; // MessageData* ReserveItemRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InNumberOfPlayersInTheItem, const MatchingQueueTicket &InMatchingTicket )

			Result ReserveItemRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				ReserveItemRes parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug4, "ReserveItem:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, NumberOfPlayersInTheItem:{5}, MatchingTicket:{6}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetNumberOfPlayersInTheItem(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result ReserveItemRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Reserve a item
			const MessageID ReserveItemsCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 9);
			Result ReserveItemsCmd::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_RouteHopCount);
				protocolCheck(*input >> m_NumberOfItemsToReserve);

				return hr;

			}; // Result ReserveItemsCmd::ParseMessage(const MessageData* pIMsg)


			Result ReserveItemsCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) ReserveItemsCmd(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result ReserveItemsCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* ReserveItemsCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InNumberOfItemsToReserve )
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
					+ SerializedSizeOf(InNumberOfItemsToReserve)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::ReserveItemsCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InNumberOfItemsToReserve);

				return hr;
			}; // MessageData* ReserveItemsCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const uint32_t &InNumberOfItemsToReserve )

			Result ReserveItemsCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				ReserveItemsCmd parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug4, "ReserveItems:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, NumberOfItemsToReserve:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetNumberOfItemsToReserve()); 
				return ResultCode::SUCCESS;
			}; // Result ReserveItemsCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID ReserveItemsRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 9);
			Result ReserveItemsRes::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);
				protocolCheck(input->Read(ArrayLen));
				uint32_t* NumberOfPlayersInTheItemPtr = nullptr;
				protocolCheck(input->ReadLink(NumberOfPlayersInTheItemPtr, ArrayLen));
				m_NumberOfPlayersInTheItem.SetLinkedBuffer(ArrayLen, NumberOfPlayersInTheItemPtr);
				protocolCheck(input->Read(ArrayLen));
				MatchingQueueTicket* MatchingTicketPtr = nullptr;
				protocolCheck(input->ReadLink(MatchingTicketPtr, ArrayLen));
				m_MatchingTicket.SetLinkedBuffer(ArrayLen, MatchingTicketPtr);

				return hr;

			}; // Result ReserveItemsRes::ParseMessage(const MessageData* pIMsg)


			Result ReserveItemsRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) ReserveItemsRes(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result ReserveItemsRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* ReserveItemsRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<uint32_t>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket )
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
					+ SerializedSizeOf(InNumberOfPlayersInTheItem)
					+ SerializedSizeOf(InMatchingTicket)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::ReserveItemsRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);
				protocolCheck(*output << InNumberOfPlayersInTheItem);
				protocolCheck(*output << InMatchingTicket);

				return hr;
			}; // MessageData* ReserveItemsRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<uint32_t>& InNumberOfPlayersInTheItem, const Array<MatchingQueueTicket>& InMatchingTicket )

			Result ReserveItemsRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				ReserveItemsRes parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug4, "ReserveItems:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, NumberOfPlayersInTheItem:{5,30}, MatchingTicket:{6,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetNumberOfPlayersInTheItem(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result ReserveItemsRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Cancel reservation
			const MessageID CancelReservationCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 10);
			Result CancelReservationCmd::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_RouteHopCount);
				protocolCheck(*input >> m_TicketToCancel);

				return hr;

			}; // Result CancelReservationCmd::ParseMessage(const MessageData* pIMsg)


			Result CancelReservationCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) CancelReservationCmd(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result CancelReservationCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* CancelReservationCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )
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
					+ SerializedSizeOf(InTicketToCancel)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::CancelReservationCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InTicketToCancel);

				return hr;
			}; // MessageData* CancelReservationCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InTicketToCancel )

			Result CancelReservationCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				CancelReservationCmd parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug4, "CancelReservation:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, TicketToCancel:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetTicketToCancel()); 
				return ResultCode::SUCCESS;
			}; // Result CancelReservationCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID CancelReservationRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 10);
			Result CancelReservationRes::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);

				return hr;

			}; // Result CancelReservationRes::ParseMessage(const MessageData* pIMsg)


			Result CancelReservationRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) CancelReservationRes(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result CancelReservationRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* CancelReservationRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
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

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::CancelReservationRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);

				return hr;
			}; // MessageData* CancelReservationRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result CancelReservationRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				CancelReservationRes parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug4, "CancelReservation:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result CancelReservationRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Cancel reservation
			const MessageID CancelReservationsCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 11);
			Result CancelReservationsCmd::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_RouteHopCount);
				protocolCheck(input->Read(ArrayLen));
				MatchingQueueTicket* TicketToCancelPtr = nullptr;
				protocolCheck(input->ReadLink(TicketToCancelPtr, ArrayLen));
				m_TicketToCancel.SetLinkedBuffer(ArrayLen, TicketToCancelPtr);

				return hr;

			}; // Result CancelReservationsCmd::ParseMessage(const MessageData* pIMsg)


			Result CancelReservationsCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) CancelReservationsCmd(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result CancelReservationsCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* CancelReservationsCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )
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
					+ SerializedSizeOf(InTicketToCancel)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::CancelReservationsCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InTicketToCancel);

				return hr;
			}; // MessageData* CancelReservationsCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const Array<MatchingQueueTicket>& InTicketToCancel )

			Result CancelReservationsCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				CancelReservationsCmd parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug4, "CancelReservations:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, TicketToCancel:{5,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetTicketToCancel()); 
				return ResultCode::SUCCESS;
			}; // Result CancelReservationsCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID CancelReservationsRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 11);
			Result CancelReservationsRes::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);

				return hr;

			}; // Result CancelReservationsRes::ParseMessage(const MessageData* pIMsg)


			Result CancelReservationsRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) CancelReservationsRes(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result CancelReservationsRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* CancelReservationsRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
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

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::CancelReservationsRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);

				return hr;
			}; // MessageData* CancelReservationsRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result CancelReservationsRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				CancelReservationsRes parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug4, "CancelReservations:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result CancelReservationsRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Dequeue a reserved item
			const MessageID DequeueItemCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 12);
			Result DequeueItemCmd::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_RouteHopCount);
				protocolCheck(*input >> m_MatchingTicket);

				return hr;

			}; // Result DequeueItemCmd::ParseMessage(const MessageData* pIMsg)


			Result DequeueItemCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) DequeueItemCmd(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result DequeueItemCmd::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* DequeueItemCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
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
					+ SerializedSizeOf(InMatchingTicket)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::DequeueItemCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InMatchingTicket);

				return hr;
			}; // MessageData* DequeueItemCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )

			Result DequeueItemCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				DequeueItemCmd parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug4, "DequeueItem:{0}:{1} , RouteContext:{2}, TransactionID:{3}, RouteHopCount:{4}, MatchingTicket:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetRouteHopCount(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result DequeueItemCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID DequeueItemRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 12);
			Result DequeueItemRes::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);
				protocolCheck(*input >> m_MatchingTicket);
				protocolCheck(*input >> m_RegisterUID);
				protocolCheck(*input >> m_RegisterID);
				protocolCheck(input->Read(ArrayLen));
				MatchingPlayerInformation* PlayersPtr = nullptr;
				protocolCheck(input->ReadLink(PlayersPtr, ArrayLen));
				m_Players.SetLinkedBuffer(ArrayLen, PlayersPtr);

				return hr;

			}; // Result DequeueItemRes::ParseMessage(const MessageData* pIMsg)


			Result DequeueItemRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) DequeueItemRes(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result DequeueItemRes::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* DequeueItemRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers )
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
					+ SerializedSizeOf(InMatchingTicket)
					+ SerializedSizeOf(InRegisterUID)
					+ SerializedSizeOf(InRegisterID)
					+ SerializedSizeOf(InPlayers)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::DequeueItemRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);
				protocolCheck(*output << InMatchingTicket);
				protocolCheck(*output << InRegisterUID);
				protocolCheck(*output << InRegisterID);
				protocolCheck(*output << InPlayers);

				return hr;
			}; // MessageData* DequeueItemRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const MatchingQueueTicket &InMatchingTicket, const uint64_t &InRegisterUID, const PlayerID &InRegisterID, const Array<MatchingPlayerInformation>& InPlayers )

			Result DequeueItemRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				DequeueItemRes parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug4, "DequeueItem:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, MatchingTicket:{5}, RegisterUID:{6}, RegisterID:{7}, Players:{8,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetMatchingTicket(), parser.GetRegisterUID(), parser.GetRegisterID(), parser.GetPlayers()); 
				return ResultCode::SUCCESS;
			}; // Result DequeueItemRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// C2S: Item error you should delete it
			const MessageID MatchingItemErrorC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHINGQUEUE, 13);
			Result MatchingItemErrorC2SEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_RouteHopCount);
				protocolCheck(*input >> m_MatchingTicket);

				return hr;

			}; // Result MatchingItemErrorC2SEvt::ParseMessage(const MessageData* pIMsg)


			Result MatchingItemErrorC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) MatchingItemErrorC2SEvt(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result MatchingItemErrorC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* MatchingItemErrorC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )
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
					+ SerializedSizeOf(InMatchingTicket)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatchingQueue::MatchingItemErrorC2SEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InMatchingTicket);

				return hr;
			}; // MessageData* MatchingItemErrorC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const MatchingQueueTicket &InMatchingTicket )

			Result MatchingItemErrorC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				MatchingItemErrorC2SEvt parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug1, "MatchingItemError:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, MatchingTicket:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetMatchingTicket()); 
				return ResultCode::SUCCESS;
			}; // Result MatchingItemErrorC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)



		}; // namespace PartyMatchingQueue
	}; // namespace Message
}; // namespace SF


