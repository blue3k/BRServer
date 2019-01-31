// SRPerf.cpp : Implementation of CSRPerf
#include "stdafx.h"

#include "Common/PerfCounter.h"
#include "Common/StrUtil.h"
#include "SRWMIPerf_i.h"
#include "SRPerf.h"
#include "SRPerfInstance.h"
#include "Refresher.h"






////////////////////////////////////////////////////////////////////////////////
//
//	Counter WMI Instances
//




WMICounter::WMICounter( const WCHAR* wszName )
	:Perf::Counter( wszName ),
	m_CounterType(CIM_UINT32),
	m_hCounter(0)
{
}

HRESULT WMICounter::ConnectWMI( IWbemObjectAccess *pAccess )
{
	HRESULT hr = S_OK;

	//get the handle of property for writing later
	TrcChk( pAccess->GetPropertyHandle( CComBSTR( GetCounterName() ), 0, &m_hCounter ) );
	BSTR bstrJunk = NULL;
	TrcChk( pAccess->GetPropertyInfoByHandle( m_hCounter, &bstrJunk, &m_CounterType ) );
	if( bstrJunk )
		SysFreeString( bstrJunk );

Proc_End:
	return hr;
}


HRESULT WMICounter::Refresh( bool bAlive, IWbemObjectAccess *pAccess )
{
    HRESULT hr = S_OK;

	DWORD dwVal = 0;
	UINT64	lwVal;
    double dbVal;

    if( m_hCounter == NULL )
        return E_FAIL;

	dwVal = (UINT32)(*this);

#ifdef _DEBUG
	if( g_pFile ) fwprintf_s( g_pFile, L"WMICounter::Refresh(%p) %s:%d, (%s) \n", pAccess, GetCounterName(), dwVal, bAlive ? L"true" : L"false" );
#endif

	if( !bAlive ) dwVal = 0;

	switch(  m_CounterType )
	{
	case CIM_REAL64:
		dbVal = (double) dwVal;
		hr = pAccess->WritePropertyValue( m_hCounter, sizeof(dbVal), (const BYTE*)&dbVal );
		return hr;
		break;

	case CIM_UINT64:
		lwVal = (UINT64)dwVal;
		hr = pAccess->WritePropertyValue( m_hCounter, sizeof(lwVal), (const BYTE*)&lwVal );
		return hr;
		break;

	case CIM_REAL32:
	case CIM_SINT32:
	case CIM_UINT32:
		dwVal = (UINT32)(*this);
		hr = pAccess->WriteDWORD( m_hCounter, dwVal);
		return hr;
		break;

	default:
		dwVal = (UINT32)(*this);
		hr = pAccess->WriteDWORD( m_hCounter, dwVal);
		return hr;
		break;

	}
}



////////////////////////////////////////////////////////////////////////////////
//
//	Counter Instance 
//



/////////////////////////////////////////////////////////////////////////////////////////////
//
//	Static counter instance list management
//

UINT WMICounterInstance::stm_GenID = 0;

WMICounterInstance::InstanceMap WMICounterInstance::stm_InstanceByFileName;
WMICounterInstance::InstanceIDMap WMICounterInstance::stm_InstanceByID;



HRESULT WMICounterInstance::RefreshInstanceList( IWbemServices __RPC_FAR *pNamespace, IWbemContext __RPC_FAR *pCtx )
{
	HRESULT hr = S_OK;

#ifdef _DEBUG
	if( g_pFile ) fwprintf_s( g_pFile, L"RefreshInstanceList \n" );
#endif

	std::list<std::wstring> fileNames;
	EnumerateFileList( fileNames );

#ifdef _DEBUG
	if( g_pFile )
	{
		fwprintf_s( g_pFile, L"NumInstance :  %d \n", fileNames.size() );
		std::for_each( fileNames.begin(), fileNames.end(), []( const std::wstring& name )
		{
			fwprintf_s( g_pFile, L"	Instance(%s) \n", name.c_str() );
		});
	}
#endif

	std::for_each( fileNames.begin(), fileNames.end(), [&]( const std::wstring& name )
	{
		WMICounterInstance* pCounterInstance = NULL;
		auto itFound = ::WMICounterInstance::stm_InstanceByFileName.find( name );
		if( itFound != ::WMICounterInstance::stm_InstanceByFileName.end() )
		{
			CComPtr<IWbemClassObject> spTemplate;

			pCounterInstance = itFound->second;
			AssertRel( pCounterInstance != nullptr );

#ifdef _DEBUG
			if( g_pFile ) fwprintf_s( g_pFile, L"Existing instance(%s:%s) \n", pCounterInstance->GetClass(), pCounterInstance->GetInstanceName() );
#endif
			// just update
			pCounterInstance->UpdateHeartBit();

			defChk( pNamespace->GetObject( CComBSTR(pCounterInstance->GetClass()),0,pCtx,&spTemplate,0) );

			defChk( pCounterInstance->RefreshInstance( spTemplate ) );

			pCounterInstance->RefreshCounterList( spTemplate );
		}
		else
		{
			// create new one
			pCounterInstance = new WMICounterInstance();

#ifdef _DEBUG
			if( g_pFile ) fwprintf_s( g_pFile, L"New instance Load(%s) \n", name.c_str() );
#endif

			defChk( pCounterInstance->OpenForRead( name.c_str() ) );

#ifdef _DEBUG
			if( g_pFile ) fwprintf_s( g_pFile, L"New instance(%s:%s) \n", pCounterInstance->GetClass(), pCounterInstance->GetInstanceName() );
#endif

			pCounterInstance->IncRefCount();
			::WMICounterInstance::stm_InstanceByFileName.insert( std::make_pair( name, pCounterInstance ) );
			::WMICounterInstance::stm_InstanceByID.insert( std::make_pair( stm_GenID, pCounterInstance ) );
		}

	Proc_End:
		DbgTracehr(hr);
		;
	});


	DbgTracehr(hr);
	return hr;
}

HRESULT WMICounterInstance::RefreshInstanceSync( IWbemObjectSink __RPC_FAR *pSink )
{
	HRESULT hr = S_OK;

	std::for_each( stm_InstanceByFileName.begin(), stm_InstanceByFileName.end(), [&]( std::pair<const std::wstring,WMICounterInstance*>& pair )
	{
		WMICounterInstance *pInst = pair.second;
		IWbemClassObject *pObject = pInst->GetWMIObject();
		if( pObject )
		{
			defChk( pSink->Indicate(1, &pObject) );
		}
Proc_End:
		;
	});


	return hr;
}


// Open counter instance if exist
HRESULT WMICounterInstance::FindPerfInstance( const WCHAR* wszInstance, WMICounterInstance* &pFound )
{
	HRESULT hr = E_FAIL;

	size_t szInstance = wcslen(wszInstance);

	std::for_each( stm_InstanceByFileName.begin(), stm_InstanceByFileName.end(), 
		[&]( std::pair<const std::wstring,WMICounterInstance*>& pair )
	{
		WMICounterInstance *pInst = pair.second;
		if( pInst->IsOpened() 
			&& pInst->GetWMIObject()
			&& StrUtil::StringCmp( pInst->GetInstanceName(), wcslen(pInst->GetInstanceName())+1, wszInstance, szInstance+1 ) == 0 )
		{
			pFound = pInst;

			hr = S_OK;
			goto Proc_End;
		}

	Proc_End:
		;
	});

	return hr;
}


// Open counter instance if exist
HRESULT WMICounterInstance::FindPerfInstanceByClass( const WCHAR* wszClass, std::vector<WMICounterInstance*>& instance )
{
	size_t szClass = wcslen(wszClass);

	std::for_each( stm_InstanceByFileName.begin(), stm_InstanceByFileName.end(), 
		[&]( std::pair<const std::wstring,WMICounterInstance*>& pair )
	{
		WMICounterInstance* pInst = pair.second;
		if( ::StrUtil::StringCmp( pInst->GetClass(), ::Perf::CounterInstanceMap::MAX_COUNTER_NAME, wszClass, szClass ) == 0 )
		{
			instance.push_back(pInst);
		}
	});

	return instance.size() > 0 ? S_OK : E_FAIL;
}





/////////////////////////////////////////////////////////////////////////////////////////////
//
//	Class interfacess
//

WMICounterInstance::WMICounterInstance()
{
	m_ID = ++stm_GenID;

	m_handleTimestamp_PerfTime = 0;
	m_handleFrequency_PerfTime = 0;
	m_handleTimestamp_Sys100NS = 0;
	m_handleFrequency_Sys100NS = 0;
	m_handleTimestamp_Object = 0;
	m_handleFrequency_Object = 0;
}

WMICounterInstance::~WMICounterInstance()
{
	Close();
}


IWbemClassObject* WMICounterInstance::GetWMIObject()
{
	return m_spObject;
}

IWbemObjectAccess* WMICounterInstance::GetWMIAccess()
{
	return m_spAccess;
}

UINT WMICounterInstance::GetID()
{
	return m_ID;
}


// Reference count
void WMICounterInstance::IncRefCount()
{
	m_RefCount.InterlockedIncrement();
}

CounterType WMICounterInstance::DecRefCount()
{
	return m_RefCount.InterlockedDecrement();
}


HRESULT WMICounterInstance::RefreshInstance( IWbemClassObject *pTemplate )
{
	HRESULT hr = S_OK;
	CComVariant varName;

	CComPtr<IWbemObjectAccess> spAccess;
	CComPtr<IWbemObjectAccess> spAccessCopy;
	CComPtr<IWbemClassObject> spObjectCopy;

	//Get the IWbemObjectAccess interface to the object
	TrcChk( pTemplate->QueryInterface(IID_IWbemObjectAccess, (PVOID*)&spAccess) );

	// Create a new instance from the template
	TrcChk( pTemplate->SpawnInstance(0, &spObjectCopy) );

	// Obtain the IWbemObjectAccess interface
	TrcChk( spObjectCopy->QueryInterface(IID_IWbemObjectAccess, (PVOID*)&spAccessCopy) );

	// Set the Name key
	varName = GetInstanceName();
	TrcChk( spObjectCopy->Put( CComBSTR(L"Name"),0,&varName, CIM_STRING ) );


	//get the handle of property for writing later
	TrcChk( spAccessCopy->GetPropertyHandle(A2BSTR("Timestamp_PerfTime"), 0, &m_handleTimestamp_PerfTime) );
	TrcChk( spAccessCopy->GetPropertyHandle(A2BSTR("Frequency_PerfTime"), 0, &m_handleFrequency_PerfTime) );
	TrcChk( spAccessCopy->GetPropertyHandle(A2BSTR("Timestamp_Sys100NS"), 0, &m_handleTimestamp_Sys100NS) );
	TrcChk( spAccessCopy->GetPropertyHandle(A2BSTR("Frequency_Sys100NS"), 0, &m_handleFrequency_Sys100NS) );
	TrcChk( spAccessCopy->GetPropertyHandle(A2BSTR("Timestamp_Object"), 0, &m_handleTimestamp_Object) );
	TrcChk( spAccessCopy->GetPropertyHandle(A2BSTR("Frequency_Object"), 0, &m_handleFrequency_Object) );

	m_spAccess = spAccessCopy;
	m_spObject = spObjectCopy;


Proc_End:

	return hr;
}


void WMICounterInstance::RefreshCounterList( IWbemClassObject *pTemplate )
{
	SAFEARRAY *psa=NULL;
	HRESULT hRes = S_OK;

#ifdef _DEBUG
	if( g_pFile ) fwprintf_s( g_pFile, L"WMICounterInstance::RefreshCounterList %s, %s\n", GetClass(), GetInstanceName() );
#endif

	// clear counter handle if exist
	std::vector<WMICounter*>::iterator iterCur = m_Counters.begin();
	for( ; iterCur != m_Counters.end(); iterCur++ )
	{
		delete *iterCur;
	}
	m_Counters.clear();

	//
	// Rather than hardcode property names
	// Get names from class definition 
	// Just a pointer to elements of safearray.
	// Then use these names to get the shared memory pointer
	// and WMI property handle
	//

	pTemplate->GetNames(NULL,WBEM_FLAG_ALWAYS |WBEM_FLAG_LOCAL_ONLY ,NULL,&psa);

	if (psa)
	{
		BSTR HUGEP *pbstr;
		HRESULT hr;
		long i, cElements; 
		USES_CONVERSION;
	    
		//
		// Get a pointer to the elements of the array.
		// Enumerate and create list of counters and intialize
		//
		hr = SafeArrayAccessData(psa, (void HUGEP**)&pbstr);
		if (SUCCEEDED(hr))
		{

			cElements = psa->rgsabound->cElements; 
			//if( g_pFile ) fwprintf_s( g_pFile, L"cElements %d\n", cElements );
			for (i = 0; i < cElements; i++)
			{
#ifdef _DEBUG
				if( g_pFile ) fwprintf_s( g_pFile, L"NewCnt %s\n", pbstr[i] );
#endif
				WMICounter *pCounter = new WMICounter( pbstr[i] );
				if (pCounter)
				{
					//Open shared memory pointer
					if( GetCounter( *pCounter ) != S_OK )
					{
						delete pCounter;
						continue;
					}

					pCounter->ConnectWMI( m_spAccess );

					m_Counters.push_back( pCounter );
				}

			}

			SafeArrayUnaccessData(psa);
		}

		SafeArrayDestroy(psa);
	}

	DbgTrace();
}


// Refresh WMI object value
HRESULT	WMICounterInstance::Refresh()
{
	HRESULT hr = WBEM_NO_ERROR;

	if( IsOpened() )
	{
		bool bAlive = CheckHeartBit();

#ifdef _DEBUG
	if( g_pFile ) fwprintf_s( g_pFile, L"WMICounterInstance::Refresh Live: %s:%s, %d , Cnt:%d\n", GetClass(), GetInstanceName(), (int)bAlive, m_Counters.size() );
#endif
		// connect counter if not connected exist
		std::vector<WMICounter*>::iterator iterCur;
		//if( bAlive )
		//{
			iterCur = m_Counters.begin();
			for( ; iterCur != m_Counters.end(); iterCur++ )
			{
				if( !(*iterCur)->IsConnected() )
					GetCounter( *(*iterCur) );
			}
		//}
		//else
		//{
		//	// Disconnect and close if counter not alive
		//	// close file map
		//	iterCur = m_Counters.begin();
		//	for( ; iterCur != m_Counters.end(); iterCur++ )
		//	{
		//		if( (*iterCur)->IsConnected() )
		//			(*iterCur)->SetCounter( NULL );
		//	}

		//	__super::Close();
		//}

		iterCur = m_Counters.begin();
		for( ; iterCur != m_Counters.end(); iterCur++ )
		{
			(*iterCur)->Refresh( bAlive, m_spAccess );
		}

	}
	else // if closed filemap
	{
		// try reopen file map
		//Open( GetClass(), GetInstanceName() );
	}

	return hr;
}

void	WMICounterInstance::Close()
{
	if (m_spAccess)
	{
		m_spAccess.Release();
	}

	if (m_spObject)
	{
		m_spObject.Release();
	}

	std::vector<WMICounter*>::iterator iterCur = m_Counters.begin();
	for( ; iterCur != m_Counters.end(); iterCur++ )
	{
		delete *iterCur;
	}
	m_Counters.clear();

	__super::Close();
}

void	WMICounterInstance::SetTimestamp( UINT64 timestamp, UINT64 frequency )
{
	m_spAccess->WritePropertyValue( m_handleTimestamp_PerfTime, sizeof(timestamp), (const BYTE*)&timestamp );
	m_spAccess->WritePropertyValue( m_handleFrequency_PerfTime, sizeof(frequency), (const BYTE*)&frequency);
	m_spAccess->WritePropertyValue( m_handleTimestamp_Sys100NS, sizeof(timestamp), (const BYTE*)&timestamp );
	m_spAccess->WritePropertyValue( m_handleFrequency_Sys100NS, sizeof(frequency), (const BYTE*)&frequency);
	m_spAccess->WritePropertyValue( m_handleTimestamp_Object, sizeof(timestamp), (const BYTE*)&timestamp );
	m_spAccess->WritePropertyValue( m_handleFrequency_Object, sizeof(frequency), (const BYTE*)&frequency);
}

