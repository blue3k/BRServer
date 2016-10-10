////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameParty Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/Message/GamePartyMsgDebug.h"
#include "Protocol/Message/GamePartyMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// GameParty Debug trace mappping
			static std::unordered_map<UINT,std::function<Result(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapGameParty;

			void RegisterDebugTraceGameParty()
			{
 				// Cmd: Join Party
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::JoinPartyCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::JoinPartyCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::JoinPartyRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::JoinPartyRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player Joined
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::PlayerJoinedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::PlayerJoinedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player Joined
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::PartyLeaderChangedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::PartyLeaderChangedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Kick player
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::LeavePartyCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::LeavePartyCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::LeavePartyRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::LeavePartyRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player left
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::PlayerLeftS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::PlayerLeftS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Kick player
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::KickPlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::KickPlayerCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::KickPlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::KickPlayerRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player kicked
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::PlayerKickedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::PlayerKickedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Chatting message
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::ChatMessageC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::ChatMessageC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Chatting message
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::ChatMessageS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::ChatMessageS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Quick Chatting message
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::QuickChatMessageC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::QuickChatMessageC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Quick Chatting message
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::QuickChatMessageS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::QuickChatMessageS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Match
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::StartGameMatchCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::StartGameMatchCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::StartGameMatchRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::StartGameMatchRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Enqueued at a game matching
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::QueuedGameMatchingS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::QueuedGameMatchingS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Match
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::CancelGameMatchCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::CancelGameMatchCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::CancelGameMatchRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::CancelGameMatchRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Canceled at a game matching
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::CanceledGameMatchingS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::CanceledGameMatchingS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Enqueued item is dequeued
				MessageDebugTraceMapGameParty.insert(std::make_pair(GameParty::MatchingItemDequeuedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameParty::MatchingItemDequeuedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
			}; // void RegisterDebugTraceGameParty()


			///////////////////////////////////////////////////////////////
			// GameParty Debug trace
			Result DebugOutGameParty( const char *Prefix, MessageData *pMsg )
			{
 
				Result hr;
				auto itFount = MessageDebugTraceMapGameParty.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// GameParty Debug trace
				itFount = MessageDebugTraceMapGameParty.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapGameParty.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // Result DebugOutGameParty( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


