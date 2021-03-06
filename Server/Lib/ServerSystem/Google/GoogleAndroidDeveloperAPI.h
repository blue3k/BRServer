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
#include "Multithread/SFSystemSynchronization.h"
#include "Container/SFArray.h"



namespace SF {
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

		StaticArray<uint8_t, 4096> m_ResultBuffer;

		std::string m_DeveloperPayload;

		static int WriteResultCB(char *data, size_t size, size_t nmemb, void *param);

		Result ToResult(const Json::Value& root);

	public:
		AndroidDeveloperAPI(OAuth* pOAuth);
		~AndroidDeveloperAPI();

		IHeap& GetHeap() { return GetSystemHeap(); }

		const std::string& GetDeveloperPayload() { return m_DeveloperPayload; }

		// Check purchase receipt
		Result CheckReceipt(const char* packageName, const char* productID, const char* purchaseToken);
	};




}; // namespace Google
}; // namespace Svr
}; // namespace SF

