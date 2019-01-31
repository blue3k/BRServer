////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves cooperations
// 
// Author : KyungKun Ko
// Date : 11/25/2008
//
// Description : Performance WMI Instance
//	
//
////////////////////////////////////////////////////////////////////////////////




#pragma once

#include "Common/Synchronize.h"
#include <wbemprov.h>
#include "SRPerf.h"

#include "Common/PerfCounter.h"


////////////////////////////////////////////////////////////////////////////////
//
//	Counter WMI extension
//

class WMICounter : public Perf::Counter
{
private:
	// Counter variable type
	CIMTYPE		m_CounterType;

	// Handle to counter variable 
	long		m_hCounter;

public:

	WMICounter( const WCHAR* wszName );


	HRESULT ConnectWMI( IWbemObjectAccess *pAccess );

    HRESULT Refresh( bool bAlive, IWbemObjectAccess *pAccess );

};




////////////////////////////////////////////////////////////////////////////////
//
//	WMI Counter Instance 
//

class WMICounterInstance : public Perf::CounterInstanceMap
{
private:
	friend class CRefresher;

	// Counter pointers
	std::vector<WMICounter*> m_Counters;

	CComPtr<IWbemClassObject> m_spObject;
	CComPtr<IWbemObjectAccess> m_spAccess;

	// ID to distingush counter
	UINT		m_ID;

	// Handler timer
	long            m_handleTimestamp_PerfTime;    
	long            m_handleFrequency_PerfTime;    
	long            m_handleTimestamp_Sys100NS;    
	long            m_handleFrequency_Sys100NS;    
	long            m_handleTimestamp_Object;    
	long            m_handleFrequency_Object;    

	// ID generation basis
	static UINT stm_GenID;

	// Reference counter
	SR::SyncCounter	m_RefCount;

public:
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Static counter instance list management
	//

	// WMI counter instance array. shared over refresher
	typedef std::unordered_map<std::wstring,WMICounterInstance*> InstanceMap;
	typedef std::unordered_map<UINT,WMICounterInstance*> InstanceIDMap;

	// instance map by filename
	static InstanceMap stm_InstanceByFileName;

	// instance map by ID
	static InstanceIDMap stm_InstanceByID;



	// refresh instance list
	static HRESULT RefreshInstanceList( IWbemServices __RPC_FAR *pNamespace, IWbemContext __RPC_FAR *pCtx );

	// Refresh Sync instance
	static HRESULT RefreshInstanceSync( IWbemObjectSink __RPC_FAR *pSink );

	// Find counter instance if exist
	static HRESULT FindPerfInstance( const WCHAR* wszInstance, WMICounterInstance* &pFound );

	// Find counter instance if exist
	static HRESULT FindPerfInstanceByClass( const WCHAR* wszClass, std::vector<WMICounterInstance*>& instance );





	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Class interfacess
	//


	WMICounterInstance();

	~WMICounterInstance();

	IWbemClassObject* GetWMIObject();
	IWbemObjectAccess* GetWMIAccess();

	// Get ID
	UINT GetID();

	// Reference count
	void IncRefCount();
	CounterType DecRefCount();


	HRESULT RefreshInstance( IWbemClassObject *pTemplate );
	void RefreshCounterList( IWbemClassObject *pTemplate );

	// Refresh WMI object value
	HRESULT	Refresh();

	// Close all counter and file map
	void	Close();

	void	SetTimestamp( UINT64 timestamp, UINT64 frequency );
};

