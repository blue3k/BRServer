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


#include "stdafx.h"
#include "Common/Utility.h"
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include <stdlib.h> 




namespace BR {
namespace Util {

	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Module Name helper
	//
	static wchar_t g_wszModulePath[MAX_PATH] = {0};
	static char g_szModulePath[MAX_PATH] = {0};
	static wchar_t g_wszModuleName[MAX_PATH] = {0};
	static char g_szModuleName[MAX_PATH] = {0};
	static wchar_t g_wszServiceName[MAX_PATH] = {0};
	static char g_szServiceName[MAX_PATH] = {0};

#if LINUX
	size_t get_executable_path(char* buffer, size_t len)
	{
		char* path_end;
		/* Read the target of /proc/self/exe. */
		if (readlink("/proc/self/exe", buffer, len) <= 0)
			return -1;
		/* Find the last occurrence of a forward slash, the path separator. */
		path_end = strrchr(buffer, '/');
		if (path_end == NULL)
			return -1;
		/* Advance to the character past the last slash. */
		++path_end;
		/* Obtain the directory containing the program by truncating the
		path after the last slash. */
		*path_end = '\0';
		/* The length of the path is the number of characters up through the
		last slash. */
		return (size_t)(path_end - buffer);
	}
#endif

	static void _InitModuleName()
	{
		if( g_wszModulePath[0] == L'\0' || g_wszModuleName[0] == L'\0')
		{
#if WINDOWS
			// initialize module name
			wchar_t drive[_MAX_DRIVE] = {0};
			wchar_t dir[_MAX_DIR] = {0};
			wchar_t ext[_MAX_EXT] = L"";

			GetModuleFileNameW(GetModuleHandle(NULL), g_wszModulePath, MAX_PATH);
			_wsplitpath_s( g_wszModulePath, drive, dir, g_wszModuleName, ext );
			_snwprintf_s( g_wszModulePath, MAX_PATH, L"%s%s", drive, dir );

			StrUtil::WCSToUTF8( g_wszModulePath, g_szModulePath );
			StrUtil::WCSToUTF8( g_wszModuleName, g_szModuleName );
#else
			get_executable_path(g_szModulePath, countof(g_szModulePath));

			char exe[1024];
			int ret;

			ret = readlink("/proc/self/exe", exe, sizeof(exe) - 1);
			if (ret == -1) {
				fprintf(stderr, "ERRORRRRR\n");
				exit(1);
			}
			exe[ret] = 0;

			char* bastNameLoc = basename(exe);
			*(bastNameLoc - 1) = '\0';
			strcpy(g_szModuleName, bastNameLoc);

			StrUtil::UTF8ToWCS(g_szModulePath, g_wszModulePath);
			StrUtil::UTF8ToWCS(g_szModuleName, g_wszModuleName);
#endif
		}

		// If the service name isn't specified, use module name for it
		if( g_wszServiceName[0] == L'\0' || g_szServiceName[0] == '\0')
		{
			StrUtil::StringCpy( g_wszServiceName, g_wszModuleName );
			StrUtil::StringCpy( g_szServiceName, g_szModuleName );
		}
	}


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Module Name helper
	//


	const wchar_t* GetServiceName()
	{
		_InitModuleName();
		return g_wszServiceName;
	}
	const char* GetServiceNameA()
	{
		_InitModuleName();
		return g_szServiceName;
	}
	void SetServiceName(const wchar_t* serviceName)
	{
		StrUtil::WCSToUTF8( serviceName, g_szServiceName );
		StrUtil::StringCpy( g_wszServiceName, serviceName );
	}

	void SetServiceName(const char* serviceName)
	{
		StrUtil::StringCpy(g_szServiceName, serviceName);
		StrUtil::UTF8ToWCS(serviceName, g_wszServiceName);
	}


	// Module Name
	const wchar_t* GetModuleName()
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
	const wchar_t* GetModulePath()
	{
		_InitModuleName();
		return g_wszModulePath;
	}
	const char* GetModulePathA()
	{
		_InitModuleName();
		return g_szModulePath;
	}

	void SetModulePathA(const char* customModulePath, const char* moduleName)
	{
		StrUtil::StringCpy(g_szModulePath, customModulePath);
		StrUtil::StringCpy(g_szModuleName, moduleName);

		StrUtil::UTF8ToWCS(g_szModulePath, g_wszModulePath);
		StrUtil::UTF8ToWCS(g_szModuleName, g_wszModuleName);

		// If the service name isn't specified, use module name for it
		if (g_wszServiceName[0] == L'\0' || g_szServiceName[0] == '\0')
		{
			StrUtil::StringCpy(g_wszServiceName, g_wszModuleName);
			StrUtil::StringCpy(g_szServiceName, g_szModuleName);
		}
	}



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Generic utility functions
	//


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
		for( UINT iOper = 0; iOper < countof(OperationTable); iOper++ )
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


