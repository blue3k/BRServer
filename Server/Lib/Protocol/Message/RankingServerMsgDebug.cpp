////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : RankingServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/Message/RankingServerMsgDebug.h"
#include "Protocol/Message/RankingServerMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// RankingServer Debug trace mappping
			static std::unordered_map<UINT,std::function<Result(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapRankingServer;

			void RegisterDebugTraceRankingServer()
			{
 				// Cmd: Add a player to ranking
				MessageDebugTraceMapRankingServer.insert(std::make_pair(RankingServer::AddPlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   RankingServer::AddPlayerCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapRankingServer.insert(std::make_pair(RankingServer::AddPlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   RankingServer::AddPlayerRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Remove a player to ranking
				MessageDebugTraceMapRankingServer.insert(std::make_pair(RankingServer::RemovePlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   RankingServer::RemovePlayerCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapRankingServer.insert(std::make_pair(RankingServer::RemovePlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   RankingServer::RemovePlayerRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Remove a player to ranking
				MessageDebugTraceMapRankingServer.insert(std::make_pair(RankingServer::GetPlayerRankingCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   RankingServer::GetPlayerRankingCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapRankingServer.insert(std::make_pair(RankingServer::GetPlayerRankingRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   RankingServer::GetPlayerRankingRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Update a player to ranking
				MessageDebugTraceMapRankingServer.insert(std::make_pair(RankingServer::UpdatePlayerScoreCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   RankingServer::UpdatePlayerScoreCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapRankingServer.insert(std::make_pair(RankingServer::UpdatePlayerScoreRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   RankingServer::UpdatePlayerScoreRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Get ranking list
				MessageDebugTraceMapRankingServer.insert(std::make_pair(RankingServer::GetRankingCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   RankingServer::GetRankingCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapRankingServer.insert(std::make_pair(RankingServer::GetRankingRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   RankingServer::GetRankingRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Debug test
				MessageDebugTraceMapRankingServer.insert(std::make_pair(RankingServer::DebugPrintALLRankingCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   RankingServer::DebugPrintALLRankingCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapRankingServer.insert(std::make_pair(RankingServer::DebugPrintALLRankingRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   RankingServer::DebugPrintALLRankingRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
			}; // void RegisterDebugTraceRankingServer()


			///////////////////////////////////////////////////////////////
			// RankingServer Debug trace
			Result DebugOutRankingServer( const char *Prefix, MessageData *pMsg )
			{
 
				Result hr;
				auto itFount = MessageDebugTraceMapRankingServer.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// RankingServer Debug trace
				itFount = MessageDebugTraceMapRankingServer.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapRankingServer.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // Result DebugOutRankingServer( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


