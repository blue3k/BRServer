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
#include "Net/Netdef.h"
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
			static std::unordered_map<UINT,std::function<HRESULT(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapRanking;

			void RegisterDebugTraceRanking()
			{
 				// Cmd: Add a player to ranking
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::AddPlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Ranking::AddPlayerCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::AddPlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Ranking::AddPlayerRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Remove a player to ranking
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::RemovePlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Ranking::RemovePlayerCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::RemovePlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Ranking::RemovePlayerRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Remove a player to ranking
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::GetPlayerRankingCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Ranking::GetPlayerRankingCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::GetPlayerRankingRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Ranking::GetPlayerRankingRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Update a player to ranking
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::UpdatePlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Ranking::UpdatePlayerCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::UpdatePlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Ranking::UpdatePlayerRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Remove a player to ranking
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::GetRankingCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Ranking::GetRankingCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapRanking.insert(std::make_pair(Ranking::GetRankingRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Ranking::GetRankingRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
			}; // void RegisterDebugTraceRanking()


			///////////////////////////////////////////////////////////////
			// Ranking Debug trace
			HRESULT DebugOutRanking( const char *Prefix, MessageData *pMsg )
			{
 
				HRESULT hr = S_OK;
				auto itFount = MessageDebugTraceMapRanking.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// Ranking Debug trace
				itFount = MessageDebugTraceMapRanking.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapRanking.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // HRESULT DebugOutRanking( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


