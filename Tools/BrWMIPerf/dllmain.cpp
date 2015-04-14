// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "SRWMIPerf_i.h"
#include "dllmain.h"
#include "SRPerf.h"


CSRWMIPerfModule _Module;
//CComModule _Module;

#ifdef _DEBUG
FILE *g_pFile = NULL;
#endif

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_SRPerf, CSRPerf)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

//extern "C"
//BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
//{
//    if (dwReason == DLL_PROCESS_ATTACH)
//    {
//        _Module.Init(ObjectMap, hInstance, &LIBID_SRWMIPerfLib);
//        DisableThreadLibraryCalls(hInstance);
//    }
//    else if (dwReason == DLL_PROCESS_DETACH)
//        _Module.Term();
//    return TRUE;    // ok
//}


// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
#ifdef _DEBUG
	if( g_pFile == NULL )
		_wfopen_s( &g_pFile, L"e:\\work\\SRWMIdbglog.txt", L"w+, ccs=UTF-16LE" );
#endif

	hInstance;
	return _Module.DllMain(dwReason, lpReserved); 
}
