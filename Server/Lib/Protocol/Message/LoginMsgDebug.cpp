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
			static std::unordered_map<UINT,std::function<Result(const char* prefix,MessageData *pMsg)>> MessageDebugTraceMapLogin;

			void RegisterDebugTraceLogin()
			{
 				// Cmd: Login request
				MessageDebugTraceMapLogin.insert(std::make_pair(Login::LoginCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Login::LoginCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapLogin.insert(std::make_pair(Login::LoginRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Login::LoginRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Login request with Facebook UID
				MessageDebugTraceMapLogin.insert(std::make_pair(Login::LoginByFacebookCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Login::LoginByFacebookCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapLogin.insert(std::make_pair(Login::LoginByFacebookRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Login::LoginByFacebookRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Login request
				MessageDebugTraceMapLogin.insert(std::make_pair(Login::CreateRandomUserCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Login::CreateRandomUserCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapLogin.insert(std::make_pair(Login::CreateRandomUserRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Login::CreateRandomUserRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Get Ranking lise
				MessageDebugTraceMapLogin.insert(std::make_pair(Login::UpdateMyRankCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Login::UpdateMyRankCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapLogin.insert(std::make_pair(Login::UpdateMyRankRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Login::UpdateMyRankRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				// Cmd: Get Ranking lise
				MessageDebugTraceMapLogin.insert(std::make_pair(Login::GetRankingListCmd::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Login::GetRankingListCmd parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
				MessageDebugTraceMapLogin.insert(std::make_pair(Login::GetRankingListRes::MID.IDSeq.MsgID,[](const char* prefix, MessageData* pMsg)->Result{   Login::GetRankingListRes parser; parser.ParseMessage(pMsg); parser.TraceOut(prefix,pMsg); return ResultCode::SUCCESS; } ));
			}; // void RegisterDebugTraceLogin()


			///////////////////////////////////////////////////////////////
			// Login Debug trace
			Result DebugOutLogin( const char *Prefix, MessageData *pMsg )
			{
 
				Result hr;
				auto itFount = MessageDebugTraceMapLogin.end();

				protocolChkPtr(pMsg);
				protocolChkPtr(Prefix);
				// Login Debug trace
				itFount = MessageDebugTraceMapLogin.find(pMsg->GetMessageHeader()->msgID.IDSeq.MsgID);
				if( itFount == MessageDebugTraceMapLogin.end() ) goto Proc_End;
				protocolChk(itFount->second(Prefix,pMsg));

			Proc_End:
				return hr;
			}; // Result DebugOutLogin( const char *Prefix, MessageData *pMsg )


		}; // namespace Debug
	}; // namespace Message
}; // namespace BR


