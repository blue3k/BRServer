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
#include "String/StrUtil.h"
#include "Util/TimeUtil.h"
#include "ResultCode/SFResultCodeNet.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"
#include "SvrConst.h"
#include "Entity/Entity.h"
#include "Transaction/MessageRoute.h"
#include "Transaction/ExternalTransaction.h"
#include "Transaction/ExternalTransactionManager.h"
#include "Server/BrServer.h"

#include "zlib.h"


SF_MEMORYPOOL_IMPLEMENT(SF::Svr::GCMHttpExternalTransaction);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::ExternalTransactionGoogleAndroidReceiptCheck);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::ExternalTransactionIOSRecepitCheck);


namespace SF {
namespace Svr
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity base class
	//



	HTTPExternalTransaction::HTTPExternalTransaction(IMemoryManager& memMgr, TransactionID parentTransID, Message::MessageID MsgID)
		: ParallelTransaction(memMgr, parentTransID, MsgID)
		, m_Curl(nullptr)
		, m_HTTPResult(memMgr)
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

		stream->AddItems(size * nmemb, (char*)data);

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

	Result HTTPExternalTransaction::CloseTransaction(Result hrRes)
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

	GCMHttpExternalTransaction::GCMHttpExternalTransaction(IMemoryManager& memMgr)
			: HTTPExternalTransaction(memMgr, TransactionID(), Message::MessageID() )
			, m_Headers(nullptr)
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
	Result GCMHttpExternalTransaction::SetParameters( const char* strRegID, const char* strMessage, uint64_t param0 )
	{
		Result hr = ResultCode::SUCCESS;

		svrChkPtr( strRegID );
		svrChkPtr( strMessage );

		svrChk( StrUtil::StringCpy( m_strRegID, strRegID ) );

		svrChk( StrUtil::StringCpy( m_strMessage, strMessage ) );

		m_Param0 = param0;

	Proc_End:

		return hr;
	}

	// Initialize Transaction
	Result GCMHttpExternalTransaction::InitializeTransaction(Entity* pOwner )
	{
		Result hr = ResultCode::SUCCESS;

		if( m_CurlResult != 0 )
		{
			svrErr(ResultCode::UNEXPECTED);
		}

		svrChk(HTTPExternalTransaction::InitializeTransaction( pOwner ) );

	Proc_End:

		return hr;
	}

	// Start Transaction
	Result GCMHttpExternalTransaction::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
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

		StrUtil::Format( strPostFields, " { \"registration_ids\" :[ \"{0}\"], \"data\": { \"Message\" :  \"{1}\" }, { \"Param\" :  \"{2}\" }   } ", m_strRegID, m_strMessage, m_Param0 );

		svrChk(ExternalTransactionManager::ToResult(m_CurlResult));

		m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_POSTFIELDS, strPostFields );
		svrChk(ExternalTransactionManager::ToResult(m_CurlResult));
		m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_POSTFIELDSIZE, strlen(strPostFields) );
		svrChk(ExternalTransactionManager::ToResult(m_CurlResult));
		m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_HTTPHEADER, m_Headers );
		svrChk(ExternalTransactionManager::ToResult(m_CurlResult));

		m_CurlResult = curl_easy_perform(GetCURL());
		svrChk(ExternalTransactionManager::ToResult(m_CurlResult));

		m_CurlResult = curl_easy_getinfo( GetCURL(), CURLINFO_CONTENT_TYPE, &resultString );
		svrChk(ExternalTransactionManager::ToResult(m_CurlResult));

Proc_End:

		if( !(hr) && m_CurlResult != 0 )
		{
			svrTrace( Error, "GCM query failed by {0}:{1}, result:{2}", (int)m_CurlResult, curl_easy_strerror(m_CurlResult), resultString );
		}

		CloseTransaction(hr);

		return hr;
	}

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//

	const Message::MessageID ExternalTransactionGoogleAndroidReceiptCheck::MID = Message::MessageID(Message::MSGTYPE_COMMAND, Message::MSGTYPE_RELIABLE, Message::MSGTYPE_NONE, PROTOCOLID_NONE, ExternalTransactionMesageCode_AndroidCheckReceipt);

	// Constructor
	ExternalTransactionGoogleAndroidReceiptCheck::ExternalTransactionGoogleAndroidReceiptCheck(IMemoryManager& memMgr, TransactionID parentTransID, Google::OAuth* pOAuth)
		: ParallelTransaction(memMgr, parentTransID, MID)
		, m_DevAPI(pOAuth)
	{
		m_strPackageName[0] = '\0';
		m_strProductID[0] = '\0';
		m_strPurchaseToken[0] = '\0';
	}

	// Set parameters
	Result ExternalTransactionGoogleAndroidReceiptCheck::SetParameters(const char* packageName, const char* productID, const char* purchaseToken)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(StrUtil::StringCpy(m_strPackageName, packageName));
		svrChk(StrUtil::StringCpy(m_strProductID, productID));
		svrChk(StrUtil::StringCpy(m_strPurchaseToken, purchaseToken));

	Proc_End:

		return hr;
	}


	// Initialize Transaction
	Result ExternalTransactionGoogleAndroidReceiptCheck::InitializeTransaction(Entity* pOwner)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(ParallelTransaction::InitializeTransaction(pOwner));

	Proc_End:

		return hr;
	}

	// Start Transaction
	Result ExternalTransactionGoogleAndroidReceiptCheck::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS, hrTem = ResultCode::SUCCESS;

		svrChk(ParallelTransaction::StartTransaction());

		hrTem = m_DevAPI.CheckReceipt(m_strPackageName, m_strProductID, m_strPurchaseToken);
		if (hrTem == ((Result)ResultCode::SVR_INVALID_EXTERNAL_AUTH))
		{
			ExternalTransactionManager* pExtMgr = nullptr;

			svrTrace(SVR_INFO, "Google Authentication error, Reauthorizing...");

			svrChkPtr(pExtMgr = GetServerComponent<ExternalTransactionManager>());

			svrChk(pExtMgr->UpdateGoogleAuth());

			// try again
			svrChk(m_DevAPI.CheckReceipt(m_strPackageName, m_strProductID, m_strPurchaseToken));
		}
		else
		{
			hr = hrTem;
		}

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}





	const Message::MessageID ExternalTransactionIOSRecepitCheck::MID = Message::MessageID(Message::MSGTYPE_COMMAND, Message::MSGTYPE_RELIABLE, Message::MSGTYPE_NONE, PROTOCOLID_NONE, ExternalTransactionMesageCode_IOSCheckReceipt);

	// Constructor
	ExternalTransactionIOSRecepitCheck::ExternalTransactionIOSRecepitCheck(IMemoryManager& memMgr, TransactionID parentTransID, const char* strURL, const char* strAltURL)
		: HTTPExternalTransaction(memMgr, parentTransID, MID)
		, m_strURL(strURL)
		, m_strAltURL(strAltURL)
		, m_strReceipt(memMgr)
		, m_strTransactionID(memMgr)
	{
		m_strPackageName[0] = '\0';
		m_strProductID[0] = '\0';
		m_strTransactionID.SetItemCount(m_strTransactionID.GetAllocatedSize());
		memset(m_strTransactionID.data(), 0, m_strTransactionID.size());
	}

	// Set parameters
	Result ExternalTransactionIOSRecepitCheck::SetParameters(const char* packageName, const char* productID, const char* transactionID, const Array<uint8_t>& purchaseToken)
	{
		const char prefix[] = "{\"receipt-data\":\"";
		const char postfix[] = "\"}";
		Result hr = ResultCode::SUCCESS;

		svrChk(StrUtil::StringCpy(m_strPackageName, packageName));
		svrChk(StrUtil::StringCpy(m_strProductID, productID));
		svrChk(StrUtil::StringCpy((char*)m_strTransactionID.data(), (INT)m_strTransactionID.GetAllocatedSize(), transactionID));

		m_strReceipt.Clear();
		svrChk(m_strReceipt.AddItems(sizeof(prefix)-1, (const uint8_t*)prefix));
		svrChk(Util::Base64Encode(purchaseToken.size(), purchaseToken.data(), m_strReceipt, '='));
		svrChk(m_strReceipt.AddItems(sizeof(postfix)-1, (const uint8_t*)postfix)); // IOS requires no null terminate string

	Proc_End:

		return hr;
	}


	// Initialize Transaction
	Result ExternalTransactionIOSRecepitCheck::InitializeTransaction(Entity* pOwner)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(HTTPExternalTransaction::InitializeTransaction(pOwner));

		if (m_CurlResult != 0)
		{
			svrErr(ResultCode::UNEXPECTED);
		}

	Proc_End:

		return hr;
	}

	Result ExternalTransactionIOSRecepitCheck::ToResult(int status)
	{
		Result hr = ResultCode::SUCCESS;
		const char* reason = "";
		switch (status)
		{
		case 0:
			return ResultCode::SUCCESS;
		case 21000:				reason = "The App Store could not read the JSON object you provided."; 
			hr = ResultCode::SVR_INVALID_PURCHASE_INFO; break;
		case 21002:				reason = "The data in the receipt-data property was malformed or missing."; 
			hr = ResultCode::SVR_INVALID_PURCHASE_INFO; break;
		case 21003:				reason = "The receipt could not be authenticated.";
			hr = ResultCode::SVR_INVALID_EXTERNAL_AUTH; break;
		case 21004:				reason = "The shared secret you provided does not match the shared secret on file for your account.";
			hr = ResultCode::SVR_INVALID_PURCHASE_INFO; break;
		case 21005:				reason = "The receipt server is not currently available.";
			hr = ResultCode::SVR_EXTERNAL_SERVER_UNAVALIABLE; break;
		case 21006:				reason = "This receipt is valid but the subscription has expired. When this status code is returned to your server, the receipt data is also decoded and returned as part of the response.";
			hr = ResultCode::SVR_PURCHASE_CANCELED; break;
		case 21007:				reason = "This receipt is from the test environment, but it was sent to the production environment for verification. Send it to the test environment instead.";
			hr = ResultCode::SVR_INVALID_PURCHASE_MODE; break;
		case 21008:				reason = "This receipt is from the production environment, but it was sent to the test environment for verification. Send it to the production environment instead.";
			hr = ResultCode::SVR_INVALID_PURCHASE_MODE; break;
		default:				reason = "Unknown error code";
			hr = ResultCode::UNEXPECTED; break;
		}

		svrTrace(Error, "IOS Purchase check Failed: {0}", reason );

		return hr;
	}

	Result ExternalTransactionIOSRecepitCheck::VerifyReceipt(const char* strURL)
	{
		Result hr = ResultCode::SUCCESS;

		char *resultString = nullptr;
		curl_slist *headers = nullptr;
		Json::Value root;
		Json::Reader reader;
		bool parsingSuccessful;

		svrChkPtr(strURL);

		m_HTTPResult.Clear();


		// "https://sandbox.itunes.apple.com/verifyReceipt"
		// À§´Â ºô¸µ Å×½ºÆ®¿ë »÷µå¹Ú½º. https://buy.itunes.apple.com/verifyReceipt ½ÇÀüÀº ¿©±â´Ù.
		m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_POST, 1L);
		svrChk(ExternalTransactionManager::ToResult(m_CurlResult));
		m_CurlResult = curl_easy_setopt(m_Curl, CURLOPT_URL, strURL);
		svrChk(ExternalTransactionManager::ToResult(m_CurlResult));

		m_CurlResult = curl_easy_setopt(m_Curl, CURLOPT_POSTFIELDSIZE, m_strReceipt.size());
		svrChk(ExternalTransactionManager::ToResult(m_CurlResult));
		//m_CurlResult = curl_easy_setopt(m_Curl, CURLOPT_COPYPOSTFIELDS, strBuffer);
		m_CurlResult = curl_easy_setopt(m_Curl, CURLOPT_POSTFIELDS, (const char*)m_strReceipt.data());
		svrChk(ExternalTransactionManager::ToResult(m_CurlResult));
		m_CurlResult = curl_easy_setopt(m_Curl, CURLOPT_SSL_VERIFYHOST, 0L);
		svrChk(ExternalTransactionManager::ToResult(m_CurlResult));

		//svrMem(headers = curl_slist_append(headers, "Content-Type: text/plain"));
		svrMem(headers = curl_slist_append(headers, "Content-Type: application/json"));
		m_CurlResult = curl_easy_setopt(m_Curl, CURLOPT_HTTPHEADER, headers);
		svrChk(ExternalTransactionManager::ToResult(m_CurlResult));


		m_CurlResult = curl_easy_perform(GetCURL());
		svrChk(ExternalTransactionManager::ToResult(m_CurlResult));

		m_CurlResult = curl_easy_getinfo(GetCURL(), CURLINFO_CONTENT_TYPE, &resultString);
		svrChk(ExternalTransactionManager::ToResult(m_CurlResult));


		parsingSuccessful = reader.parse((char*)m_HTTPResult.data(), root);
		if (!parsingSuccessful)
		{
			svrErr(ResultCode::UNEXPECTED);
		}

		{
			int status;
			Json::Value receiptInfo;
			Json::Value inAppPurchases;
			std::string bundleID;
			auto value = root.get("status", "");
			if (value.isNull() || value.isInt() == false)
			{
				svrErr(ResultCode::SVR_INVALID_PURCHASE_INFO);
			}

			status = value.asInt();
			svrChk(ToResult(status));

			receiptInfo = root.get("receipt", "Invalid");
			if (receiptInfo.isNull() || receiptInfo.isObject() == false)
			{
				svrErr(ResultCode::SVR_INVALID_PURCHASE_FORMAT);
			}

			value = receiptInfo.get("bundle_id", "Invalid");
			if (value.isNull() || value.isString() == false)
			{
				svrErr(ResultCode::SVR_INVALID_PURCHASE_FORMAT);
			}
			bundleID = std::forward<std::string>(value.asString());

			if (bundleID != m_strPackageName)
			{
				svrErr(ResultCode::SVR_INVALID_PURCHASE_INFO);
			}

			inAppPurchases = receiptInfo.get("in_app", "");
			if (inAppPurchases.isNull() || inAppPurchases.isArray() == false)
			{
				svrErr(ResultCode::SVR_INVALID_PURCHASE_FORMAT);
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
					svrErr(ResultCode::SVR_INVALID_PURCHASE_FORMAT);
				}
				product_id = std::forward<std::string>(value.asString());

				value = purchase.get("original_transaction_id", "Invalid");
				if (value.isNull() || value.isString() == false)
				{
					svrErr(ResultCode::SVR_INVALID_PURCHASE_FORMAT);
				}
				original_transaction_id = std::forward<std::string>(value.asString());

				if (product_id == m_strProductID && original_transaction_id == (char*)m_strTransactionID.data())
				{
					goto Proc_End;
				}
			}

			svrErr(ResultCode::SVR_INVALID_PURCHASE_INFO);
		}


	Proc_End:

		if (!(hr) && m_CurlResult != 0)
		{
			svrTrace(Error, "IOS receipt query failed by {0}:{1}, result:{2}", (int)m_CurlResult, curl_easy_strerror(m_CurlResult), resultString);
		}

		return hr;
	}

	// Start Transaction
	Result ExternalTransactionIOSRecepitCheck::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());


		hr = VerifyReceipt(m_strURL);
		if (hr == Result(ResultCode::SVR_INVALID_PURCHASE_MODE))
		{
			if (m_strAltURL != nullptr)
			{
				svrTrace(SVR_INFO, "IOS receipt query failed with invalid environment retrying with alt url");
				// try with sand box url
				//m_strURL = "https://sandbox.itunes.apple.com/verifyReceipt";
				hr = VerifyReceipt(m_strAltURL);
			}
		}


	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}






}; // namespace Svr
}; // namespace SF



