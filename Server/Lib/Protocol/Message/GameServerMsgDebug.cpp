////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/Message/GameServerMsgDebug.h"
#include "Protocol/Message/GameServerMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// GameServer Debug trace mappping
			static std::unordered_map<UINT,std::function<HRESULT(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapGameServer;

			void RegisterDebugTraceGameServer()
			{
 				// Cmd: Kick
				MessageDebugTraceMapGameServer.insert(std::make_pair(GameServer::RegisterPlayerToJoinGameServerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameServer::RegisterPlayerToJoinGameServerCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGameServer.insert(std::make_pair(GameServer::RegisterPlayerToJoinGameServerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameServer::RegisterPlayerToJoinGameServerRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Kick
				MessageDebugTraceMapGameServer.insert(std::make_pair(GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGameServer.insert(std::make_pair(GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// C2S: Chatting message
				MessageDebugTraceMapGameServer.insert(std::make_pair(GameServer::ChatMessageC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameServer::ChatMessageC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// C2S: Notification
				MessageDebugTraceMapGameServer.insert(std::make_pair(GameServer::NotifyC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameServer::NotifyC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// C2S: Friend Accept
				MessageDebugTraceMapGameServer.insert(std::make_pair(GameServer::FriendAcceptedC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameServer::FriendAcceptedC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// C2S: Friend Remove
				MessageDebugTraceMapGameServer.insert(std::make_pair(GameServer::FriendRemovedC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameServer::FriendRemovedC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// C2S: Request Player Status Update
				MessageDebugTraceMapGameServer.insert(std::make_pair(GameServer::RequestPlayerStatusUpdateC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameServer::RequestPlayerStatusUpdateC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// C2S: Notify Player Status Updated
				MessageDebugTraceMapGameServer.insert(std::make_pair(GameServer::NotifyPlayerStatusUpdatedC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameServer::NotifyPlayerStatusUpdatedC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// C2S: Notification
				MessageDebugTraceMapGameServer.insert(std::make_pair(GameServer::NotifyPartyInviteC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameServer::NotifyPartyInviteC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
			}; // void RegisterDebugTraceGameServer()


			///////////////////////////////////////////////////////////////
			// GameServer Debug trace
			HRESULT DebugOutGameServer( const char *Prefix, MessageData *pMsg )
			{
 
				HRESULT hr = S_OK;
				auto itFount = MessageDebugTraceMapGameServer.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// GameServer Debug trace
				itFount = MessageDebugTraceMapGameServer.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapGameServer.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // HRESULT DebugOutGameServer( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


