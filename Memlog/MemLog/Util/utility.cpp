////////////////////////////////////////////////////////////////////////////////
//  $Id: utility.cpp,v 1.24 2006/11/18 03:12:35 dmouldin Exp $
//
//  Visual Leak Detector - Various Utility Functions
//  Copyright (c) 2005-2006 Dan Moulding
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
//  See COPYING.txt for the full terms of the GNU Lesser General Public License.
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <cassert>
#include <cstdio>
#include <windows.h>

#include <dbghelp.h>    // Provides portable executable (PE) image access functions.

#include "utility.h"    // Provides various utility functions and macros.

namespace CallStackSystem
{
	extern HANDLE           currentprocess; // Pseudo-handle for the current process.
	extern CRITICAL_SECTION symbollock;     // Serializes calls to the Debug Help Library symbold handling APIs.
};


static LoggingMask g_LogMask = LOG_ALL;

void SetLogMask( LoggingMask mask )
{
	g_LogMask = mask;
}

VOID report (LPCSTR format, ...)
{
	va_list args;
	//size_t  count;
	CHAR   messagew [MAXREPORTLENGTH + 1];

	va_start(args, format);
	_vsnprintf_s(messagew, MAXREPORTLENGTH + 1, _TRUNCATE, format, args);
	va_end(args);
	messagew[MAXREPORTLENGTH] = L'\0';

	if( g_LogMask&LOG_DebugOut )
		OutputDebugStringA(messagew);

	if( g_LogMask&LOG_Console )
		printf("%s",messagew);


	//if (reporttodebugger && (reportdelay == TRUE)) {
	//	Sleep(10); // Workaround the Visual Studio 6 bug where debug strings are sometimes lost if they're sent too fast.
	//}
}


VOID report (LPCWSTR format, ...)
{
	va_list args;
	//size_t  count;
	WCHAR   messagew [MAXREPORTLENGTH + 1];

	va_start(args, format);
	_vsnwprintf_s(messagew, MAXREPORTLENGTH + 1, _TRUNCATE, format, args);
	va_end(args);
	messagew[MAXREPORTLENGTH] = L'\0';


	if( g_LogMask&LOG_DebugOut )
		OutputDebugStringW(messagew);

	if( g_LogMask&LOG_Console )
		printf("%w",messagew);

	//if (reporttodebugger && (reportdelay == TRUE)) {
	//	Sleep(10); // Workaround the Visual Studio 6 bug where debug strings are sometimes lost if they're sent too fast.
	//}
}

// strapp - Appends the specified source string to the specified destination
//   string. Allocates additional space so that the destination string "grows"
//   as new strings are appended to it. This function is fairly infrequently
//   used so efficiency is not a major concern.
//
//  - dest (IN/OUT): Address of the destination string. Receives the resulting
//      combined string after the append operation.
//
//  - source (IN): Source string to be appended to the destination string.
//
//  Return Value:
//
//    None.
//
VOID strapp (LPWSTR *dest, LPCWSTR source)
{
    SIZE_T length;
    LPWSTR temp;

    temp = *dest;
    length = wcslen(*dest) + wcslen(source);
    *dest = new WCHAR [length + 1];
    wcsncpy_s(*dest, length + 1, temp, _TRUNCATE);
    wcsncat_s(*dest, length + 1, source, _TRUNCATE);
    delete [] temp;
}

// strapp - Appends the specified source string to the specified destination
//   string. Allocates additional space so that the destination string "grows"
//   as new strings are appended to it. This function is fairly infrequently
//   used so efficiency is not a major concern.
//
//  - dest (IN/OUT): Address of the destination string. Receives the resulting
//      combined string after the append operation.
//
//  - source (IN): Source string to be appended to the destination string.
//
//  Return Value:
//
//    None.
//
VOID strapp (LPSTR *dest, LPCSTR source)
{
    SIZE_T length;
    LPSTR temp;

    temp = *dest;
    length = strlen(*dest) + strlen(source);
    *dest = new CHAR [length + 1];
    strncpy_s(*dest, length + 1, temp, _TRUNCATE);
    strncat_s(*dest, length + 1, source, _TRUNCATE);
    delete [] temp;
}



BOOL CALLBACK addloadedmodule ( PSTR modulepath, DWORD64 modulebase, ULONG modulesize, PVOID context )
{
//	size_t        count;
//	patchentry_t *entry;
	CHAR          extension [_MAX_EXT];
	CHAR          filename [_MAX_FNAME];
//	UINT          index;
	moduleinfo_t  moduleinfo;
	LPSTR         modulenamea;
	LPSTR         modulepatha;
	ModuleSet*    newmodules = (ModuleSet*)context;
	SIZE_T        size;
//	UINT          tablesize = sizeof(m_patchtable) / sizeof(patchentry_t);

	// Convert the module path to ASCII.
	size = strlen(modulepath) + 1;
	modulepatha = new CHAR [size];
//#ifdef _UNICODE
//	wcstombs_s(&count, modulepatha, size, modulepath, _TRUNCATE);
//#else
	strcpy_s( modulepatha, size, modulepath );
//#endif

	// Extract just the filename and extension from the module path.
	_splitpath_s(modulepatha, NULL, 0, NULL, 0, filename, _MAX_FNAME, extension, _MAX_EXT);
	size = strlen(filename) + strlen(extension) + 1;
	modulenamea = new CHAR [size];
	strncpy_s(modulenamea, size, filename, _TRUNCATE);
	strncat_s(modulenamea, size, extension, _TRUNCATE);
	_strlwr_s(modulenamea, size);

	// Record the module's information and store it in the set.
	moduleinfo.addrlow  = (SIZE_T)modulebase;
	moduleinfo.addrhigh = (SIZE_T)(modulebase + modulesize) - 1;
	moduleinfo.flags    = 0x0;
	moduleinfo.name     = modulenamea;
	moduleinfo.path     = modulepatha;
	newmodules->insert(moduleinfo);

	return TRUE;
}


VOID CALLBACK attachtoloadedmodules (ModuleSet *newmodules)
{
//	size_t                count;
	DWORD64               modulebase;
	UINT32                moduleflags;
	IMAGEHLP_MODULE64     moduleimageinfo;
	LPCSTR                modulename;
#define MAXMODULENAME (_MAX_FNAME + _MAX_EXT)
//	WCHAR                 modulenamew [MAXMODULENAME];
	LPSTR                modulepath;
	DWORD                 modulesize;
	ModuleSet::iterator   newit;
	ModuleSet::iterator   oldit;
//	ModuleSet            *oldmodules;
	BOOL                  refresh;
//	UINT                  tablesize = sizeof(m_patchtable) / sizeof(patchentry_t);
//	ModuleSet::Muterator  updateit;

	// Iterate through the supplied set, until all modules have been attached.
	for (newit = newmodules->begin(); newit != newmodules->end(); ++newit)
	{
		modulebase  = (DWORD64)(*newit).addrlow;
		moduleflags = 0x0;
		modulename  = (*newit).name;
		modulepath  = (LPSTR)(*newit).path;
		modulesize  = (DWORD)((*newit).addrhigh - (*newit).addrlow) + 1;

		refresh = FALSE;
		EnterCriticalSection(&CallStackSystem::symbollock);
		if ((refresh == TRUE) && (moduleflags & VLD_MODULE_SYMBOLSLOADED)) {
			// Discard the previously loaded symbols, so we can refresh them.
			if (SymUnloadModule64(CallStackSystem::currentprocess, modulebase) == FALSE) {
				report(L"WARNING: Visual Leak Detector: Failed to unload the symbols for %s. Function names and line"
					   L" numbers shown in the memory leak report for %s may be inaccurate.", modulename, modulename);
			}
		}

		// Try to load the module's symbols. This ensures that we have loaded
		// the symbols for every module that has ever been loaded into the
		// process, guaranteeing the symbols' availability when generating the
		// leak report.
		moduleimageinfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
		if ((SymGetModuleInfo64(CallStackSystem::currentprocess, (DWORD64)modulebase, &moduleimageinfo) == TRUE) ||
			((SymLoadModule64(CallStackSystem::currentprocess, NULL, modulepath, NULL, modulebase, modulesize) == modulebase) &&
			(SymGetModuleInfo64(CallStackSystem::currentprocess, modulebase, &moduleimageinfo) == TRUE))) {
			moduleflags |= VLD_MODULE_SYMBOLSLOADED;
		}
		LeaveCriticalSection(&CallStackSystem::symbollock);

		//// Attach to the module.
		//patchmodule((HMODULE)modulebase, m_patchtable, tablesize);
    }
}
