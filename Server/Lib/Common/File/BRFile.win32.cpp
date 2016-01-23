////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : MadK
//
// Description : File
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common/TypeUtility.h"
#include "Common/File/BRFile.h"
#include "Common/ResultCode/BRResultCodeSystem.h"

#if WINDOWS

namespace BR {
namespace IO {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	File
	//

	static INT ToWin32SeekMode[] = 
	{
		FILE_BEGIN,		// Begin,
		FILE_CURRENT,	// Current,
		FILE_END,		// End,
	};

	File::File()
		: m_FileHandle(INVALID_NATIVE_HANDLE_VALUE)
	{
	}

	File::~File()
	{
		Close();
	}

	bool File::IsOpened()
	{
		return m_FileHandle != INVALID_NATIVE_HANDLE_VALUE;
	}

	HRESULT File::Seek(SeekMode seekMode, LONGLONG offset)
	{
		if (m_FileHandle == INVALID_NATIVE_HANDLE_VALUE)
			return E_SYSTEM_FAIL;

		LARGE_INTEGER lInt;
		lInt.QuadPart = offset;

		lInt.LowPart = SetFilePointer(m_FileHandle, lInt.LowPart, &lInt.HighPart, ToWin32SeekMode[(int)seekMode]);
		if (lInt.LowPart == INVALID_SET_FILE_POINTER)
		{
			return GetLastHRESULT();
		}

		return S_SYSTEM_OK;
	}

	LONGLONG File::GetLocation()
	{
		if (m_FileHandle == INVALID_NATIVE_HANDLE_VALUE)
			return E_SYSTEM_FAIL;

		LARGE_INTEGER lInt;
		lInt.QuadPart = 0;
		lInt.LowPart = SetFilePointer(m_FileHandle, 0, &lInt.HighPart, FILE_CURRENT);

		return lInt.QuadPart;
	}

	LONGLONG File::GetFileSize()
	{
		if (m_FileHandle == INVALID_NATIVE_HANDLE_VALUE)
			return E_SYSTEM_FAIL;

		LARGE_INTEGER szSize;
		if (GetFileSizeEx(m_FileHandle, &szSize) == INVALID_FILE_SIZE)
		{
			return 0;
		}
		return szSize.QuadPart;
	}

	HRESULT File::Open(const char* filePath, OpenMode openMode, SharingMode sharingMode)
	{
		Close();

		DWORD accessMode = 0;
		DWORD shareMode = 0;
		DWORD creationDesc = 0;

		switch (openMode)
		{
		case OpenMode::Read:
			accessMode = GENERIC_READ;
			creationDesc = OPEN_EXISTING;
			break;
		case OpenMode::CreateNew:
			accessMode = GENERIC_READ | GENERIC_WRITE;
			creationDesc = CREATE_ALWAYS;
			break;
		case OpenMode::Append:
			accessMode = GENERIC_READ | GENERIC_WRITE;
			creationDesc = OPEN_ALWAYS;
			break;
		default:
			return E_SYSTEM_UNEXPECTED;
		}

		switch (sharingMode)
		{
		case SharingMode::Shared:
			shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
			break;
		case SharingMode::ReadShared:
			shareMode = FILE_SHARE_READ;
			break;
		case SharingMode::WriteShared:
			shareMode = FILE_SHARE_WRITE;
			break;
		case SharingMode::Exclusive:
			shareMode = 0;
			break;
		default:
			return E_SYSTEM_UNEXPECTED;
		}

		m_FileHandle = CreateFileA(
			filePath,
			accessMode,
			shareMode,
			nullptr,
			creationDesc,
			FILE_ATTRIBUTE_NORMAL
			| FILE_FLAG_SEQUENTIAL_SCAN,
			nullptr);

		if (m_FileHandle == INVALID_NATIVE_HANDLE_VALUE)
			return E_SYSTEM_FAIL;

		m_OpenMode = openMode;
		m_SharingMode = sharingMode;

		return S_SYSTEM_OK;

	}

	void File::Close()
	{
		if (m_FileHandle == INVALID_NATIVE_HANDLE_VALUE)
			return;

		CloseHandle(m_FileHandle);
		m_FileHandle = INVALID_NATIVE_HANDLE_VALUE;
	}


	HRESULT File::Read(BYTE* buffer, size_t bufferLen, size_t &read)
	{
		if (m_FileHandle == INVALID_NATIVE_HANDLE_VALUE)
			return E_SYSTEM_UNEXPECTED;

		DWORD dwRead = 0;
		if (ReadFile(m_FileHandle, buffer, (DWORD)bufferLen, &dwRead, nullptr) == FALSE)
		{
			return GetLastHRESULT();
		}

		read = dwRead;

		return S_SYSTEM_OK;
	}

	HRESULT File::Write(const BYTE* buffer, size_t bufferLen, size_t &writen)
	{
		if (m_FileHandle == INVALID_NATIVE_HANDLE_VALUE)
			return E_SYSTEM_UNEXPECTED;

		DWORD dwWritten = 0;
		if (WriteFile(m_FileHandle, buffer, (DWORD)bufferLen, &dwWritten, nullptr) == FALSE)
		{
			return GetLastHRESULT();
		}

		writen = dwWritten;

		return S_SYSTEM_OK;
	}


}
}

#endif
