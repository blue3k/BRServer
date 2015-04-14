////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Performance Counter library
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/Trace.h"




namespace BR { 
namespace Perf {
	class Counter;


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Counter Instance Map
	//

	class CounterInstanceMap
	{
	public:

		enum {
			MAX_COUNTER_NAME		= 64,		// Counter name max size
			MAX_INSTANCE_NAME		= 128,		// Instance name max size
			MAX_INSTANCE_LIFE_TIME	= 10,		// Maximum counter life time in sec

			MAGIC_NUMBER			= 0x0,
		};

		// Counter map definitions
#pragma pack(push)
#pragma pack(4)
		struct _Counter
		{
			WCHAR szName[MAX_COUNTER_NAME];
			CounterType uiValue;
		};

		struct CounterMap
		{
			UINT32 tHeartBitTime;
			UINT32 NumCounter;
			WCHAR  szClass[MAX_COUNTER_NAME];
			WCHAR  szName[MAX_INSTANCE_NAME];
			_Counter counters[1];
		};

#pragma pack(pop)

		// Counter name map
		typedef std::unordered_map<std::wstring,_Counter*> CounterNameMap;

	private:
		// Seeds for generator
		CounterMap *m_pCounters;

		// Counter name table
		CounterNameMap	m_CounterNameMap;


		// Mapped file name
		WCHAR m_wszMapFileName[MAX_PATH];

		// File handle for memory mapped file
		HANDLE		m_hFile;
		HANDLE		m_hFileMap;


		// Max counter count
		size_t		m_szMaxCounter;

		// Is opened with read mode?
		bool		m_bIsReadMode;


		// Is Alive
		bool		m_bIsAlive;


	public:
		// Constructor
		CounterInstanceMap();

		// Constructor with create instance
		CounterInstanceMap( const WCHAR* wszClass, const WCHAR* wszInstance, ... );

		// Destructor
		virtual ~CounterInstanceMap();

		// Get name of instance
		inline const WCHAR *GetInstanceName();

		// Get Class name
		inline const WCHAR *GetClass();

		inline bool IsOpened();

		// create counter file map
		HRESULT Create( const WCHAR* wszClass, const WCHAR* wszInstance, ... );

		// create counter file map
		HRESULT Create( const WCHAR* wszClass, const WCHAR* wszInstance, va_list args );

		// Enumerate instance files
		static HRESULT EnumerateFileList( std::list<std::wstring>& fileNames );

		static HRESULT FileNameFromInstanceName( WCHAR (&fileName)[1024], const WCHAR* strInstanceName );

		// Open counter file map for read. with instance file name
		HRESULT OpenForRead( const WCHAR *wszFileName );

		// reopen
		//HRESULT Reopen();

		// Close file map
		virtual void Close();

		// Create counter at counter instance
		HRESULT CreateCounter( Perf::Counter &counterInst );

		// Get counter at counter instance
		HRESULT GetCounter( Perf::Counter &counterInst );

		// Heart bit
		void UpdateHeartBit();

		// Check Heart Bit
		bool CheckHeartBit();

	};




	////////////////////////////////////////////////////////////////////////////////
	//
	//	Counter wrapper
	//

	class Counter
	{
	private:
		// Counter Name
		WCHAR	m_wszName[CounterInstanceMap::MAX_COUNTER_NAME];

		// Counter Link
		CounterType* m_pCounter;

	public:
		Counter( const WCHAR* wszName );
		~Counter();

		inline const WCHAR* GetCounterName() const;

		void SetCounter( CounterType *pCounter );

		inline bool IsConnected();

		inline operator CounterType();

		inline CounterType operator++();   // prefix

		inline CounterType operator++(int);    // postfix

		inline CounterType operator--();   // prefix

		inline CounterType operator--(int);    // postfix

		inline CounterType operator+=(int value);

		inline CounterType operator-=(int value);


		inline CounterType operator=(CounterType value);


		inline CounterType operator=(Counter& value);
	};




#include "PerfCounter.inl"


}; // namespace Perf
} // namespace BR






