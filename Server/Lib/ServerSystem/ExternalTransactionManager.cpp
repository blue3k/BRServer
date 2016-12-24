////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : External task class implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/ResultCode/BRResultCodeNet.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ExternalTransactionManager.h"
#include "ServerSystem/ExternalTransaction.h"



namespace BR {
namespace Svr
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	External task class
	//

	MemoryAllocator &ExternalTransactionManager::m_Allocator(STDAllocator::GetInstance());


	ExternalTransactionManager::ExternalTransactionManager()
		:ParallelTransactionManager()
	{
		SetTickInterval(DurationMS(Svr::Const::PARALLEL_TRANSACTION_MANAGER_TICKTASK_INTERVAL));
	}

	ExternalTransactionManager::~ExternalTransactionManager()
	{
	}

	void* ExternalTransactionManager::CURL_malloc(size_t size)
	{
		void *pPtr = nullptr;
		m_Allocator.Alloc(size, pPtr);
		return pPtr;
	}

	void ExternalTransactionManager::CURL_free(void *ptr)
	{
		m_Allocator.Free(ptr);
	}

	void* ExternalTransactionManager::CURL_realloc(void *ptr, size_t size)
	{
		m_Allocator.Realloc(size, ptr);
		return ptr;
	}

	char* ExternalTransactionManager::CURL_strdup(const char *str)
	{
		char *strTarget = nullptr;
		StrUtil::StringDup(strTarget, str);
		return strTarget;
	}

	void* ExternalTransactionManager::CURL_calloc(size_t nmemb, size_t size)
	{
		size_t totalSize = size*nmemb;
		void *pPtr = nullptr;
		if ((m_Allocator.Alloc(totalSize, pPtr)))
		{
			memset(pPtr, 0, totalSize);
		}
		return pPtr;
	}


	// convert CURL error to Result
	Result ExternalTransactionManager::ToResult(CURLcode code)
	{
		switch (code)
		{
		case CURLE_OK:							return ResultCode::SUCCESS;
		case CURLE_UNSUPPORTED_PROTOCOL:		return ResultCode::E_SVR_CURL_UNSUPPORTED_PROTOCOL;
		case CURLE_FAILED_INIT:					return ResultCode::E_SVR_CURL_FAILED_INIT;
		case CURLE_URL_MALFORMAT:				return ResultCode::E_SVR_CURL_URL_MALFORMAT;
		case CURLE_NOT_BUILT_IN:				return ResultCode::E_SVR_CURL_NOT_BUILT_IN;
		case CURLE_COULDNT_RESOLVE_PROXY:		return ResultCode::E_SVR_CURL_COULDNT_RESOLVE_PROXY;
		case CURLE_COULDNT_RESOLVE_HOST:		return ResultCode::E_SVR_CURL_COULDNT_RESOLVE_HOST;
		case CURLE_COULDNT_CONNECT:				return ResultCode::E_SVR_CURL_COULDNT_CONNECT;
		case CURLE_HTTP_POST_ERROR:				return ResultCode::E_SVR_CURL_HTTP_POST_ERROR;
		case CURLE_SSL_ENGINE_NOTFOUND:			return ResultCode::E_SVR_CURL_SSL_ENGINE_NOTFOUND;
		case CURLE_SSL_ENGINE_SETFAILED:		return ResultCode::E_SVR_CURL_SSL_ENGINE_SETFAILED;
		case CURLE_SEND_ERROR:					return ResultCode::E_SVR_CURL_SEND_ERROR;
		case CURLE_RECV_ERROR:					return ResultCode::E_SVR_CURL_RECV_ERROR;
		case CURLE_USE_SSL_FAILED:				return ResultCode::E_SVR_CURL_USE_SSL_FAILED;
		case CURLE_NO_CONNECTION_AVAILABLE:		return ResultCode::E_SVR_CURL_NO_CONNECTION_AVAILABLE;
		default:
			return ResultCode::UNEXPECTED;
		};
	}

	Result ExternalTransactionManager::UpdateGoogleAuth()
	{
		return m_GoogleAuth.UpdateAuthentication();
	}

	Result ExternalTransactionManager::InitializeManagerGoogle(const char* strP12KeyFile, const char* strAccount, const char* scopes)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(m_GoogleAuth.Initialize(strP12KeyFile, strAccount, scopes));

	Proc_End:

		return hr;
	}

	Result ExternalTransactionManager::InitializeManagerIOS(const char* strURL)
	{
		Result hr = ResultCode::SUCCESS;

		m_IOSURL = strURL;
		//svrChk(m_GoogleAuth.Initialize(strP12KeyFile, strAccount, scopes));

	//Proc_End:

		return hr;
	}

	// Initialize Manager
	Result ExternalTransactionManager::InitializeComponent()
	{
		Result hr = ResultCode::SUCCESS;
		CURLcode res;

		svrChk(ParallelTransactionManager::InitializeComponent());

		// Initialize HTTP interface
		res = curl_global_init_mem(CURL_GLOBAL_ALL, CURL_malloc, CURL_free, CURL_realloc, CURL_strdup, CURL_calloc);
		if (res != 0) svrErr(ResultCode::UNEXPECTED);

		svrChk(m_GoogleAuth.Authenticate());

	Proc_End:

		return hr;
	}

	// Terminate Manager
	void ExternalTransactionManager::TerminateComponent()
	{
		ParallelTransactionManager::TerminateComponent();
	}

	Result ExternalTransactionManager::TickUpdate(TimerAction *pAction)
	{
		Result hr = ResultCode::SUCCESS;

		hr = ParallelTransactionManager::TickUpdate(pAction);
		if (hr == Result(ResultCode::SUCCESS_FALSE) || !(hr))
			return hr;

		//svrChk(m_GoogleAuth.UpdateAuthentication(false));


	//Proc_End:

		return hr;
	}

	// Send gcm notify
	Result ExternalTransactionManager::SendGCMNotify( TransactionID sender, const char* strRegisterIDs, const char* strMessage, UINT64 param0 )
	{
		Result hr = ResultCode::SUCCESS;
		GCMHttpExternalTransaction *newTrans = nullptr;

		svrMem( newTrans = new GCMHttpExternalTransaction );

		svrChk( newTrans->SetParameters( strRegisterIDs, strMessage, param0 ) );

		newTrans->SetParentTransID( sender );
		svrChk( newTrans->InitializeTransaction(this) );
		svrChk(EnqueueTransaction(newTrans));
		newTrans = nullptr;

	Proc_End:

		if( !(hr) )
		{
			Util::SafeRelease( newTrans );
		}

		return hr;
	}


	// check receipt
	Result ExternalTransactionManager::AndroidCheckReceipt(TransactionID sender, const char* packageName, const char* productID, const char* purchaseToken)
	{
		Result hr = ResultCode::SUCCESS;
		ExternalTransactionGoogleAndroidReceiptCheck *newTrans = nullptr;

		svrMem(newTrans = new ExternalTransactionGoogleAndroidReceiptCheck(sender, &m_GoogleAuth));

		svrChk(newTrans->SetParameters(packageName, productID, purchaseToken));

		newTrans->SetParentTransID(sender);
		svrChk(newTrans->InitializeTransaction(this));
		svrChk(EnqueueTransaction(newTrans));
		newTrans = nullptr;

	Proc_End:

		if (!(hr))
		{
			Util::SafeDelete(newTrans);
		}

		return hr;
	}

	Result ExternalTransactionManager::IOSCheckReceipt(TransactionID sender, const char* packageName, const char* productID, const char* transactionID, const Array<BYTE>& purchaseToken)
	{
		Result hr = ResultCode::SUCCESS;
		ExternalTransactionIOSRecepitCheck *newTrans = nullptr;

		svrMem(newTrans = new ExternalTransactionIOSRecepitCheck(sender, m_IOSURL.c_str()));

		svrChk(newTrans->SetParameters(packageName, productID, transactionID, purchaseToken));

		newTrans->SetParentTransID(sender);
		svrChk(newTrans->InitializeTransaction(this));
		svrChk(EnqueueTransaction(newTrans));
		newTrans = nullptr;

	Proc_End:

		if (!(hr))
		{
			Util::SafeDelete(newTrans);
		}

		return hr;
	}


}; // namespace Svr
}; // namespace BR



