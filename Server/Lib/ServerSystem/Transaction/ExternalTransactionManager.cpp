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


#include "ServerSystemPCH.h"
#include "Util/SFStrUtil.h"
#include "Util/SFTimeUtil.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ServerLog/SvrLog.h"
#include "Multithread/SFThread.h"
#include "SvrConst.h"
#include "Entity/Entity.h"
#include "Transaction/MessageRoute.h"
#include "Transaction/ExternalTransactionManager.h"
#include "Transaction/ExternalTransaction.h"



namespace SF {
namespace Svr
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	External task class
	//


	ExternalTransactionManager::ExternalTransactionManager()
		:ParallelTransactionManager()
	{
		SetTickInterval(Svr::Const::PARALLEL_TRANSACTION_MANAGER_TICKTASK_INTERVAL);
	}

	ExternalTransactionManager::~ExternalTransactionManager()
	{
	}

	void* ExternalTransactionManager::CURL_malloc(size_t size)
	{
		return GetSystemHeap().Alloc(size);
	}

	void ExternalTransactionManager::CURL_free(void *ptr)
	{
		IHeap::Free(ptr);
	}

	void* ExternalTransactionManager::CURL_realloc(void *ptr, size_t size)
	{
		return GetSystemHeap().Realloc(ptr, size);
	}

	char* ExternalTransactionManager::CURL_strdup(const char *str)
	{
		char *strTarget = nullptr;
		StrUtil::StringDup(GetSystemHeap(), strTarget, str);
		return strTarget;
	}

	void* ExternalTransactionManager::CURL_calloc(size_t nmemb, size_t size)
	{
		size_t totalSize = size*nmemb;
		void *pPtr = GetSystemHeap().Alloc(totalSize);
		if (pPtr != nullptr)
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
		case CURLE_UNSUPPORTED_PROTOCOL:		return ResultCode::SVR_CURL_UNSUPPORTED_PROTOCOL;
		case CURLE_FAILED_INIT:					return ResultCode::SVR_CURL_FAILED_INIT;
		case CURLE_URL_MALFORMAT:				return ResultCode::SVR_CURL_URL_MALFORMAT;
		case CURLE_NOT_BUILT_IN:				return ResultCode::SVR_CURL_NOT_BUILT_IN;
		case CURLE_COULDNT_RESOLVE_PROXY:		return ResultCode::SVR_CURL_COULDNT_RESOLVE_PROXY;
		case CURLE_COULDNT_RESOLVE_HOST:		return ResultCode::SVR_CURL_COULDNT_RESOLVE_HOST;
		case CURLE_COULDNT_CONNECT:				return ResultCode::SVR_CURL_COULDNT_CONNECT;
		case CURLE_HTTP_POST_ERROR:				return ResultCode::SVR_CURL_HTTP_POST_ERROR;
		case CURLE_SSL_ENGINE_NOTFOUND:			return ResultCode::SVR_CURL_SSL_ENGINE_NOTFOUND;
		case CURLE_SSL_ENGINE_SETFAILED:		return ResultCode::SVR_CURL_SSL_ENGINE_SETFAILED;
		case CURLE_SEND_ERROR:					return ResultCode::SVR_CURL_SEND_ERROR;
		case CURLE_RECV_ERROR:					return ResultCode::SVR_CURL_RECV_ERROR;
		case CURLE_USE_SSL_FAILED:				return ResultCode::SVR_CURL_USE_SSL_FAILED;
		case CURLE_NO_CONNECTION_AVAILABLE:		return ResultCode::SVR_CURL_NO_CONNECTION_AVAILABLE;
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

		svrChk(m_GoogleAuth.Authenticate());

	Proc_End:

		return hr;
	}

	Result ExternalTransactionManager::InitializeManagerIOS(const char* strURL, const char* strAltURL)
	{
		Result hr = ResultCode::SUCCESS;

		m_IOSURL = strURL;
		m_IOSAltURL = strAltURL;

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
	Result ExternalTransactionManager::SendGCMNotify( TransactionID sender, const char* strRegisterIDs, const char* strMessage, uint64_t param0 )
	{
		Result hr = ResultCode::SUCCESS;
		GCMHttpExternalTransaction *newTrans = nullptr;

		svrMem( newTrans = new(GetHeap()) GCMHttpExternalTransaction(GetHeap()));

		svrChk( newTrans->SetParameters( strRegisterIDs, strMessage, param0 ) );

		newTrans->SetParentTransID( sender );
		svrChk( newTrans->InitializeTransaction(this) );
		svrChk(EnqueueTransaction(newTrans));
		newTrans = nullptr;

	Proc_End:

		if( !(hr) )
		{
			Util::SafeDelete( newTrans );
		}

		return hr;
	}


	// check receipt
	Result ExternalTransactionManager::AndroidCheckReceipt(TransactionID sender, const char* packageName, const char* productID, const char* purchaseToken)
	{
		Result hr = ResultCode::SUCCESS;
		ExternalTransactionGoogleAndroidReceiptCheck *newTrans = nullptr;

		svrMem(newTrans = new(GetHeap()) ExternalTransactionGoogleAndroidReceiptCheck((GetHeap()), sender, &m_GoogleAuth));

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

	Result ExternalTransactionManager::IOSCheckReceipt(TransactionID sender, const char* packageName, const char* productID, const char* transactionID, const Array<uint8_t>& purchaseToken)
	{
		Result hr = ResultCode::SUCCESS;
		ExternalTransactionIOSRecepitCheck *newTrans = nullptr;

		svrMem(newTrans = new(GetHeap()) ExternalTransactionIOSRecepitCheck((GetHeap()), sender, m_IOSURL, m_IOSAltURL));

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
}; // namespace SF



