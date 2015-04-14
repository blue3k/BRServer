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


#include "Common/Typedefs.h"
#include "Common/Thread.h"
#include "Common/ObjectPool.h"
#include "Common/Memory.h"
#include "Net/NetDef.h"
#include "ServerSystem/SimpleEntity.h"
#include "ServerSystem/Transaction.h"
#include "ServerSystem/SvrTypes.h"
#include "ServerSystem/ServerComponent.h"
#include "Common/MemoryBufferUtil.h"

#include "curl/curl.h"
#include "json\json.h"


namespace BR {

	namespace Message {
		class MessageData;
	};


namespace Svr{


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	External task base class
	//

	class ExternalTransaction : public SubTransactionWithResult
	{
	public:
		ExternalTransaction( TransactionID parentTransID , Message::MessageID MsgID )
			:SubTransactionWithResult( parentTransID, MsgID )
		{}
	};

	class HTTPExternalTransaction : public ExternalTransaction
	{
	public:

		typedef StringBuffer<2048> ResultBuffer;

	protected:
		// allocator for CURL library
		static MemoryAllocator &m_Allocator;

	public:
		static void* CURL_malloc(size_t size);
		static void CURL_free(void *ptr);
		static void* CURL_realloc(void *ptr, size_t size);
		static char* CURL_strdup(const char *str);
		static void* CURL_calloc(size_t nmemb, size_t size);
		static int ResultWriter(char *data, size_t size, size_t nmemb, void *param);

		// convert CURL error to HRESULT
		static HRESULT ToHRESULT( CURLcode code );


	protected:
		CURL *m_Curl;

		// HTTP query result
		ResultBuffer m_HTTPResult;

	protected:

		CURLcode m_CurlResult;

	public:
		HTTPExternalTransaction( TransactionID parentTransID , Message::MessageID MsgID )
			:ExternalTransaction( parentTransID, MsgID )
			,m_Curl(nullptr)
			,m_CurlResult(CURLE_OK)
		{
			m_Curl = curl_easy_init();
			if( m_Curl != nullptr )
				m_CurlResult = curl_easy_setopt(GetCURL(), CURLOPT_WRITEFUNCTION, ResultWriter);
			else
				m_CurlResult = CURLE_FAILED_INIT;
		}

		virtual ~HTTPExternalTransaction()
		{
			if( m_Curl )
			{
				curl_easy_cleanup( m_Curl );
			}
		}

		CURL *GetCURL()				{ return m_Curl; }
		CURLcode GetCURLResult()	{ return m_CurlResult; }
	};

	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GCM External task class
	//

	class GCMHttpExternalTransaction : public HTTPExternalTransaction, public ObjectPoolObjectT<GCMHttpExternalTransaction>
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


		// Override release
		virtual void Release() { ObjectPoolObjectT::Release(); }

		// Initialize Transaction
		virtual HRESULT InitializeTransaction( Entity* pOwner );

		// Start Transaction
		virtual HRESULT StartTransaction();
	};


	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	External task class
	//

	class ExternalTransactionManager : private SimpleEntity, public IServerComponent
	{
	public:

		enum {
			ComponentID = ServerComponentID_ExternalTaskManager,
		};

		class TaskWorker : public Thread
		{
		private:
			PageQueue<ExternalTransaction*> &m_QuerieQueue;

		public:
			TaskWorker( PageQueue<ExternalTransaction*> &querieQueue );

			virtual bool Run();
		};

	private:
		// Working thread list
		typedef std::vector<TaskWorker*>	WorkerList;

		// Query Worker class
		WorkerList							m_QueryWorker;

		// Pending transaction Queue
		PageQueue<ExternalTransaction*>		m_PendingQueries;

		virtual MemoryAllocator& GetAllocator() { return STDAllocator::GetInstance(); }

	public:
		ExternalTransactionManager();
		~ExternalTransactionManager();

		// Initialize server component
		virtual HRESULT InitializeComponent();

		// Terminate server component
		virtual void TerminateComponent();

		// Send gcm notify
		HRESULT SendGCMNotify( TransactionID sender, const char* strRegisterIDs, const char* strMessage, UINT64 param0 = 0 ); 
	};



#include "ExternalTask.inl"


}; // namespace Svr
}; // namespace BR




