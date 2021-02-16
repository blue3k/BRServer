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



#include "ServerSystemPCH.h"
#include "Util/SFStrUtil.h"
#include "Util/SFUtility.h"
#include "Util/SFTimeUtil.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "SvrTrace.h"

#include "curl/curl.h"
#include "json/json.h"

#include "ServerSystem/Google/GoogleOAuth.h"
#include "ServerSystem/Google/GoogleAndroidDeveloperAPI.h"


namespace SF {
namespace Svr {
namespace Google {


	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	AndroidDeveloperAPI class
	//

	AndroidDeveloperAPI::AndroidDeveloperAPI(OAuth* pOAuth)
		: m_OAuth(pOAuth)
		, m_ResultBuffer(GetHeap())
	{
	}

	AndroidDeveloperAPI::~AndroidDeveloperAPI()
	{
	}

	int AndroidDeveloperAPI::WriteResultCB(char *data, size_t size, size_t nmemb, void *param)
	{
		if (param == nullptr) return 0;

		auto stream = (Array<char>*)param;

		stream->push_back(size * nmemb, (const char*)data);
		return (int)(size * nmemb);
	}

	Result AndroidDeveloperAPI::ToResult(const Json::Value& root)
	{
		Result hr = ResultCode::SUCCESS;
		Json::Value errorValue, errors;

		if (root.isNull())
			svrErr(ResultCode::UNEXPECTED);

		errorValue = root.get("error", "Invalid");
		if (errorValue.isNull() || errorValue.isObject() != true)
			svrErr(ResultCode::UNEXPECTED);

		errors = errorValue.get("errors", "Invalid");
		if (errors.isNull() || errors.isArray() != true)
			svrErr(ResultCode::UNEXPECTED);

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
				hr = ResultCode::SVR_INVALID_EXTERNAL_AUTH;
			}
			else
			{
				hr = ResultCode::SVR_INVALID_PURCHASE_INFO;
			}
			goto Proc_End;
		}

		hr = ResultCode::SVR_INVALID_PURCHASE_INFO;

	Proc_End:


		return hr;
	}


	int debug_cb(CURL *handle, curl_infotype type, char *data, size_t size, void *userp)
	{
		switch (type) {
		case CURLINFO_TEXT: {
			printf("INFO: %s\n", data);
			break;
		}
		case CURLINFO_HEADER_IN: {
			printf("RESPONSE: %s", data);
			break;
		}
		case CURLINFO_HEADER_OUT: { /* There is no null-terminator on this one ! */
			size_t i;
			printf("REQUEST: \n");
			for (i = 0; i < size; i++) printf("%c", data[i]);
			break;
		}
		case CURLINFO_DATA_IN: {
			printf("RECIEVED: %d bytes\n", (int)size);
			break;
		}
		case CURLINFO_DATA_OUT: {
			printf("TRANSMIT: %d bytes\n", (int)size);
			break;
		}
		case CURLINFO_END: {
			printf("This should never happen!");
			break;
		default:
			break;
		}
		}

		return 0;
	}

	// Check purchase receipt
	Result AndroidDeveloperAPI::CheckReceipt(const char* packageName, const char* productID, const char* purchaseToken)
	{
		const char* authChar = m_OAuth->GetAuthString();
		std::string errs;

		ScopeContext hr([this, authChar, &errs](Result result)
		{
			if (result == ((Result)ResultCode::SVR_INVALID_EXTERNAL_AUTH))
			{
				// silently ignore it
			}
			else if (!result)
			{
				if (m_ResultBuffer.size() > 0)
				{
					m_ResultBuffer.push_back('\0');
					svrTrace(Error, "Invalid purchase status: hr:{0:X8}, {1}, token:{2}, payload:{3}, error:{4}", result, (const char*)m_ResultBuffer.data(), authChar, m_DeveloperPayload, errs);
				}
				else
				{
					svrTrace(Error, "Failed to check purchase status: hr:{0:X8}, token:{1}, payload:{2}, error:{4}", result, authChar, m_DeveloperPayload, errs);
				}
			}
		});

		const char* urlFormat = "https://www.googleapis.com/androidpublisher/v2/applications/{0}/purchases/products/{1}/tokens/{2}";
		CURL *curl = nullptr;
		char strRequest[2048];
		CURLcode res;
		struct curl_slist *headers = nullptr; // init to NULL is important 
		Json::Value root;
		Json::CharReaderBuilder jsonReader;
		bool parsingSuccessful;
		char *ct = nullptr;

		m_ResultBuffer.Clear();

		StrUtil::Format(strRequest, urlFormat, packageName, productID, purchaseToken);

		svrCheckMem(curl = curl_easy_init());

		svrCheckPtr(authChar);

		res = curl_easy_setopt(curl, CURLOPT_URL, strRequest);
		if (res != CURLE_OK)
			svrCheck(ResultCode::UNEXPECTED);
		Assert(res == 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

		headers = curl_slist_append(headers, authChar);
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		if (res != CURLE_OK)
			svrCheck(ResultCode::UNEXPECTED);

		res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteResultCB);
		if (res != CURLE_OK)
			svrCheck(ResultCode::UNEXPECTED);

		res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m_ResultBuffer);
		if (res != CURLE_OK)
			svrCheck(ResultCode::UNEXPECTED);
		Assert(res == 0);

		res = curl_easy_perform(curl); /* ignores error */
		if (res != CURLE_OK)
			svrCheck(ResultCode::UNEXPECTED);
		Assert(res == 0);

		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, debug_cb);

		res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
		if (res != CURLE_OK)
			svrCheck(ResultCode::UNEXPECTED);
		Assert(res == 0);

		curl_easy_cleanup(curl);

		svrCheck(m_ResultBuffer.push_back('\0'));

		std::stringstream inputStream(std::string(reinterpret_cast<const char*>(m_ResultBuffer.data()), m_ResultBuffer.size()), std::ios_base::in);
		parsingSuccessful = Json::parseFromStream(jsonReader, inputStream, &root, &errs);
		if (!parsingSuccessful)
		{
			// report to the user the failure and their locations in the document.
			//std::cout << "Failed to parse configuration\n"
			//	<< reader.getFormatedErrorMessages();
			svrCheck(ToResult(root));
		}

		{
			int purchaseState;
			//int consumptionState;

			auto value = root.get("purchaseState", "");
			if (value.isNull() || value.isInt() == false)
				return hr = ToResult(root);

			purchaseState = value.asInt();

			value = root.get("consumptionState", "");
			if (value.isNull() || value.isInt() == false)
				return hr = ToResult(root);

			value = root.get("developerPayload", "");
			if (value.isNull() || value.isString() == false)
				return hr = ToResult(root);

			m_DeveloperPayload = std::forward<std::string>(value.asString());
			if (m_DeveloperPayload.length() == 0)
				return hr = ToResult(root);

			if (purchaseState == 1)
				return hr = ResultCode::SVR_PURCHASE_CANCELED;
		}


		return hr;
	}



}; // namespace Google
}; // namespace Svr
}; // namespace SF

