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
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
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
			static std::unordered_map<UINT,std::function<HRESULT(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapServer;

			void RegisterDebugTraceServer()
			{
 				// Cmd: 
				MessageDebugTraceMapServer.insert(std::make_pair(Server::GenericFailureCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Server::GenericFailureCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				MessageDebugTraceMapServer.insert(std::make_pair(Server::GenericFailureRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Server::GenericFailureRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// C2S: Server Started or Connected
				MessageDebugTraceMapServer.insert(std::make_pair(Server::ServerConnectedC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Server::ServerConnectedC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
			}; // void RegisterDebugTraceServer()


			///////////////////////////////////////////////////////////////
			// Server Debug trace
			HRESULT DebugOutServer( const char *Prefix, MessageData *pMsg )
			{
 
				HRESULT hr = S_SYSTEM_OK;
				auto itFount = MessageDebugTraceMapServer.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// Server Debug trace
				itFount = MessageDebugTraceMapServer.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapServer.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // HRESULT DebugOutServer( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


