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




#pragma once

#include <array>

#include <wbemprov.h>
#include "SRPerf.h"
#include "SRPerfInstance.h"



class CRefresher : public IWbemRefresher
{
public:
	enum { ENUMID_OFFSET = 1024 };

private:
	// COM reference counter
	long	m_lRef;

    // The parent provider
    // ===================

    CSRPerf* m_pProvider;

	// Perf counter list, refresh cache
	std::unordered_map<UINT,WMICounterInstance*> m_Instances;

	// The enumerators that have been added to the refresher
	std::tr1::array<IWbemHiPerfEnum*, 32>	m_Enumerators;


public:
	CRefresher( CSRPerf* pProvider );
	virtual ~CRefresher();

	DWORD Initialize();

	// Instance management functions
	DWORD AddObject( IWbemClassObject *pTemplate, IWbemObjectAccess **ppReturnObj, long *plId );
	DWORD RemoveObject( long lId );

	// Enumerator management functions
	DWORD AddEnum(IWbemServices* pNamespace,IWbemContext* pContext,IWbemHiPerfEnum *pHiPerfEnum, LPCWSTR wszClass, long *plId);
	DWORD RemoveEnum(long lId);

	// COM methods
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv);
	STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();
	STDMETHODIMP Refresh(/* [in] */ long lFlags);
};



