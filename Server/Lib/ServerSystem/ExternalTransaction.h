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


#include "Common/Typedefs.h"
#include "Common/Thread.h"
#include "Common/ObjectPool.h"
#include "Common/MemoryPool.h"
#include "Common/Memory.h"
#include "Net/NetDef.h"
#include "ServerSystem/SimpleEntity.h"
#include "ServerSystem/Transaction.h"
#include "ServerSystem/SvrTypes.h"
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

		virtual HRESULT CloseTransaction(HRESULT hrRes) override;
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
		UINT64 m_Param0;

	public:

		// Constructor
		GCMHttpExternalTransaction();

		// Set parameters
		HRESULT SetParameters( const char* strRegID, const char* strMessage, UINT64 param0 );

		// Initialize Transaction
		virtual HRESULT InitializeTransaction( Entity* pOwner );

		// Start Transaction
		virtual HRESULT StartTransaction();
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
		HRESULT SetParameters(const char* packageName, const char* productID, const char* purchaseToken);


		// Initialize Transaction
		virtual HRESULT InitializeTransaction(Entity* pOwner) override;

		// Start Transaction
		virtual HRESULT StartTransaction() override;
	};


	class ExternalTransactionIOSRecepitCheck : public HTTPExternalTransaction, public MemoryPoolObject<ExternalTransactionIOSRecepitCheck>
	{
	public:

		static const Message::MessageID MID;

	private:
		const char* m_strURL;
		char m_strPackageName[128];
		char m_strProductID[128];
		StaticArray<BYTE, 20 * 1024> m_strReceipt;

		StaticArray<BYTE,64> m_strTransactionID;

	public:

		HRESULT ToHRESULT(int status);

		// Constructor
		ExternalTransactionIOSRecepitCheck(TransactionID parentTransID, const char* strURL);

		// Set parameters
		HRESULT SetParameters(const char* packageName, const char* productID, const char* transactionID, const Array<BYTE>& purchaseToken);

		const Array<BYTE>& GetPurchaseTransactionID()         { return m_strTransactionID; }

		// Initialize Transaction
		virtual HRESULT InitializeTransaction(Entity* pOwner) override;

		HRESULT VerifyReceipt();

		// Start Transaction
		virtual HRESULT StartTransaction() override;
	};


	


}; // namespace Svr
}; // namespace BR




