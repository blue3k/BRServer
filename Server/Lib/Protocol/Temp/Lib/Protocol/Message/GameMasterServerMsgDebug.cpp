////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameMasterServer Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/Netdef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/Message/GameMasterServerMsgDebug.h"
#include "Protocol/Message/GameMasterServerMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// GameMasterServer Debug trace mappping
			static std::unordered_map<UINT,std::function<HRESULT(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapGameMasterServer;

			void RegisterDebugTraceGameMasterServer()
			{
 				// C2S: Player entered
				MessageDebugTraceMapGameMasterServer.insert(std::make_pair(GameMasterServer::PlayerEnteredC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameMasterServer::PlayerEnteredC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// C2S: Player leaved
				MessageDebugTraceMapGameMasterServer.insert(std::make_pair(GameMasterServer::PlayerLeavedC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GameMasterServer::PlayerLeavedC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
			}; // void RegisterDebugTraceGameMasterServer()


			///////////////////////////////////////////////////////////////
			// GameMasterServer Debug trace
			HRESULT DebugOutGameMasterServer( const char *Prefix, MessageData *pMsg )
			{
 
				HRESULT hr = S_OK;
				auto itFount = MessageDebugTraceMapGameMasterServer.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// GameMasterServer Debug trace
				itFount = MessageDebugTraceMapGameMasterServer.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapGameMasterServer.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // HRESULT DebugOutGameMasterServer( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


