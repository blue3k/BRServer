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
#include "SFTypedefs.h"
#include "Protocol/Protocol.h"
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
			static std::unordered_map<UINT,std::function<Result(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapGamePartyManager;

			void RegisterDebugTraceGamePartyManager()
			{
 				// Cmd: Create a party instance
				MessageDebugTraceMapGamePartyManager.insert(std::make_pair(GamePartyManager::CreatePartyCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GamePartyManager::CreatePartyCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapGamePartyManager.insert(std::make_pair(GamePartyManager::CreatePartyRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GamePartyManager::CreatePartyRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// C2S: Party instance notify of deletion
				MessageDebugTraceMapGamePartyManager.insert(std::make_pair(GamePartyManager::PartyDeletedC2SEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   GamePartyManager::PartyDeletedC2SEvt parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
			}; // void RegisterDebugTraceGamePartyManager()


			///////////////////////////////////////////////////////////////
			// GamePartyManager Debug trace
			Result DebugOutGamePartyManager( const char *Prefix, MessageData *pMsg )
			{
 
				Result hr;
				auto itFount = MessageDebugTraceMapGamePartyManager.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// GamePartyManager Debug trace
				itFount = MessageDebugTraceMapGamePartyManager.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapGamePartyManager.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // Result DebugOutGamePartyManager( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


