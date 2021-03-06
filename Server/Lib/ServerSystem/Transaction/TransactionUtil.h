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


#include "SFTypedefs.h"

namespace SF {
	namespace Svr {

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Transaction helper utility
		//


		// User transaction close, just return Result by default
#define BR_IMPLEMENT_USERMSGTRANS_CLOSE( PolicyClass, MessageName ) \
	virtual Result OnCloseTransaction( Result hrRes ) override\
	{\
		if( IsClosed() ) return ResultCode::SUCCESS;\
		ScopeContext hr([this, hrRes](Result hr){ super::OnCloseTransaction(hrRes); });\
		svrCheck( PolicyClass(GetRemoteEndpoint()).MessageName( GetMessageContext(), hrRes ) );\
		return hr;\
	}\


		// User transaction close with argument, just return Result by default
		// When OwnerEntity is nullptr, it should try to access input parameters
#define BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS( PolicyClass, MessageName, ... ) \
	virtual Result OnCloseTransaction( Result hrRes ) override\
	{\
		if( IsClosed() ) return ResultCode::SUCCESS;\
		ScopeContext hr([this, hrRes](Result hr){ super::OnCloseTransaction(hrRes); });\
		if(GetOwnerEntity() != nullptr) {\
			svrCheck( PolicyClass(GetRemoteEndpoint()).MessageName( GetMessageContext(), hrRes, ##__VA_ARGS__ ) );\
		}\
		return hr;\
	}\


		// Generic transaction close, return Context and Result by default
#define BR_IMPLEMENT_MSGTRANS_CLOSE( PolicyClass, MessageName ) \
	virtual Result OnCloseTransaction( Result hrRes ) override\
	{\
		ScopeContext hr([this, hrRes](Result hr){ super::OnCloseTransaction(hrRes); });\
		svrCheck( PolicyClass(GetRemoteEndpoint()).MessageName( GetTransactionID(), hrRes ) );\
		return hr;\
	}\


		// Generic transaction close with argument, return Context and Result by default
#define BR_IMPLEMENT_MSGTRANS_CLOSE_ARGS( PolicyClass, MessageName, ... ) \
	virtual Result OnCloseTransaction( Result hrRes ) override\
	{\
		ScopeContext hr([this, hrRes](Result hr){ super::OnCloseTransaction(hrRes); });\
		svrCheck( PolicyClass(GetRemoteEndpoint()).MessageName( GetTransactionID(), hrRes, ##__VA_ARGS__ ) );\
		return hr;\
	}\



		// Generic transaction close with argument, return Context and Result by default
#define BR_SVR_MSGTRANS_CLOSE( PolicyClass, MessageName, routeContext ) \
	virtual Result OnCloseTransaction( Result hrRes ) override\
	{\
		ScopeContext hr([this, hrRes](Result hr){ super::OnCloseTransaction(hrRes); });\
		svrCheck( PolicyClass(super::GetRemoteEndpoint()).MessageName( routeContext, GetTransactionID(), hrRes ) );\
		return hr;\
	}\

		// Generic transaction close with argument, return Context and Result by default
#define BR_SVR_MSGTRANS_CLOSE_ARGS( PolicyClass, MessageName, routeContext, ... ) \
	virtual Result OnCloseTransaction( Result hrRes ) override\
	{\
		ScopeContext hr([this, hrRes](Result hr){ super::OnCloseTransaction(hrRes); });\
		svrCheck( PolicyClass(super::GetRemoteEndpoint()).MessageName( routeContext, super::GetTransactionID(), hrRes, ##__VA_ARGS__ ) );\
		return hr;\
	}\


		// Event transaction close, no default argument
#define BR_IMPLEMENT_EVTTRANS_CLOSE( PolicyClass, MessageName ) \
	virtual Result OnCloseTransaction( Result hrRes ) override\
	{\
		ScopeContext hr([this, hrRes](Result hr){ super::OnCloseTransaction(hrRes); });\
		svrCheck(PolicyClass(GetConnection()).MessageName() );\
		return hr;\
	}\


		// Event transaction close with argument, no default argument
#define BR_IMPLEMENT_EVTTRANS_CLOSE_ARGS( PolicyClass, MessageName, ... ) \
	virtual Result OnCloseTransaction( Result hrRes ) override\
	{\
		ScopeContext hr([this, hrRes](Result hr){ super::OnCloseTransaction(hrRes); });\
		svrCheck( PolicyClass(GetConnection()).MessageName( __VA_ARGS__ ) );\
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
				svrChk( Result.ParseMessage(pMsgRes->GetMessage()) );\
			}\




// Net message process case macro with input message variable
#define BR_TRANSPROC_CASE_MSGRES_NET_VARIABLE( Policy, MsgName, ResVar ) \
		case Message::MsgID::Policy::MID_##MsgName##Res:\
		{\
			Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;\
			if( (pMsgRes->GetResult()) )\
			{\
				svrChk( ResVar.ParseMessage(pMsgRes->GetMessage()) );\
			}\


// Generic message proc handler
#define BR_TRANSPROC_CASE_MSGRES( Policy, MsgName ) \
		case Policy::MID_##MsgName:\
		{\




#define BR_TRANSPROC_CASE_END() \
		break; }\


}; // namespace Svr
}; // namespace SF




