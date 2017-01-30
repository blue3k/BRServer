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
			static std::unordered_map<UINT,std::function<Result(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapLoginServer;

			void RegisterDebugTraceLoginServer()
			{
 				// Cmd: Notify user joind and see it's valid authticket instance
				MessageDebugTraceMapLoginServer.insert(std::make_pair(LoginServer::PlayerJoinedToGameServerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   LoginServer::PlayerJoinedToGameServerCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapLoginServer.insert(std::make_pair(LoginServer::PlayerJoinedToGameServerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   LoginServer::PlayerJoinedToGameServerRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Kick logged in player
				MessageDebugTraceMapLoginServer.insert(std::make_pair(LoginServer::KickPlayerCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   LoginServer::KickPlayerCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapLoginServer.insert(std::make_pair(LoginServer::KickPlayerRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   LoginServer::KickPlayerRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
			}; // void RegisterDebugTraceLoginServer()


			///////////////////////////////////////////////////////////////
			// LoginServer Debug trace
			Result DebugOutLoginServer( const char *Prefix, MessageData *pMsg )
			{
 
				Result hr;
				auto itFount = MessageDebugTraceMapLoginServer.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// LoginServer Debug trace
				itFount = MessageDebugTraceMapLoginServer.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapLoginServer.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // Result DebugOutLoginServer( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


