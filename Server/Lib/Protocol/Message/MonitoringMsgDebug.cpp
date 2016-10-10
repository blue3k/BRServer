////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Monitoring Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/Message/MonitoringMsgDebug.h"
#include "Protocol/Message/MonitoringMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// Monitoring Debug trace mappping
			static std::unordered_map<UINT,std::function<Result(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapMonitoring;

			void RegisterDebugTraceMonitoring()
			{
 				// Cmd: Add a player to ranking
				MessageDebugTraceMapMonitoring.insert(std::make_pair(Monitoring::GetInstanceListCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Monitoring::GetInstanceListCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapMonitoring.insert(std::make_pair(Monitoring::GetInstanceListRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Monitoring::GetInstanceListRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Remove a player to ranking
				MessageDebugTraceMapMonitoring.insert(std::make_pair(Monitoring::RequestCounterValuesCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Monitoring::RequestCounterValuesCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapMonitoring.insert(std::make_pair(Monitoring::RequestCounterValuesRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Monitoring::RequestCounterValuesRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Counter instance is created
				MessageDebugTraceMapMonitoring.insert(std::make_pair(Monitoring::PerformanceCounterNewC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Monitoring::PerformanceCounterNewC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Counter instance is deleted
				MessageDebugTraceMapMonitoring.insert(std::make_pair(Monitoring::PerformanceCounterFreeC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Monitoring::PerformanceCounterFreeC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Counter update broadcast
				MessageDebugTraceMapMonitoring.insert(std::make_pair(Monitoring::PerformanceCounterUpdateC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Monitoring::PerformanceCounterUpdateC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Request from server
				MessageDebugTraceMapMonitoring.insert(std::make_pair(Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
			}; // void RegisterDebugTraceMonitoring()


			///////////////////////////////////////////////////////////////
			// Monitoring Debug trace
			Result DebugOutMonitoring( const char *Prefix, MessageData *pMsg )
			{
 
				Result hr;
				auto itFount = MessageDebugTraceMapMonitoring.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// Monitoring Debug trace
				itFount = MessageDebugTraceMapMonitoring.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapMonitoring.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // Result DebugOutMonitoring( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


