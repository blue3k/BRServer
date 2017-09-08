////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : External transaction class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Thread/Thread.h"
#include "Object/ObjectPool.h"
#include "Memory/MemoryPool.h"
#include "Memory/SFMemory.h"
#include "Net/NetDef.h"
#include "Entity/SimpleEntity.h"
#include "Transaction/Transaction.h"
#include "Types/BrSvrTypes.h"
#include "Component/ServerComponent.h"
#include "Transaction/ParallelTransaction.h"


#include "curl/curl.h"
#include "json/json.h"

#include "ServerSystem/Google/GoogleOAuth.h"
#include "ServerSystem/Google/GoogleAndroidDeveloperAPI.h"



namespace SF {

	namespace Message {
		class MessageData;
	};


namespace Svr{


	enum ExternalTransactionMesageCode {
		ExternalTransactionMesageCode_AndroidCheckReceipt,
		ExternalTransactionMesageCode_IOSCheckReceipt,
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	External task base class
	//



	class HTTPExternalTransaction : public ParallelTransaction
	{
	public:

		typedef StaticArray<char,20*1024> ResultBuffer;

	protected:
	public:
		static int ResultWriter(char *data, size_t size, size_t nmemb, void *param);

	protected:
		CURL *m_Curl;

		// HTTP query result
		ResultBuffer m_HTTPResult;

	protected:

		CURLcode m_CurlResult;

	public:

		HTTPExternalTransaction(IMemoryManager& memMgr, TransactionID parentTransID, Message::MessageID MsgID);
		virtual ~HTTPExternalTransaction();

		CURL *GetCURL()				{ return m_Curl; }
		CURLcode GetCURLResult()	{ return m_CurlResult; }

		virtual Result CloseTransaction(Result hrRes) override;
	};

	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GCM External task class
	//

	class GCMHttpExternalTransaction : public HTTPExternalTransaction
	{

	private:
		// connection URL for GCM service
		static const char* stm_URL;
		// API key
		static const char* stm_strAPIKey;

		// Header list
		struct curl_slist *m_Headers;

		// GCM device registration ID
		char m_strRegID[1024];

		// message
		char m_strMessage[128];

		// Parameter 0
		uint64_t m_Param0;

	public:

		// Constructor
		GCMHttpExternalTransaction(IMemoryManager& memMgr);

		// Set parameters
		Result SetParameters( const char* strRegID, const char* strMessage, uint64_t param0 );

		// Initialize Transaction
		virtual Result InitializeTransaction(Entity* pOwner );

		// Start Transaction
		virtual Result StartTransaction();
	};


	class ExternalTransactionGoogleAndroidReceiptCheck : public ParallelTransaction
	{
	public:

		static const Message::MessageID MID;

	private:
		char m_strPackageName[128];
		char m_strProductID[128];
		char m_strPurchaseToken[1024];

		Google::AndroidDeveloperAPI m_DevAPI;

	public:

		// Constructor
		ExternalTransactionGoogleAndroidReceiptCheck(IMemoryManager& memMgr, TransactionID parentTransID, Google::OAuth* pOAuth);

		const std::string& GetDeveloperPayload() { return m_DevAPI.GetDeveloperPayload(); }

		// Set parameters
		Result SetParameters(const char* packageName, const char* productID, const char* purchaseToken);


		// Initialize Transaction
		virtual Result InitializeTransaction(Entity* pOwner) override;

		// Start Transaction
		virtual Result StartTransaction() override;
	};


	class ExternalTransactionIOSRecepitCheck : public HTTPExternalTransaction
	{
	public:

		static const Message::MessageID MID;

	private:
		const char* m_strURL;
		const char* m_strAltURL;
		char m_strPackageName[128];
		char m_strProductID[128];
		StaticArray<uint8_t, 20 * 1024> m_strReceipt;

		StaticArray<uint8_t,64> m_strTransactionID;

		typedef HTTPExternalTransaction super;

	public:

		Result ToResult(int status);

		// Constructor
		ExternalTransactionIOSRecepitCheck(IMemoryManager& memMgr, TransactionID parentTransID, const char* strURL, const char* strAltURL);

		// Set parameters
		Result SetParameters(const char* packageName, const char* productID, const char* transactionID, const Array<uint8_t>& purchaseToken);

		const Array<uint8_t>& GetPurchaseTransactionID()         { return m_strTransactionID; }

		// Initialize Transaction
		virtual Result InitializeTransaction(Entity* pOwner) override;

		Result VerifyReceipt(const char* strURL);

		// Start Transaction
		virtual Result StartTransaction() override;
	};


	


}; // namespace Svr
}; // namespace SF




