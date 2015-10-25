////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : KyungKun Ko
//
// Description : Google AndroidDeveloperAPI
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Common/Utility.h"
#include "Common/TimeUtil.h"
#include "Common/HRESSvrSys.h"
#include "ServerSystem/SvrTrace.h"

#include "curl/curl.h"
#include "json/json.h"

#include <openssl/err.h>
#include <openssl/pkcs12.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>

#include "ServerSystem/Google/GoogleOAuth.h"
#include "ServerSystem/Google/GoogleAndroidDeveloperAPI.h"


namespace BR {
namespace Svr {
namespace Google {


	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	AndroidDeveloperAPI class
	//

	AndroidDeveloperAPI::AndroidDeveloperAPI(OAuth* pOAuth)
		: m_OAuth(pOAuth)
	{
	}

	AndroidDeveloperAPI::~AndroidDeveloperAPI()
	{
	}

	int AndroidDeveloperAPI::WriteResultCB(char *data, size_t size, size_t nmemb, void *param)
	{
		if (param == nullptr) return 0;

		auto stream = (Array<char>*)param;

		stream->AddItems(size * nmemb, (const char*)data);
		return (int)(size * nmemb);
	}

	HRESULT AndroidDeveloperAPI::ToHRESULT(const Json::Value& root)
	{
		HRESULT hr = S_OK;
		Json::Value errorValue, errors;

		if (root.isNull())
			svrErr(E_UNEXPECTED);

		errorValue = root.get("error", "Invalid");
		if (errorValue.isNull() || errorValue.isObject() != true)
			svrErr(E_UNEXPECTED);

		errors = errorValue.get("errors", "Invalid");
		if (errors.isNull() || errors.isArray() != true)
			svrErr(E_UNEXPECTED);

		for (auto itError = errors.begin(); itError != errors.end(); ++itError)
		{
			std::string reason;
			Json::Value reasonValue;
			auto& errObj = *itError;
			if (errObj.isObject() == false)
				continue;

			reasonValue = errObj.get("reason", "Invalid");
			if (reasonValue.isNull() || reasonValue.isString() == false)
				continue;

			reason = std::forward<std::string>(reasonValue.asString());
			if (reason == "authError")
			{
				hr = E_SVR_INVALID_EXTERNAL_AUTH;
			}
			else
			{
				hr = E_SVR_INVALID_PURCHASE_INFO;
			}
			goto Proc_End;
		}

		hr = E_SVR_INVALID_PURCHASE_INFO;

	Proc_End:


		return hr;
	}

	// Check purchase receipt
	HRESULT AndroidDeveloperAPI::CheckReceipt(const char* packageName, const char* productID, const char* purchaseToken)
	{
		HRESULT hr = S_OK;
		const char* urlFormat = "https://www.googleapis.com/androidpublisher/v2/applications/%0%/purchases/products/%1%/tokens/%2%";
		CURL *curl = nullptr;
		char strRequest[2048];
		CURLcode res;
		struct curl_slist *headers = nullptr; // init to NULL is important 
		Json::Value root;
		Json::Reader reader;
		const char* authChar = m_OAuth->GetAuthString();

		m_ResultBuffer.Clear();

		svrChk(StrUtil::Format(strRequest, urlFormat, packageName, productID, purchaseToken));

		svrMem(curl = curl_easy_init());

		svrChkPtr(authChar);

		res = curl_easy_setopt(curl, CURLOPT_URL, strRequest);
		if (res != CURLE_OK)
			svrErr(E_UNEXPECTED);
		Assert(res == 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

		headers = curl_slist_append(headers, authChar);
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		if (res != CURLE_OK)
			svrErr(E_UNEXPECTED);

		res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteResultCB);
		if (res != CURLE_OK)
			svrErr(E_UNEXPECTED);

		res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m_ResultBuffer);
		if (res != CURLE_OK)
			svrErr(E_UNEXPECTED);
		Assert(res == 0);

		res = curl_easy_perform(curl); /* ignores error */
		if (res != CURLE_OK)
			svrErr(E_UNEXPECTED);
		Assert(res == 0);

		char *ct = nullptr;
		res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
		if (res != CURLE_OK)
			svrErr(E_UNEXPECTED);
		Assert(res == 0);

		curl_easy_cleanup(curl);

		svrChk(m_ResultBuffer.push_back('\0'));

		//const char* testString = "{ \n\"error\": {\n"
		//	"\"errors\": [\n"
		//	"{\n"
		//	"	\"domain\": \"global\",\n"
		//	"			\"reason\" : \"authError\",\n"
		//	"			\"message\" : \"Invalid Credentials\",\n"
		//	"			\"locationType\" : \"header\",\n"
		//	"			\"location\" : \"Authorization\"\n"
		//	"	}\n"
		//	"	],\n"
		//	"		\"code\": 401,\n"
		//	"		\"message\" : \"Invalid Credentials\"\n"
		//	"} \n }\n";
		//bool parsingSuccessful = reader.parse((char*)testString, root);
		bool parsingSuccessful = reader.parse((char*)m_ResultBuffer.data(), root);
		if (!parsingSuccessful)
		{
			// report to the user the failure and their locations in the document.
			//std::cout << "Failed to parse configuration\n"
			//	<< reader.getFormatedErrorMessages();
			svrErr(ToHRESULT(root));
		}

		{
			int purchaseState;
			int consumptionState;

			auto value = root.get("purchaseState", "");
			if (value.isNull() || value.isInt() == false)
			{
				hr = ToHRESULT(root);
				goto Proc_End;
			}

			purchaseState = value.asInt();

			value = root.get("consumptionState", "");
			if (value.isNull() || value.isInt() == false)
			{
				hr = ToHRESULT(root);
				goto Proc_End;
			}

			consumptionState = value.asInt();

			value = root.get("developerPayload", "");
			if (value.isNull() || value.isString() == false)
			{
				hr = ToHRESULT(root);
				goto Proc_End;
			}

			m_DeveloperPayload = std::forward<std::string>(value.asString());
			if (m_DeveloperPayload.length() == 0)
			{
				hr = ToHRESULT(root);
				goto Proc_End;
			}

			if (purchaseState == 1)
				return E_SVR_PURCHASE_CANCELED;
		}


	Proc_End:

		if (FAILED(hr))
		{
			if (hr == E_SVR_INVALID_EXTERNAL_AUTH)
			{
				// silently ignore it
				//svrTrace(Trace::TRC_ERROR, "Invalid auth: hr:%0%, %1%, token:%2%", ArgHex32<UINT32>(hr), (const char*)m_ResultBuffer.data(), authChar);
				return hr;
			}
			else
			{
				if (m_ResultBuffer.GetSize() > 0)
				{
					m_ResultBuffer.push_back('\0');
					svrTrace(Trace::TRC_ERROR, "Invalid purchase status: hr:%0%, %1%, token:%2%, payload:%3%", ArgHex32<UINT32>(hr), (const char*)m_ResultBuffer.data(), authChar, m_DeveloperPayload);
				}
				else
				{
					svrTrace(Trace::TRC_ERROR, "Failed to check purchase status: hr:%0%, token:%1%, payload:%2%", ArgHex32<UINT32>(hr), authChar, m_DeveloperPayload);
				}
			}
		}

		return hr;
	}



}; // namespace Google
}; // namespace Svr
}; // namespace BR
