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
#include "Common/ToStringSvr.h"
#include "Common/ToStringGame.h"
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
			HRESULT GetInstanceListCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );


			Proc_End:

				return hr;

			}; // HRESULT GetInstanceListCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GetInstanceListCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Monitoring::GetInstanceListCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetInstanceListCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext )



			VOID GetInstanceListCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GetInstanceListCmd:%1%:%2% , Context:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context); 
			}; // VOID GetInstanceListCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetInstanceListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_MONITORING, 0);
			HRESULT GetInstanceListRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				UINT16 numberofCounterInstances = 0; PerformanceCounterInstanceInfo* pCounterInstances = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofCounterInstances, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pCounterInstances, pCur, iMsgSize, sizeof(PerformanceCounterInstanceInfo)*numberofCounterInstances ) );
				m_CounterInstances.SetLinkedBuffer(numberofCounterInstances, numberofCounterInstances, pCounterInstances);
				protocolChk( Protocol::StreamParamCopy( &m_TotalInstanceCount, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT GetInstanceListRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GetInstanceListRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const UINT32 &InTotalInstanceCount )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(PerformanceCounterInstanceInfo)*InCounterInstances.GetSize() + sizeof(UINT16)
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Monitoring::GetInstanceListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				UINT16 numberOfInCounterInstances = (UINT16)InCounterInstances.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInCounterInstances, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InCounterInstances.data(), (INT)(sizeof(PerformanceCounterInstanceInfo)*InCounterInstances.GetSize())); 
				Protocol::PackParamCopy( pMsgData, &InTotalInstanceCount, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GetInstanceListRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const Array<PerformanceCounterInstanceInfo>& InCounterInstances, const UINT32 &InTotalInstanceCount )



			VOID GetInstanceListRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GetInstanceListRes:%1%:%2% , Context:%3%, Result:{4}, CounterInstances:%5%, TotalInstanceCount:%6%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_Result, m_CounterInstances, m_TotalInstanceCount); 
			}; // VOID GetInstanceListRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Remove a player to ranking
			const MessageID RequestCounterValuesCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_MONITORING, 1);
			HRESULT RequestCounterValuesCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // HRESULT RequestCounterValuesCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT RequestCounterValuesCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const EntityUID &InInstanceUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(EntityUID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Monitoring::RequestCounterValuesCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RequestCounterValuesCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const EntityUID &InInstanceUID )



			VOID RequestCounterValuesCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:RequestCounterValuesCmd:%1%:%2% , Context:%3%, InstanceUID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_InstanceUID); 
			}; // VOID RequestCounterValuesCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RequestCounterValuesRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_MONITORING, 1);
			HRESULT RequestCounterValuesRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, sizeof(EntityUID) ) );
				UINT16 numberofCounterValues = 0; UINT64* pCounterValues = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofCounterValues, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pCounterValues, pCur, iMsgSize, sizeof(UINT64)*numberofCounterValues ) );
				m_CounterValues.SetLinkedBuffer(numberofCounterValues, numberofCounterValues, pCounterValues);


			Proc_End:

				return hr;

			}; // HRESULT RequestCounterValuesRes::ParseIMsg( MessageData* pIMsg )

			HRESULT RequestCounterValuesRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const EntityUID &InInstanceUID, const Array<UINT64>& InCounterValues )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(EntityUID)
					+ sizeof(UINT64)*InCounterValues.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Monitoring::RequestCounterValuesRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(EntityUID));
				UINT16 numberOfInCounterValues = (UINT16)InCounterValues.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInCounterValues, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InCounterValues.data(), (INT)(sizeof(UINT64)*InCounterValues.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RequestCounterValuesRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const EntityUID &InInstanceUID, const Array<UINT64>& InCounterValues )



			VOID RequestCounterValuesRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:RequestCounterValuesRes:%1%:%2% , Context:%3%, Result:{4}, InstanceUID:%5%, CounterValues:%6%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_Result, m_InstanceUID, m_CounterValues); 
			}; // VOID RequestCounterValuesRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Counter instance is created
			const MessageID PerformanceCounterNewC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_MONITORING, 2);
			HRESULT PerformanceCounterNewC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				UINT16 uiSizeOfInstanceName = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfInstanceName, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_InstanceName, pCur, iMsgSize, sizeof(char)*uiSizeOfInstanceName ) );
				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, sizeof(EntityUID) ) );
				UINT16 numberofNewCounters = 0; PerformanceCounterInfo* pNewCounters = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofNewCounters, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pNewCounters, pCur, iMsgSize, sizeof(PerformanceCounterInfo)*numberofNewCounters ) );
				m_NewCounters.SetLinkedBuffer(numberofNewCounters, numberofNewCounters, pNewCounters);


			Proc_End:

				return hr;

			}; // HRESULT PerformanceCounterNewC2SEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PerformanceCounterNewC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInInstanceNameLength = InInstanceName ? (UINT16)(strlen(InInstanceName)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) +  + sizeof(UINT16) + __uiInInstanceNameLength 
					+ sizeof(EntityUID)
					+ sizeof(PerformanceCounterInfo)*InNewCounters.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Monitoring::PerformanceCounterNewC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInInstanceNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InInstanceName ? InInstanceName : "", __uiInInstanceNameLength );
				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(EntityUID));
				UINT16 numberOfInNewCounters = (UINT16)InNewCounters.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInNewCounters, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InNewCounters.data(), (INT)(sizeof(PerformanceCounterInfo)*InNewCounters.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PerformanceCounterNewC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const char* InInstanceName, const EntityUID &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters )



			VOID PerformanceCounterNewC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG2, "%0%:PerformanceCounterNewC2SEvt:%1%:%2% , InstanceName:%3%, InstanceUID:%4%, NewCounters:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InstanceName, m_InstanceUID, m_NewCounters); 
			}; // VOID PerformanceCounterNewC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Counter instance is deleted
			const MessageID PerformanceCounterFreeC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_MONITORING, 3);
			HRESULT PerformanceCounterFreeC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				UINT16 numberofFreeInstances = 0; EntityUID* pFreeInstances = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofFreeInstances, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pFreeInstances, pCur, iMsgSize, sizeof(EntityUID)*numberofFreeInstances ) );
				m_FreeInstances.SetLinkedBuffer(numberofFreeInstances, numberofFreeInstances, pFreeInstances);


			Proc_End:

				return hr;

			}; // HRESULT PerformanceCounterFreeC2SEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PerformanceCounterFreeC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const Array<EntityUID>& InFreeInstances )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(EntityUID)*InFreeInstances.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Monitoring::PerformanceCounterFreeC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				UINT16 numberOfInFreeInstances = (UINT16)InFreeInstances.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInFreeInstances, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InFreeInstances.data(), (INT)(sizeof(EntityUID)*InFreeInstances.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PerformanceCounterFreeC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const Array<EntityUID>& InFreeInstances )



			VOID PerformanceCounterFreeC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG2, "%0%:PerformanceCounterFreeC2SEvt:%1%:%2% , FreeInstances:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_FreeInstances); 
			}; // VOID PerformanceCounterFreeC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Counter update broadcast
			const MessageID PerformanceCounterUpdateC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_MONITORING, 4);
			HRESULT PerformanceCounterUpdateC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, sizeof(EntityUID) ) );
				UINT16 numberofCounterValues = 0; UINT64* pCounterValues = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofCounterValues, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pCounterValues, pCur, iMsgSize, sizeof(UINT64)*numberofCounterValues ) );
				m_CounterValues.SetLinkedBuffer(numberofCounterValues, numberofCounterValues, pCounterValues);


			Proc_End:

				return hr;

			}; // HRESULT PerformanceCounterUpdateC2SEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PerformanceCounterUpdateC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const EntityUID &InInstanceUID, const Array<UINT64>& InCounterValues )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(EntityUID)
					+ sizeof(UINT64)*InCounterValues.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Monitoring::PerformanceCounterUpdateC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(EntityUID));
				UINT16 numberOfInCounterValues = (UINT16)InCounterValues.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInCounterValues, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InCounterValues.data(), (INT)(sizeof(UINT64)*InCounterValues.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PerformanceCounterUpdateC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const EntityUID &InInstanceUID, const Array<UINT64>& InCounterValues )



			VOID PerformanceCounterUpdateC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG2, "%0%:PerformanceCounterUpdateC2SEvt:%1%:%2% , InstanceUID:%3%, CounterValues:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InstanceUID, m_CounterValues); 
			}; // VOID PerformanceCounterUpdateC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Request from server
			const MessageID PerformanceCounterUpdateCounterInfoS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_MONITORING, 5);
			HRESULT PerformanceCounterUpdateCounterInfoS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InstanceUID, pCur, iMsgSize, sizeof(EntityUID) ) );


			Proc_End:

				return hr;

			}; // HRESULT PerformanceCounterUpdateCounterInfoS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PerformanceCounterUpdateCounterInfoS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const EntityUID &InInstanceUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(EntityUID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInstanceUID, sizeof(EntityUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PerformanceCounterUpdateCounterInfoS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const EntityUID &InInstanceUID )



			VOID PerformanceCounterUpdateCounterInfoS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG2, "%0%:PerformanceCounterUpdateCounterInfoS2CEvt:%1%:%2% , InstanceUID:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InstanceUID); 
			}; // VOID PerformanceCounterUpdateCounterInfoS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace Monitoring
	}; // namespace Message
}; // namespace BR


