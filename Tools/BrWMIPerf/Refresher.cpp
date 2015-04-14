////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
// Date : 11/25/2010
//
// Description : Performance WMI Refresher
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/StrUtil.h"
#include "Refresher.h"
#include "SRPerfInstance.h"





//	CRefresher
CRefresher::CRefresher( CSRPerf* pProvider )
	:m_lRef(0),
	m_pProvider(NULL)
{
	int nIndex = 0;

	// Retain a copy of the provider
	// =============================

	m_pProvider = pProvider;
	if (m_pProvider)
		m_pProvider->AddRef();

	for( UINT iEnum = 0; iEnum < m_Enumerators.max_size(); iEnum++ )
		m_Enumerators[iEnum] = NULL;
}


CRefresher::~CRefresher()
{
	auto iterCur = m_Instances.begin();
	for( ; iterCur != m_Instances.end(); iterCur++ )
	{
		iterCur->second;
		delete iterCur->second;
	}
	m_Instances.clear();


	// Release the provider
	// ====================

	if (m_pProvider)
		m_pProvider->Release();

	for( UINT iEnum = 0; iEnum < m_Enumerators.max_size(); iEnum++ )
	{
		if( m_Enumerators[iEnum] )
			m_Enumerators[iEnum]->Release();
		m_Enumerators[iEnum] = NULL;
	}
	
}

//////////////////////////////////////////////////////////////////////
//  Initializes the instances cache.
//     
//  Parameters:
//		pDSClass	- A template of the WMI object from which we 
//							will create our instances
//////////////////////////////////////////////////////////////////////
DWORD CRefresher::Initialize()
{
	DWORD dwRes = WBEM_NO_ERROR;
    
	return WBEM_NO_ERROR;
}


//  Adds an object to the refresher.   This is a private mechanism
//  used by CSRPerf and not part of the COM interface.
//
//	All of the objects are maintained by the m_apInstances array.  This
//	is set up during a call to Initialize.  Multiple additions of the same
//	object to a refresher is legal, so to optimize the refresh, the 
//	same object is passed back but with a different ID.  The ID:object
//	relationships are maintained by m_apInstMap.
DWORD CRefresher::AddObject( IWbemClassObject *pTemplate, IWbemObjectAccess **ppReturnObj, long *plId )
{
	HRESULT hr   = WBEM_NO_ERROR;

	int     nIndex  = 0;
	bool    bDone   = false;
	CComVariant     vName;
	WMICounterInstance *pInstance = NULL;
	const WCHAR *strInstanceName = NULL;


	if (NULL == pTemplate || !plId || !ppReturnObj)
		return WBEM_E_INVALID_PARAMETER;

	*ppReturnObj=NULL;
	*plId=0;


	// Grab the instance ID
	// ====================

	TrcChk( pTemplate->Get(CComBSTR(L"Name"),0,&vName,NULL,NULL) );

	strInstanceName = CComBSTR(vName.bstrVal);
	WMICounterInstance::FindPerfInstance( strInstanceName, pInstance );

	WCHAR strFileName[1024];
	WMICounterInstance::FileNameFromInstanceName( strFileName, strInstanceName );

	// Not cached yet create new one
	if( pInstance == NULL )
	{
		TrcMem( pInstance = new WMICounterInstance() );
		TrcChk( pInstance->OpenForRead( strFileName ) );
		TrcChk( pInstance->RefreshInstance( pTemplate ) );

		pInstance->RefreshCounterList( pTemplate );

		pInstance->IncRefCount();
		WMICounterInstance::stm_InstanceByFileName.insert( std::make_pair(std::wstring(strFileName), pInstance) );
		WMICounterInstance::stm_InstanceByID.insert( std::make_pair( pInstance->GetID(), pInstance ) );

		m_Instances.insert( std::make_pair( pInstance->GetID(), pInstance ) );
	}

	*ppReturnObj = pInstance->GetWMIAccess();

	//Must add reference here because we are returning object
	(*ppReturnObj)->AddRef();

	*plId=(DWORD ) Util::Crc32( wcslen(strFileName), (BYTE*)strFileName ); //use crc32 for ID

Proc_End:

	return WBEM_NO_ERROR;
}


//  Removes an object from the refresher.  This is a private mechanism 
//	used by CSRPerf and not part of the COM interface.
//
//  Removes an object from the refresher by ID.   Use a simple linear
//	search to locate the unique ID in the m_apInstMap.  Leave the 
//	cache instance intact until the refrehser is destroyed.
//
//	Parameters:
//		plID		- the unique ID
DWORD CRefresher::RemoveObject(long lId)
{
#ifdef _DEBUG
	if( g_pFile ) fwprintf_s( g_pFile, L"CRefresher::RemoveObject Obj[%d], \n", lId );
#endif

	DWORD   dwRes = WBEM_E_NOT_FOUND;


	auto itInstance = m_Instances.find( lId );
	if( itInstance == m_Instances.end() )
		return dwRes;


	if( itInstance->second->DecRefCount()  <= 0 )
	{
		//delete itInstance->second;
		m_Instances.erase( itInstance );
	}


	return dwRes;
}

//  Adds an enumerator to the refresher.   This is a private mechanism
//  used by CSRPerf and not part of the COM interface.
//
//  The ID we return for future identification is simply
//  the array index.
//
//	Note that the the refresher uses an array of enumerators.  For a 
//	hiperf provider that only supports one class, having more than one 
//	enumerator does not really make sense since a single enumerator will
//	manage all instances of a single class for a given refresher.  
//	NUM_ENUMERATORS may be set to reflect the number of classes if the 
//	proper class / enumerator management code is added.  The implementation
//	is up to the provider.
//
//	Parameters:
//		pHiPerfEnum	- the enumerator
//		plID		- the ID
DWORD CRefresher::AddEnum(IWbemServices* pNamespace,IWbemContext* pContext,IWbemHiPerfEnum *pHiPerfEnum, LPCWSTR wszClass, long *plId)
{
	HRESULT hr = S_OK;
	CComPtr<IWbemClassObject> spTemplate;
    const long MAX_OBJECTS = 255;
    LONG  lObjectIds[MAX_OBJECTS];
    IWbemObjectAccess *pObjectArray[MAX_OBJECTS];
	std::vector<WMICounterInstance*> instances;
	UINT uiNumObj = 0, uiNumResObj = 0;

	if (NULL == pHiPerfEnum)
        return WBEM_E_INVALID_PARAMETER;



    TrcChk( pNamespace->GetObject(W2BSTR(wszClass),0,pContext,&spTemplate,0) );

	DbgTrace();
	
#ifdef _DEBUG
	if( g_pFile ) fwprintf_s( g_pFile, L"CRefresher::AddEnum Class:%s\n", wszClass );
#endif

	TrcChk( WMICounterInstance::FindPerfInstanceByClass( wszClass, instances ) );

	DbgTrace();

#ifdef _DEBUG
	if( g_pFile ) fwprintf_s( g_pFile, L"CRefresher::AddEnum Class:%s, Obj[%d], \n", wszClass, uiNumObj );
#endif

	std::for_each( instances.begin(), instances.end(), [&]( WMICounterInstance* pInst )
	{
		auto localFound = m_Instances.find( pInst->GetID() );
		if( localFound == m_Instances.end() )
		{
			m_Instances.insert( std::make_pair( pInst->GetID(), pInst ) );
		}

		DbgTrace();

		pInst->RefreshInstance( spTemplate );

		pInst->RefreshCounterList( spTemplate );

		lObjectIds[uiNumResObj] = pInst->GetID();
		pObjectArray[uiNumResObj] = pInst->GetWMIAccess();
		uiNumResObj++;
	});



	TrcMem( pHiPerfEnum );

	DbgTrace();

	if( uiNumResObj > 0 )
	{
		for( UINT iEnum = 0; iEnum < m_Enumerators.max_size(); iEnum++ )
		{
			if( m_Enumerators[iEnum] == NULL )
			{
#ifdef _DEBUG
	if( g_pFile ) fwprintf_s( g_pFile, L"AddEnum Class:%s, Obj[%d], \n", wszClass, uiNumResObj );
#endif
				TrcChk( pHiPerfEnum->AddObjects( 0, uiNumResObj, lObjectIds, pObjectArray ) );
				DbgTrace();
				m_Enumerators[iEnum] = pHiPerfEnum;
				pHiPerfEnum->AddRef();
				*plId = (long)iEnum + ENUMID_OFFSET;
				goto Proc_End;
			}
		}

		// Enumerator max
		hr = WBEM_E_OUT_OF_MEMORY;
	}
	else
		hr = WBEM_E_NOT_FOUND;


Proc_End:

	DbgTracehr(hr);

	return hr;
}


//  Removes an enumerator from the refresher.  This is a private mechanism 
//	used by CSRPerf and not part of the COM interface.
//
//  Removes an enumerator from the refresher by ID.   In our case, the ID
//  is actually the array index we used internally, so it is simple
//  to locate and remove the object.
//
//	Parameters:
//		plID		- the ID
DWORD CRefresher::RemoveEnum(long lId)
{
	// Verify the element
	// ==================
#ifdef _DEBUG
	if( g_pFile ) fwprintf_s( g_pFile, L"CRefresher::RemoveEnum Obj[%d], \n", lId );
#endif

	lId -= ENUMID_OFFSET;

	if( lId < 0 || lId >= (long)m_Enumerators.max_size() || m_Enumerators[lId] == NULL )
		return WBEM_E_NOT_FOUND;

	m_Enumerators[lId]->Release();
	m_Enumerators[lId] = NULL;

	return WBEM_S_NO_ERROR;
}


STDMETHODIMP CRefresher::QueryInterface(REFIID riid, void** ppv)
{
    if (riid == IID_IUnknown)
        *ppv = (LPVOID)(IUnknown*)this;
	else if (riid == IID_IWbemRefresher)
		*ppv = (LPVOID)(IWbemRefresher*)this;
    else return E_NOINTERFACE;

   	((IUnknown*)*ppv)->AddRef();
    return S_OK;
}

//	Standard COM AddRef
STDMETHODIMP_(ULONG) CRefresher::AddRef()
{
    return InterlockedIncrement(&m_lRef);
}

//	Standard COM Release
STDMETHODIMP_(ULONG) CRefresher::Release()
{
    long lRef = InterlockedDecrement(&m_lRef);
    if(lRef == 0)
        delete this;

    return lRef;
}

//  Executed to refresh a set of instances bound to the particular 
//  refresher.
//
//	In most situations the instance data, such as counter values and 
//	the set of current instances within any existing enumerators, would 
//	be updated whenever Refresh was called.  For this example, we are 
//	going to or data source to update the counter data.  Since we are 
//	using a fixed set of instances, we are not adding or removing anything
//	from the enumerator.
//
//	Parameters:
//		lFlags	- not used
STDMETHODIMP CRefresher::Refresh(/* [in] */ long lFlags)
{
#ifdef _DEBUG
	if( g_pFile ) fwprintf_s( g_pFile, L"CRefresher::Refresh NumInst:%d\n", m_Instances.size() );
#endif

	HRESULT	hRes = WBEM_NO_ERROR;

	// A simple loop that iterates through all instances that have been
	// added to the refresher, and updates their counter values
	// ================================================================

	auto iterInst = m_Instances.begin();
	for( ; iterInst != m_Instances.end(); iterInst++ )
	{
		iterInst->second->Refresh();
	}

	return hRes;
}
