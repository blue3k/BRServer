////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : KyungKun Ko
//
// Description : Google Authentication
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Common/Utility.h"
#include "Common/TimeUtil.h"
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


namespace BR {
namespace Svr {
namespace Google {


	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	OAuth class
	//

	OAuth::OAuth()
		: m_privateKey(nullptr)
		, m_AuthenticatedTime(TimeStampMS::min())
		, m_ActiveAuthString(nullptr)
		, m_AuthStringIndex(0)
	{
		m_ActiveAuthString = m_AuthString[m_AuthStringIndex];
		memset(m_AuthString, 0, sizeof m_AuthString);
	}

	OAuth::~OAuth()
	{
		if (m_privateKey != nullptr)
			EVP_PKEY_free(m_privateKey);
	}

	HRESULT OAuth::LoadPrivateKey(const char* strPKeyFile)
	{
		HRESULT hr = S_OK;

		if (m_privateKey != nullptr)
			return S_FALSE;

		// load all open ssl algorithms
		OpenSSL_add_all_algorithms();

		// ssl signing
		// Load key file
		FILE *fp = nullptr;
		fopen_s(&fp, strPKeyFile, "rb");
		PKCS12 * p12 = d2i_PKCS12_fp(fp, nullptr);
		X509 *cert = nullptr;
		STACK_OF(X509) *ca = nullptr;

		auto sslResult = PKCS12_verify_mac(p12, GoogleDefaultPassword, (int)strlen(GoogleDefaultPassword));
		if (sslResult == FALSE) // invalid password
			svrErr(ERROR_INVALID_PASSWORD);

		sslResult = PKCS12_parse(p12, GoogleDefaultPassword, &m_privateKey, &cert, &ca);
		if (sslResult == FALSE)
		{
			char errorMessage[512];
			auto error = ERR_get_error();
			ERR_error_string(error, errorMessage);
			svrErr(E_UNEXPECTED);
		}

	Proc_End:

		if (fp != nullptr)
			fclose(fp);

		if (ca != nullptr)
			sk_X509_pop_free(ca, X509_free);

		if (cert != nullptr)
			X509_free(cert);

		if (p12)
			PKCS12_free(p12);

		return hr;
	}

	HRESULT OAuth::BuildAuthRequestString(const char* strAccount, const char* scopes, Array<BYTE>& requestString)
	{
		HRESULT hr = S_OK;

		StaticArray<BYTE, 128> digest;
		int sslResult = TRUE;
		BYTE sign_buffer[512];
		UINT sign_len = _countof(sign_buffer);

		const char header[] = "{\"alg\":\"RS256\",\"typ\":\"JWT\"}";
		const char bodyFormat[] =
			"{{"
			"\"iss\":\"{0}\","
			"\"scope\":\"{1}\"," // https://www.googleapis.com/auth/androidpublisher
			"\"aud\":\"https://www.googleapis.com/oauth2/v3/token\","
			"\"exp\":{2},"
			"\"iat\":{2}"
			"}}";
		char body[1024];

		if (m_privateKey == nullptr
			|| strAccount == nullptr
			|| scopes == nullptr)
			return E_FAIL;

		RSA* pkey = m_privateKey->pkey.rsa;
		if (pkey == nullptr)
			return E_UNEXPECTED;


		// 1hour later from now
		time_t timeToRequest = time(0) + 3;
		StrUtil::Format(body, bodyFormat, strAccount, scopes, timeToRequest);


		//////////////////////////////////////////////////////////////////////
		// Combine hashed strings

		svrChk(Util::Base64URLEncode(_countof(header) - 1, (const BYTE*)header, requestString));
		svrChk(requestString.push_back('.'));
		svrChk(Util::Base64URLEncode((INT)strlen(body), (const BYTE*)body, requestString));

		//////////////////////////////////////////////////////////////////////
		// signing

		// SHA256
		svrChk(Util::SHA256Hash(requestString.GetSize(), requestString.data(), digest));

		// RSA sign
		sslResult = RSA_sign(NID_sha256, digest.data(), (UINT)digest.GetSize(), sign_buffer, &sign_len, pkey);
		if (sslResult == FALSE)
		{
			svrErr(E_UNEXPECTED);
		}

		svrChk(requestString.push_back('.'));

		svrChk(Util::Base64URLEncode(sign_len, sign_buffer, requestString));

		svrChk(requestString.push_back('\0')); // append null

	Proc_End:

		return hr;
	}

	HRESULT OAuth::Initialize(const char* strPKeyFile, const char* strAccount, const char* scopes)
	{
		HRESULT hr = S_OK;

		if (strAccount == nullptr || scopes == nullptr)
			return E_POINTER;

		m_Account = strAccount;
		m_Scopes = scopes;

		svrChk(LoadPrivateKey(strPKeyFile));


	Proc_End:

		return hr;
	}

	int OAuth::WriteResultCB(char *data, size_t size, size_t nmemb, void *param)
	{
		if (param == nullptr) return 0;

		auto stream = (Array<BYTE>*)param;

		stream->AddItems(size * nmemb, (const BYTE*)data);
		return (int)(size * nmemb);
	}

	HRESULT OAuth::ProcessAuthRequest(const Array<BYTE>& requestString)
	{
		HRESULT hr = S_OK;
		const char* url = "https://www.googleapis.com/oauth2/v3/token";
		CURL *curl = nullptr;
		char strPostFields[2048];
		CURLcode res;
		struct curl_slist *headers = nullptr; // init to NULL is important 

		m_ResultBuffer.Clear();
		svrChk(StrUtil::Format(strPostFields, "{0}{1}", "grant_type=urn:ietf:params:oauth:grant-type:jwt-bearer&assertion=", (const char*)requestString.data()));

		svrMem(curl = curl_easy_init());

		res = curl_easy_setopt(curl, CURLOPT_URL, url);
		if (res != CURLE_OK)
			svrErr(E_UNEXPECTED);
		Assert(res == 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

		res = curl_easy_setopt(curl, CURLOPT_POST, 1L);
		if (res != CURLE_OK)
			svrErr(E_UNEXPECTED);

		res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPostFields);
		if (res != CURLE_OK)
			svrErr(E_UNEXPECTED);

		res = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(strPostFields));
		if (res != CURLE_OK)
			svrErr(E_UNEXPECTED);

		headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
		//headers = curl_slist_append(headers, "charsets: utf-8");
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


	Proc_End:

		return hr;

	}


	// Authenticate
	HRESULT OAuth::Authenticate()
	{
		HRESULT hr = S_OK;
		std::string accessToken;

		StaticArray<BYTE, 2048> requestString;
		Json::Value root;
		Json::Reader reader;

		m_ResultBuffer.Clear();

		svrChk(BuildAuthRequestString(m_Account.c_str(), m_Scopes.c_str(), requestString));

		svrChk(ProcessAuthRequest(requestString));

		svrChk(m_ResultBuffer.push_back('\0'));

		bool parsingSuccessful = reader.parse((char*)m_ResultBuffer.data(), root);
		if (!parsingSuccessful)
		{
			// report to the user the failure and their locations in the document.
			//std::cout << "Failed to parse configuration\n"
			//	<< reader.getFormatedErrorMessages();
			svrErr(E_FAIL);
		}

		{
			auto value = root.get("access_token", "");
			if (value.isNull() || value.isString() != true)
				svrErr(E_FAIL);

			accessToken = std::forward<std::string>(value.asString());
			if (accessToken.length() == 0)
				svrErr(E_FAIL);
		}

		m_AuthStringIndex++;
		svrChk(StrUtil::Format(m_AuthString[m_AuthStringIndex % 2], "Authorization: Bearer {0}", accessToken.c_str()));
		m_ActiveAuthString = m_AuthString[m_AuthStringIndex % 2];

		m_AuthenticatedTime = Util::Time.GetTimeMs();

	Proc_End:

		if (FAILED(hr))
		{
			svrTrace(Trace::TRC_ERROR, "Failed to Authorize google API hr:{0:X8}, account:{1}, {2}", hr, m_Account, m_ResultBuffer.GetSize() > 0 ? (char*)m_ResultBuffer.data() : "null");
		}
		else
		{
			//svrTrace(Trace::TRC_TRACE, "Google API Authorization is updated hr:%0%, %1%", ArgHex32<UINT32>(hr), (char*)m_ResultBuffer.data());
		}

		return hr;
	}

	// Refresh
	HRESULT OAuth::UpdateAuthentication(bool forceUpdate)
	{
		HRESULT hr = S_OK;

		if (!forceUpdate
			&& Util::TimeSince(m_AuthenticatedTime) < DurationMS(AUTHTICKET_TIMEOUT))
			return hr;

		MutexScopeLock localLock(m_AuthenticationLock);

		if (!forceUpdate
			&& Util::TimeSince(m_AuthenticatedTime) < DurationMS(AUTHTICKET_TIMEOUT))
			return hr;

		svrChk(Authenticate());

	Proc_End:

		return hr;
	}


}; // namespace Google
}; // namespace Svr
}; // namespace BR

