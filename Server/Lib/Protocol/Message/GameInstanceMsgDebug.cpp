////////////////////////////////////////////////////////////////////////////////
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
			static std::unordered_map<UINT,std::function<HRESULT(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapGameInstance;

			void RegisterDebugTraceGameInstance()
			{
 				// C2S: Game instance deletion
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::DeleteGameC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::DeleteGameC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// Cmd: Join Game
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::JoinGameCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::JoinGameCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::JoinGameRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::JoinGameRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// S2C: Player Joined
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::PlayerJoinedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::PlayerJoinedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// C2S: Change configue preset
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::SetConfigPresetC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::SetConfigPresetC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// Cmd: Leave Game
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::LeaveGameCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::LeaveGameCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::LeaveGameRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::LeaveGameRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// S2C: Player left
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::PlayerLeftS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::PlayerLeftS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// Cmd: Kick player
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::KickPlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::KickPlayerCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::KickPlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::KickPlayerRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// S2C: Player kicked
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::PlayerKickedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::PlayerKickedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// Cmd: Assign role
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::AssignRoleCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::AssignRoleCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::AssignRoleRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::AssignRoleRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// S2C: Assign role
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::RoleAssignedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::RoleAssignedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// C2S: Chatting message
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::ChatMessageC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::ChatMessageC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// Cmd: Advance game
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::AdvanceGameCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::AdvanceGameCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::AdvanceGameRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::AdvanceGameRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// S2C: The game state is advanced
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GameAdvancedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::GameAdvancedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// S2C: Game is ended
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GameEndedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::GameEndedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// Cmd: *Vote game advance
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::VoteGameAdvanceCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::VoteGameAdvanceCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::VoteGameAdvanceRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::VoteGameAdvanceRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// S2C: *GameAdvance is Voted
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GameAdvanceVotedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::GameAdvanceVotedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// Cmd: Assign role
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::VoteCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::VoteCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::VoteRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::VoteRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// S2C: Player Voted
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::VotedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::VotedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// S2C: Player Voted
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::VoteEndS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::VoteEndS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// S2C: Player Voted
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::PlayerRevealedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::PlayerRevealedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// S2C: Player Killed
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::PlayerKilledS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::PlayerKilledS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// Cmd: Play again with the current players
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GamePlayAgainCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::GamePlayAgainCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GamePlayAgainRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::GamePlayAgainRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GamePlayAgainS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::GamePlayAgainS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// Cmd: Player. revive himself
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GameRevealPlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::GameRevealPlayerCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GameRevealPlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::GameRevealPlayerRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// Cmd: Player. revive himself
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GamePlayerReviveCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::GamePlayerReviveCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GamePlayerReviveRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::GamePlayerReviveRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// S2C: Player is revived
				MessageDebugTraceMapGameInstance.insert(std::make_pair(GameInstance::GamePlayerRevivedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstance::GamePlayerRevivedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
			}; // void RegisterDebugTraceGameInstance()


			///////////////////////////////////////////////////////////////
			// GameInstance Debug trace
			HRESULT DebugOutGameInstance( const char *Prefix, MessageData *pMsg )
			{
 
				HRESULT hr = S_SYSTEM_OK;
				auto itFount = MessageDebugTraceMapGameInstance.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// GameInstance Debug trace
				itFount = MessageDebugTraceMapGameInstance.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapGameInstance.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // HRESULT DebugOutGameInstance( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


