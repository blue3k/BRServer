////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : Monitoring Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/MonitoringMsgDebug.h"
#include "Protocol/Message/MonitoringMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 		///////////////////////////////////////////////////////////////
		// Monitoring Debug trace mapping
		extern std::unordered_map<uint32_t,MessageHandlingFunction> MessageDebugTraceMap;

		void RegisterDebugTraceMonitoring()
		{
 			// Cmd: Add a player to ranking
			MessageDebugTraceMap.insert(std::make_pair(Message::Monitoring::GetInstanceListCmd::MID.IDSeq.MsgID,&Message::Monitoring::GetInstanceListCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::Monitoring::GetInstanceListRes::MID.IDSeq.MsgID,&Message::Monitoring::GetInstanceListRes::TraceOut));
			// Cmd: Remove a player to ranking
			MessageDebugTraceMap.insert(std::make_pair(Message::Monitoring::RequestCounterValuesCmd::MID.IDSeq.MsgID,&Message::Monitoring::RequestCounterValuesCmd::TraceOut));
			MessageDebugTraceMap.insert(std::make_pair(Message::Monitoring::RequestCounterValuesRes::MID.IDSeq.MsgID,&Message::Monitoring::RequestCounterValuesRes::TraceOut));
			// C2S: Counter instance is created
			MessageDebugTraceMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterNewC2SEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterNewC2SEvt::TraceOut));
			// C2S: Counter instance is deleted
			MessageDebugTraceMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterFreeC2SEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterFreeC2SEvt::TraceOut));
			// C2S: Counter update broadcast
			MessageDebugTraceMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterUpdateC2SEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterUpdateC2SEvt::TraceOut));
			// S2C: Request from server
			MessageDebugTraceMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::TraceOut));
		}; // void RegisterDebugTraceMonitoring()


	}; // namespace Protocol
}; // namespace SF


