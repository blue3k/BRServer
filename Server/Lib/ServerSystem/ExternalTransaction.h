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
#include "Common/ObjectPool.h"
#include "Memory/MemoryPool.h"
#include "Memory/SFMemory.h"
#include "Net/NetDef.h"
#include "ServerSystem/SimpleEntity.h"
#include "ServerSystem/Transaction.h"
#include "Types/SvrTypes.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/ParallelTransaction.h"
#include "Common/MemoryBufferUtil.h"

#include "curl/curl.h"
#include "json/json.h"

#include "ServerSystem/Google/GoogleOAuth.h"
#include "ServerSystem/Google/GoogleAndroidDeveloperAPI.h"



namespace BR {

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

		typedef StringBuffer<20*1024> ResultBuffer;

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

		HTTPExternalTransaction(TransactionID parentTransID, Message::MessageID MsgID);
		virtual ~HTTPExternalTransaction();

		CURL *GetCURL()				{ return m_Curl; }
		CURLcode GetCURLResult()	{ return m_CurlResult; }

		virtual Result CloseTransaction(Result hrRes) override;
	};

	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GCM External task class
	//

	class GCMHttpExternalTransaction : public HTTPExternalTransaction, public MemoryPoolObject<GCMHttpExternalTransaction>
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
		GCMHttpExternalTransaction();

		// Set parameters
		Result SetParameters( const char* strRegID, const char* strMessage, uint64_t param0 );

		// Initialize Transaction
		virtual Result InitializeTransaction(Entity* pOwner );

		// Start Transaction
		virtual Result StartTransaction();
	};


	class ExternalTransactionGoogleAndroidReceiptCheck : public ParallelTransaction, public MemoryPoolObject<ExternalTransactionGoogleAndroidReceiptCheck>
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
		ExternalTransactionGoogleAndroidReceiptCheck(TransactionID parentTransID, Google::OAuth* pOAuth);

		const std::string& GetDeveloperPayload() { return m_DevAPI.GetDeveloperPayload(); }

		// Set parameters
		Result SetParameters(const char* packageName, const char* productID, const char* purchaseToken);


		// Initialize Transaction
		virtual Result InitializeTransaction(Entity* pOwner) override;

		// Start Transaction
		virtual Result StartTransaction() override;
	};


	class ExternalTransactionIOSRecepitCheck : public HTTPExternalTransaction, public MemoryPoolObject<ExternalTransactionIOSRecepitCheck>
	{
	public:

		static const Message::MessageID MID;

	private:
		const char* m_strURL;
		char m_strPackageName[128];
		char m_strProductID[128];
		StaticArray<uint8_t, 20 * 1024> m_strReceipt;

		StaticArray<uint8_t,64> m_strTransactionID;

		typedef HTTPExternalTransaction super;

	public:

		Result ToResult(int status);

		// Constructor
		ExternalTransactionIOSRecepitCheck(TransactionID parentTransID, const char* strURL);

		// Set parameters
		Result SetParameters(const char* packageName, const char* productID, const char* transactionID, const Array<uint8_t>& purchaseToken);

		const Array<uint8_t>& GetPurchaseTransactionID()         { return m_strTransactionID; }

		// Initialize Transaction
		virtual Result InitializeTransaction(Entity* pOwner) override;

		Result VerifyReceipt();

		// Start Transaction
		virtual Result StartTransaction() override;
	};


	


}; // namespace Svr
}; // namespace BR




