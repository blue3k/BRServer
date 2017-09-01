////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Server Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/Message/ServerMsgDebug.h"
#include "Protocol/Message/ServerMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// Server Debug trace mappping
			static std::unordered_map<UINT,std::function<Result(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapServer;

			void RegisterDebugTraceServer()
			{
 				// Cmd: 
				MessageDebugTraceMapServer.insert(std::make_pair(Server::GenericFailureCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Server::GenericFailureCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapServer.insert(std::make_pair(Server::GenericFailureRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Server::GenericFailureRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Server Started or Connected
				MessageDebugTraceMapServer.insert(std::make_pair(Server::ServerConnectedC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Server::ServerConnectedC2SEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
			}; // void RegisterDebugTraceServer()


			///////////////////////////////////////////////////////////////
			// Server Debug trace
			Result DebugOutServer( const char *Prefix, MessageData *pMsg )
			{
 
				Result hr;
				auto itFount = MessageDebugTraceMapServer.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// Server Debug trace
				itFount = MessageDebugTraceMapServer.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapServer.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // Result DebugOutServer( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


