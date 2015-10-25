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
#include "Common/ArrayUtil.h"



namespace BR {
namespace Svr {
namespace Google {

	class OAuth;

	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	AndroidDeveloperAPI
	//

	class AndroidDeveloperAPI
	{
	private:

	private:

		class OAuth* m_OAuth;

		StaticArray<BYTE, 4096> m_ResultBuffer;

		std::string m_DeveloperPayload;

		static int WriteResultCB(char *data, size_t size, size_t nmemb, void *param);

		HRESULT ToHRESULT(const Json::Value& root);

	public:
		AndroidDeveloperAPI(OAuth* pOAuth);
		~AndroidDeveloperAPI();

		const std::string& GetDeveloperPayload() { return m_DeveloperPayload; }

		// Check purchase receipt
		HRESULT CheckReceipt(const char* packageName, const char* productID, const char* purchaseToken);
	};




}; // namespace Google
}; // namespace Svr
}; // namespace BR
