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


#include "StdAfx.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/HRESNet.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ExternalTask.h"


BR_OBJECTPOOL_IMPLEMENT(Svr::GCMHttpExternalTransaction);

namespace BR {
namespace Svr
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity base class
	//

	MemoryAllocator &HTTPExternalTransaction::m_Allocator(STDAllocator::GetInstance());
	
	void* HTTPExternalTransaction::CURL_malloc(size_t size)
	{
		void *pPtr = nullptr;
		m_Allocator.Alloc( size, pPtr );
		return pPtr;
	}

	void HTTPExternalTransaction::CURL_free(void *ptr)
	{
		m_Allocator.Free( ptr );
	}

	void* HTTPExternalTransaction::CURL_realloc(void *ptr, size_t size)
	{
		m_Allocator.Realloc(size, ptr );
		return ptr;
	}

	char* HTTPExternalTransaction::CURL_strdup(const char *str)
	{
		char *strTarget = nullptr;
		StrUtil::StringDup( strTarget, str );
		return strTarget;
	}

	void* HTTPExternalTransaction::CURL_calloc(size_t nmemb, size_t size)
	{
		size_t totalSize = size*nmemb;
		void *pPtr = nullptr;
		if( SUCCEEDED(m_Allocator.Alloc( totalSize, pPtr )) )
		{
			memset(pPtr, 0, totalSize);
		}
		return pPtr;
	}

	int HTTPExternalTransaction::ResultWriter(char *data, size_t size, size_t nmemb, void *param)
	{
		if( param == nullptr ) return 0;

		HTTPExternalTransaction::ResultBuffer *stream = (HTTPExternalTransaction::ResultBuffer*)param;

		stream->Append( size * nmemb, (const char*)data );

		return (int)(size * nmemb);
	}

	
	// convert CURL error to HRESULT
	HRESULT HTTPExternalTransaction::ToHRESULT( CURLcode code )
	{
		switch( code )
		{
		case CURLE_OK:							return S_OK;
		case CURLE_UNSUPPORTED_PROTOCOL:		return E_CURLE_UNSUPPORTED_PROTOCOL;
		case CURLE_FAILED_INIT:					return E_CURLE_FAILED_INIT;
		case CURLE_URL_MALFORMAT:				return E_CURLE_URL_MALFORMAT;
		case CURLE_NOT_BUILT_IN:				return E_CURLE_NOT_BUILT_IN;
		case CURLE_COULDNT_RESOLVE_PROXY:		return E_CURLE_COULDNT_RESOLVE_PROXY;
		case CURLE_COULDNT_RESOLVE_HOST:		return E_CURLE_COULDNT_RESOLVE_HOST;
		case CURLE_COULDNT_CONNECT:				return E_CURLE_COULDNT_CONNECT;
		case CURLE_HTTP_POST_ERROR:				return E_CURLE_HTTP_POST_ERROR;
		case CURLE_SSL_ENGINE_NOTFOUND:			return E_CURLE_SSL_ENGINE_NOTFOUND;
		case CURLE_SSL_ENGINE_SETFAILED:		return E_CURLE_SSL_ENGINE_SETFAILED;
		case CURLE_SEND_ERROR:					return E_CURLE_SEND_ERROR;
		case CURLE_RECV_ERROR:					return E_CURLE_RECV_ERROR;
		case CURLE_USE_SSL_FAILED:				return E_CURLE_USE_SSL_FAILED;
		case CURLE_NO_CONNECTION_AVAILABLE:		return E_CURLE_NO_CONNECTION_AVAILABLE;
		default:
			return E_UNEXPECTED;
		};
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GCM External task class
	//


	const char* GCMHttpExternalTransaction::stm_URL= "https://android.googleapis.com/gcm/send";
	const char* GCMHttpExternalTransaction::stm_strAPIKey= "AIzaSyBfTbM73k-MxPQtzQvKDHm1z-q0Q8T1KBA";

	GCMHttpExternalTransaction::GCMHttpExternalTransaction()
			:HTTPExternalTransaction( TransactionID(), Message::MessageID() )
			,m_Headers(nullptr)
	{
		m_strRegID[0] = 0;
		m_strMessage[0] = 0;

		if( GetCURL() )
		{
			m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_URL, stm_URL);
			if( m_CurlResult ) return;
			m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_SSL_VERIFYPEER, false);
			if( m_CurlResult ) return;
			m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_POST, 1L);
			if( m_CurlResult ) return;
		}
		else
		{
			m_CurlResult = CURLE_FAILED_INIT;
		}
	}

	
	// Set parameters
	HRESULT GCMHttpExternalTransaction::SetParameters( const char* strRegID, const char* strMessage, UINT64 param0 )
	{
		HRESULT hr = S_OK;

		svrChkPtr( strRegID );
		svrChkPtr( strMessage );

		svrChk( StrUtil::StringCpy( m_strRegID, strRegID ) );

		svrChk( StrUtil::StringCpy( m_strMessage, strMessage ) );

		m_Param0 = param0;

	Proc_End:

		return hr;
	}

	// Initialize Transaction
	HRESULT GCMHttpExternalTransaction::InitializeTransaction( Entity* pOwner )
	{
		HRESULT hr = S_OK;

		if( m_CurlResult != 0 )
		{
			svrErr(E_UNEXPECTED);
		}

		svrChk( __super::InitializeTransaction( pOwner ) );

	Proc_End:

		return hr;
	}

	// Start Transaction
	HRESULT GCMHttpExternalTransaction::StartTransaction()
	{
		HRESULT hr = S_OK;
		char *resultString = nullptr;

		svrChk( __super::StartTransaction() );

		if( m_Headers == nullptr )
		{
			char strAPIKeyOpt[512];
			StrUtil::Format( strAPIKeyOpt, "Authorization: key=%0%", stm_strAPIKey );
			svrMem( m_Headers = curl_slist_append( m_Headers, strAPIKeyOpt) );
			svrMem( m_Headers = curl_slist_append( m_Headers, "Content-Type: application/json") );
			svrMem( m_Headers = curl_slist_append( m_Headers, "charsets: utf-8") );
		}

		char strPostFields[1024];

		svrChk( StrUtil::Format( strPostFields, " { \"registration_ids\" :[ \"%0%\"], \"data\": { \"Message\" :  \"%1%\" }, { \"Param\" :  \"%2%\" }   } ", m_strRegID, m_strMessage, m_Param0 ) );

		svrChk( ToHRESULT(m_CurlResult) );

		m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_POSTFIELDS, strPostFields );
		svrChk( ToHRESULT(m_CurlResult) );
		m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_POSTFIELDSIZE, strlen(strPostFields) );
		svrChk( ToHRESULT(m_CurlResult) );
		m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_HTTPHEADER, m_Headers );
		svrChk( ToHRESULT(m_CurlResult) );

		m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_WRITEDATA, &m_HTTPResult);
		svrChk( ToHRESULT(m_CurlResult) );
		m_CurlResult = curl_easy_perform(GetCURL());
		svrChk( ToHRESULT(m_CurlResult) );

		m_CurlResult = curl_easy_getinfo( GetCURL(), CURLINFO_CONTENT_TYPE, &resultString );
		svrChk( ToHRESULT(m_CurlResult) );

Proc_End:


		if( FAILED(hr) && m_CurlResult != 0 )
		{
			svrTrace( Trace::TRC_ERROR, "GCM query failed by %0%:%1%, result:%2%", (int)m_CurlResult, curl_easy_strerror(m_CurlResult), resultString );
		}

		CloseTransaction(hr);

		return hr;
	}

	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	External task class
	//


	ExternalTransactionManager::TaskWorker::TaskWorker( PageQueue<ExternalTransaction*> &querieQueue )
		:m_QuerieQueue(querieQueue)
	{
	}

	bool ExternalTransactionManager::TaskWorker::Run()
	{
		const ULONG desiredLoopInterval = 500;
		
		while( 1 )
		{
			ExternalTransaction* pTransaction = nullptr;
			ULONG loopInterval = UpdateInterval( desiredLoopInterval );

			//DWORD dwWaitRes = WaitForSingleObject( GetKillEvent(), loopInterval );
			//if( dwWaitRes == WAIT_OBJECT_0 ) 
			if (CheckKillEvent(loopInterval))
			{
				// Kill Event signaled
				break;
			}

			if( SUCCEEDED(m_QuerieQueue.DequeueMT( pTransaction, desiredLoopInterval )) )
			{
				HRESULT hr = pTransaction->StartTransaction();
				if( FAILED(hr) )
				{
					svrTrace( Trace::TRC_ERROR, "Transaction Failed hr = 0x%0%, %1%", ArgHex32(hr), typeid(*pTransaction).name() );
					break;
				}

				if( pTransaction->IsClosed() )
					pTransaction->FlushTransaction();
				else
				{
					svrTrace( Trace::TRC_ERROR, "Transaction must be closed hr = 0x%0%, %1%", ArgHex32(hr), typeid(*pTransaction).name() );
					pTransaction->CloseTransaction(S_OK);
					pTransaction->FlushTransaction();
				}
			}
		}

		return true;
	}


	ExternalTransactionManager::ExternalTransactionManager()
		:SimpleEntity(0,0)
		,IServerComponent(ComponentID)
	{
	}

	ExternalTransactionManager::~ExternalTransactionManager()
	{
	}

	// Initialize Manager
	HRESULT ExternalTransactionManager::InitializeComponent()
	{
		HRESULT hr = S_OK;

		// Initialize HTTP interface
		CURLcode res = curl_global_init_mem( CURL_GLOBAL_ALL, HTTPExternalTransaction::CURL_malloc, HTTPExternalTransaction::CURL_free, HTTPExternalTransaction::CURL_realloc, HTTPExternalTransaction::CURL_strdup, HTTPExternalTransaction::CURL_calloc );
		if( res != 0 ) svrErr(E_UNEXPECTED);

		for( int iworker = 0; iworker < 20; iworker++ )
		{
			TaskWorker *pWorker = new TaskWorker(m_PendingQueries);
			pWorker->Start();
			m_QueryWorker.push_back(pWorker);
		}

	Proc_End:


		return hr;
	}

	// Terminate Manager
	void ExternalTransactionManager::TerminateComponent()
	{
		for( size_t iworker = 0; iworker < m_QueryWorker.size(); iworker++ )
		{
			TaskWorker *pWorker = m_QueryWorker[iworker];
			// Just kill the thread
			pWorker->Stop(false);
		}

		m_QueryWorker.clear();
	}


	// Send gcm notify
	HRESULT ExternalTransactionManager::SendGCMNotify( TransactionID sender, const char* strRegisterIDs, const char* strMessage, UINT64 param0 )
	{
		HRESULT hr = S_OK;
		GCMHttpExternalTransaction *newTrans = nullptr;

		svrMem( newTrans = GCMHttpExternalTransaction::AllocateObject() );

		svrChk( newTrans->SetParameters( strRegisterIDs, strMessage, param0 ) );

		newTrans->SetParentTransID( sender );
		svrChk( newTrans->InitializeTransaction(this) );
		svrChk( m_PendingQueries.Enqueue(newTrans) );

	Proc_End:

		if( FAILED(hr) )
		{
			Util::SafeRelease( newTrans );
		}

		return hr;
	}




}; // namespace Svr
}; // namespace BR



