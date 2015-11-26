////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GamePartyManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/Message/GamePartyManagerMsgDebug.h"
#include "Protocol/Message/GamePartyManagerMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// GamePartyManager Debug trace mappping
			static std::unordered_map<UINT,std::function<HRESULT(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapGamePartyManager;

			void RegisterDebugTraceGamePartyManager()
			{
 				// Cmd: Create a party instance
				MessageDebugTraceMapGamePartyManager.insert(std::make_pair(GamePartyManager::CreatePartyCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GamePartyManager::CreatePartyCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				MessageDebugTraceMapGamePartyManager.insert(std::make_pair(GamePartyManager::CreatePartyRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GamePartyManager::CreatePartyRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
				// C2S: Party instance notify of deletion
				MessageDebugTraceMapGamePartyManager.insert(std::make_pair(GamePartyManager::PartyDeletedC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   GamePartyManager::PartyDeletedC2SEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_OK; } ));
			}; // void RegisterDebugTraceGamePartyManager()


			///////////////////////////////////////////////////////////////
			// GamePartyManager Debug trace
			HRESULT DebugOutGamePartyManager( const char *Prefix, MessageData *pMsg )
			{
 
				HRESULT hr = S_OK;
				auto itFount = MessageDebugTraceMapGamePartyManager.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// GamePartyManager Debug trace
				itFount = MessageDebugTraceMapGamePartyManager.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapGamePartyManager.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // HRESULT DebugOutGamePartyManager( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


