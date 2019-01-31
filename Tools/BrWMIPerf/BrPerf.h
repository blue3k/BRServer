// SRPerf.h : Declaration of the CWiconPerf

#pragma once
#include "resource.h"       // main symbols

#include <wbemidl.h>
#include <wmiatlprov.h>
#include <wmiutils.h>   //path parser
#include "SRWMIPerf_i.h"



/////////////////////////////////////////////////////////////////////////////
// CWiconPerf

class ATL_NO_VTABLE CSRPerf : 
                            public CComObjectRootEx<CComMultiThreadModel>,
                            public CComCoClass<CSRPerf, &CLSID_SRPerf>,
                            //public IWbemInstProviderImpl,
							public IWbemProviderInit,
							public IWbemHiPerfProvider
{
		CComPtr<IWbemServices>  m_pNamespace;   //cached IWbemServices pointer
		//CComPtr<IWbemClassObject> m_pClass;     //cached provided class definition      
		//CComPtr<IWbemClassObject> m_pErrorObject;//cached error object class definition pointer
		//CComPtr<IClassFactory> m_pPathFactory;   //cached pointer to path parser's class factory

		CInstanceProviderHelper * m_pHelper;

		STDMETHODIMP GetInstanceByPath (BSTR bstrPath,
										IWbemClassObject ** ppInstance );

	public:
		CSRPerf()
		{
			m_pHelper = NULL;                   
		}

		~CSRPerf()
		{
			if (NULL != m_pHelper)
			{
				delete m_pHelper;
			}
			//interface pointer data members are smart and get released automatically
		}

		DECLARE_REGISTRY_RESOURCEID(IDR_SRPERF)

		DECLARE_NOT_AGGREGATABLE(CSRPerf)

		BEGIN_COM_MAP(CSRPerf)
			//COM_INTERFACE_ENTRY(IWbemServices)
			COM_INTERFACE_ENTRY(IWbemProviderInit)
			COM_INTERFACE_ENTRY(IWbemHiPerfProvider)
		END_COM_MAP()

		//IWbemProviderInit
		HRESULT STDMETHODCALLTYPE Initialize( 
											 _In_opt_ LPWSTR pszUser,
											 LONG lFlags,
											 _In_ LPWSTR pszNamespace,
											 _In_opt_ LPWSTR pszLocale,
											 IWbemServices *pNamespace,
											 IWbemContext *pCtx,
											 IWbemProviderInitSink *pInitSink);


		/*************************Implement IWbemHiPerfProvider interfaces*****************************/
		//Returns instances of the specified class by using the supplied IWbemObjectSink object
		STDMETHODIMP QueryInstances( 
			/* [in] */ IWbemServices __RPC_FAR *pNamespace,
			/* [string][in] */ WCHAR __RPC_FAR *wszClass,
			/* [in] */ long lFlags,
			/* [in] */ IWbemContext __RPC_FAR *pCtx,
			/* [in] */ IWbemObjectSink __RPC_FAR *pSink );

		//Retrieves the specified objects.
		STDMETHODIMP GetObjects(
			/* [in] */ IWbemServices* pNamespace,
			/* [in] */ long lNumObjects,
			/* [in,size_is(lNumObjects)] */ IWbemObjectAccess** apObj,
			/* [in] */ long lFlags,
			/* [in] */ IWbemContext* pContext);

		//Creates a refresher.
		STDMETHODIMP CreateRefresher( 
			/* [in] */ IWbemServices __RPC_FAR *pNamespace,
			/* [in] */ long lFlags,
			/* [out] */ IWbemRefresher __RPC_FAR *__RPC_FAR *ppRefresher );

		//Creates a refreshable instance object
		STDMETHODIMP CreateRefreshableObject(    
			/* [in] */ IWbemServices __RPC_FAR *pNamespace,
			/* [in] */ IWbemObjectAccess __RPC_FAR *pTemplate,
			/* [in] */ IWbemRefresher __RPC_FAR *pRefresher,
			/* [in] */ long lFlags,
			/* [in] */ IWbemContext __RPC_FAR *pContext,
			/* [out] */ IWbemObjectAccess __RPC_FAR *__RPC_FAR *ppRefreshable,
			/* [out] */ long __RPC_FAR *plId );
			
		//Creates a refreshable enumerator.
		STDMETHODIMP CreateRefreshableEnum(
			/* [in] */ IWbemServices* pNamespace,
			/* [in, string] */ LPCWSTR wszClass,
			/* [in] */ IWbemRefresher* pRefresher,
			/* [in] */ long lFlags,
			/* [in] */ IWbemContext* pContext,
			/* [in] */ IWbemHiPerfEnum* pHiPerfEnum,
			/* [out] */ long* plId);

		//Stops refreshing an enumerator or instance object.
		STDMETHODIMP StopRefreshing( 
			/* [in] */ IWbemRefresher __RPC_FAR *pRefresher,
			/* [in] */ long lId,
			/* [in] */ long lFlags );

};

OBJECT_ENTRY_AUTO(__uuidof(SRPerf), CSRPerf)


#ifdef _DEBUG

extern FILE *g_pFile;

#define DbgTrace() 	{if( g_pFile ) fwprintf_s( g_pFile, L"File: %s, L:%d \n", _CRT_WIDE(__FILE__), __LINE__ );}
#define DbgTracehr(hres) 	{if( g_pFile && FAILED(hres) ) fwprintf_s( g_pFile, L"File: %s, L:%d hr=%08x\n", _CRT_WIDE(__FILE__), __LINE__, hres );}

#else

#define DbgTrace()
#define DbgTracehr(hres)

#endif


