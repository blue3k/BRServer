﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameInstance Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/Message/GameInstanceMsgDebug.h"
#include "Protocol/Message/GameInstanceMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// GameInstance Debug trace mappping
			static std::unordered_map<UINT,std::function<Result(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapGameInstance;

			void RegisterDebugTraceGameInstance()
			{
 				// C2S: Game instance deletion
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::DeleteGameC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::DeleteGameC2SEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Join Game
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::JoinGameCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::JoinGameCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::JoinGameRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::JoinGameRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player Joined
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::PlayerJoinedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::PlayerJoinedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Change configue preset
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::SetConfigPresetC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::SetConfigPresetC2SEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Leave Game
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::LeaveGameCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::LeaveGameCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::LeaveGameRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::LeaveGameRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player left
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::PlayerLeftS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::PlayerLeftS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Kick player
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::KickPlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::KickPlayerCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::KickPlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::KickPlayerRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player kicked
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::PlayerKickedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::PlayerKickedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Assign role
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::AssignRoleCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::AssignRoleCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::AssignRoleRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::AssignRoleRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Assign role
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::RoleAssignedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::RoleAssignedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Chatting message
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::ChatMessageC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::ChatMessageC2SEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Advance game
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::AdvanceGameCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::AdvanceGameCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::AdvanceGameRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::AdvanceGameRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: The game state is advanced
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GameAdvancedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::GameAdvancedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Game is ended
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GameEndedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::GameEndedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: *Vote game advance
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::VoteGameAdvanceCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::VoteGameAdvanceCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::VoteGameAdvanceRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::VoteGameAdvanceRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: *GameAdvance is Voted
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GameAdvanceVotedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::GameAdvanceVotedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Assign role
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::VoteCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::VoteCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::VoteRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::VoteRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player Voted
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::VotedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::VotedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player Voted
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::VoteEndS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::VoteEndS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player Voted
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::PlayerRevealedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::PlayerRevealedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player Killed
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::PlayerKilledS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::PlayerKilledS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Play again with the current players
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GamePlayAgainCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::GamePlayAgainCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GamePlayAgainRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::GamePlayAgainRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GamePlayAgainS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::GamePlayAgainS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Player. revive himself
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GameRevealPlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::GameRevealPlayerCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GameRevealPlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::GameRevealPlayerRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Player. revive himself
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GamePlayerReviveCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::GamePlayerReviveCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GamePlayerReviveRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::GamePlayerReviveRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: Player is revived
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GamePlayerRevivedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstance::GamePlayerRevivedS2CEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
			}; // void RegisterDebugTraceGameInstance()


			///////////////////////////////////////////////////////////////
			// GameInstance Debug trace
			Result DebugOutGameInstance( const char *Prefix, MessageData *pMsg )
			{
 
				Result hr;
				auto itFount = MessageDebugTraceMapGameInstance.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// GameInstance Debug trace
				itFount = MessageDebugTraceMapGameInstance.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapGameInstance.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // Result DebugOutGameInstance( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


