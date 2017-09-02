////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : Generated
// 
// Description : Monitoring Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/Message/MonitoringMsgParsing.h"
#include "Protocol/Message/MonitoringMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// Monitoring message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageTo> MessageParseToVariableMap;
		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserMonitoring()
		{
 			// Cmd: Add a player to ranking
			MessageParseToVariableMap.insert(std::make_pair(Message::Monitoring::GetInstanceListCmd::MID.IDSeq.MsgID,&Message::Monitoring::GetInstanceListCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Monitoring::GetInstanceListCmd::MID.IDSeq.MsgID,&Message::Monitoring::GetInstanceListCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::Monitoring::GetInstanceListRes::MID.IDSeq.MsgID,&Message::Monitoring::GetInstanceListRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Monitoring::GetInstanceListRes::MID.IDSeq.MsgID,&Message::Monitoring::GetInstanceListRes::ParseMessageToMessageBase));
			// Cmd: Remove a player to ranking
			MessageParseToVariableMap.insert(std::make_pair(Message::Monitoring::RequestCounterValuesCmd::MID.IDSeq.MsgID,&Message::Monitoring::RequestCounterValuesCmd::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Monitoring::RequestCounterValuesCmd::MID.IDSeq.MsgID,&Message::Monitoring::RequestCounterValuesCmd::ParseMessageToMessageBase));
			MessageParseToVariableMap.insert(std::make_pair(Message::Monitoring::RequestCounterValuesRes::MID.IDSeq.MsgID,&Message::Monitoring::RequestCounterValuesRes::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Monitoring::RequestCounterValuesRes::MID.IDSeq.MsgID,&Message::Monitoring::RequestCounterValuesRes::ParseMessageToMessageBase));
			// C2S: Counter instance is created
			MessageParseToVariableMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterNewC2SEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterNewC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterNewC2SEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterNewC2SEvt::ParseMessageToMessageBase));
			// C2S: Counter instance is deleted
			MessageParseToVariableMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterFreeC2SEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterFreeC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterFreeC2SEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterFreeC2SEvt::ParseMessageToMessageBase));
			// C2S: Counter update broadcast
			MessageParseToVariableMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterUpdateC2SEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterUpdateC2SEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterUpdateC2SEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterUpdateC2SEvt::ParseMessageToMessageBase));
			// S2C: Request from server
			MessageParseToVariableMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessageTo));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::MID.IDSeq.MsgID,&Message::Monitoring::PerformanceCounterUpdateCounterInfoS2CEvt::ParseMessageToMessageBase));
		}; // void RegisterParserMonitoring()


	}; // namespace Protocol
}; // namespace SF


