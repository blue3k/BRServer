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

#include "Common/Typedefs.h"
#include "openssl/crypto.h"
#include "Common/SystemSynchronize.h"



namespace BR {
namespace Svr {
namespace Google {


		////////////////////////////////////////////////////////////////////////////////////////
		//
		//	GoogleAuto - Authenticate
		//

		class OAuth
		{
		private:

			const LONG AUTHTICKET_TIMEOUT = 10 * 60 * 1000;

			const char* GoogleDefaultPassword = "notasecret";

			// private key
			EVP_PKEY* m_privateKey;

			StaticArray<BYTE, 4096> m_ResultBuffer;
			ULONG m_AuthStringIndex;
			char m_AuthString[2][1024];
			const char* m_ActiveAuthString;

			std::string m_Account;
			std::string m_Scopes;
			std::string m_AccessToken;

			CriticalSection m_AuthenticationLock;
			ULONG m_AuthenticatedTime;

		private:

			HRESULT LoadPrivateKey(const char* strPKeyFile);

			HRESULT BuildAuthRequestString(const char* strAccount, const char* scopes, Array<BYTE>& requestString);
			HRESULT ProcessAuthRequest(const Array<BYTE>& requestString);

			static int WriteResultCB(char *data, size_t size, size_t nmemb, void *param);

		public:
			OAuth();
			~OAuth();

			const char* GetAuthString()                     { return m_ActiveAuthString; }

			HRESULT Initialize(const char* strPKeyFile, const char* strAccount, const char* scopes);

			// Authenticate
			HRESULT Authenticate();

			// Refresh
			HRESULT UpdateAuthentication(bool forceUpdate = true);
		};




}; // namespace Google
}; // namespace Svr
}; // namespace BR

