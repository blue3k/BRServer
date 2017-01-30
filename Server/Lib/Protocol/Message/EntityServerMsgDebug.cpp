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
#include "Net/NetDef.h"
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
			static std::unordered_map<UINT,std::function<Result(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapEntityServer;

			void RegisterDebugTraceEntityServer()
			{
 				// Cmd: Register entity
				MessageDebugTraceMapEntityServer.insert(std::make_pair(EntityServer::RegisterEntityCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   EntityServer::RegisterEntityCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapEntityServer.insert(std::make_pair(EntityServer::RegisterEntityRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   EntityServer::RegisterEntityRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Find Entity
				MessageDebugTraceMapEntityServer.insert(std::make_pair(EntityServer::UnregisterEntityCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   EntityServer::UnregisterEntityCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapEntityServer.insert(std::make_pair(EntityServer::UnregisterEntityRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   EntityServer::UnregisterEntityRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Find Entity
				MessageDebugTraceMapEntityServer.insert(std::make_pair(EntityServer::FindEntityCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   EntityServer::FindEntityCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapEntityServer.insert(std::make_pair(EntityServer::FindEntityRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   EntityServer::FindEntityRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
			}; // void RegisterDebugTraceEntityServer()


			///////////////////////////////////////////////////////////////
			// EntityServer Debug trace
			Result DebugOutEntityServer( const char *Prefix, MessageData *pMsg )
			{
 
				Result hr;
				auto itFount = MessageDebugTraceMapEntityServer.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// EntityServer Debug trace
				itFount = MessageDebugTraceMapEntityServer.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapEntityServer.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // Result DebugOutEntityServer( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


