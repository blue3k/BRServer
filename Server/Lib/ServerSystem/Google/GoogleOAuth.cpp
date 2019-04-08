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
#include "String/SFStrUtil.h"
#include "Util/SFUtility.h"
#include "Util/SFTimeUtil.h"
#include "IO/SFFile.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "ResultCode/SFResultCodeSystem.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "SvrTrace.h"

#include "curl/curl.h"
#include "json/json.h"

#if defined(SF_USE_MBEDTLS)
#include "mbedtls/pkcs12.h"
#include "mbedtls/error.h"
#include "mbedtls/rsa.h"
#include "mbedtls/x509.h"

#else // SF_USE_MBEDTLS
#include <openssl/err.h>
#include <openssl/pkcs12.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>

#endif // SF_USE_MBEDTLS

#include "ServerSystem/Google/GoogleOAuth.h"


namespace SF {
namespace Svr {
namespace Google {


	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	OAuth class
	//

	OAuth::OAuth()
		: m_privateKey(nullptr)
		, m_ResultBuffer(GetSystemHeap())
		, m_AuthStringIndex(0)
		, m_ActiveAuthString(nullptr)
		, m_AuthenticatedTime(TimeStampMS::min())
	{
		m_ActiveAuthString = m_AuthString[m_AuthStringIndex];
		memset(m_AuthString, 0, sizeof m_AuthString);
#if defined(SF_USE_MBEDTLS)
		mbedtls_pk_init(&m_privateKey);
#endif
	}

	OAuth::~OAuth()
	{
#if defined(SF_USE_MBEDTLS)
		mbedtls_pk_free(&m_privateKey);
#else
		if (m_privateKey != nullptr)
			EVP_PKEY_free(m_privateKey);
#endif
	}

	Result OAuth::LoadPrivateKey(const char* strPKeyFile)
	{
		Result hr = ResultCode::SUCCESS;
		char filePath[1024];

#if defined(SF_USE_MBEDTLS)

		// TODO: support reloading
		if (mbedtls_pk_get_type(&m_privateKey) == MBEDTLS_PK_RSA)
			return ResultCode::SUCCESS;

		StrUtil::Format(filePath, "{0}{1}", Util::GetModulePath(), strPKeyFile);

		auto sslResult = mbedtls_pk_parse_keyfile(&m_privateKey, filePath, GoogleDefaultPassword);
		if (sslResult != 0) // invalid password?
			return ResultCode::INVALID_PASSWORD;

		return ResultCode::SUCCESS;
#else
		StaticArray<uint8_t, 4096> buffer(GetHeap());
		size_t read;

		if (m_privateKey != nullptr)
			return ResultCode::SUCCESS_FALSE;

		// load all open ssl algorithms
		OpenSSL_add_all_algorithms();

		StrUtil::Format(filePath, "{0}{1}", Util::GetModulePath(), strPKeyFile);

		// ssl signing
		BIO* memBIO = BIO_new(BIO_s_mem());

		File file;
		file.Open(filePath, File::OpenMode::Read, File::SharingMode::ReadShared);
		if (!file.IsOpened())
		{
			BIO_free(memBIO);
			return ResultCode::SUCCESS_FALSE;
		}

		file.Read(buffer.data(), buffer.GetAllocatedSize(), read);
		buffer.resize(read);

		BIO_write(memBIO, buffer.data(), static_cast<int>(buffer.size()));

		// Load key file
		//FILE *fp = nullptr;
		//fp = fopen(filePath, "rb");
		//PKCS12 * p12 = d2i_PKCS12_fp(fp, nullptr);
		PKCS12 * p12 = d2i_PKCS12_bio(memBIO, nullptr);
		X509 *cert = nullptr;
		STACK_OF(X509) *ca = nullptr;

		auto sslResult = PKCS12_verify_mac(p12, GoogleDefaultPassword, (int)strlen(GoogleDefaultPassword));
		if (sslResult == FALSE) // invalid password
			svrErr(ResultCode::INVALID_PASSWORD);

		sslResult = PKCS12_parse(p12, GoogleDefaultPassword, &m_privateKey, &cert, &ca);
		if (sslResult == FALSE)
		{
			char errorMessage[512];
			auto error = ERR_get_error();
			ERR_error_string(error, errorMessage);
			svrErr(ResultCode::UNEXPECTED);
		}

	Proc_End:

		//if (fp != nullptr)
		//	fclose(fp);

		if (memBIO != nullptr)
			BIO_free(memBIO);

		if (ca != nullptr)
			sk_X509_pop_free(ca, X509_free);

		if (cert != nullptr)
			X509_free(cert);

		if (p12)
			PKCS12_free(p12);
#endif
		return hr;
	}

	Result OAuth::BuildAuthRequestString(const char* strAccount, const char* scopes, Array<uint8_t>& requestString)
	{
		Result hr = ResultCode::SUCCESS;

		StaticArray<uint8_t, 128> digest(GetHeap());
		int sslResult = TRUE;
		uint8_t sign_buffer[512];
		uint sign_len = (uint)countof(sign_buffer);

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

		// 1hour later from now
		time_t timeToRequest = time(0) + 3;
		StrUtil::Format(body, bodyFormat, strAccount, scopes, timeToRequest);


		//////////////////////////////////////////////////////////////////////
		// Combine hashed strings

		svrChk(Util::Base64URLEncode(countof(header) - 1, (const uint8_t*)header, requestString));
		svrChk(requestString.push_back('.'));
		svrChk(Util::Base64URLEncode((INT)strlen(body), (const uint8_t*)body, requestString));

		//////////////////////////////////////////////////////////////////////
		// signing

		// SHA256
		svrChk(Util::SHA256Hash(requestString.size(), requestString.data(), digest));


#if defined(SF_USE_MBEDTLS)

		auto pRsaKey = mbedtls_pk_rsa(&m_privateKey);
		if (pRsaKey == nullptr
			|| strAccount == nullptr
			|| scopes == nullptr)
			return ResultCode::FAIL;

		// RSA sign
		sslResult = mbedtls_rsa_pkcs1_sign(&rsa, NULL, NULL, MBEDTLS_RSA_PRIVATE, MBEDTLS_MD_SHA256,
			20, digest.data(), sign_buffer);
		if (sslResult != 0)
		{
			svrErr(ResultCode::UNEXPECTED);
		}


#else // SF_USE_MBEDTLS
		if (m_privateKey == nullptr
			|| strAccount == nullptr
			|| scopes == nullptr)
			return ResultCode::FAIL;

		// EVP_PKEY_get1_RSA ?
		RSA* pkey = EVP_PKEY_get0_RSA(m_privateKey);
		if (pkey == nullptr)
			return ResultCode::UNEXPECTED;


		//////////////////////////////////////////////////////////////////////
		// signing

		// RSA sign
		sslResult = RSA_sign(NID_sha256, digest.data(), (uint)digest.size(), sign_buffer, &sign_len, pkey);
		if (sslResult == FALSE)
		{
			svrErr(ResultCode::UNEXPECTED);
		}
#endif // SF_USE_MBEDTLS

		svrChk(requestString.push_back('.'));

		svrChk(Util::Base64URLEncode(sign_len, sign_buffer, requestString));

		svrChk(requestString.push_back('\0')); // append null

	Proc_End:

		return hr;
	}

	Result OAuth::Initialize(const char* strPKeyFile, const char* strAccount, const char* scopes)
	{
		Result hr = ResultCode::SUCCESS;

		if (strAccount == nullptr || scopes == nullptr)
			return ResultCode::INVALID_POINTER;

		m_Account = strAccount;
		m_Scopes = scopes;

		svrChk(LoadPrivateKey(strPKeyFile));


	Proc_End:

		return hr;
	}

	int OAuth::WriteResultCB(char *data, size_t size, size_t nmemb, void *param)
	{
		if (param == nullptr) return 0;

		auto stream = (Array<uint8_t>*)param;

		stream->push_back(size * nmemb, (const uint8_t*)data);
		return (int)(size * nmemb);
	}

	Result OAuth::ProcessAuthRequest(const Array<uint8_t>& requestString)
	{
		Result hr = ResultCode::SUCCESS;
		const char* url = "https://www.googleapis.com/oauth2/v3/token";
		CURL *curl = nullptr;
		char strPostFields[2048];
		CURLcode res;
		struct curl_slist *headers = nullptr; // init to NULL is important 
		char *ct = nullptr;

		m_ResultBuffer.Clear();
		StrUtil::Format(strPostFields, "{0}{1}", "grant_type=urn:ietf:params:oauth:grant-type:jwt-bearer&assertion=", (const char*)requestString.data());

		svrMem(curl = curl_easy_init());

		res = curl_easy_setopt(curl, CURLOPT_URL, url);
		if (res != CURLE_OK)
			svrErr(ResultCode::UNEXPECTED);
		Assert(res == 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

		res = curl_easy_setopt(curl, CURLOPT_POST, 1L);
		if (res != CURLE_OK)
			svrErr(ResultCode::UNEXPECTED);

		res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPostFields);
		if (res != CURLE_OK)
			svrErr(ResultCode::UNEXPECTED);

		res = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(strPostFields));
		if (res != CURLE_OK)
			svrErr(ResultCode::UNEXPECTED);

		headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
		//headers = curl_slist_append(headers, "charsets: utf-8");
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		if (res != CURLE_OK)
			svrErr(ResultCode::UNEXPECTED);

		res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteResultCB);
		if (res != CURLE_OK)
			svrErr(ResultCode::UNEXPECTED);

		res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m_ResultBuffer);
		if (res != CURLE_OK)
			svrErr(ResultCode::UNEXPECTED);
		Assert(res == 0);

		res = curl_easy_perform(curl); /* ignores error */
		if (res != CURLE_OK)
			svrErr(ResultCode::UNEXPECTED);
		Assert(res == 0);

		res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
		if (res != CURLE_OK)
			svrErr(ResultCode::UNEXPECTED);
		Assert(res == 0);

		curl_easy_cleanup(curl);


	Proc_End:

		return hr;

	}


	// Authenticate
	Result OAuth::Authenticate()
	{
		Result hr = ResultCode::SUCCESS;
		std::string accessToken;
		bool parsingSuccessful;

		StaticArray<uint8_t, 2048> requestString(GetHeap());
		Json::Value root;
		Json::Reader reader;

		m_ResultBuffer.Clear();

		svrChk(BuildAuthRequestString(m_Account.c_str(), m_Scopes.c_str(), requestString));

		svrChk(ProcessAuthRequest(requestString));

		svrChk(m_ResultBuffer.push_back('\0'));

		parsingSuccessful = reader.parse((char*)m_ResultBuffer.data(), root);
		if (!parsingSuccessful)
		{
			// report to the user the failure and their locations in the document.
			//std::cout << "Failed to parse configuration\n"
			//	<< reader.getFormatedErrorMessages();
			svrErr(ResultCode::FAIL);
		}

		{
			auto value = root.get("access_token", "");
			if (value.isNull() || value.isString() != true)
				svrErr(ResultCode::FAIL);

			accessToken = std::forward<std::string>(value.asString());
			if (accessToken.length() == 0)
				svrErr(ResultCode::FAIL);
		}

		m_AuthStringIndex++;
		StrUtil::Format(m_AuthString[m_AuthStringIndex % 2], "Authorization: Bearer {0}", accessToken.c_str());
		m_ActiveAuthString = m_AuthString[m_AuthStringIndex % 2];

		m_AuthenticatedTime = Util::Time.GetTimeMs();

	Proc_End:

		if (!(hr))
		{
			svrTrace(Error, "Failed to Authorize google API hr:{0:X8}, account:{1}, {2}", hr, m_Account.c_str(), m_ResultBuffer.size() > 0 ? (char*)m_ResultBuffer.data() : "null");
		}
		else
		{
			//svrTrace(Info, "Google API Authorization is updated hr:{0}, {1}", ArgHex32<uint32_t>(hr), (char*)m_ResultBuffer.data());
		}

		return hr;
	}

	// Refresh
	Result OAuth::UpdateAuthentication(bool forceUpdate)
	{
		Result hr = ResultCode::SUCCESS;

		if (!forceUpdate
			&& Util::TimeSince(m_AuthenticatedTime) <= DurationMS(AUTHTICKET_TIMEOUT))
			return hr;

		MutexScopeLock localLock(m_AuthenticationLock);

		if (!forceUpdate
			&& Util::TimeSince(m_AuthenticatedTime) <= DurationMS(AUTHTICKET_TIMEOUT))
			return hr;

		svrChk(Authenticate());

	Proc_End:

		return hr;
	}


}; // namespace Google
}; // namespace Svr
}; // namespace SF

