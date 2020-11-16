////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : Monitoring Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "Protocol/SFProtocol.h"
#include "String/SFToString.h"
#include "Net/SFNetToString.h"
#include "Container/SFArray.h"
#include "Protocol/SFProtocolHelper.h"
#include "Protocol/Message/MonitoringMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace Monitoring
		{
 			// Cmd: [deprecated]
			const MessageID GetInstanceListCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 0);
			Result GetInstanceListCmd::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_TransactionID);

				return hr;

			}; // Result GetInstanceListCmd::ParseMessage(const MessageData* pIMsg)


			Result GetInstanceListCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) GetInstanceListCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result GetInstanceListCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* GetInstanceListCmd::Create( IHeap& memHeap, const uint64_t &InTransactionID )
			{
 				MessageData *pNewMsg = nullptr;
				ScopeContext hr([&pNewMsg](Result hr) -> MessageData*
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
					+ SerializedSizeOf(InTransactionID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, Monitoring::GetInstanceListCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InTransactionID);

				return hr;
			}; // MessageData* GetInstanceListCmd::Create( IHeap& memHeap, const uint64_t &InTransactionID )



			Result GetInstanceListCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				GetInstanceListCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GetInstanceList:{0}:{1} , TransactionID:{2}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetTransactionID()); 
				return ResultCode::SUCCESS;
			}; // Result GetInstanceListCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID GetInstanceListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 0);
			Result GetInstanceListRes::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);
				protocolCheck(input->Read(ArrayLen));
				PerformanceCounterInstanceInfo* CounterInstancesPtr = nullptr;
				protocolCheck(input->ReadLink(CounterInstancesPtr, ArrayLen));
				m_CounterInstances.SetLinkedBuffer(ArrayLen, CounterInstancesPtr);
				protocolCheck(*input >> m_TotalInstanceCount);

				return hr;

			}; // Result GetInstanceListRes::ParseMessage(const MessageData* pIMsg)


			Result GetInstanceListRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) GetInstanceListRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result GetInstanceListRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* GetInstanceListRes::Create( IHeap& memHeap, const uint64_t &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount )
			{
 				MessageData *pNewMsg = nullptr;
				ScopeContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				uint16_t numberOfInCounterInstances = (uint16_t)InCounterInstances.size(); 
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InResult)
					+ SerializedSizeOf(InCounterInstances)
					+ SerializedSizeOf(InTotalInstanceCount)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, Monitoring::GetInstanceListRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);
				protocolCheck(*output << InCounterInstances);
				protocolCheck(*output << InTotalInstanceCount);

				return hr;
			}; // MessageData* GetInstanceListRes::Create( IHeap& memHeap, const uint64_t &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount )



			Result GetInstanceListRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				GetInstanceListRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GetInstanceList:{0}:{1} , TransactionID:{2}, Result:{3:X8}, CounterInstances:{4,30}, TotalInstanceCount:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetTransactionID(), parser.GetResult(), parser.GetCounterInstances(), parser.GetTotalInstanceCount()); 
				return ResultCode::SUCCESS;
			}; // Result GetInstanceListRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: [deprecated]
			const MessageID RequestCounterValuesCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 1);
			Result RequestCounterValuesCmd::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_InstanceUID);

				return hr;

			}; // Result RequestCounterValuesCmd::ParseMessage(const MessageData* pIMsg)


			Result RequestCounterValuesCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RequestCounterValuesCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RequestCounterValuesCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* RequestCounterValuesCmd::Create( IHeap& memHeap, const uint64_t &InTransactionID, const uint64_t &InInstanceUID )
			{
 				MessageData *pNewMsg = nullptr;
				ScopeContext hr([&pNewMsg](Result hr) -> MessageData*
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
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InInstanceUID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, Monitoring::RequestCounterValuesCmd::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InInstanceUID);

				return hr;
			}; // MessageData* RequestCounterValuesCmd::Create( IHeap& memHeap, const uint64_t &InTransactionID, const uint64_t &InInstanceUID )



			Result RequestCounterValuesCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RequestCounterValuesCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "RequestCounterValues:{0}:{1} , TransactionID:{2}, InstanceUID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetTransactionID(), parser.GetInstanceUID()); 
				return ResultCode::SUCCESS;
			}; // Result RequestCounterValuesCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID RequestCounterValuesRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 1);
			Result RequestCounterValuesRes::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_TransactionID);
				protocolCheck(*input >> m_Result);
				protocolCheck(*input >> m_InstanceUID);
				protocolCheck(input->Read(ArrayLen));
				uint64_t* CounterValuesPtr = nullptr;
				protocolCheck(input->ReadLink(CounterValuesPtr, ArrayLen));
				m_CounterValues.SetLinkedBuffer(ArrayLen, CounterValuesPtr);

				return hr;

			}; // Result RequestCounterValuesRes::ParseMessage(const MessageData* pIMsg)


			Result RequestCounterValuesRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RequestCounterValuesRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RequestCounterValuesRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* RequestCounterValuesRes::Create( IHeap& memHeap, const uint64_t &InTransactionID, const Result &InResult, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )
			{
 				MessageData *pNewMsg = nullptr;
				ScopeContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				uint16_t numberOfInCounterValues = (uint16_t)InCounterValues.size(); 
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InResult)
					+ SerializedSizeOf(InInstanceUID)
					+ SerializedSizeOf(InCounterValues)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, Monitoring::RequestCounterValuesRes::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InTransactionID);
				protocolCheck(*output << InResult);
				protocolCheck(*output << InInstanceUID);
				protocolCheck(*output << InCounterValues);

				return hr;
			}; // MessageData* RequestCounterValuesRes::Create( IHeap& memHeap, const uint64_t &InTransactionID, const Result &InResult, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )



			Result RequestCounterValuesRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RequestCounterValuesRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "RequestCounterValues:{0}:{1} , TransactionID:{2}, Result:{3:X8}, InstanceUID:{4}, CounterValues:{5,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetTransactionID(), parser.GetResult(), parser.GetInstanceUID(), parser.GetCounterValues()); 
				return ResultCode::SUCCESS;
			}; // Result RequestCounterValuesRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// C2S: Message when new performance counter is added.
			const MessageID PerformanceCounterNewC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 2);
			Result PerformanceCounterNewC2SEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_InstanceName, ArrayLen));
				protocolCheck(*input >> m_InstanceUID);
				protocolCheck(input->Read(ArrayLen));
				PerformanceCounterInfo* NewCountersPtr = nullptr;
				protocolCheck(input->ReadLink(NewCountersPtr, ArrayLen));
				m_NewCounters.SetLinkedBuffer(ArrayLen, NewCountersPtr);

				return hr;

			}; // Result PerformanceCounterNewC2SEvt::ParseMessage(const MessageData* pIMsg)


			Result PerformanceCounterNewC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PerformanceCounterNewC2SEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PerformanceCounterNewC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* PerformanceCounterNewC2SEvt::Create( IHeap& memHeap, const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
			{
 				MessageData *pNewMsg = nullptr;
				ScopeContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				uint16_t numberOfInNewCounters = (uint16_t)InNewCounters.size(); 
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InInstanceName)
					+ SerializedSizeOf(InInstanceUID)
					+ SerializedSizeOf(InNewCounters)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, Monitoring::PerformanceCounterNewC2SEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InInstanceName);
				protocolCheck(*output << InInstanceUID);
				protocolCheck(*output << InNewCounters);

				return hr;
			}; // MessageData* PerformanceCounterNewC2SEvt::Create( IHeap& memHeap, const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )



			Result PerformanceCounterNewC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PerformanceCounterNewC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug2, "PerformanceCounterNew:{0}:{1} , InstanceName:{2,60}, InstanceUID:{3}, NewCounters:{4,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetInstanceName(), parser.GetInstanceUID(), parser.GetNewCounters()); 
				return ResultCode::SUCCESS;
			}; // Result PerformanceCounterNewC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// C2S: Counter instance is deleted
			const MessageID PerformanceCounterFreeC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 3);
			Result PerformanceCounterFreeC2SEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(ArrayLen));
				uint64_t* FreeInstancesPtr = nullptr;
				protocolCheck(input->ReadLink(FreeInstancesPtr, ArrayLen));
				m_FreeInstances.SetLinkedBuffer(ArrayLen, FreeInstancesPtr);

				return hr;

			}; // Result PerformanceCounterFreeC2SEvt::ParseMessage(const MessageData* pIMsg)


			Result PerformanceCounterFreeC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PerformanceCounterFreeC2SEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PerformanceCounterFreeC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* PerformanceCounterFreeC2SEvt::Create( IHeap& memHeap, const Array<uint64_t>& InFreeInstances )
			{
 				MessageData *pNewMsg = nullptr;
				ScopeContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				uint16_t numberOfInFreeInstances = (uint16_t)InFreeInstances.size(); 
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InFreeInstances)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, Monitoring::PerformanceCounterFreeC2SEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InFreeInstances);

				return hr;
			}; // MessageData* PerformanceCounterFreeC2SEvt::Create( IHeap& memHeap, const Array<uint64_t>& InFreeInstances )



			Result PerformanceCounterFreeC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PerformanceCounterFreeC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug2, "PerformanceCounterFree:{0}:{1} , FreeInstances:{2,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetFreeInstances()); 
				return ResultCode::SUCCESS;
			}; // Result PerformanceCounterFreeC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// C2S: Counter update broadcast
			const MessageID PerformanceCounterUpdateC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 4);
			Result PerformanceCounterUpdateC2SEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_InstanceUID);
				protocolCheck(input->Read(ArrayLen));
				uint64_t* CounterValuesPtr = nullptr;
				protocolCheck(input->ReadLink(CounterValuesPtr, ArrayLen));
				m_CounterValues.SetLinkedBuffer(ArrayLen, CounterValuesPtr);

				return hr;

			}; // Result PerformanceCounterUpdateC2SEvt::ParseMessage(const MessageData* pIMsg)


			Result PerformanceCounterUpdateC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PerformanceCounterUpdateC2SEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PerformanceCounterUpdateC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* PerformanceCounterUpdateC2SEvt::Create( IHeap& memHeap, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )
			{
 				MessageData *pNewMsg = nullptr;
				ScopeContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				uint16_t numberOfInCounterValues = (uint16_t)InCounterValues.size(); 
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InInstanceUID)
					+ SerializedSizeOf(InCounterValues)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, Monitoring::PerformanceCounterUpdateC2SEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InInstanceUID);
				protocolCheck(*output << InCounterValues);

				return hr;
			}; // MessageData* PerformanceCounterUpdateC2SEvt::Create( IHeap& memHeap, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )



			Result PerformanceCounterUpdateC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PerformanceCounterUpdateC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug2, "PerformanceCounterUpdate:{0}:{1} , InstanceUID:{2}, CounterValues:{3,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetInstanceUID(), parser.GetCounterValues()); 
				return ResultCode::SUCCESS;
			}; // Result PerformanceCounterUpdateC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Server will send this message to an instance to get performance counters.
			const MessageID PerformanceCounterUpdateCounterInfoS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 5);
			Result PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(*input >> m_InstanceUID);

				return hr;

			}; // Result PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PerformanceCounterUpdateCounterInfoS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* PerformanceCounterUpdateCounterInfoS2CEvt::Create( IHeap& memHeap, const uint64_t &InInstanceUID )
			{
 				MessageData *pNewMsg = nullptr;
				ScopeContext hr([&pNewMsg](Result hr) -> MessageData*
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
					+ SerializedSizeOf(InInstanceUID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InInstanceUID);

				return hr;
			}; // MessageData* PerformanceCounterUpdateCounterInfoS2CEvt::Create( IHeap& memHeap, const uint64_t &InInstanceUID )



			Result PerformanceCounterUpdateCounterInfoS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PerformanceCounterUpdateCounterInfoS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug2, "PerformanceCounterUpdateCounterInfo:{0}:{1} , InstanceUID:{2}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetInstanceUID()); 
				return ResultCode::SUCCESS;
			}; // Result PerformanceCounterUpdateCounterInfoS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)



		}; // namespace Monitoring
	}; // namespace Message
}; // namespace SF


