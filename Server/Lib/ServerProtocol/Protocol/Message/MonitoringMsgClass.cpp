////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : Monitoring Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
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
			Result GetInstanceListCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );


			Proc_End:

				return hr;

			}; // Result GetInstanceListCmd::ParseMessage( MessageData* pIMsg )


			Result GetInstanceListCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) GetInstanceListCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GetInstanceListCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* GetInstanceListCmd::Create( IHeap& memHeap, const TransactionID &InTransactionID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(TransactionID));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, Monitoring::GetInstanceListCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GetInstanceListCmd::Create( IHeap& memHeap, const TransactionID &InTransactionID )



			Result GetInstanceListCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				GetInstanceListCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GetInstanceList:{0}:{1} , TransactionID:{2}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetTransactionID()); 
				return ResultCode::SUCCESS;
			}; // Result GetInstanceListCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID GetInstanceListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 0);
			Result GetInstanceListRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofCounterInstances = 0; PerformanceCounterInstanceInfo* pCounterInstances = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofCounterInstances, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pCounterInstances, pCur, iMsgSize, sizeof(PerformanceCounterInstanceInfo)*numberofCounterInstances ) );
				m_CounterInstances.SetLinkedBuffer(numberofCounterInstances, numberofCounterInstances, pCounterInstances);
				protocolChk( Protocol::StreamParamCopy( &m_TotalInstanceCount, pCur, iMsgSize, sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result GetInstanceListRes::ParseMessage( MessageData* pIMsg )


			Result GetInstanceListRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) GetInstanceListRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GetInstanceListRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* GetInstanceListRes::Create( IHeap& memHeap, const TransactionID &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(PerformanceCounterInstanceInfo)*InCounterInstances.size() + sizeof(uint16_t)
					+ sizeof(uint32_t));


				uint16_t numberOfInCounterInstances = (uint16_t)InCounterInstances.size(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, Monitoring::GetInstanceListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInCounterInstances, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InCounterInstances.data(), (INT)(sizeof(PerformanceCounterInstanceInfo)*InCounterInstances.size())); 
				Protocol::PackParamCopy( pMsgData, &InTotalInstanceCount, sizeof(uint32_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GetInstanceListRes::Create( IHeap& memHeap, const TransactionID &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount )



			Result GetInstanceListRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				GetInstanceListRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GetInstanceList:{0}:{1} , TransactionID:{2}, Result:{3:X8}, CounterInstances:{4,30}, TotalInstanceCount:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetTransactionID(), parser.GetResult(), parser.GetCounterInstances(), parser.GetTotalInstanceCount()); 
				return ResultCode::SUCCESS;
			}; // Result GetInstanceListRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// Cmd: [deprecated]
			const MessageID RequestCounterValuesCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 1);
			Result RequestCounterValuesCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result RequestCounterValuesCmd::ParseMessage( MessageData* pIMsg )


			Result RequestCounterValuesCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) RequestCounterValuesCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RequestCounterValuesCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* RequestCounterValuesCmd::Create( IHeap& memHeap, const TransactionID &InTransactionID, const uint64_t &InInstanceUID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(uint64_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, Monitoring::RequestCounterValuesCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(uint64_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RequestCounterValuesCmd::Create( IHeap& memHeap, const TransactionID &InTransactionID, const uint64_t &InInstanceUID )



			Result RequestCounterValuesCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				RequestCounterValuesCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "RequestCounterValues:{0}:{1} , TransactionID:{2}, InstanceUID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetTransactionID(), parser.GetInstanceUID()); 
				return ResultCode::SUCCESS;
			}; // Result RequestCounterValuesCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID RequestCounterValuesRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 1);
			Result RequestCounterValuesRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofCounterValues = 0; uint64_t* pCounterValues = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofCounterValues, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pCounterValues, pCur, iMsgSize, sizeof(uint64_t)*numberofCounterValues ) );
				m_CounterValues.SetLinkedBuffer(numberofCounterValues, numberofCounterValues, pCounterValues);


			Proc_End:

				return hr;

			}; // Result RequestCounterValuesRes::ParseMessage( MessageData* pIMsg )


			Result RequestCounterValuesRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) RequestCounterValuesRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RequestCounterValuesRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* RequestCounterValuesRes::Create( IHeap& memHeap, const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(uint64_t)
					+ sizeof(uint64_t)*InCounterValues.size() + sizeof(uint16_t));


				uint16_t numberOfInCounterValues = (uint16_t)InCounterValues.size(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, Monitoring::RequestCounterValuesRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &numberOfInCounterValues, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InCounterValues.data(), (INT)(sizeof(uint64_t)*InCounterValues.size())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RequestCounterValuesRes::Create( IHeap& memHeap, const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )



			Result RequestCounterValuesRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				RequestCounterValuesRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "RequestCounterValues:{0}:{1} , TransactionID:{2}, Result:{3:X8}, InstanceUID:{4}, CounterValues:{5,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetTransactionID(), parser.GetResult(), parser.GetInstanceUID(), parser.GetCounterValues()); 
				return ResultCode::SUCCESS;
			}; // Result RequestCounterValuesRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// C2S: Message when new performance counter is added.
			const MessageID PerformanceCounterNewC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 2);
			Result PerformanceCounterNewC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfInstanceName = 0;
				uint16_t numberofNewCounters = 0; PerformanceCounterInfo* pNewCounters = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &uiSizeOfInstanceName, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_InstanceName, pCur, iMsgSize, sizeof(char)*uiSizeOfInstanceName ) );
				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofNewCounters, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pNewCounters, pCur, iMsgSize, sizeof(PerformanceCounterInfo)*numberofNewCounters ) );
				m_NewCounters.SetLinkedBuffer(numberofNewCounters, numberofNewCounters, pNewCounters);


			Proc_End:

				return hr;

			}; // Result PerformanceCounterNewC2SEvt::ParseMessage( MessageData* pIMsg )


			Result PerformanceCounterNewC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) PerformanceCounterNewC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PerformanceCounterNewC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* PerformanceCounterNewC2SEvt::Create( IHeap& memHeap, const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInInstanceNameLength = InInstanceName ? (uint16_t)(strlen(InInstanceName)+1) : 1;
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) +  + sizeof(uint16_t) + __uiInInstanceNameLength 
					+ sizeof(uint64_t)
					+ sizeof(PerformanceCounterInfo)*InNewCounters.size() + sizeof(uint16_t));


				uint16_t numberOfInNewCounters = (uint16_t)InNewCounters.size(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, Monitoring::PerformanceCounterNewC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInInstanceNameLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InInstanceName ? InInstanceName : "", __uiInInstanceNameLength );
				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &numberOfInNewCounters, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InNewCounters.data(), (INT)(sizeof(PerformanceCounterInfo)*InNewCounters.size())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PerformanceCounterNewC2SEvt::Create( IHeap& memHeap, const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )



			Result PerformanceCounterNewC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				PerformanceCounterNewC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug2, "PerformanceCounterNew:{0}:{1} , InstanceName:{2,60}, InstanceUID:{3}, NewCounters:{4,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetInstanceName(), parser.GetInstanceUID(), parser.GetNewCounters()); 
				return ResultCode::SUCCESS;
			}; // Result PerformanceCounterNewC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// C2S: Counter instance is deleted
			const MessageID PerformanceCounterFreeC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 3);
			Result PerformanceCounterFreeC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofFreeInstances = 0; uint64_t* pFreeInstances = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &numberofFreeInstances, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pFreeInstances, pCur, iMsgSize, sizeof(uint64_t)*numberofFreeInstances ) );
				m_FreeInstances.SetLinkedBuffer(numberofFreeInstances, numberofFreeInstances, pFreeInstances);


			Proc_End:

				return hr;

			}; // Result PerformanceCounterFreeC2SEvt::ParseMessage( MessageData* pIMsg )


			Result PerformanceCounterFreeC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) PerformanceCounterFreeC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PerformanceCounterFreeC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* PerformanceCounterFreeC2SEvt::Create( IHeap& memHeap, const Array<uint64_t>& InFreeInstances )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(uint64_t)*InFreeInstances.size() + sizeof(uint16_t));


				uint16_t numberOfInFreeInstances = (uint16_t)InFreeInstances.size(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, Monitoring::PerformanceCounterFreeC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &numberOfInFreeInstances, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InFreeInstances.data(), (INT)(sizeof(uint64_t)*InFreeInstances.size())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PerformanceCounterFreeC2SEvt::Create( IHeap& memHeap, const Array<uint64_t>& InFreeInstances )



			Result PerformanceCounterFreeC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				PerformanceCounterFreeC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug2, "PerformanceCounterFree:{0}:{1} , FreeInstances:{2,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetFreeInstances()); 
				return ResultCode::SUCCESS;
			}; // Result PerformanceCounterFreeC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// C2S: Counter update broadcast
			const MessageID PerformanceCounterUpdateC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 4);
			Result PerformanceCounterUpdateC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofCounterValues = 0; uint64_t* pCounterValues = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofCounterValues, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pCounterValues, pCur, iMsgSize, sizeof(uint64_t)*numberofCounterValues ) );
				m_CounterValues.SetLinkedBuffer(numberofCounterValues, numberofCounterValues, pCounterValues);


			Proc_End:

				return hr;

			}; // Result PerformanceCounterUpdateC2SEvt::ParseMessage( MessageData* pIMsg )


			Result PerformanceCounterUpdateC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) PerformanceCounterUpdateC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PerformanceCounterUpdateC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* PerformanceCounterUpdateC2SEvt::Create( IHeap& memHeap, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(uint64_t)
					+ sizeof(uint64_t)*InCounterValues.size() + sizeof(uint16_t));


				uint16_t numberOfInCounterValues = (uint16_t)InCounterValues.size(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, Monitoring::PerformanceCounterUpdateC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &numberOfInCounterValues, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InCounterValues.data(), (INT)(sizeof(uint64_t)*InCounterValues.size())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PerformanceCounterUpdateC2SEvt::Create( IHeap& memHeap, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )



			Result PerformanceCounterUpdateC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				PerformanceCounterUpdateC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug2, "PerformanceCounterUpdate:{0}:{1} , InstanceUID:{2}, CounterValues:{3,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetInstanceUID(), parser.GetCounterValues()); 
				return ResultCode::SUCCESS;
			}; // Result PerformanceCounterUpdateC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Server will send this message to an instance to get performance counters.
			const MessageID PerformanceCounterUpdateCounterInfoS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 5);
			Result PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memHeap) PerformanceCounterUpdateCounterInfoS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* PerformanceCounterUpdateCounterInfoS2CEvt::Create( IHeap& memHeap, const uint64_t &InInstanceUID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(uint64_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memHeap, Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(uint64_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) delete pNewMsg;
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PerformanceCounterUpdateCounterInfoS2CEvt::Create( IHeap& memHeap, const uint64_t &InInstanceUID )



			Result PerformanceCounterUpdateCounterInfoS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				PerformanceCounterUpdateCounterInfoS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug2, "PerformanceCounterUpdateCounterInfo:{0}:{1} , InstanceUID:{2}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetInstanceUID()); 
				return ResultCode::SUCCESS;
			}; // Result PerformanceCounterUpdateCounterInfoS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)



		}; // namespace Monitoring
	}; // namespace Message
}; // namespace SF


