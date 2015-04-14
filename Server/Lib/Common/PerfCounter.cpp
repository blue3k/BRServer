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


#include "StdAfx.h"
#include "Common/StrUtil.h"
#include "Common/Utility.h"
#include "Common/TimeUtil.h"
#include "Common/PerfCounter.h"
#include <dbghelp.h>
#include <process.h>


#pragma intrinsic (_WriteBarrier)

namespace BR {
namespace Perf {

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	NULL Security Descriptor create
	//


	SECURITY_ATTRIBUTES *CreateNULLSD()
	{
		DWORD dwResult=NOERROR;
		SECURITY_ATTRIBUTES  *psa=nullptr;
		PSECURITY_DESCRIPTOR  pSD=nullptr;
	    
		psa = (SECURITY_ATTRIBUTES *)LocalAlloc( LPTR, sizeof(SECURITY_ATTRIBUTES) );
		pSD = (PSECURITY_DESCRIPTOR)LocalAlloc( LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);

		if ( nullptr == pSD || nullptr == psa )
		{
			goto Proc_End;
		}

		if( !InitializeSecurityDescriptor( pSD, SECURITY_DESCRIPTOR_REVISION ) )
		{
			goto Proc_End;
		}

		// Add a nullptr DACL to the security descriptor..
		if( !SetSecurityDescriptorDacl(pSD, TRUE, nullptr, FALSE) )
		{
			goto Proc_End;
		}

		psa->nLength = sizeof(SECURITY_ATTRIBUTES);
		psa->lpSecurityDescriptor = pSD;
		psa->bInheritHandle = TRUE;

		return psa;

	Proc_End:
		if ( pSD )
			LocalFree(pSD);
	    
		if ( psa )
			LocalFree(psa);
	    
		return nullptr;
	}



	//////////////////////////////////////////////////////////////////////
	//
	// FreeNULLSecurity
	//
	void FreeNULLSD( SECURITY_ATTRIBUTES * psa )
	{
		if( psa )
		{
			if( psa->lpSecurityDescriptor )
				LocalFree( psa->lpSecurityDescriptor );
			LocalFree( psa );
		}
	}



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	CounterInstance class
	//

	static const WCHAR *g_szInstanceFileNameTemplateBase=L"%s\\TEMP\\%s";
	static const WCHAR *g_szInstanceFileNameTemplate=L"%s\\TEMP\\SRPerf%s.tmp";


	// Constructor
	CounterInstanceMap::CounterInstanceMap()
		:m_pCounters(nullptr),
		m_hFile( INVALID_HANDLE_VALUE ),
		m_hFileMap( INVALID_HANDLE_VALUE ),
		m_szMaxCounter(0),
		m_bIsReadMode(false),
		m_bIsAlive(false)
	{
		m_wszMapFileName[0] = L'\0';
		//Open( wszInstance, uiMaxCounter );
	}


	// Constructor
	CounterInstanceMap::CounterInstanceMap( const WCHAR* wszClass, const WCHAR* wszInstance, ... )
		:m_pCounters(nullptr),
		m_hFile( INVALID_HANDLE_VALUE ),
		m_hFileMap( INVALID_HANDLE_VALUE ),
		m_szMaxCounter(0),
		m_bIsReadMode(false),
		m_bIsAlive(false)
	{
		m_wszMapFileName[0] = L'\0';

		va_list vaArgs;
		va_start(vaArgs, wszInstance);

		HRESULT hr = Create( wszClass, wszInstance, vaArgs );

		va_end(vaArgs);
	}

	// Destructor
	CounterInstanceMap::~CounterInstanceMap()
	{
		Close();
	}


	// create counter file map
	HRESULT CounterInstanceMap::Create( const WCHAR* wszClass, const WCHAR* wszInstance, ... )
	{
		va_list vaArgs;
		va_start(vaArgs, wszInstance);

		HRESULT hr = Create( wszClass, wszInstance, vaArgs );

		va_end(vaArgs);

		return hr;
	}

	// create counter file map
	HRESULT CounterInstanceMap::Create( const WCHAR* wszClass, const WCHAR* wszInstance, va_list vaArgs )
	{
		HRESULT hr = S_OK;

		if( m_pCounters )
			Close();

		std::list<Counter*> counterInsts;
		std::list<Counter*>::iterator iterCur;
		Counter* pCntCur = va_arg( vaArgs, Counter* );
		while( pCntCur != nullptr )
		{
			counterInsts.push_back( pCntCur );
			pCntCur = va_arg( vaArgs, Counter* );
		}
		size_t uiMaxCounter = counterInsts.size();
		m_szMaxCounter = uiMaxCounter;



		WCHAR szFullPath[1024];
		SECURITY_ATTRIBUTES* psa;
		size_t uiMapSize = uiMaxCounter*sizeof(Counter) + sizeof(CounterInstanceMap);

		psa = CreateNULLSD();
		defChkPtr( wszInstance );
		defChkPtr( psa );


		FileNameFromInstanceName( szFullPath, wszInstance );



		m_hFile = CreateFileW( szFullPath, 
				GENERIC_READ | GENERIC_WRITE ,FILE_SHARE_READ | FILE_SHARE_WRITE,       
				psa, OPEN_ALWAYS,
				FILE_ATTRIBUTE_NORMAL , nullptr);

		if (m_hFile == INVALID_HANDLE_VALUE)
		{
			defTrace( Trace::TRC_ERROR, "Opening Counter instance Failed, %0%:%1%", wszClass, wszInstance );
			defErr( E_UNEXPECTED );
		}


		if( m_hFile == INVALID_HANDLE_VALUE )
		{
			// Failed even maximum try
			hr = E_FAIL;
			goto Proc_End;
		}

		m_hFileMap = CreateFileMapping(  m_hFile,              
			psa,PAGE_READWRITE,           
			0,   
			(DWORD)uiMapSize,    
			nullptr);

		if ( m_hFileMap == nullptr )
		{
			hr = E_FAIL;
			goto Proc_End;
		}

		m_pCounters = (CounterMap *)MapViewOfFile( m_hFileMap, FILE_MAP_WRITE, 0, 0 ,0 );
		if ( m_pCounters == nullptr )
		{
			hr = E_FAIL;
			goto Proc_End;
		}

		StrUtil::StringCpy( m_wszMapFileName, szFullPath );

		FreeNULLSD( psa );


		// Mapping succeeded so initialize counter instance
		memset( (void*)m_pCounters, 0, uiMapSize );

		StrUtil::StringCpy( m_pCounters->szName, wszInstance );
		StrUtil::StringCpy( m_pCounters->szClass, wszClass );


		m_bIsReadMode = false;


		// create all counters
		iterCur = counterInsts.begin();
		for( ; iterCur != counterInsts.end(); iterCur++ )
		{
			CreateCounter( **iterCur );
		}

		return hr;

	Proc_End:

		HRESULT error = GetLastHRESULT();

		FreeNULLSD( psa );

		Close();

		return error;
	}



	// Enumerate instance files
	HRESULT CounterInstanceMap::EnumerateFileList( std::list<std::wstring>& fileNames )
	{
		HRESULT hr = S_OK;

		WCHAR fileName[1024];
		WCHAR szSearchPath[1024];
		WCHAR szTemp[MAX_PATH];
		SECURITY_ATTRIBUTES* psa;

		psa = CreateNULLSD();
		defChkPtr( psa );


		//Create file prefix
		GetWindowsDirectoryW( szTemp, sizeof(szTemp) );


		FileNameFromInstanceName( szSearchPath, L"*" );

		// search file pattern
		WIN32_FIND_DATAW findData;
		memset( &findData, 0, sizeof(findData) );
		HANDLE hFind = FindFirstFileW( szSearchPath, &findData );
		if (hFind == INVALID_HANDLE_VALUE)
			defErr(E_FAIL);

		do {
			swprintf_s( fileName, g_szInstanceFileNameTemplateBase, szTemp, findData.cFileName );
			fileNames.push_back( std::wstring( fileName ) );
		} while( FindNextFile( hFind, &findData ) );


		FindClose(hFind);

	Proc_End:

		return hr;
	}


	HRESULT CounterInstanceMap::FileNameFromInstanceName( WCHAR (&fileName)[1024], const WCHAR* strInstanceName )
	{
		WCHAR szTemp[MAX_PATH];

		//Create file prefix
		GetWindowsDirectoryW( szTemp, sizeof(szTemp) );

		// Instance name is file name
		swprintf_s( fileName, g_szInstanceFileNameTemplate, szTemp, strInstanceName );

		return S_OK;
	}


	// Open counter file map for read. with instance file name
	HRESULT CounterInstanceMap::OpenForRead( const WCHAR *wszFileName )
	{
		HRESULT hr = S_OK;

		if( m_pCounters )
			Close();

		SECURITY_ATTRIBUTES* psa;
		DWORD uiMapSize = 0;
		//DWORD tCurTime = (DWORD)Util::Time.GetUTCSec();

		HANDLE hFile = INVALID_HANDLE_VALUE;
		HANDLE hFileMap = INVALID_HANDLE_VALUE;
		CounterMap *pCounters = nullptr;

		psa = CreateNULLSD();
		if( psa == nullptr )
			goto Proc_End;

		defChkPtr(wszFileName);

		hFile = CreateFileW( wszFileName, 
			  GENERIC_READ | GENERIC_WRITE ,FILE_SHARE_READ | FILE_SHARE_WRITE,       
			  psa, OPEN_EXISTING,
			  FILE_ATTRIBUTE_NORMAL , NULL);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			uiMapSize = GetFileSize( hFile, NULL );

			if( uiMapSize == 0 ) // no counter initialized yet, check next
				trcErr( E_FAIL );

			hFileMap = CreateFileMapping( hFile,              
				psa,PAGE_READONLY,           
				0,   
				uiMapSize,    
				NULL );

			if ( hFileMap == NULL )
				trcErr( E_UNEXPECTED );

			pCounters = (CounterMap *)MapViewOfFile( hFileMap, FILE_MAP_READ, 0, 0 ,0 );
			if ( pCounters == NULL )
				trcErr( E_UNEXPECTED );

			// We will use counter even life time expired
			//// No counter initialized or Counter life time expired
			//if( pCounters->NumCounter == 0 || (int)(tCurTime-pCounters->tHeartBitTime) > MAX_INSTANCE_LIFE_TIME )
			//	trcErr( E_UNEXPECTED );

			if( wcslen(pCounters->szClass) == 0 && wcslen(pCounters->szName) == 0 )
				trcErr( E_FAIL );
		}

		// Failed finding instance?
		if( hFile == INVALID_HANDLE_VALUE )
		{
			// Failed even maximum try
			trcErr( E_FAIL );
		}

		trcChk( StrUtil::StringCpy( m_wszMapFileName, wszFileName ) );

		FreeNULLSD( psa );


		// Mapping succeeded
		m_pCounters = pCounters;
		m_hFile = hFile;
		m_hFileMap = m_hFileMap;

		m_szMaxCounter = m_pCounters->NumCounter;

		m_bIsReadMode = true;

		CheckHeartBit();


		return hr;


	Proc_End:

		HRESULT error = GetLastHRESULT();

		FreeNULLSD( psa );

		Close();


		if( pCounters )
		{
			UnmapViewOfFile( (void*)pCounters );
		}

		if( hFileMap )
			CloseHandle( hFileMap );

		if( hFile )
			CloseHandle( hFile );

		m_bIsAlive = false;


		return SUCCEEDED(error) ? hr : error;
	}

	// Close file map
	void CounterInstanceMap::Close()
	{
		if( m_pCounters )
		{
			//if( m_bIsRead )
			//{
			//	FlushViewOfFile 
			//}

			//ZeroMemory( (void*)m_pCounters, m_pCounters->NumCounter*sizeof(Counter) + sizeof(CounterInstanceMap) );
			UnmapViewOfFile( (void*)m_pCounters );
		}

		if( m_hFileMap )
			CloseHandle(m_hFileMap);

		if( m_hFile )
		{
			CloseHandle( m_hFile );

			// Will do kill it?
			//DeleteFileW( m_wszMapFileName );
			m_wszMapFileName[0] = L'\0';
		}
	}

	// Create counter at counter instance
	HRESULT CounterInstanceMap::CreateCounter( Counter &counterInst )
	{
		HRESULT hr = S_OK;

		if( counterInst.GetCounterName() == NULL || counterInst.GetCounterName()[0] == L'\0' )
			trcErr( E_POINTER );

		if( m_pCounters == NULL || m_bIsReadMode )
			trcErr( E_ABORT );

		if( (m_pCounters->NumCounter) >= m_szMaxCounter )
			trcErr( E_OUTOFMEMORY );


		UINT uiCounter = 0;
		for( ; uiCounter < m_pCounters->NumCounter; uiCounter++ )
		{
			if( StrUtil::StringCmp( counterInst.GetCounterName(), (INT)wcslen(counterInst.GetCounterName())+1, m_pCounters->counters[uiCounter].szName ) == 0 )
			{
				counterInst.SetCounter( &m_pCounters->counters[uiCounter].uiValue );
				return E_ABORT;
			}
		}

		trcChk( StrUtil::StringCpy( m_pCounters->counters[uiCounter].szName, counterInst.GetCounterName() ) );
		counterInst.SetCounter( &m_pCounters->counters[uiCounter].uiValue );
		m_pCounters->NumCounter++;

	Proc_End:

		return hr;
	}

	// Get counter at counter instance
	HRESULT CounterInstanceMap::GetCounter( Counter &counterInst )
	{
		HRESULT hr = S_OK;

		if( counterInst.GetCounterName() == NULL || counterInst.GetCounterName()[0] == L'\0' )
			trcErr( E_POINTER );

		if( m_pCounters == NULL )
			trcErr( E_ABORT );

		for( UINT uiCounter = 0; uiCounter < m_pCounters->NumCounter; uiCounter++ )
		{
			if( StrUtil::StringCmp( counterInst.GetCounterName(), (INT)wcslen(counterInst.GetCounterName())+1, m_pCounters->counters[uiCounter].szName ) == 0 )
			{
				if( !m_bIsAlive )
					m_pCounters->counters[uiCounter].uiValue = 0;

				counterInst.SetCounter( &m_pCounters->counters[uiCounter].uiValue );

				return S_OK;
			}
		}

		return E_FAIL;

	Proc_End:

		return hr;
	}


	// Heart bit
	void CounterInstanceMap::UpdateHeartBit()
	{
		if( m_pCounters == NULL || m_bIsReadMode )
			return;

		m_pCounters->tHeartBitTime = (UINT32)Util::Time.GetTimeUTCSec();
	}

	// Check Heart Bit
	bool CounterInstanceMap::CheckHeartBit()
	{
		m_bIsAlive = IsOpened() && ((Util::Time.GetTimeUTCSec() - m_pCounters->tHeartBitTime) < MAX_INSTANCE_LIFE_TIME);

		return m_bIsAlive;
	}



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Counter wrapper
	//

	Counter::Counter( const WCHAR* wszName )
		:m_pCounter(NULL)
	{
		StrUtil::StringCpy( m_wszName, wszName );
	}

	Counter::~Counter()
	{
	}

	void Counter::SetCounter( CounterType *pCounter )
	{
		m_pCounter = pCounter;
	}


}; // namespace Perf
} // namespace BR


