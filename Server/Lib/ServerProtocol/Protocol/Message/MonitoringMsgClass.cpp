﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : Monitoring Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Protocol/Protocol.h"
#include "String/ToString.h"
#include "Net/NetToString.h"
#include "Container/SFArray.h"
#include "Protocol/ProtocolHelper.h"
#include "Protocol/Message/MonitoringMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace Monitoring
		{
 			// Cmd: Add a player to ranking
			const MessageID GetInstanceListCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 0);
			Result GetInstanceListCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );


			Proc_End:

				return hr;

			}; // Result GetInstanceListCmd::ParseMessage( MessageData* pIMsg )

			Result GetInstanceListCmd::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				GetInstanceListCmd parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("TransactionID", parser.GetTransactionID());


			Proc_End:

				return hr;

			}; // Result GetInstanceListCmd::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result GetInstanceListCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GetInstanceListCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GetInstanceListCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* GetInstanceListCmd::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(TransactionID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, Monitoring::GetInstanceListCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GetInstanceListCmd::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID )



			Result GetInstanceListCmd::TraceOut(MessageData* pMsg)
			{
 				GetInstanceListCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "GetInstanceList:{0}:{1} , TransactionID:{2}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetTransactionID()); 
				return ResultCode::SUCCESS;
			}; // Result GetInstanceListCmd::TraceOut(MessageData* pMsg)

			const MessageID GetInstanceListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 0);
			Result GetInstanceListRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofCounterInstances = 0; PerformanceCounterInstanceInfo* pCounterInstances = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
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

			Result GetInstanceListRes::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				GetInstanceListRes parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("TransactionID", parser.GetTransactionID());
				variableBuilder.SetVariable("Result", parser.GetResult());
				variableBuilder.SetVariable("CounterInstances", (int)parser.GetCounterInstances().GetItemCount(), (const PerformanceCounterInstanceInfo*)parser.GetCounterInstances().data());
				variableBuilder.SetVariable("TotalInstanceCount", parser.GetTotalInstanceCount());


			Proc_End:

				return hr;

			}; // Result GetInstanceListRes::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result GetInstanceListRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GetInstanceListRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GetInstanceListRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* GetInstanceListRes::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(PerformanceCounterInstanceInfo)*InCounterInstances.GetItemCount() + sizeof(uint16_t)
					+ sizeof(uint32_t));


				uint16_t numberOfInCounterInstances = (uint16_t)InCounterInstances.GetItemCount(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, Monitoring::GetInstanceListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInCounterInstances, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InCounterInstances.data(), (INT)(sizeof(PerformanceCounterInstanceInfo)*InCounterInstances.GetItemCount())); 
				Protocol::PackParamCopy( pMsgData, &InTotalInstanceCount, sizeof(uint32_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GetInstanceListRes::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount )



			Result GetInstanceListRes::TraceOut(MessageData* pMsg)
			{
 				GetInstanceListRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "GetInstanceList:{0}:{1} , TransactionID:{2}, Result:{3:X8}, CounterInstances:{4,30}, TotalInstanceCount:{5}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetTransactionID(), parser.GetResult(), parser.GetCounterInstances(), parser.GetTotalInstanceCount()); 
				return ResultCode::SUCCESS;
			}; // Result GetInstanceListRes::TraceOut(MessageData* pMsg)

			// Cmd: Remove a player to ranking
			const MessageID RequestCounterValuesCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 1);
			Result RequestCounterValuesCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result RequestCounterValuesCmd::ParseMessage( MessageData* pIMsg )

			Result RequestCounterValuesCmd::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				RequestCounterValuesCmd parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("TransactionID", parser.GetTransactionID());
				variableBuilder.SetVariable("InstanceUID", parser.GetInstanceUID());


			Proc_End:

				return hr;

			}; // Result RequestCounterValuesCmd::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result RequestCounterValuesCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RequestCounterValuesCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RequestCounterValuesCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* RequestCounterValuesCmd::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const uint64_t &InInstanceUID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(uint64_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, Monitoring::RequestCounterValuesCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(uint64_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RequestCounterValuesCmd::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const uint64_t &InInstanceUID )



			Result RequestCounterValuesCmd::TraceOut(MessageData* pMsg)
			{
 				RequestCounterValuesCmd parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "RequestCounterValues:{0}:{1} , TransactionID:{2}, InstanceUID:{3}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetTransactionID(), parser.GetInstanceUID()); 
				return ResultCode::SUCCESS;
			}; // Result RequestCounterValuesCmd::TraceOut(MessageData* pMsg)

			const MessageID RequestCounterValuesRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 1);
			Result RequestCounterValuesRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofCounterValues = 0; uint64_t* pCounterValues = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
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

			Result RequestCounterValuesRes::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				RequestCounterValuesRes parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("TransactionID", parser.GetTransactionID());
				variableBuilder.SetVariable("Result", parser.GetResult());
				variableBuilder.SetVariable("InstanceUID", parser.GetInstanceUID());
				variableBuilder.SetVariable("CounterValues", (int)parser.GetCounterValues().GetItemCount(), (const uint64_t*)parser.GetCounterValues().data());


			Proc_End:

				return hr;

			}; // Result RequestCounterValuesRes::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result RequestCounterValuesRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RequestCounterValuesRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RequestCounterValuesRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* RequestCounterValuesRes::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(uint64_t)
					+ sizeof(uint64_t)*InCounterValues.GetItemCount() + sizeof(uint16_t));


				uint16_t numberOfInCounterValues = (uint16_t)InCounterValues.GetItemCount(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, Monitoring::RequestCounterValuesRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &numberOfInCounterValues, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InCounterValues.data(), (INT)(sizeof(uint64_t)*InCounterValues.GetItemCount())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RequestCounterValuesRes::Create( IMemoryManager& memoryManager, const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )



			Result RequestCounterValuesRes::TraceOut(MessageData* pMsg)
			{
 				RequestCounterValuesRes parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "RequestCounterValues:{0}:{1} , TransactionID:{2}, Result:{3:X8}, InstanceUID:{4}, CounterValues:{5,30}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetTransactionID(), parser.GetResult(), parser.GetInstanceUID(), parser.GetCounterValues()); 
				return ResultCode::SUCCESS;
			}; // Result RequestCounterValuesRes::TraceOut(MessageData* pMsg)

			// C2S: Counter instance is created
			const MessageID PerformanceCounterNewC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 2);
			Result PerformanceCounterNewC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfInstanceName = 0;
				uint16_t numberofNewCounters = 0; PerformanceCounterInfo* pNewCounters = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
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

			Result PerformanceCounterNewC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				PerformanceCounterNewC2SEvt parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("InstanceName", parser.GetInstanceName());
				variableBuilder.SetVariable("InstanceUID", parser.GetInstanceUID());
				variableBuilder.SetVariable("NewCounters", (int)parser.GetNewCounters().GetItemCount(), (const PerformanceCounterInfo*)parser.GetNewCounters().data());


			Proc_End:

				return hr;

			}; // Result PerformanceCounterNewC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result PerformanceCounterNewC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) PerformanceCounterNewC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PerformanceCounterNewC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* PerformanceCounterNewC2SEvt::Create( IMemoryManager& memoryManager, const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInInstanceNameLength = InInstanceName ? (uint16_t)(strlen(InInstanceName)+1) : 1;
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) +  + sizeof(uint16_t) + __uiInInstanceNameLength 
					+ sizeof(uint64_t)
					+ sizeof(PerformanceCounterInfo)*InNewCounters.GetItemCount() + sizeof(uint16_t));


				uint16_t numberOfInNewCounters = (uint16_t)InNewCounters.GetItemCount(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, Monitoring::PerformanceCounterNewC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInInstanceNameLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InInstanceName ? InInstanceName : "", __uiInInstanceNameLength );
				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &numberOfInNewCounters, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InNewCounters.data(), (INT)(sizeof(PerformanceCounterInfo)*InNewCounters.GetItemCount())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PerformanceCounterNewC2SEvt::Create( IMemoryManager& memoryManager, const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )



			Result PerformanceCounterNewC2SEvt::TraceOut(MessageData* pMsg)
			{
 				PerformanceCounterNewC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "PerformanceCounterNew:{0}:{1} , InstanceName:{2,60}, InstanceUID:{3}, NewCounters:{4,30}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetInstanceName(), parser.GetInstanceUID(), parser.GetNewCounters()); 
				return ResultCode::SUCCESS;
			}; // Result PerformanceCounterNewC2SEvt::TraceOut(MessageData* pMsg)

			// C2S: Counter instance is deleted
			const MessageID PerformanceCounterFreeC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 3);
			Result PerformanceCounterFreeC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofFreeInstances = 0; uint64_t* pFreeInstances = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &numberofFreeInstances, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pFreeInstances, pCur, iMsgSize, sizeof(uint64_t)*numberofFreeInstances ) );
				m_FreeInstances.SetLinkedBuffer(numberofFreeInstances, numberofFreeInstances, pFreeInstances);


			Proc_End:

				return hr;

			}; // Result PerformanceCounterFreeC2SEvt::ParseMessage( MessageData* pIMsg )

			Result PerformanceCounterFreeC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				PerformanceCounterFreeC2SEvt parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("FreeInstances", (int)parser.GetFreeInstances().GetItemCount(), (const uint64_t*)parser.GetFreeInstances().data());


			Proc_End:

				return hr;

			}; // Result PerformanceCounterFreeC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result PerformanceCounterFreeC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) PerformanceCounterFreeC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PerformanceCounterFreeC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* PerformanceCounterFreeC2SEvt::Create( IMemoryManager& memoryManager, const Array<uint64_t>& InFreeInstances )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(uint64_t)*InFreeInstances.GetItemCount() + sizeof(uint16_t));


				uint16_t numberOfInFreeInstances = (uint16_t)InFreeInstances.GetItemCount(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, Monitoring::PerformanceCounterFreeC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &numberOfInFreeInstances, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InFreeInstances.data(), (INT)(sizeof(uint64_t)*InFreeInstances.GetItemCount())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PerformanceCounterFreeC2SEvt::Create( IMemoryManager& memoryManager, const Array<uint64_t>& InFreeInstances )



			Result PerformanceCounterFreeC2SEvt::TraceOut(MessageData* pMsg)
			{
 				PerformanceCounterFreeC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "PerformanceCounterFree:{0}:{1} , FreeInstances:{2,30}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetFreeInstances()); 
				return ResultCode::SUCCESS;
			}; // Result PerformanceCounterFreeC2SEvt::TraceOut(MessageData* pMsg)

			// C2S: Counter update broadcast
			const MessageID PerformanceCounterUpdateC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 4);
			Result PerformanceCounterUpdateC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofCounterValues = 0; uint64_t* pCounterValues = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofCounterValues, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pCounterValues, pCur, iMsgSize, sizeof(uint64_t)*numberofCounterValues ) );
				m_CounterValues.SetLinkedBuffer(numberofCounterValues, numberofCounterValues, pCounterValues);


			Proc_End:

				return hr;

			}; // Result PerformanceCounterUpdateC2SEvt::ParseMessage( MessageData* pIMsg )

			Result PerformanceCounterUpdateC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				PerformanceCounterUpdateC2SEvt parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("InstanceUID", parser.GetInstanceUID());
				variableBuilder.SetVariable("CounterValues", (int)parser.GetCounterValues().GetItemCount(), (const uint64_t*)parser.GetCounterValues().data());


			Proc_End:

				return hr;

			}; // Result PerformanceCounterUpdateC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result PerformanceCounterUpdateC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) PerformanceCounterUpdateC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PerformanceCounterUpdateC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* PerformanceCounterUpdateC2SEvt::Create( IMemoryManager& memoryManager, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(uint64_t)
					+ sizeof(uint64_t)*InCounterValues.GetItemCount() + sizeof(uint16_t));


				uint16_t numberOfInCounterValues = (uint16_t)InCounterValues.GetItemCount(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, Monitoring::PerformanceCounterUpdateC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &numberOfInCounterValues, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InCounterValues.data(), (INT)(sizeof(uint64_t)*InCounterValues.GetItemCount())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PerformanceCounterUpdateC2SEvt::Create( IMemoryManager& memoryManager, const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues )



			Result PerformanceCounterUpdateC2SEvt::TraceOut(MessageData* pMsg)
			{
 				PerformanceCounterUpdateC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "PerformanceCounterUpdate:{0}:{1} , InstanceUID:{2}, CounterValues:{3,30}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetInstanceUID(), parser.GetCounterValues()); 
				return ResultCode::SUCCESS;
			}; // Result PerformanceCounterUpdateC2SEvt::TraceOut(MessageData* pMsg)

			// S2C: Request from server
			const MessageID PerformanceCounterUpdateCounterInfoS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_MONITORING, 5);
			Result PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessage( MessageData* pIMsg )

			Result PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				PerformanceCounterUpdateCounterInfoS2CEvt parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("InstanceUID", parser.GetInstanceUID());


			Proc_End:

				return hr;

			}; // Result PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) PerformanceCounterUpdateCounterInfoS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* PerformanceCounterUpdateCounterInfoS2CEvt::Create( IMemoryManager& memoryManager, const uint64_t &InInstanceUID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(uint64_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(uint64_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PerformanceCounterUpdateCounterInfoS2CEvt::Create( IMemoryManager& memoryManager, const uint64_t &InInstanceUID )



			Result PerformanceCounterUpdateCounterInfoS2CEvt::TraceOut(MessageData* pMsg)
			{
 				PerformanceCounterUpdateCounterInfoS2CEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug2, "PerformanceCounterUpdateCounterInfo:{0}:{1} , InstanceUID:{2}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetInstanceUID()); 
				return ResultCode::SUCCESS;
			}; // Result PerformanceCounterUpdateCounterInfoS2CEvt::TraceOut(MessageData* pMsg)



		}; // namespace Monitoring
	}; // namespace Message
}; // namespace SF


