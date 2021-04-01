////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 Kyungkun Ko
// 
// Author : Generated
// 
// Description : Monitoring Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/MonitoringMsgParsing.h"
#include "Protocol/MonitoringMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// Monitoring message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserMonitoring()
		{
 			// Cmd: [deprecated]
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Monitoring::GetInstanceListCmd::MID.IDSeq.MsgID,&Message::Monitoring::GetInstanceListCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Monitoring::GetInstanceListRes::MID.IDSeq.MsgID,&Message::Monitoring::GetInstanceListRes::ParseMessageToMessageBase));
			// Cmd: [deprecated]
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Monitoring::RequestCounterValuesCmd::MID.IDSeq.MsgID,&Message::Monitoring::RequestCounterValuesCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Monitoring::RequestCounterValuesRes::MID.IDSeq.MsgID,&Message::Monitoring::RequestCounterValuesRes::ParseMessageToMessageBase));
			// C2S: Message when new performance counter is added.
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterNewC2SEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterNewC2SEvt::ParseMessageToMessageBase));
			// C2S: Counter instance is deleted
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterFreeC2SEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterFreeC2SEvt::ParseMessageToMessageBase));
			// C2S: Counter update broadcast
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterUpdateC2SEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterUpdateC2SEvt::ParseMessageToMessageBase));
			// S2C: Server will send this message to an instance to get performance counters.
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessageToMessageBase));
		}; // void RegisterParserMonitoring()


	}; // namespace Protocol
}; // namespace SF


