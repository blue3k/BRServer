////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Ranking Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/Message/RankingMsgDebug.h"
#include "Protocol/Message/RankingMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// Ranking Debug trace mappping
			static std::unordered_map<UINT,std::function<Result(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapRanking;

			void RegisterDebugTraceRanking()
			{
 				// Cmd: Add a player to ranking
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::AddPlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Ranking::AddPlayerCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::AddPlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Ranking::AddPlayerRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Remove a player to ranking
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::RemovePlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Ranking::RemovePlayerCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::RemovePlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Ranking::RemovePlayerRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Remove a player to ranking
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::GetPlayerRankingCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Ranking::GetPlayerRankingCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::GetPlayerRankingRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Ranking::GetPlayerRankingRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Update a player to ranking
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::UpdatePlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Ranking::UpdatePlayerCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::UpdatePlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Ranking::UpdatePlayerRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Remove a player to ranking
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::GetRankingCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Ranking::GetRankingCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::GetRankingRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Ranking::GetRankingRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
			}; // void RegisterDebugTraceRanking()


			///////////////////////////////////////////////////////////////
			// Ranking Debug trace
			Result DebugOutRanking( const char *Prefix, MessageData *pMsg )
			{
 
				Result hr;
				auto itFount = MessageDebugTraceMapRanking.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// Ranking Debug trace
				itFount = MessageDebugTraceMapRanking.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapRanking.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // Result DebugOutRanking( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


