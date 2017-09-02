////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : External task class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Thread/Thread.h"
#include "Common/ObjectPool.h"
#include "Memory/SFMemory.h"
#include "Net/NetDef.h"
#include "ServerSystem/SimpleEntity.h"
#include "ServerSystem/Transaction.h"
#include "Types/BrSvrTypes.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ParallelTransaction.h"
#include "Common/MemoryBufferUtil.h"

#include "curl/curl.h"
#include "json/json.h"

#include "ServerSystem/Google/GoogleOAuth.h"

namespace SF {

	namespace Message {
		class MessageData;
	};


namespace Svr{


	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	External task class
	//


	class ExternalTransactionManager : public ParallelTransactionManager
	{
	public:
		// convert CURL error to Result
		static Result ToResult(CURLcode code);

	private:

		static void* CURL_malloc(size_t size);
		static void CURL_free(void *ptr);
		static void* CURL_realloc(void *ptr, size_t size);
		static char* CURL_strdup(const char *str);
		static void* CURL_calloc(size_t nmemb, size_t size);

		// allocator for CURL library
		static MemoryAllocator &m_Allocator;

		Google::OAuth m_GoogleAuth;
		std::string m_IOSURL;

	public:
		ExternalTransactionManager();
		~ExternalTransactionManager();

		Result UpdateGoogleAuth();

		Result InitializeManagerGoogle(const char* strP12KeyFile, const char* strAccount, const char* scopes);
		Result InitializeManagerIOS(const char* strURL);

		// Initialize server component
		virtual Result InitializeComponent() override;

		// Terminate server component
		virtual void TerminateComponent() override;

		virtual Result TickUpdate(TimerAction *pAction = nullptr) override;

		// Send gcm notify
		Result SendGCMNotify(TransactionID sender, const char* strRegisterIDs, const char* strMessage, uint64_t param0 = 0);

		// check receipt
		Result AndroidCheckReceipt(TransactionID sender, const char* packageName, const char* productID, const char* purchaseToken);
		Result IOSCheckReceipt(TransactionID sender, const char* packageName, const char* productID, const char* transactionID, const Array<uint8_t>& purchaseToken);

	};



#include "ExternalTransactionManager.inl"


}; // namespace Svr
}; // namespace SF




