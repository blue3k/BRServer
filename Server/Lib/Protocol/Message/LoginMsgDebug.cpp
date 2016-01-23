////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Login Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/Message/LoginMsgDebug.h"
#include "Protocol/Message/LoginMsgClass.h"



namespace BR
{
 	namespace Message
	{
 		namespace Debug
		{
 
			///////////////////////////////////////////////////////////////
			// Login Debug trace mappping
			static std::unordered_map<UINT,std::function<HRESULT(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapLogin;

			void RegisterDebugTraceLogin()
			{
 				// Cmd: Login request
				MessageDebugTraceMapLogin.insert(std::make_pair(Login::LoginCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Login::LoginCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				MessageDebugTraceMapLogin.insert(std::make_pair(Login::LoginRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Login::LoginRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// Cmd: Login request with Facebook UID
				MessageDebugTraceMapLogin.insert(std::make_pair(Login::LoginByFacebookCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Login::LoginByFacebookCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				MessageDebugTraceMapLogin.insert(std::make_pair(Login::LoginByFacebookRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Login::LoginByFacebookRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				// Cmd: Login request
				MessageDebugTraceMapLogin.insert(std::make_pair(Login::CreateRandomUserCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Login::CreateRandomUserCmd parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
				MessageDebugTraceMapLogin.insert(std::make_pair(Login::CreateRandomUserRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->HRESULT{   Login::CreateRandomUserRes parser; parser.ParseIMsg(pMsg); parser.TraceOut(prefix,pMsg); return S_SYSTEM_OK; } ));
			}; // void RegisterDebugTraceLogin()


			///////////////////////////////////////////////////////////////
			// Login Debug trace
			HRESULT DebugOutLogin( const char *Prefix, MessageData *pMsg )
			{
 
				HRESULT hr = S_SYSTEM_OK;
				auto itFount = MessageDebugTraceMapLogin.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// Login Debug trace
				itFount = MessageDebugTraceMapLogin.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapLogin.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // HRESULT DebugOutLogin( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


