////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : utility
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "Common/Utility.h"
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include <stdlib.h> 
#include <conio.h>



namespace BR {
namespace Util {

	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Module Name helper
	//
	static WCHAR g_wszModulePath[MAX_PATH] = {0};
	static CHAR g_szModulePath[MAX_PATH] = {0};
	static WCHAR g_wszModuleName[MAX_PATH] = {0};
	static CHAR g_szModuleName[MAX_PATH] = {0};
	static WCHAR g_wszServiceName[MAX_PATH] = {0};
	static CHAR g_szServiceName[MAX_PATH] = {0};

	static void _InitModuleName()
	{
		if( g_wszModulePath[0] == NULL || g_wszModuleName[0] == NULL )
		{
			// initialize module name
			WCHAR drive[_MAX_DRIVE] = {0};
			WCHAR dir[_MAX_DIR] = {0};
			WCHAR ext[_MAX_EXT] = L"";

			GetModuleFileNameW(GetModuleHandle(NULL), g_wszModulePath, MAX_PATH);
			_wsplitpath_s( g_wszModulePath, drive, dir, g_wszModuleName, ext );
			_snwprintf_s( g_wszModulePath, MAX_PATH, L"%s%s", drive, dir );

			StrUtil::WCSToMBCS( g_wszModulePath, g_szModulePath );
			StrUtil::WCSToMBCS( g_wszModuleName, g_szModuleName );
		}

		// If the service name isn't specified, use module name for it
		if( g_wszServiceName[0] == NULL || g_szServiceName[0] == NULL )
		{
			StrUtil::StringCpy( g_wszServiceName, g_wszModuleName );
			StrUtil::StringCpy( g_szServiceName, g_szModuleName );
		}
	}


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Module Name helper
	//


	const WCHAR* GetServiceName()
	{
		_InitModuleName();
		return g_wszServiceName;
	}
	const char* GetServiceNameA()
	{
		_InitModuleName();
		return g_szServiceName;
	}
	void SetServiceName(const WCHAR* serviceName)
	{
		StrUtil::WCSToMBCS( serviceName, g_szServiceName );
		StrUtil::StringCpy( g_wszServiceName, serviceName );
	}


	// Module Name
	const WCHAR* GetModuleName()
	{
		_InitModuleName();
		return g_wszModuleName;
	}
	const char* GetModuleNameA()
	{
		_InitModuleName();
		return g_szModuleName;
	}

	// Module Path
	const WCHAR* GetModulePath()
	{
		_InitModuleName();
		return g_wszModulePath;
	}
	const char* GetModulePathA()
	{
		_InitModuleName();
		return g_szModulePath;
	}



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Generic utility functions
	//


	// Peek key from console
	HRESULT PeekKey( int &inputKey, HANDLE hConsole )
	{
		HRESULT hr = S_OK;
		DWORD cNumRead, fdwSaveOldMode = 0; 
		INPUT_RECORD irInBuf; 

		if( hConsole == INVALID_HANDLE_VALUE )
		{
			// Get the standard input handle. 
			hConsole = GetStdHandle(STD_INPUT_HANDLE); 
			if (hConsole == INVALID_HANDLE_VALUE) 
				trcErr( E_FAIL );
		}

		// Save the current input mode, to be restored on exit. 
		if (!GetConsoleMode(hConsole, &fdwSaveOldMode))
		{
			//trcErr( E_FAIL );
			goto Proc_End;
		}

		// Enable the window and mouse input events. 
		if (! SetConsoleMode(hConsole, 0) ) 
			trcErr( E_FAIL );

		// Wait for the events. 
		if (! ReadConsoleInput( 
				hConsole,      // input buffer handle 
				&irInBuf,     // buffer to read into 
				1,         // size of read buffer 
				&cNumRead) ) // number of records read 
			trcErr( E_FAIL );

		// Dispatch the events to the appropriate handler. 
		switch(irInBuf.EventType) 
		{ 
			case KEY_EVENT: // keyboard input 
				if( irInBuf.Event.KeyEvent.bKeyDown )
				{
					inputKey = irInBuf.Event.KeyEvent.uChar.AsciiChar;
					goto Proc_End;
				}
				break; 

			case MOUSE_EVENT: // mouse input 
				break; 

			case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
				break; 

			case FOCUS_EVENT:  // disregard focus events 

			case MENU_EVENT:   // disregard menu events 
				break; 

			default: 
				trcErr( E_FAIL );
				break; 
		};

	Proc_End:

		if( hConsole != INVALID_HANDLE_VALUE && fdwSaveOldMode != 0 )
			SetConsoleMode(hConsole, fdwSaveOldMode);

		return hr; 
	}

	// Calculate near power of w
	UINT NearPowerOf2( UINT32 uiNumber )
	{
		static const struct {
			UINT32 Mask;
			UINT32 Shift;
		} OperationTable[] = {
				{ 0xFFFF0000, 16 },
				{ 0xFF00, 8 },
				{ 0xF0, 4 },
				{ 0xC, 2 },
				{ 0x2, 1 },
			};

		AssertRel( (uiNumber & 0x80000000L) == 0 );

		UINT power = 0;
		bool bIsOver = false;

		// Bineary search
		for( int iOper = 0; iOper < _countof(OperationTable); iOper++ )
		{
			if( uiNumber & OperationTable[iOper].Mask )
			{
				bIsOver |= (uiNumber & (~OperationTable[iOper].Mask)) != 0;
				power += OperationTable[iOper].Shift;
				uiNumber >>= OperationTable[iOper].Shift;
			}
		}

		// power has the bit shift of most significient bit
		//  We need bigger number than this
		if( bIsOver ) power++;
		return 1<<power;
	}

} // namespace Util
} // namespace BR


