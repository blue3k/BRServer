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
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
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
			static std::unordered_map<UINT,std::function<HRESULT(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapGameInstanceManager;

			void RegisterDebugTraceGameInstanceManager()
			{
 				// Cmd: Create a game instance
				MessageDebugTraceMapGameInstanceManager.insert(std::make_pair(GameInstanceManager::CreateGameCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstanceManager::CreateGameCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				MessageDebugTraceMapGameInstanceManager.insert(std::make_pair(GameInstanceManager::CreateGameRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstanceManager::CreateGameRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// C2S: Game instance notify of deletion
				MessageDebugTraceMapGameInstanceManager.insert(std::make_pair(GameInstanceManager::GameDeletedC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameInstanceManager::GameDeletedC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
			}; // void RegisterDebugTraceGameInstanceManager()


			///////////////////////////////////////////////////////////////
			// GameInstanceManager Debug trace
			HRESULT DebugOutGameInstanceManager( const char *Prefix, MessageData *pMsg )
			{
 
				HRESULT hr = S_SYSTEM_OK;
				auto itFount = MessageDebugTraceMapGameInstanceManager.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// GameInstanceManager Debug trace
				itFount = MessageDebugTraceMapGameInstanceManager.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapGameInstanceManager.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // HRESULT DebugOutGameInstanceManager( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


