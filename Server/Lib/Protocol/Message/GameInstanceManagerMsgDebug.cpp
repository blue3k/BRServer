////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameInstanceManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/Message/GameInstanceManagerMsgDebug.h"
#include "Protocol/Message/GameInstanceManagerMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// GameInstanceManager Debug trace mappping
			static std::unordered_map<UINT,std::function<Result(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapGameInstanceManager;

			void RegisterDebugTraceGameInstanceManager()
			{
 				// Cmd: Create a game instance
				MessageDebugTraceMapGameInstanceManager.insert(std::make_pair(GameInstanceManager::CreateGameCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstanceManager::CreateGameCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGameInstanceManager.insert(std::make_pair(GameInstanceManager::CreateGameRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstanceManager::CreateGameRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Game instance notify of deletion
				MessageDebugTraceMapGameInstanceManager.insert(std::make_pair(GameInstanceManager::GameDeletedC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GameInstanceManager::GameDeletedC2SEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
			}; // void RegisterDebugTraceGameInstanceManager()


			///////////////////////////////////////////////////////////////
			// GameInstanceManager Debug trace
			Result DebugOutGameInstanceManager( const char *Prefix, MessageData *pMsg )
			{
 
				Result hr;
				auto itFount = MessageDebugTraceMapGameInstanceManager.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// GameInstanceManager Debug trace
				itFount = MessageDebugTraceMapGameInstanceManager.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapGameInstanceManager.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // Result DebugOutGameInstanceManager( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


