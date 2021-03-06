////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : KyungKun Ko
//
// Description : Google Authentication
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#if defined(SF_USE_MBEDTLS)
#include "mbedtls/pk.h"
#else
#include "openssl/crypto.h"
#endif
#include "Multithread/SFSystemSynchronization.h"



namespace SF {
namespace Svr {
namespace Google {


		////////////////////////////////////////////////////////////////////////////////////////
		//
		//	GoogleAuto - Authenticate
		//

		class OAuth
		{
		private:

			const long AUTHTICKET_TIMEOUT = 10 * 60 * 1000;

			const char* GoogleDefaultPassword = "notasecret";

			// private key
#if defined(SF_USE_MBEDTLS)
			mbedtls_pk_context m_privateKey;
#else
			EVP_PKEY* m_privateKey = nullptr;
#endif


			StaticArray<uint8_t, 4096> m_ResultBuffer;
			uint64_t m_AuthStringIndex;
			char m_AuthString[2][1024];
			const char* m_ActiveAuthString;

			std::string m_Account;
			std::string m_Scopes;
			std::string m_AccessToken;

			CriticalSection m_AuthenticationLock;
			TimeStampMS m_AuthenticatedTime;

		private:

			Result LoadPrivateKey(const char* strPKeyFile);

			Result BuildAuthRequestString(const char* strAccount, const char* scopes, Array<uint8_t>& requestString);
			Result ProcessAuthRequest(const Array<uint8_t>& requestString);

			static int WriteResultCB(char *data, size_t size, size_t nmemb, void *param);

		public:
			OAuth();
			~OAuth();

			IHeap& GetHeap() { return GetSystemHeap(); }

			const char* GetAuthString()                     { return m_ActiveAuthString; }

			Result Initialize(const char* strPKeyFile, const char* strAccount, const char* scopes);

			// Authenticate
			Result Authenticate();

			// Refresh
			Result UpdateAuthentication(bool forceUpdate = true);
		};




}; // namespace Google
}; // namespace Svr
}; // namespace SF

