////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : Monitoring Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/MonitoringMsgDebug.h"
#include "Protocol/MonitoringMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// Monitoring Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceMonitoring()
		{
 			// Cmd: [deprecated]
			MessageDebugTraceMap.insert(std::make_pair(Message::Monitoring::GetInstanceListCmd::MID.IDSeq.MsgID,&Message::Monitoring::GetInstanceListCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::Monitoring::GetInstanceListRes::MID.IDSeq.MsgID,&Message::Monitoring::GetInstanceListRes::TraceOut));
			// Cmd: [deprecated]
			MessageDebugTraceMap.insert(std::make_pair(Message::Monitoring::RequestCounterValuesCmd::MID.IDSeq.MsgID,&Message::Monitoring::RequestCounterValuesCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::Monitoring::RequestCounterValuesRes::MID.IDSeq.MsgID,&Message::Monitoring::RequestCounterValuesRes::TraceOut));
			// C2S: Message when new performance counter is added.
			MessageDebugTraceMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterNewC2SEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterNewC2SEvt::TraceOut));
			// C2S: Counter instance is deleted
			MessageDebugTraceMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterFreeC2SEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterFreeC2SEvt::TraceOut));
			// C2S: Counter update broadcast
			MessageDebugTraceMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterUpdateC2SEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterUpdateC2SEvt::TraceOut));
			// S2C: Server will send this message to an instance to get performance counters.
			MessageDebugTraceMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::TraceOut));
		}; // void RegisterDebugTraceMonitoring()


	}; // namespace Protocol
}; // namespace SF


