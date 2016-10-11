////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server Transaction definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"

namespace BR {
	namespace Svr {

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Transaction helper utility
		//


		// User transaction close, just return Result by default
#define BR_IMPLEMENT_USERMSGTRANS_CLOSE( MessageName ) \
	virtual Result OnCloseTransaction( Result hrRes )\
		{\
		Result hr = ResultCode::SUCCESS;\
		if( IsClosed() )\
			return ResultCode::SUCCESS;\
		auto pPolicy = GetPolicy();\
		if( pPolicy != nullptr ) {\
			svrChk( pPolicy->MessageName( hrRes ) );\
				}\
				Proc_End:\
		super::OnCloseTransaction(hrRes);\
		return hr;\
		}\


		// User transaction close with argument, just return Result by default
#define BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS( MessageName, ... ) \
	virtual Result OnCloseTransaction( Result hrRes )\
		{\
		Result hr = ResultCode::SUCCESS;\
		if( IsClosed() )\
			return ResultCode::SUCCESS;\
		auto pPolicy = GetPolicy();\
		if( pPolicy != nullptr ) {\
			svrChk( pPolicy->MessageName( hrRes, ##__VA_ARGS__ ) );\
				}\
				Proc_End:\
		super::OnCloseTransaction(hrRes);\
		return hr;\
		}\


		// Generic transaction close, return Context and Result by default
#define BR_IMPLEMENT_MSGTRANS_CLOSE( MessageName ) \
	virtual Result OnCloseTransaction( Result hrRes )\
		{\
		Result hr = ResultCode::SUCCESS;\
		auto pPolicy = GetPolicy();\
		if( pPolicy != nullptr ) {\
			svrChk( pPolicy->MessageName( GetTransactionID(), hrRes ) );\
				}\
				Proc_End:\
		super::OnCloseTransaction(hrRes);\
		return hr;\
		}\


		// Generic transaction close with argument, return Context and Result by default
#define BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS( MessageName, ... ) \
	virtual Result OnCloseTransaction( Result hrRes )\
		{\
		Result hr = ResultCode::SUCCESS;\
		auto pPolicy = GetPolicy();\
		if( pPolicy != nullptr ) {\
			svrChk( pPolicy->MessageName( GetTransactionID(), hrRes, ##__VA_ARGS__ ) );\
				}\
				Proc_End:\
		super::OnCloseTransaction(hrRes);\
		return hr;\
		}\



		// Generic transaction close with argument, return Context and Result by default
#define BR_SVR_MSGTRANS_CLOSE( MessageName, routeContext ) \
	virtual Result OnCloseTransaction( Result hrRes )\
		{\
		Result hr = ResultCode::SUCCESS;\
		auto pPolicy = GetPolicy();\
		if( pPolicy != nullptr ) {\
			svrChk( pPolicy->MessageName( routeContext, GetTransactionID(), hrRes ) );\
				}\
				Proc_End:\
		super::OnCloseTransaction(hrRes);\
		return hr;\
		}\

		// Generic transaction close with argument, return Context and Result by default
#define BR_SVR_MSGTRANS_CLOSE_ARGS( MessageName, routeContext, ... ) \
	virtual Result OnCloseTransaction( Result hrRes )\
		{\
		Result hr = ResultCode::SUCCESS;\
		auto pPolicy = GetPolicy();\
		if( pPolicy != nullptr ) {\
			svrChk( pPolicy->MessageName( routeContext, super::GetTransactionID(), hrRes, ##__VA_ARGS__ ) );\
				}\
				Proc_End:\
		super::OnCloseTransaction(hrRes);\
		return hr;\
		}\


		// Event transaction close, no default arguemnt
#define BR_IMPLEMENT_EVTTRANS_CLOSE( MessageName ) \
	virtual Result OnCloseTransaction( Result hrRes )\
		{\
		Result hr = ResultCode::SUCCESS;\
		auto pPolicy = GetPolicy();\
		if( pPolicy != nullptr ) {\
			svrChk( pPolicy->MessageName() );\
				}\
				Proc_End:\
		super::OnCloseTransaction(hrRes);\
		return hr;\
		}\


		// Event transaction close with argument, no default arguemnt
#define BR_IMPLEMENT_EVTTRANS_CLOSE_ARGS( MessageName, ... ) \
	virtual Result OnCloseTransaction( Result hrRes )\
		{\
		Result hr = ResultCode::SUCCESS;\
		auto pPolicy = GetPolicy();\
		if( pPolicy != nullptr ) {\
			svrChk( pPolicy->MessageName( __VA_ARGS__ ) );\
		}\
	Proc_End:\
		super::OnCloseTransaction(hrRes);\
		return hr;\
	}\
	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Transaction process utility
	//


// Net message process case macro
#define BR_TRANSPROC_CASE_MSGRES_NET( Policy, MsgName ) \
		case Message::MsgID::Policy::MID_##MsgName##Res:\
		{\
			Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;\
			Message::Policy::Msg##MsgName##Res Result;\
			\
			if( (pMsgRes->GetResult()) )\
			{\
				svrChk( Result.ParseIMsg(pMsgRes->GetMessage()) );\
			}\




// Net message process case macro with input message variable
#define BR_TRANSPROC_CASE_MSGRES_NET_VARIABLE( Policy, MsgName, ResVar ) \
		case Message::MsgID::Policy::MID_##MsgName##Res:\
		{\
			Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;\
			if( (pMsgRes->GetResult()) )\
			{\
				svrChk( ResVar.ParseIMsg(pMsgRes->GetMessage()) );\
			}\


// Generic message proc handler
#define BR_TRANSPROC_CASE_MSGRES( Policy, MsgName ) \
		case Policy::MID_##MsgName:\
		{\




#define BR_TRANSPROC_CASE_END() \
		break; }\


}; // namespace Svr
}; // namespace BR




