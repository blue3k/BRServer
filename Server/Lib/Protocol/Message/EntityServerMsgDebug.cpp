////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : EntityServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/Message/EntityServerMsgDebug.h"
#include "Protocol/Message/EntityServerMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// EntityServer Debug trace mappping
			static std::unordered_map<UINT,std::function<HRESULT(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapEntityServer;

			void RegisterDebugTraceEntityServer()
			{
 				// Cmd: Register entity
				MessageDebugTraceMapEntityServer.insert(std::make_pair(EntityServer::RegisterEntityCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   EntityServer::RegisterEntityCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapEntityServer.insert(std::make_pair(EntityServer::RegisterEntityRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   EntityServer::RegisterEntityRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Find Entity
				MessageDebugTraceMapEntityServer.insert(std::make_pair(EntityServer::UnregisterEntityCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   EntityServer::UnregisterEntityCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapEntityServer.insert(std::make_pair(EntityServer::UnregisterEntityRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   EntityServer::UnregisterEntityRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// Cmd: Find Entity
				MessageDebugTraceMapEntityServer.insert(std::make_pair(EntityServer::FindEntityCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   EntityServer::FindEntityCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapEntityServer.insert(std::make_pair(EntityServer::FindEntityRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   EntityServer::FindEntityRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
			}; // void RegisterDebugTraceEntityServer()


			///////////////////////////////////////////////////////////////
			// EntityServer Debug trace
			HRESULT DebugOutEntityServer( const char *Prefix, MessageData *pMsg )
			{
 
				HRESULT hr = S_OK;
				auto itFount = MessageDebugTraceMapEntityServer.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// EntityServer Debug trace
				itFount = MessageDebugTraceMapEntityServer.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapEntityServer.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // HRESULT DebugOutEntityServer( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


