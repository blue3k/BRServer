// SRPerf.cpp : Implementation of CSRPerf
#include "stdafx.h"

#include "Common/PerfCounter.h"
#include "Common/StrUtil.h"
#include "SRWMIPerf_i.h"
#include "SRPerf.h"
#include "SRPerfInstance.h"
#include "Refresher.h"
#include <stdio.h>





/////////////////////////////////////////////////////////////////////////////
// CSRPerf

//////////////////////////////////////////////////////////////////////////////
// Class, property and method names

// IMPORTANT: the strings defined below are not localizable

//TODO: define provided class name, e.g.:
//const static WCHAR * s_pMyClassName = L"MyClassName"; 

//TODO: define property names of the provided class, e.g.:
//const static WCHAR * pMyProperty = L"MyProperty";

//TODO: define method names of the provided class, e.g.:
//const static WCHAR * pMyMethod = L"MyMethod";

//////////////////////////////////////////////////////////////////////////////
// CSRPerf::Initialize
//Refer to MSDN documentation for IWbemProviderInit::Initialize()
//for details about implementing this method

STDMETHODIMP CSRPerf::Initialize(_In_opt_ LPWSTR pszUser,   
                                      LONG lFlags,
                                      _In_ LPWSTR pszNamespace,
                                      _In_opt_ LPWSTR pszLocale,
                                      IWbemServices *pNamespace,
                                      IWbemContext *pCtx,
                                      IWbemProviderInitSink *pInitSink) 
{

	//CImpersonateClientHelper impersonateClient;
	HRESULT hr = WBEM_S_NO_ERROR;
	DWORD dwImpersonationLevel = 0;

#ifdef _DEBUG
	if( g_pFile ) fwprintf_s( g_pFile, L"CSRPerf::Initialize \n" );
#endif


	//if(SUCCEEDED(hr = impersonateClient.GetCurrentImpersonationLevel(dwImpersonationLevel)))
	//{
	//	// If the current thread has impersonation level set to RPC_C_IMP_LEVEL_IDENTIFY or RPC_C_IMP_LEVEL_ANONYMOUS
	//	if(dwImpersonationLevel < RPC_C_IMP_LEVEL_IMPERSONATE)
	//	{
	//		hr = WBEM_E_ACCESS_DENIED;
	//		// Do the access check yourself on the resources accessed based on security configuration for the provider
	//		// and change the return code hr appropriately
	//	}
	//	else
	//	{
	//		hr = impersonateClient.ImpersonateClient();
	//	}
	//}

	//if(FAILED(hr))
	//{
	//	return hr;
	//}

	//DbgTrace();

	if ( NULL == pNamespace || NULL == pInitSink) 
	{
		return WBEM_E_INVALID_PARAMETER;
	}


	//cache IWbemServices pointer 
	//Note: m_pNamespace is a smart pointer, so it AddRef()'s automatically
	m_pNamespace = pNamespace;

	//cache provided class definition
	//NOTE: the code below assumes that your class definition doesn't change while
	//your provider is running.  If this is not true, you should implement a consumer
	//for class modification and class deletion events. Refer to WMI documentation
	//for event consumers on MSDN.

	//BSTR bstrClassName=SysAllocString(s_pMyClassName);
	//if (NULL == bstrClassName)
	//{
	//	hr = WBEM_E_OUT_OF_MEMORY;
	//	return hr;
	//}   
	//hr = m_pNamespace->GetObject(bstrClassName, 
	//							 0, 
	//							 pCtx,   //passing IWbemContext pointer to prevent deadlocks
	//							 &m_pClass, 
	//							 NULL);
	//SysFreeString(bstrClassName);
	//bstrClassName = NULL;
	//if(FAILED(hr))
	//{
	//	return hr;
	//}

	////cache path parser class factory
	//hr = CoGetClassObject(CLSID_WbemDefPath, 
	//						CLSCTX_INPROC_SERVER, 
	//						NULL,
	//						IID_IClassFactory,
	//						(void **) &m_pPathFactory);
	//if (FAILED(hr))
	//{
	//	return hr;
	//}

	WMICounterInstance::RefreshInstanceList( pNamespace, pCtx );

	//create helper object
	m_pHelper = new CInstanceProviderHelper(pNamespace, pCtx);

	if(m_pHelper == NULL)
	{
		return WBEM_E_OUT_OF_MEMORY;
	}

	//NOTE: to report a detailed error or status to WMI, you can call 
	//ConstructErrorObject() on m_pHelper anywhere in your provider

	return pInitSink->SetStatus(WBEM_S_INITIALIZED,0);
}


//  Called whenever a complete, fresh list of instances for a given
//  class is required.   The objects are constructed and sent back to the
//  caller through the sink.  The sink can be used in-line as here, or
//  the call can return and a separate thread could be used to deliver
//  the instances to the sink.
//
//  Parameters:
//		pNamespace		- A pointer to the relevant namespace.  This
//							should not be AddRef'ed.
//		wszClass		- The class name for which instances are required.
//		lFlags			- Reserved.
//		pCtx			- The user-supplied context (not used here).
//		pSink			- The sink to which to deliver the objects.  The objects
//							can be delivered synchronously through the duration
//							of this call or asynchronously (assuming we
//							had a separate thread).  A IWbemObjectSink::SetStatus
//							call is required at the end of the sequence.
STDMETHODIMP CSRPerf::QueryInstances( 
    /* [in] */          IWbemServices __RPC_FAR *pNamespace,
    /* [string][in] */  WCHAR __RPC_FAR *wszClass,
    /* [in] */          long lFlags,
    /* [in] */          IWbemContext __RPC_FAR *pCtx,
    /* [in] */          IWbemObjectSink __RPC_FAR *pSink )
{
	HRESULT hr = WBEM_NO_ERROR;

#ifdef _DEBUG
	if( g_pFile ) fwprintf_s( g_pFile, L"CSRPerf::QueryInstances \n" );
#endif


	if (pNamespace == 0 || wszClass == 0 || pSink == 0)
		return WBEM_E_INVALID_PARAMETER;



	CComPtr<IWbemClassObject>	spObjectCopy = NULL;
	CComPtr<IWbemObjectAccess>	spAccessCopy = NULL;
    CComPtr<IWbemClassObject> spTemplate = NULL;

    TrcChk( pNamespace->GetObject(wszClass,0,pCtx,&spTemplate, 0) );

	TrcChk( WMICounterInstance::RefreshInstanceList( pNamespace, pCtx ) );
	TrcChk( WMICounterInstance::RefreshInstanceSync( pSink ) );

	// Tell WINMGMT we are all finished supplying objects
	pSink->SetStatus(0, WBEM_NO_ERROR, 0, 0);

Proc_End:

	return hr;
}    

//  Called whenever a new refresher is needed by the client.
//
//  Parameters:
//		pNamespace		- A pointer to the relevant namespace.  Not used.
//		lFlags			- Reserved.
//		ppRefresher		- Receives the requested refresher.
STDMETHODIMP CSRPerf::CreateRefresher( 
     /* [in] */ IWbemServices __RPC_FAR *pNamespace,
     /* [in] */ long lFlags,
     /* [out] */ IWbemRefresher __RPC_FAR *__RPC_FAR *ppRefresher )
{
#ifdef _DEBUG
	if( g_pFile ) fwprintf_s( g_pFile, L"CSRPerf::CreateRefresher \n" );
#endif


	if (pNamespace == 0 || ppRefresher == 0)
		return WBEM_E_INVALID_PARAMETER;

	// Construct and initialize a new empty refresher
	// ==============================================
	CRefresher* pNewRefresher = new CRefresher(this);
	pNewRefresher->Initialize();

	// Follow COM rules and AddRef() the thing before sending it back
	// ==============================================================

	pNewRefresher->AddRef();
	*ppRefresher = pNewRefresher;

	return WBEM_NO_ERROR;
}

//  Called whenever a user wants to include an object in a refresher.
//
//	Note that the object returned in ppRefreshable is a clone of the 
//	actual instance maintained by the provider.  If refreshers shared
//	a copy of the same instance, then a refresh call on one of the 
//	refreshers would impact the state of both refreshers.  This would 
//	break the refresher rules.	Instances in a refresher are only 
//	allowed to be updated when 'Refresh' is called.
//     
//  Parameters:
//		pNamespace		- A pointer to the relevant namespace in WINMGMT.
//		pTemplate		- A pointer to a copy of the object which is to be
//							added.  This object itself cannot be used, as
//							it not owned locally.        
//		pRefresher		- The refresher to which to add the object.
//		lFlags			- Not used.
//		pContext		- Not used here.
//		ppRefreshable	- A pointer to the internal object which was added
//							to the refresher.
//		plId			- The Object Id (for identification during removal).        
STDMETHODIMP CSRPerf::CreateRefreshableObject( 
    /* [in] */ IWbemServices __RPC_FAR *pNamespace,
    /* [in] */ IWbemObjectAccess __RPC_FAR *pTemplate,
    /* [in] */ IWbemRefresher __RPC_FAR *pRefresher,
    /* [in] */ long lFlags,
    /* [in] */ IWbemContext __RPC_FAR *pContext,
    /* [out] */ IWbemObjectAccess __RPC_FAR *__RPC_FAR *ppRefreshable,
    /* [out] */ long __RPC_FAR *plId )

{
    // Find out which instance is being requested for addition
	// =======================================================
#ifdef _DEBUG
	if( g_pFile ) fwprintf_s( g_pFile, L"CSRPerf::CCreateRefreshableObject \n" );
#endif


	HRESULT hRes = WBEM_NO_ERROR;

    if (pNamespace == 0 || pTemplate == 0 || pRefresher == 0)
        return WBEM_E_INVALID_PARAMETER;
    // The refresher being supplied by the caller is actually
    // one of our own refreshers, so a simple cast is convenient
    // so that we can access private members.
	// =========================================================
	CRefresher *pOurRefresher = (CRefresher *) pRefresher;
	// Add the object to the refresher. The ID is set by AddObject
	// ===========================================================
    hRes = pOurRefresher->AddObject(pTemplate, ppRefreshable, plId);

	if (FAILED(hRes))
		return hRes;

    return WBEM_NO_ERROR;
}

//  Called when an enumerator is being added to a refresher.  The 
//	enumerator will obtain a fresh set of instances of the specified 
//	class every time that refresh is called.
//     
//	Since this example provider only provides one class, then any 
//	enumerator in this refresher will only return instances from this 
//	class.  In the case where there is more than one class, then 
//	wszClass must be examined to determine which class the enumerator 
//	is being assigned.
//
//  Parameters:
//		pNamespace		- A pointer to the relevant namespace.  
//		wszClass		- The class name for the requested enumerator.
//		pRefresher		- The refresher object for which we will add 
//							the enumerator
//		lFlags			- Reserved.
//		pContext		- Not used here.
//		pHiPerfEnum		- The enumerator to add to the refresher.
//		plId			- A provider specified ID for the enumerator.
STDMETHODIMP CSRPerf::CreateRefreshableEnum( 
	/* [in] */ IWbemServices* pNamespace,
	/* [in, string] */ LPCWSTR wszClass,
	/* [in] */ IWbemRefresher* pRefresher,
	/* [in] */ long lFlags,
	/* [in] */ IWbemContext* pContext,
	/* [in] */ IWbemHiPerfEnum* pHiPerfEnum,
	/* [out] */ long* plId )
{
    // The refresher being supplied by the caller is actually
    // one of our own refreshers, so a simple cast is convenient
    // so that we can access private members.
#ifdef _DEBUG
	if( g_pFile ) fwprintf_s( g_pFile, L"CSRPerf::CreateRefreshableEnum\n" );
#endif


	CRefresher *pOurRefresher = (CRefresher *) pRefresher;

	// Add the enumerator to the refresher  
	// The ID is generated by AddEnum.
	// ===================================

	return pOurRefresher->AddEnum( pNamespace, pContext, pHiPerfEnum, wszClass, plId );
}

//  Called whenever a user wants to remove an object from a refresher.
//     
//  Parameters:
//		pRefresher			- The refresher object from which we are to 
//								remove the perf object.
//		lId					- The ID of the object.
//		lFlags				- Not used.
STDMETHODIMP CSRPerf::StopRefreshing( 
    /* [in] */ IWbemRefresher __RPC_FAR *pRefresher,
    /* [in] */ long lId,
    /* [in] */ long lFlags )
{
	HRESULT hRes = WBEM_NO_ERROR;

#ifdef _DEBUG
	if( g_pFile ) fwprintf_s( g_pFile, L"CSRPerf::StopRefreshing \n" );
#endif


    // The refresher being supplied by the caller is actually
    // one of our own refreshers, so a simple cast is convenient
    // so that we can access private members.
	// =========================================================

    CRefresher *pOurRefresher = (CRefresher *) pRefresher;

	// An ID that is less than MAX_ENUMERATORS is an enumerator ID.
	// Everything else is an object ID
	// ============================================================

	if (lId >= CRefresher::ENUMID_OFFSET)
		hRes = pOurRefresher->RemoveEnum(lId);
	else
		hRes = pOurRefresher->RemoveObject(lId);

	return hRes;
}

//  Called when a request is made to provide all instances currently 
//	being managed by the provider in the specified namespace.
//     
//  Parameters:
//		pNamespace		- A pointer to the relevant namespace.  
//		lNumObjects		- The number of instances being returned.
//		apObj			- The array of instances being returned.
//		lFlags			- Reserved.
//		pContext		- Not used here.
STDMETHODIMP CSRPerf::GetObjects( 
    /* [in] */ IWbemServices* pNamespace,
	/* [in] */ long lNumObjects,
	/* [in,size_is(lNumObjects)] */ IWbemObjectAccess** apObj,
    /* [in] */ long lFlags,
    /* [in] */ IWbemContext* pContext)
{
	HRESULT hRes = WBEM_E_PROVIDER_NOT_CAPABLE;//WBEM_NO_ERROR;
#ifdef _DEBUG
	if( g_pFile ) fwprintf_s( g_pFile, L"CSRPerf::GetObjects\n" );
#endif

	return hRes;
}



