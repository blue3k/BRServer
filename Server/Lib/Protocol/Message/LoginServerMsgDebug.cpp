////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : LoginServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/Message/LoginServerMsgDebug.h"
#include "Protocol/Message/LoginServerMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// LoginServer Debug trace mappping
			static std::unordered_map<UINT,std::function<HRESULT(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapLoginServer;

			void RegisterDebugTraceLoginServer()
			{
 				// Cmd: Notify user joind and see it's valid authticket instance
				MessageDebugTraceMapLoginServer.insert(std::make_pair(LoginServer::PlayerJoinedToGameServerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   LoginServer::PlayerJoinedToGameServerCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				MessageDebugTraceMapLoginServer.insert(std::make_pair(LoginServer::PlayerJoinedToGameServerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   LoginServer::PlayerJoinedToGameServerRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// Cmd: Kick logged in player
				MessageDebugTraceMapLoginServer.insert(std::make_pair(LoginServer::KickPlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   LoginServer::KickPlayerCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				MessageDebugTraceMapLoginServer.insert(std::make_pair(LoginServer::KickPlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   LoginServer::KickPlayerRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
			}; // void RegisterDebugTraceLoginServer()


			///////////////////////////////////////////////////////////////
			// LoginServer Debug trace
			HRESULT DebugOutLoginServer( const char *Prefix, MessageData *pMsg )
			{
 
				HRESULT hr = S_SYSTEM_OK;
				auto itFount = MessageDebugTraceMapLoginServer.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// LoginServer Debug trace
				itFount = MessageDebugTraceMapLoginServer.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapLoginServer.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // HRESULT DebugOutLoginServer( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


