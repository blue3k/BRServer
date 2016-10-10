////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : PartyMatching Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/Message/PartyMatchingMsgDebug.h"
#include "Protocol/Message/PartyMatchingMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// PartyMatching Debug trace mappping
			static std::unordered_map<UINT,std::function<Result(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapPartyMatching;

			void RegisterDebugTracePartyMatching()
			{
 				// S2C: 
				MessageDebugTraceMapPartyMatching.insert(std::make_pair(PartyMatching::PartyGameMatchedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatching::PartyGameMatchedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// S2C: 
				MessageDebugTraceMapPartyMatching.insert(std::make_pair(PartyMatching::PlayerGameMatchedS2CEvt::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   PartyMatching::PlayerGameMatchedS2CEvt parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
			}; // void RegisterDebugTracePartyMatching()


			///////////////////////////////////////////////////////////////
			// PartyMatching Debug trace
			Result DebugOutPartyMatching( const char *Prefix, MessageData *pMsg )
			{
 
				Result hr;
				auto itFount = MessageDebugTraceMapPartyMatching.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// PartyMatching Debug trace
				itFount = MessageDebugTraceMapPartyMatching.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapPartyMatching.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // Result DebugOutPartyMatching( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


