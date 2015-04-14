// TestApp.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <gtest/gtest.h>
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/StrFormat.h"
#include "Common/BrBaseTypes.h"
#include "Common/Memory.h"

#include "ServerSystem/ExternalTask.h"

#include "HTTPTest.h"

#include "curl/curl.h"
#include "json\json.h"


using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest; 
using namespace ::BR;


namespace BRTest
{
	STDAllocator allocator;

	void* CURL_malloc(size_t size)
	{
		void *pPtr = nullptr;
		allocator.Alloc( size, pPtr );
		return pPtr;
	}

	void CURL_free(void *ptr)
	{
		allocator.Free( ptr );
	}

	void* CURL_realloc(void *ptr, size_t size)
	{
		allocator.Realloc(size, ptr );
		return ptr;
	}

	char* CURL_strdup(const char *str)
	{
		char *strTarget = nullptr;
		StrUtil::StringDup( strTarget, str );
		return strTarget;
	}

	void* CURL_calloc(size_t nmemb, size_t size)
	{
		size_t totalSize = size*nmemb;
		void *pPtr = nullptr;
		if( SUCCEEDED(allocator.Alloc( totalSize, pPtr )) )
		{
			memset(pPtr, 0, totalSize);
		}
		return pPtr;
	}


TEST_F(HTTPTest, CURL)
{
	const int THREAD_COUNT = 10;
	const int TEST_COUNT = 10;
	const int URL_COUNT = 3;
	SyncCounter workCounter;

	const char * urls[URL_COUNT]= {
	  "http://curl.haxx.se/",
	  "http://www.contactor.se/",
	  "www.haxx.se"
	};

	CURLcode res = curl_global_init_mem( CURL_GLOBAL_ALL, CURL_malloc, CURL_free, CURL_realloc, CURL_strdup, CURL_calloc );
	EXPECT_EQ(0,res);
	if(res)
	{
		return;
	}

	for( INT64 ID = 0; ID < THREAD_COUNT; ID++ )
	{
		auto pTask = new BR::FunctorThread([&, ID](Thread* pThread)
		{
			workCounter.fetch_add(1,std::memory_order_relaxed);

			for(int i=0; i<(TEST_COUNT/THREAD_COUNT); i++)
			{
				void *pPtr = NULL;
				CURL *curl = nullptr;
				CURLcode res = CURLE_OK;

				const char* url = urls[rand()%URL_COUNT];
 
				curl = curl_easy_init();
				EXPECT_NE(nullptr,curl);
				res = curl_easy_setopt(curl, CURLOPT_URL, url);
				EXPECT_EQ(0,res);
				Assert(res== 0);
				res = curl_easy_perform(curl); /* ignores error */ 
				EXPECT_EQ(0,res);
				Assert(res== 0);
				curl_easy_cleanup(curl);
			}

			workCounter.fetch_sub(1,std::memory_order_relaxed);
		} );
		pTask->Start();
		m_Threads.push_back(pTask);


	};

	Sleep(1000);

	while( workCounter != 0 )
	{
		Sleep(1000);
	}

	StopAllThread();
	
}

TEST_F(HTTPTest, JSON)
{
	const int THREAD_COUNT = 1;
	const int TEST_COUNT = 1;
	SyncCounter workCounter;

	const char* jsonPattern =
	"{ \n"
	"	\"encoding\" : \"UTF-8\",	\n"
	"	\"plug-ins\" : [			\n"
	"		\"python\",				\n"
	"		\"c++\",				\n"
	"		\"ruby\"				\n"
	"	],							\n"
	"	\"indent\" : { \"length\" : 3, \"use_space\": true }\n"
	"}";


	for( INT64 ID = 0; ID < THREAD_COUNT; ID++ )
	{
		auto pTask = new BR::FunctorThread([&, ID](Thread* pThread)
		{
			workCounter.fetch_add(1,std::memory_order_relaxed);

			for(int i=0; i<(TEST_COUNT/THREAD_COUNT); i++)
			{
				void *pPtr = NULL;
				Json::Value root;
				Json::Reader reader;
				bool parsingSuccessful = reader.parse( jsonPattern, root );
				if ( !parsingSuccessful )
				{
					// report to the user the failure and their locations in the document.
					std::cout	<< "Failed to parse configuration\n"
								<< reader.getFormatedErrorMessages();
					break;
				}

				std::string encoding = root.get("encoding", "DefaultEncoding" ).asString();
				std::string encoding2 = root.get("encoding2", "DefaultEncoding" ).asString();
				const Json::Value pluginsInvalid = root["plug-ins¤¤¤¤¤¤"];
				const Json::Value plugins = root["plug-ins"];
				for ( UINT index = 0; index < plugins.size(); ++index )  // Iterates over the sequence elements.
					std::string value = plugins[index].asString();

				EXPECT_STREQ( "python", plugins[(UINT)0].asString().c_str() );
				EXPECT_STREQ( "c++", plugins[1].asString().c_str() );
				EXPECT_STREQ( "ruby", plugins[2].asString().c_str() );

				EXPECT_EQ( 3, root["indent"].get("length", 4).asInt() );
				EXPECT_EQ( true, root["indent"].get("use_space", true).asBool() );

				EXPECT_EQ( 3, root["indent"]["length"].asInt() );
				EXPECT_EQ( true, root["indent"]["use_space"].asBool() );
			}

			workCounter.fetch_sub(1,std::memory_order_relaxed);
		} );
		pTask->Start();
		m_Threads.push_back(pTask);


	};

	Sleep(1000);

	while( workCounter != 0 )
	{
		Sleep(1000);
	}

	StopAllThread();
	
}

struct MemoryStream
{
	char m_Buffer[2048];
	size_t m_CurrentSize;

	MemoryStream()
	{
		m_CurrentSize =0;
	}

	HRESULT AppendData( size_t size, const BYTE* data )
	{
		memcpy(&m_Buffer[m_CurrentSize], data, size );
		m_CurrentSize += size;

		if( m_CurrentSize < sizeof(m_Buffer) )
			m_Buffer[m_CurrentSize] = 0;

		return S_OK;
	}
};

static int writer(char *data, size_t size, size_t nmemb, void *param)
{
	if( param == nullptr ) return 0;

	MemoryStream *stream = (MemoryStream*)param;

	stream->AppendData( size * nmemb, (const BYTE*)data );
	return (int)(size * nmemb);
}   

TEST_F(HTTPTest, GCMQuery)
{
	const int THREAD_COUNT = 1;
	const int TEST_COUNT = 10;

	SyncCounter workCounter;

	const char * url= "https://android.googleapis.com/gcm/send";

	const char* strRegID = "APA91bHoK9ellfHl0-p4mvcWASH57f4Pu2tF4TlcVlprmzzgj5dWIElpmHPaJQ2kbPoGpFdr-4JCQDrZ2YALaYGYRz2CsQISGXDExuY2_NMP3qf1HcUhwqTbEvyXQEMml23rTeAwzhlgWOtje_0AnfyaQ3c-B1SDYg";
	const char* strAPIKey = "AIzaSyAhtG0C5uH-q1MJRUoazlDkMjVZEVY-93I";

	char strPostFields[1024];
	StrUtil::Format( strPostFields, " { \"registration_ids\" :[ \"%0%\"], \"data\": { \"Message\" :  \"%1%\" }   } ", strRegID, "messageIm" );

	CURLcode res = curl_global_init_mem( CURL_GLOBAL_ALL, CURL_malloc, CURL_free, CURL_realloc, CURL_strdup, CURL_calloc );
	EXPECT_EQ(0,res);
	if(res)
	{
		return;
	}

	for( INT64 ID = 0; ID < THREAD_COUNT; ID++ )
	{
		auto pTask = new BR::FunctorThread([&, ID](Thread* pThread)
		{
			workCounter.fetch_add(1,std::memory_order_relaxed);

			MemoryStream stream;

			for(int i=0; i<(TEST_COUNT/THREAD_COUNT); i++)
			{
				void *pPtr = NULL;
				CURL *curl = nullptr;
				CURLcode res = CURLE_OK;
				MemoryStream recvStream;

				struct curl_slist *headers=NULL; // init to NULL is important 
				char strAPIKeyOpt[512];
				StrUtil::Format( strAPIKeyOpt, "Authorization: key=%0%", strAPIKey );
				headers = curl_slist_append( headers, strAPIKeyOpt);  
				headers = curl_slist_append( headers, "Content-Type: application/json");
				headers = curl_slist_append( headers, "charsets: utf-8"); 

				curl = curl_easy_init();
				EXPECT_NE(nullptr,curl);
				res = curl_easy_setopt(curl, CURLOPT_URL, url);
				EXPECT_EQ(0,res);
				Assert(res== 0);
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

				res = curl_easy_setopt(curl, CURLOPT_POST, 1L);
				res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPostFields );
				res = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(strPostFields) );

				res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers );

				res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
				res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &recvStream);
				EXPECT_EQ(0,res);
				Assert(res== 0);

				res = curl_easy_perform(curl); /* ignores error */ 
				EXPECT_EQ(0,res);
				Assert(res== 0);

				char *ct = nullptr;
				res = curl_easy_getinfo( curl, CURLINFO_CONTENT_TYPE, &ct );
				EXPECT_EQ(0,res);
				Assert(res== 0);

				curl_easy_cleanup(curl);
			}

			workCounter.fetch_sub(1,std::memory_order_relaxed);
		} );
		pTask->Start();
		m_Threads.push_back(pTask);
 	};

	Sleep(1000);

	while( workCounter != 0 )
	{
		Sleep(1000);
	}

	StopAllThread();
	
}


TEST_F(HTTPTest, HTTPModule)
{
	const int THREAD_COUNT = 1;
	const int TEST_COUNT = 10;
	SyncCounter workCounter;

	const char* strRegID = "APA91bHoK9ellfHl0-p4mvcWASH57f4Pu2tF4TlcVlprmzzgj5dWIElpmHPaJQ2kbPoGpFdr-4JCQDrZ2YALaYGYRz2CsQISGXDExuY2_NMP3qf1HcUhwqTbEvyXQEMml23rTeAwzhlgWOtje_0AnfyaQ3c-B1SDYg";

	Svr::ExternalTransactionManager *pManager = new Svr::ExternalTransactionManager;
	EXPECT_HRESULT_SUCCEEDED(pManager->InitializeComponent());
	
	for( INT64 ID = 0; ID < THREAD_COUNT; ID++ )
	{
		auto pTask = new BR::FunctorThread([&, ID](Thread* pThread)
		{
			workCounter.fetch_add(1,std::memory_order_relaxed);

			for( int iTest = 0; iTest < TEST_COUNT; iTest++ )
			{
				EXPECT_HRESULT_SUCCEEDED( pManager->SendGCMNotify( 0, strRegID, "Test" ) );
			}
			
			workCounter.fetch_sub(1,std::memory_order_relaxed);
		} );
		pTask->Start();
		m_Threads.push_back(pTask);
 	};

	Sleep(1000);

	while( workCounter != 0 )
	{
		Sleep(1000);
	}

	StopAllThread();
	
	pManager->TerminateComponent();
}




}  // namespace BRTest
