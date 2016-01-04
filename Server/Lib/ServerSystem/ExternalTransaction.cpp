////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : External transaction class implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/HRESNet.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ExternalTransaction.h"
#include "ServerSystem/ExternalTransactionManager.h"
#include "ServerSystem/BrServer.h"

#if WINDOWS
#include "zlib/zlib.h"
#else
#include "zlib.h"
#endif


BR_MEMORYPOOL_IMPLEMENT(Svr::GCMHttpExternalTransaction);
BR_MEMORYPOOL_IMPLEMENT(Svr::ExternalTransactionGoogleAndroidReceiptCheck);
BR_MEMORYPOOL_IMPLEMENT(Svr::ExternalTransactionIOSRecepitCheck);


namespace BR {
namespace Svr
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity base class
	//



	HTTPExternalTransaction::HTTPExternalTransaction(TransactionID parentTransID, Message::MessageID MsgID)
		:ParallelTransaction(parentTransID, MsgID)
		, m_Curl(nullptr)
		, m_CurlResult(CURLE_OK)
	{
		m_Curl = curl_easy_init();
		if (m_Curl != nullptr)
		{
			m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_WRITEDATA, &m_HTTPResult);
			m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_WRITEFUNCTION, ResultWriter);
		}
		else
			m_CurlResult = CURLE_FAILED_INIT;

		m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_SSL_VERIFYPEER, false);
		if (m_CurlResult) return;
	}

	int HTTPExternalTransaction::ResultWriter(char *data, size_t size, size_t nmemb, void *param)
	{
		if (param == nullptr) return 0;

		auto *stream = (ResultBuffer*)param;

		stream->Append(size * nmemb, (const char*)data);

		return (int)(size * nmemb);
	}

	HTTPExternalTransaction::~HTTPExternalTransaction()
	{
		if (m_Curl)
		{
			curl_easy_cleanup(m_Curl);
			m_Curl = nullptr;
		}
	}

	HRESULT HTTPExternalTransaction::CloseTransaction(HRESULT hrRes)
	{
		if (m_Curl)
		{
			curl_easy_cleanup(m_Curl);
			m_Curl = nullptr;
		}

		return ParallelTransaction::CloseTransaction(hrRes);
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

		svrChk(HTTPExternalTransaction::InitializeTransaction( pOwner ) );

	Proc_End:

		return hr;
	}

	// Start Transaction
	HRESULT GCMHttpExternalTransaction::StartTransaction()
	{
		HRESULT hr = S_OK;
		char *resultString = nullptr;

		svrChk(HTTPExternalTransaction::StartTransaction() );

		if( m_Headers == nullptr )
		{
			char strAPIKeyOpt[512];
			StrUtil::Format( strAPIKeyOpt, "Authorization: key={0}", stm_strAPIKey );
			svrMem( m_Headers = curl_slist_append( m_Headers, strAPIKeyOpt) );
			svrMem( m_Headers = curl_slist_append( m_Headers, "Content-Type: application/json") );
			svrMem( m_Headers = curl_slist_append( m_Headers, "charsets: utf-8") );
		}

		char strPostFields[1024];

		svrChk( StrUtil::Format( strPostFields, " { \"registration_ids\" :[ \"{0}\"], \"data\": { \"Message\" :  \"{1}\" }, { \"Param\" :  \"{2}\" }   } ", m_strRegID, m_strMessage, m_Param0 ) );

		svrChk(ExternalTransactionManager::ToHRESULT(m_CurlResult));

		m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_POSTFIELDS, strPostFields );
		svrChk(ExternalTransactionManager::ToHRESULT(m_CurlResult));
		m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_POSTFIELDSIZE, strlen(strPostFields) );
		svrChk(ExternalTransactionManager::ToHRESULT(m_CurlResult));
		m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_HTTPHEADER, m_Headers );
		svrChk(ExternalTransactionManager::ToHRESULT(m_CurlResult));

		m_CurlResult = curl_easy_perform(GetCURL());
		svrChk(ExternalTransactionManager::ToHRESULT(m_CurlResult));

		m_CurlResult = curl_easy_getinfo( GetCURL(), CURLINFO_CONTENT_TYPE, &resultString );
		svrChk(ExternalTransactionManager::ToHRESULT(m_CurlResult));

Proc_End:

		if( FAILED(hr) && m_CurlResult != 0 )
		{
			svrTrace( Trace::TRC_ERROR, "GCM query failed by {0}:{1}, result:{2}", (int)m_CurlResult, curl_easy_strerror(m_CurlResult), resultString );
		}

		CloseTransaction(hr);

		return hr;
	}

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//

	const Message::MessageID ExternalTransactionGoogleAndroidReceiptCheck::MID = Message::MessageID(Message::MSGTYPE_COMMAND, Message::MSGTYPE_RELIABLE, Message::MSGTYPE_NONE, POLICY_NONE, ExternalTransactionMesageCode_AndroidCheckReceipt);

	// Constructor
	ExternalTransactionGoogleAndroidReceiptCheck::ExternalTransactionGoogleAndroidReceiptCheck(TransactionID parentTransID, Google::OAuth* pOAuth)
		: ParallelTransaction(parentTransID, MID)
		, m_DevAPI(pOAuth)
	{
		m_strPackageName[0] = '\0';
		m_strProductID[0] = '\0';
		m_strPurchaseToken[0] = '\0';
	}

	// Set parameters
	HRESULT ExternalTransactionGoogleAndroidReceiptCheck::SetParameters(const char* packageName, const char* productID, const char* purchaseToken)
	{
		HRESULT hr = S_OK;

		svrChk(StrUtil::StringCpy(m_strPackageName, packageName));
		svrChk(StrUtil::StringCpy(m_strProductID, productID));
		svrChk(StrUtil::StringCpy(m_strPurchaseToken, purchaseToken));

	Proc_End:

		return hr;
	}


	// Initialize Transaction
	HRESULT ExternalTransactionGoogleAndroidReceiptCheck::InitializeTransaction(Entity* pOwner)
	{
		HRESULT hr = S_OK;

		svrChk(ParallelTransaction::InitializeTransaction(pOwner));

	Proc_End:

		return hr;
	}

	// Start Transaction
	HRESULT ExternalTransactionGoogleAndroidReceiptCheck::StartTransaction()
	{
		HRESULT hr = S_OK, hrTem = S_OK;

		svrChk(ParallelTransaction::StartTransaction());

		hrTem = m_DevAPI.CheckReceipt(m_strPackageName, m_strProductID, m_strPurchaseToken);
		if (hrTem == E_SVR_INVALID_EXTERNAL_AUTH)
		{
			ExternalTransactionManager* pExtMgr = nullptr;

			svrChkPtr(pExtMgr = GetServerComponent<ExternalTransactionManager>());

			svrChk(pExtMgr->UpdateGoogleAuth());

			svrChk(m_DevAPI.CheckReceipt(m_strPackageName, m_strProductID, m_strPurchaseToken));
		}
		else
		{
			hr = hrTem;
		}

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}





	const Message::MessageID ExternalTransactionIOSRecepitCheck::MID = Message::MessageID(Message::MSGTYPE_COMMAND, Message::MSGTYPE_RELIABLE, Message::MSGTYPE_NONE, POLICY_NONE, ExternalTransactionMesageCode_IOSCheckReceipt);

	// Constructor
	ExternalTransactionIOSRecepitCheck::ExternalTransactionIOSRecepitCheck(TransactionID parentTransID, const char* strURL)
		: HTTPExternalTransaction(parentTransID, MID)
		, m_strURL(strURL)
	{
		m_strPackageName[0] = '\0';
		m_strProductID[0] = '\0';
		m_strTransactionID.SetSize(m_strTransactionID.GetAllocatedSize());
		memset(m_strTransactionID.data(), 0, m_strTransactionID.GetSize());
	}

	// Set parameters
	HRESULT ExternalTransactionIOSRecepitCheck::SetParameters(const char* packageName, const char* productID, const char* transactionID, const Array<BYTE>& purchaseToken)
	{
		const char prefix[] = "{\"receipt-data\":\"";
		const char postfix[] = "\"}";
		HRESULT hr = S_OK;

		svrChk(StrUtil::StringCpy(m_strPackageName, packageName));
		svrChk(StrUtil::StringCpy(m_strProductID, productID));
		svrChk(StrUtil::StringCpy((char*)m_strTransactionID.data(), (INT)m_strTransactionID.GetAllocatedSize(), transactionID));

		m_strReceipt.Clear();
		svrChk(m_strReceipt.AddItems(sizeof(prefix)-1, (const BYTE*)prefix));
		svrChk(Util::Base64Encode(purchaseToken.GetSize(), purchaseToken.data(), m_strReceipt, '='));
		svrChk(m_strReceipt.AddItems(sizeof(postfix)-1, (const BYTE*)postfix)); // IOS requires no null terminate string

	Proc_End:

		return hr;
	}


	// Initialize Transaction
	HRESULT ExternalTransactionIOSRecepitCheck::InitializeTransaction(Entity* pOwner)
	{
		HRESULT hr = S_OK;

		svrChk(HTTPExternalTransaction::InitializeTransaction(pOwner));

		if (m_CurlResult != 0)
		{
			svrErr(E_UNEXPECTED);
		}

	Proc_End:

		return hr;
	}

	HRESULT ExternalTransactionIOSRecepitCheck::ToHRESULT(int status)
	{
		HRESULT hr = S_OK;
		const char* reason = "";
		switch (status)
		{
		case 0:
			return S_OK;
		case 21000:				reason = "The App Store could not read the JSON object you provided."; 
			hr = E_SVR_INVALID_PURCHASE_INFO; break;
		case 21002:				reason = "The data in the receipt-data property was malformed or missing."; 
			hr = E_SVR_INVALID_PURCHASE_INFO; break;
		case 21003:				reason = "The receipt could not be authenticated.";
			hr = E_SVR_INVALID_EXTERNAL_AUTH; break;
		case 21004:				reason = "The shared secret you provided does not match the shared secret on file for your account.";
			hr = E_SVR_INVALID_PURCHASE_INFO; break;
		case 21005:				reason = "The receipt server is not currently available.";
			hr = E_SVR_EXTERNAL_SERVER_UNAVALIABLE; break;
		case 21006:				reason = "This receipt is valid but the subscription has expired. When this status code is returned to your server, the receipt data is also decoded and returned as part of the response.";
			hr = E_SVR_PURCHASE_CANCELED; break;
		case 21007:				reason = "This receipt is from the test environment, but it was sent to the production environment for verification. Send it to the test environment instead.";
			hr = E_SVR_INVALID_PURCHASE_INFO; break;
		case 21008:				reason = "This receipt is from the production environment, but it was sent to the test environment for verification. Send it to the production environment instead.";
			hr = E_SVR_INVALID_PURCHASE_INFO; break;
		default:				reason = "Unknown error code";
			hr = E_UNEXPECTED; break;
		}

		svrTrace(Trace::TRC_ERROR, "IOSAuth Failed: {0}", reason );

		return hr;
	}

	HRESULT ExternalTransactionIOSRecepitCheck::VerifyReceipt()
	{
		HRESULT hr = S_OK;

		char *resultString = nullptr;
		curl_slist *headers = nullptr;
		Json::Value root;
		Json::Reader reader;
		bool parsingSuccessful;

		svrChk(HTTPExternalTransaction::StartTransaction());

		svrChkPtr(m_strURL);

		m_HTTPResult.Clear();


		// "https://sandbox.itunes.apple.com/verifyReceipt"
		// À§´Â ºô¸µ Å×½ºÆ®¿ë »÷µå¹Ú½º. https://buy.itunes.apple.com/verifyReceipt ½ÇÀüÀº ¿©±â´Ù.
		m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_POST, 1L);
		svrChk(ExternalTransactionManager::ToHRESULT(m_CurlResult));
		m_CurlResult = curl_easy_setopt(m_Curl, CURLOPT_URL, m_strURL);
		svrChk(ExternalTransactionManager::ToHRESULT(m_CurlResult));

		m_CurlResult = curl_easy_setopt(m_Curl, CURLOPT_POSTFIELDSIZE, m_strReceipt.GetSize());
		svrChk(ExternalTransactionManager::ToHRESULT(m_CurlResult));
		//m_CurlResult = curl_easy_setopt(m_Curl, CURLOPT_COPYPOSTFIELDS, strBuffer);
		m_CurlResult = curl_easy_setopt(m_Curl, CURLOPT_POSTFIELDS, (const char*)m_strReceipt.data());
		svrChk(ExternalTransactionManager::ToHRESULT(m_CurlResult));
		m_CurlResult = curl_easy_setopt(m_Curl, CURLOPT_SSL_VERIFYHOST, 0L);
		svrChk(ExternalTransactionManager::ToHRESULT(m_CurlResult));

		//svrMem(headers = curl_slist_append(headers, "Content-Type: text/plain"));
		svrMem(headers = curl_slist_append(headers, "Content-Type: application/json"));
		m_CurlResult = curl_easy_setopt(m_Curl, CURLOPT_HTTPHEADER, headers);
		svrChk(ExternalTransactionManager::ToHRESULT(m_CurlResult));


		m_CurlResult = curl_easy_perform(GetCURL());
		svrChk(ExternalTransactionManager::ToHRESULT(m_CurlResult));

		m_CurlResult = curl_easy_getinfo(GetCURL(), CURLINFO_CONTENT_TYPE, &resultString);
		svrChk(ExternalTransactionManager::ToHRESULT(m_CurlResult));


		parsingSuccessful = reader.parse((char*)m_HTTPResult.GetPtr(), root);
		if (!parsingSuccessful)
		{
			svrErr(E_UNEXPECTED);
		}

		{
			int status;
			Json::Value receiptInfo;
			Json::Value inAppPurchases;
			std::string bundleID;
			auto value = root.get("status", "");
			if (value.isNull() || value.isInt() == false)
			{
				svrErr(E_SVR_INVALID_PURCHASE_INFO);
			}

			status = value.asInt();
			svrChk(ToHRESULT(status));

			receiptInfo = root.get("receipt", "Invalid");
			if (receiptInfo.isNull() || receiptInfo.isObject() == false)
			{
				svrErr(E_SVR_INVALID_PURCHASE_FORMAT);
			}

			value = receiptInfo.get("bundle_id", "Invalid");
			if (value.isNull() || value.isString() == false)
			{
				svrErr(E_SVR_INVALID_PURCHASE_FORMAT);
			}
			bundleID = std::forward<std::string>(value.asString());

			if (bundleID != m_strPackageName)
			{
				svrErr(E_SVR_INVALID_PURCHASE_INFO);
			}

			inAppPurchases = receiptInfo.get("in_app", "");
			if (inAppPurchases.isNull() || inAppPurchases.isArray() == false)
			{
				svrErr(E_SVR_INVALID_PURCHASE_FORMAT);
			}

			for (auto itPurchase = inAppPurchases.begin(); itPurchase != inAppPurchases.end(); ++itPurchase)
			{
				std::string product_id, original_transaction_id;

				auto& purchase = *itPurchase;
				if (purchase.isObject() == false)
					continue;

				value = purchase.get("product_id", "Invalid");
				if (value.isNull() || value.isString() == false)
				{
					svrErr(E_SVR_INVALID_PURCHASE_FORMAT);
				}
				product_id = std::forward<std::string>(value.asString());

				value = purchase.get("original_transaction_id", "Invalid");
				if (value.isNull() || value.isString() == false)
				{
					svrErr(E_SVR_INVALID_PURCHASE_FORMAT);
				}
				original_transaction_id = std::forward<std::string>(value.asString());

				if (product_id == m_strProductID && original_transaction_id == (char*)m_strTransactionID.data())
				{
					goto Proc_End;
				}
			}

			svrErr(E_SVR_INVALID_PURCHASE_INFO);
		}


	Proc_End:

		if (FAILED(hr) && m_CurlResult != 0)
		{
			svrTrace(Trace::TRC_ERROR, "IOS receipt query failed by {0}:{1}, result:{2}", (int)m_CurlResult, curl_easy_strerror(m_CurlResult), resultString);
		}

		return hr;
	}

	// Start Transaction
	HRESULT ExternalTransactionIOSRecepitCheck::StartTransaction()
	{
		HRESULT hr = S_OK;

		hr = VerifyReceipt();
		if (FAILED(hr))
		{
			// try with sand box url
			m_strURL = "https://sandbox.itunes.apple.com/verifyReceipt";
			hr = VerifyReceipt();
		}


	//Proc_End:

		CloseTransaction(hr);

		return S_OK;
	}






}; // namespace Svr
}; // namespace BR



