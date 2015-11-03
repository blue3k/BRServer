////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : MadK
//
// Description : File
//
////////////////////////////////////////////////////////////////////////////////


#include "Common/Typedefs.h"


namespace BR {
namespace IO {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	File
	//

	class File
	{
	public:

		enum class OpenMode
		{
			Read,
			CreateNew,
			Append,
		};

		enum class SharingMode
		{
			Shared,
			ReadShared,
			WriteShared,
			Exclusive,
		};

		enum class SeekMode
		{
			Begin,
			Current,
			End
		};

	private:
		// Access mode
		NativeHandle m_FileHandle;
		OpenMode m_OpenMode;
		SharingMode m_SharingMode;

	public:
		File();
		~File();

		bool IsOpened();

		HRESULT Seek(SeekMode seekMode, LONGLONG offset = 0);
		LONGLONG GetLocation();
		LONGLONG GetFileSize();

		HRESULT Open(const char* filePath, OpenMode openMode, SharingMode sharingMode = SharingMode::Shared);
		void Close();

		HRESULT Read(BYTE* buffer, size_t bufferLen, size_t &read);
		HRESULT Write(const BYTE* buffer, size_t bufferLen, size_t &writen);

	};



}
}
