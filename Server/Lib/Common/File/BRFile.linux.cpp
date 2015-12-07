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
#include "Common/Typedefs.h"
#include "Common/TypeUtility.h"
#include "Common/File/BRFile.h"

#if LINUX



namespace BR {
namespace IO {



	/////////////////////////////////////////////////////////////////////////////
	//
	//	File
	//

	static INT ToOSSeekMode[] =
	{
		SEEK_SET,		// Begin,
		SEEK_CUR,	// Current,
		SEEK_END,		// End,
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
			return E_FAIL;

		lseek((int)(intptr_t)m_FileHandle, offset, ToOSSeekMode[(int)seekMode]);

		return S_OK;
	}

	LONGLONG File::GetLocation()
	{
		if (m_FileHandle == INVALID_NATIVE_HANDLE_VALUE)
			return E_FAIL;

		return lseek((int)(intptr_t)m_FileHandle, 0, SEEK_CUR);
	}

	LONGLONG File::GetFileSize()
	{
		if (m_FileHandle == INVALID_NATIVE_HANDLE_VALUE)
			return E_FAIL;

		struct stat buf;
		fstat((int)(intptr_t)m_FileHandle, &buf);
		return buf.st_size;
	}

	HRESULT File::Open(const char* filePath, OpenMode openMode, SharingMode sharingMode)
	{
		Close();

		DWORD accessMode = 0;

		switch (openMode)
		{
		case OpenMode::Read:
			accessMode = O_RDONLY;
			break;
		case OpenMode::CreateNew:
			accessMode = O_RDWR | O_CREAT | O_TRUNC;
			break;
		case OpenMode::Append:
			accessMode = O_RDWR | O_CREAT | O_APPEND;
			break;
		default:
			return E_UNEXPECTED;
		}

		m_FileHandle = (NativeHandle)(intptr_t)open(
			filePath,
			accessMode );

		if (m_FileHandle == INVALID_NATIVE_HANDLE_VALUE)
		{
			return GetLastHRESULT();
		}

		flock fl;
		fl.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
		fl.l_start = 0;        /* Offset from l_whence         */
		fl.l_len = 0;        /* length, 0 = to EOF           */
		fl.l_pid = getpid(); /* our PID                      */
		switch (sharingMode)
		{
		case SharingMode::Shared:
			fl.l_type = F_UNLCK;  /* F_RDLCK, F_WRLCK, F_UNLCK    */
			break;
		case SharingMode::ReadShared:
			fl.l_type = F_WRLCK;  /* F_RDLCK, F_WRLCK, F_UNLCK    */
			fcntl((int)(intptr_t)m_FileHandle, F_SETLK, &fl);
			break;
		case SharingMode::WriteShared:
			fl.l_type = F_RDLCK;  /* F_RDLCK, F_WRLCK, F_UNLCK    */
			fcntl((int)(intptr_t)m_FileHandle, F_SETLK, &fl);
			break;
		case SharingMode::Exclusive:
			fl.l_type = F_RDLCK | F_WRLCK;  /* F_RDLCK, F_WRLCK, F_UNLCK    */
			fcntl((int)(intptr_t)m_FileHandle, F_SETLK, &fl);
			break;
		default:
			return E_UNEXPECTED;
		}

		m_OpenMode = openMode;
		m_SharingMode = sharingMode;

		return S_OK;

	}

	void File::Close()
	{
		if (m_FileHandle == INVALID_NATIVE_HANDLE_VALUE)
			return;

		close((int)(intptr_t)m_FileHandle);
		m_FileHandle = INVALID_NATIVE_HANDLE_VALUE;
	}


	HRESULT File::Read(BYTE* buffer, size_t bufferLen, size_t &readSize)
	{
		if (m_FileHandle == INVALID_NATIVE_HANDLE_VALUE)
			return E_UNEXPECTED;

		int dwRead = 0;
		dwRead = read((int)(intptr_t)m_FileHandle, buffer, bufferLen);
		if(dwRead < 0)
		{
			return E_FAIL;
		}

		readSize = dwRead;

		return S_OK;
	}

	HRESULT File::Write(const BYTE* buffer, size_t bufferLen, size_t &writen)
	{
		if (m_FileHandle == INVALID_NATIVE_HANDLE_VALUE)
			return E_UNEXPECTED;

		int dwWritten = write((int)(intptr_t)m_FileHandle, buffer, bufferLen);
		if (dwWritten < 0)
		{
			return E_FAIL;
		}

		writen = dwWritten;

		return S_OK;
	}




}
}
#else

void Dummy_BRFile_Linux() {}

#endif
