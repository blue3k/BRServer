////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Monitoring Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/PolicyID.h"
#include "String/ToStringSvr.h"
#include "String/ToStringGame.h"
#include "Common/ArrayUtil.h"
#include "Protocol/ProtocolMessageCPPInc.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/ProtocolHelper.h"
#include "Protocol/Message/MonitoringMsgClass.h"
#include "Protocol/ProtocolTrace.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Message
	{
 		namespace Monitoring
		{
 			// Cmd: Add a player to ranking
			const MessageID GetInstanceListCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_MONITORING, 0);
			Result GetInstanceListCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );


			Proc_End:

				return hr;

			}; // Result GetInstanceListCmd::ParseMessage( MessageData* pIMsg )

			Result GetInstanceListCmd::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(TransactionID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Monitoring::GetInstanceListCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetInstanceListCmd::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID )



			void GetInstanceListCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetInstanceListCmd:{1}:{2} , TransactionID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TransactionID); 
			}; // void GetInstanceListCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetInstanceListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_MONITORING, 0);
			Result GetInstanceListRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofCounterInstances = 0; PerformanceCounterInstanceInfo* pCounterInstances = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofCounterInstances, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pCounterInstances, pCur, iMsgSize, (int)sizeof(PerformanceCounterInstanceInfo)*numberofCounterInstances ) );
				m_CounterInstances.SetLinkedBuffer(numberofCounterInstances, numberofCounterInstances, pCounterInstances);
				protocolChk( Protocol::StreamParamCopy( &m_TotalInstanceCount, pCur, iMsgSize, (int)sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result GetInstanceListRes::ParseMessage( MessageData* pIMsg )

			Result GetInstanceListRes::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(PerformanceCounterInstanceInfo)*InCounterInstances.GetSize() + sizeof(UINT16)
					+ sizeof(uint32_t));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInCounterInstances = (UINT16)InCounterInstances.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Monitoring::GetInstanceListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInCounterInstances, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InCounterInstances.data(), (INT)(sizeof(PerformanceCounterInstanceInfo)*InCounterInstances.GetSize())); 
				Protocol::PackParamCopy( pMsgData, &InTotalInstanceCount, sizeof(uint32_t));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetInstanceListRes::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const Result &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const uint32_t &InTotalInstanceCount )



			void GetInstanceListRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetInstanceListRes:{1}:{2} , TransactionID:{3}, Result:{4:X8}, CounterInstances:{5}, TotalInstanceCount:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TransactionID, m_Result, m_CounterInstances, m_TotalInstanceCount); 
			}; // void GetInstanceListRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Remove a player to ranking
			const MessageID RequestCounterValuesCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_MONITORING, 1);
			Result RequestCounterValuesCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, (int)sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // Result RequestCounterValuesCmd::ParseMessage( MessageData* pIMsg )

			Result RequestCounterValuesCmd::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const EntityUID &InInstanceUID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(EntityUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Monitoring::RequestCounterValuesCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RequestCounterValuesCmd::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const EntityUID &InInstanceUID )



			void RequestCounterValuesCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RequestCounterValuesCmd:{1}:{2} , TransactionID:{3}, InstanceUID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TransactionID, m_InstanceUID); 
			}; // void RequestCounterValuesCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RequestCounterValuesRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_MONITORING, 1);
			Result RequestCounterValuesRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofCounterValues = 0; uint64_t* pCounterValues = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, (int)sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, (int)sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, (int)sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofCounterValues, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pCounterValues, pCur, iMsgSize, (int)sizeof(uint64_t)*numberofCounterValues ) );
				m_CounterValues.SetLinkedBuffer(numberofCounterValues, numberofCounterValues, pCounterValues);


			Proc_End:

				return hr;

			}; // Result RequestCounterValuesRes::ParseMessage( MessageData* pIMsg )

			Result RequestCounterValuesRes::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(EntityUID)
					+ sizeof(uint64_t)*InCounterValues.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInCounterValues = (UINT16)InCounterValues.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Monitoring::RequestCounterValuesRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &numberOfInCounterValues, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InCounterValues.data(), (INT)(sizeof(uint64_t)*InCounterValues.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RequestCounterValuesRes::BuildIMsg( OUT MessageData* &pMsg, const TransactionID &InTransactionID, const Result &InResult, const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues )



			void RequestCounterValuesRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RequestCounterValuesRes:{1}:{2} , TransactionID:{3}, Result:{4:X8}, InstanceUID:{5}, CounterValues:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TransactionID, m_Result, m_InstanceUID, m_CounterValues); 
			}; // void RequestCounterValuesRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Counter instance is created
			const MessageID PerformanceCounterNewC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_MONITORING, 2);
			Result PerformanceCounterNewC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfInstanceName = 0;
				UINT16 numberofNewCounters = 0; PerformanceCounterInfo* pNewCounters = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &uiSizeOfInstanceName, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_InstanceName, pCur, iMsgSize, (int)sizeof(char)*uiSizeOfInstanceName ) );
				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, (int)sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofNewCounters, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pNewCounters, pCur, iMsgSize, (int)sizeof(PerformanceCounterInfo)*numberofNewCounters ) );
				m_NewCounters.SetLinkedBuffer(numberofNewCounters, numberofNewCounters, pNewCounters);


			Proc_End:

				return hr;

			}; // Result PerformanceCounterNewC2SEvt::ParseMessage( MessageData* pIMsg )

			Result PerformanceCounterNewC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInInstanceNameLength = InInstanceName ? (UINT16)(strlen(InInstanceName)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) +  + sizeof(UINT16) + __uiInInstanceNameLength 
					+ sizeof(EntityUID)
					+ sizeof(PerformanceCounterInfo)*InNewCounters.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInNewCounters = (UINT16)InNewCounters.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Monitoring::PerformanceCounterNewC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInInstanceNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InInstanceName ? InInstanceName : "", __uiInInstanceNameLength );
				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &numberOfInNewCounters, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InNewCounters.data(), (INT)(sizeof(PerformanceCounterInfo)*InNewCounters.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PerformanceCounterNewC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )



			void PerformanceCounterNewC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:PerformanceCounterNewC2SEvt:{1}:{2} , InstanceName:{3,60}, InstanceUID:{4}, NewCounters:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InstanceName, m_InstanceUID, m_NewCounters); 
			}; // void PerformanceCounterNewC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Counter instance is deleted
			const MessageID PerformanceCounterFreeC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_MONITORING, 3);
			Result PerformanceCounterFreeC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofFreeInstances = 0; EntityUID* pFreeInstances = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &numberofFreeInstances, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pFreeInstances, pCur, iMsgSize, (int)sizeof(EntityUID)*numberofFreeInstances ) );
				m_FreeInstances.SetLinkedBuffer(numberofFreeInstances, numberofFreeInstances, pFreeInstances);


			Proc_End:

				return hr;

			}; // Result PerformanceCounterFreeC2SEvt::ParseMessage( MessageData* pIMsg )

			Result PerformanceCounterFreeC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const Array<EntityUID>& InFreeInstances )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(EntityUID)*InFreeInstances.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInFreeInstances = (UINT16)InFreeInstances.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Monitoring::PerformanceCounterFreeC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &numberOfInFreeInstances, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InFreeInstances.data(), (INT)(sizeof(EntityUID)*InFreeInstances.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PerformanceCounterFreeC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const Array<EntityUID>& InFreeInstances )



			void PerformanceCounterFreeC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:PerformanceCounterFreeC2SEvt:{1}:{2} , FreeInstances:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_FreeInstances); 
			}; // void PerformanceCounterFreeC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Counter update broadcast
			const MessageID PerformanceCounterUpdateC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_MONITORING, 4);
			Result PerformanceCounterUpdateC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofCounterValues = 0; uint64_t* pCounterValues = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, (int)sizeof(EntityUID) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofCounterValues, pCur, iMsgSize, (int)sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pCounterValues, pCur, iMsgSize, (int)sizeof(uint64_t)*numberofCounterValues ) );
				m_CounterValues.SetLinkedBuffer(numberofCounterValues, numberofCounterValues, pCounterValues);


			Proc_End:

				return hr;

			}; // Result PerformanceCounterUpdateC2SEvt::ParseMessage( MessageData* pIMsg )

			Result PerformanceCounterUpdateC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(EntityUID)
					+ sizeof(uint64_t)*InCounterValues.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInCounterValues = (UINT16)InCounterValues.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Monitoring::PerformanceCounterUpdateC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(EntityUID));
				Protocol::PackParamCopy( pMsgData, &numberOfInCounterValues, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InCounterValues.data(), (INT)(sizeof(uint64_t)*InCounterValues.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PerformanceCounterUpdateC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const EntityUID &InInstanceUID, const Array<uint64_t>& InCounterValues )



			void PerformanceCounterUpdateC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:PerformanceCounterUpdateC2SEvt:{1}:{2} , InstanceUID:{3}, CounterValues:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InstanceUID, m_CounterValues); 
			}; // void PerformanceCounterUpdateC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Request from server
			const MessageID PerformanceCounterUpdateCounterInfoS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_MONITORING, 5);
			Result PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (INT)pIMsg->GetMessageSize() - (INT)sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, (int)sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // Result PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessage( MessageData* pIMsg )

			Result PerformanceCounterUpdateCounterInfoS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const EntityUID &InInstanceUID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(EntityUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PerformanceCounterUpdateCounterInfoS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const EntityUID &InInstanceUID )



			void PerformanceCounterUpdateCounterInfoS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG2, "{0}:PerformanceCounterUpdateCounterInfoS2CEvt:{1}:{2} , InstanceUID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InstanceUID); 
			}; // void PerformanceCounterUpdateCounterInfoS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace Monitoring
	}; // namespace Message
}; // namespace BR


