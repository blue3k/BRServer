////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Crc32 calculation from zlib
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/Utility.h"


// Precalculated table for crc32
#include "Common/CryptoCRC32Table.inl"


namespace BR {
namespace Util {
	
	// Endian specific implementation
	static UINT32 _Crc32Lt( UINT32 uiCrc32, size_t uiLen, const BYTE *pBuff );
	static UINT32 _Crc32Big( UINT32 uiCrc32, size_t uiLen, const BYTE *pBuff );


	
	// Crc32 for Utility
	UINT32 Crc32( UINT32 crc, size_t uiLen, const BYTE* pBuf )
	{
		if( pBuf == nullptr )
			return 0;

		UINT32 endian = 1;

		if (*((unsigned char *)(&endian)))
			return _Crc32Lt(crc, uiLen, pBuf);
		else
			return _Crc32Big(crc, uiLen, pBuf);

	}

	// Make crc32 checksum
	UINT32 Crc32( size_t uiBuffSize, const BYTE* pBuff )
	{
		return Crc32( 0, uiBuffSize, pBuff );
	}

	///////////////////////////////////////////////////////////////////////////////////////
	//
	//	Crc32 of little endian
	//

	static inline void _Crc32Lt4( UINT32& uiCrc32, const UINT32* &pBuffCur )
	{
		uiCrc32 ^= *pBuffCur++;
		uiCrc32 = __g_CRC32Table[3][uiCrc32 & 0xff] ^ __g_CRC32Table[2][(uiCrc32 >> 8) & 0xff] ^ 
					__g_CRC32Table[1][(uiCrc32 >> 16) & 0xff] ^ __g_CRC32Table[0][uiCrc32 >> 24];
	}

	static inline void _Crc32Lt32( UINT32& uiCrc32, const UINT32* &pBuffCur )
	{
		_Crc32Lt4( uiCrc32, pBuffCur );
		_Crc32Lt4( uiCrc32, pBuffCur );
		_Crc32Lt4( uiCrc32, pBuffCur );
		_Crc32Lt4( uiCrc32, pBuffCur );
		_Crc32Lt4( uiCrc32, pBuffCur );
		_Crc32Lt4( uiCrc32, pBuffCur );
		_Crc32Lt4( uiCrc32, pBuffCur );
		_Crc32Lt4( uiCrc32, pBuffCur );
	}


	static UINT32 _Crc32Lt( UINT32 uiCrc32, size_t uiLen, const BYTE *pBuff )
	{
		UINT32 crc;
		const UINT32 *pBuff4;

		crc = uiCrc32;
		crc = ~crc;
		// process untile 32bit boundary
		while (uiLen && ((ptrdiff_t)pBuff & 3))
		{
			crc = __g_CRC32Table[0][(crc ^ *pBuff++) & 0xff] ^ (crc >> 8);
			uiLen--;
		}

		pBuff4 = (const UINT32*)pBuff;
		while (uiLen >= 32) {
			_Crc32Lt32( crc, pBuff4 );
			uiLen -= 32;
		}
		while (uiLen >= 4)
		{
			_Crc32Lt4( crc, pBuff4 );
			uiLen -= 4;
		}
		pBuff = (const BYTE*)pBuff4;

		if (uiLen)
		{
			do
			{
				crc = __g_CRC32Table[0][(crc ^ *pBuff++) & 0xff] ^ (crc >> 8);
			} while (--uiLen);
		}
		crc = ~crc;

		return crc;
	}



	///////////////////////////////////////////////////////////////////////////////////////
	//
	//	Crc32 of big endian
	//


	static inline void _Crc32Big4( UINT32& uiCrc32, const UINT32* &pBuffCur )
	{
		uiCrc32 ^= *pBuffCur++;
		uiCrc32 = __g_CRC32Table[4][uiCrc32 & 0xff] ^ __g_CRC32Table[5][(uiCrc32 >> 8) & 0xff] ^ 
					__g_CRC32Table[6][(uiCrc32 >> 16) & 0xff] ^ __g_CRC32Table[7][uiCrc32 >> 24];
	}

	static inline void _Crc32Big32( UINT32& uiCrc32, const UINT32* &pBuffCur )
	{
		_Crc32Big4( uiCrc32, pBuffCur );
		_Crc32Big4( uiCrc32, pBuffCur );
		_Crc32Big4( uiCrc32, pBuffCur );
		_Crc32Big4( uiCrc32, pBuffCur );
		_Crc32Big4( uiCrc32, pBuffCur );
		_Crc32Big4( uiCrc32, pBuffCur );
		_Crc32Big4( uiCrc32, pBuffCur );
		_Crc32Big4( uiCrc32, pBuffCur );
	}

	// NOTE: This is not tested
	static UINT32 _Crc32Big( UINT32 uiCrc32, size_t uiLen, const BYTE *pBuff )
	{
		UINT32 crc;
		const UINT32 *pBuf4;

		crc = Util::SwapByte(uiCrc32);
		crc = ~crc;
		// process untile 32bit boundary
		while (uiLen && ((ptrdiff_t)pBuff & 3))
		{
			crc = __g_CRC32Table[4][(crc >> 24) ^ *pBuff++] ^ (crc << 8);
			uiLen--;
		}

		pBuf4 = (const UINT32*)pBuff;
		while (uiLen >= 32)
		{
			_Crc32Big32( crc, pBuf4 );
			uiLen -= 32;
		}
		while (uiLen >= 4)
		{
			_Crc32Big4( crc, pBuf4 );
			uiLen -= 4;
		}
		pBuff = (const BYTE*)pBuf4;

		if (uiLen)
		{
			do 
			{
				crc = __g_CRC32Table[4][(crc >> 24) ^ *pBuff++] ^ (crc << 8);
			} while (--uiLen);
		}
		crc = ~crc;

		return Util::SwapByte(crc);
	}
	

	
	///////////////////////////////////////////////////////////////////////////////////////
	//
	//	Crc32 and crypto
	//

	#define CRYPTO_MASK (0x45027602L)

	///////////////////////////////////////////////////////////////////////////////////////
	//
	//	Crc32 and crypto of little endian
	//

	static inline void _Crc32NEncLt4( UINT32& uiCrc32, UINT32* &pBuffCur )
	{
		UINT32 oldCrc = uiCrc32;

		uiCrc32 ^= *pBuffCur;
		uiCrc32 = __g_CRC32Table[3][uiCrc32 & 0xff] ^ __g_CRC32Table[2][(uiCrc32 >> 8) & 0xff] ^ 
					__g_CRC32Table[1][(uiCrc32 >> 16) & 0xff] ^ __g_CRC32Table[0][uiCrc32 >> 24];

		*pBuffCur++ ^= (oldCrc^CRYPTO_MASK);
	}

	static inline void _Crc32NEncLt32( UINT32& uiCrc32, UINT32* &pBuffCur )
	{
		_Crc32NEncLt4( uiCrc32, pBuffCur );
		_Crc32NEncLt4( uiCrc32, pBuffCur );
		_Crc32NEncLt4( uiCrc32, pBuffCur );
		_Crc32NEncLt4( uiCrc32, pBuffCur );
		_Crc32NEncLt4( uiCrc32, pBuffCur );
		_Crc32NEncLt4( uiCrc32, pBuffCur );
		_Crc32NEncLt4( uiCrc32, pBuffCur );
		_Crc32NEncLt4( uiCrc32, pBuffCur );
	}


	static UINT32 _Crc32NEncLt( UINT32 uiCrc32, size_t uiLen, BYTE *pBuff )
	{
		UINT32 crc;
		UINT32 *pBuff4;

		crc = uiCrc32;
		crc = ~crc;

		pBuff4 = (UINT32*)pBuff;
		while (uiLen >= 32) {
			_Crc32NEncLt32( crc, pBuff4 );
			uiLen -= 32;
		}
		while (uiLen >= 4)
		{
			_Crc32NEncLt4( crc, pBuff4 );
			uiLen -= 4;
		}
		pBuff = (BYTE*)pBuff4;

		if (uiLen)
		{
			do
			{
				crc = __g_CRC32Table[0][(crc ^ *pBuff++) & 0xff] ^ (crc >> 8);
			} while (--uiLen);
		}
		crc = ~crc;

		return crc;
	}



	///////////////////////////////////////////////////////////////////////////////////////
	//
	//	Crc32 of big endian
	//


	static inline void _Crc32NEncBig4( UINT32& uiCrc32, UINT32* &pBuffCur )
	{
		UINT32 oldCrc = uiCrc32;

		uiCrc32 ^= *pBuffCur;
		uiCrc32 = __g_CRC32Table[4][uiCrc32 & 0xff] ^ __g_CRC32Table[5][(uiCrc32 >> 8) & 0xff] ^ 
					__g_CRC32Table[6][(uiCrc32 >> 16) & 0xff] ^ __g_CRC32Table[7][uiCrc32 >> 24];

		*pBuffCur++ ^= (oldCrc^CRYPTO_MASK);
	}

	static inline void _Crc32NEncBig32( UINT32& uiCrc32, UINT32* &pBuffCur )
	{
		_Crc32NEncBig4( uiCrc32, pBuffCur );
		_Crc32NEncBig4( uiCrc32, pBuffCur );
		_Crc32NEncBig4( uiCrc32, pBuffCur );
		_Crc32NEncBig4( uiCrc32, pBuffCur );
		_Crc32NEncBig4( uiCrc32, pBuffCur );
		_Crc32NEncBig4( uiCrc32, pBuffCur );
		_Crc32NEncBig4( uiCrc32, pBuffCur );
		_Crc32NEncBig4( uiCrc32, pBuffCur );
	}

	// NOTE: This is not tested
	static UINT32 _Crc32NEncBig( UINT32 uiCrc32, size_t uiLen, BYTE *pBuff )
	{
		UINT32 crc;
		UINT32 *pBuf4;

		crc = Util::SwapByte(uiCrc32);
		crc = ~crc;

		pBuf4 = (UINT32*)pBuff;
		while (uiLen >= 32)
		{
			_Crc32NEncBig32( crc, pBuf4 );
			uiLen -= 32;
		}
		while (uiLen >= 4)
		{
			_Crc32NEncBig4( crc, pBuf4 );
			uiLen -= 4;
		}
		pBuff = (BYTE*)pBuf4;

		if (uiLen)
		{
			do 
			{
				crc = __g_CRC32Table[4][(crc >> 24) ^ *pBuff++] ^ (crc << 8);
			} while (--uiLen);
		}
		crc = ~crc;

		return Util::SwapByte(crc);
	}
	
	// Make crc32 checksum and encrypt
	UINT32 Crc32NEncrypt( size_t uiBuffSize, BYTE* pBuff )
	{
		if( pBuff == nullptr )
			return 0;

		UINT32 endian = 1;

		if (*((unsigned char *)(&endian)))
			return _Crc32NEncLt(0, uiBuffSize, pBuff);
		else
			return _Crc32NEncBig(0, uiBuffSize, pBuff);
	}


	

	///////////////////////////////////////////////////////////////////////////////////////
	//
	//	Crc32 and crypto of little endian
	//

	static inline void _Crc32NDecLt4( UINT32& uiCrc32, UINT32* &pBuffCur )
	{
		*pBuffCur ^= (uiCrc32^CRYPTO_MASK);
		uiCrc32 ^= *pBuffCur++;
		uiCrc32 = __g_CRC32Table[3][uiCrc32 & 0xff] ^ __g_CRC32Table[2][(uiCrc32 >> 8) & 0xff] ^ 
					__g_CRC32Table[1][(uiCrc32 >> 16) & 0xff] ^ __g_CRC32Table[0][uiCrc32 >> 24];

	}

	static inline void _Crc32NDecLt32( UINT32& uiCrc32, UINT32* &pBuffCur )
	{
		_Crc32NDecLt4( uiCrc32, pBuffCur );
		_Crc32NDecLt4( uiCrc32, pBuffCur );
		_Crc32NDecLt4( uiCrc32, pBuffCur );
		_Crc32NDecLt4( uiCrc32, pBuffCur );
		_Crc32NDecLt4( uiCrc32, pBuffCur );
		_Crc32NDecLt4( uiCrc32, pBuffCur );
		_Crc32NDecLt4( uiCrc32, pBuffCur );
		_Crc32NDecLt4( uiCrc32, pBuffCur );
	}


	static UINT32 _Crc32NDecLt( UINT32 uiCrc32, size_t uiLen, BYTE *pBuff )
	{
		UINT32 crc;
		UINT32 *pBuff4;

		crc = uiCrc32;
		crc = ~crc;

		pBuff4 = (UINT32*)pBuff;
		while (uiLen >= 32) {
			_Crc32NDecLt32( crc, pBuff4 );
			uiLen -= 32;
		}
		while (uiLen >= 4)
		{
			_Crc32NDecLt4( crc, pBuff4 );
			uiLen -= 4;
		}
		pBuff = (BYTE*)pBuff4;

		if (uiLen)
		{
			do
			{
				crc = __g_CRC32Table[0][(crc ^ *pBuff++) & 0xff] ^ (crc >> 8);
				--uiLen;
			} while (uiLen > 0);
		}
		crc = ~crc;

		return crc;
	}



	///////////////////////////////////////////////////////////////////////////////////////
	//
	//	Crc32 of big endian
	//


	static inline void _Crc32NDecBig4( UINT32& uiCrc32, UINT32* &pBuffCur )
	{
		*pBuffCur ^= (uiCrc32^CRYPTO_MASK);
		uiCrc32 ^= *pBuffCur++;

		uiCrc32 = __g_CRC32Table[4][uiCrc32 & 0xff] ^ __g_CRC32Table[5][(uiCrc32 >> 8) & 0xff] ^ 
					__g_CRC32Table[6][(uiCrc32 >> 16) & 0xff] ^ __g_CRC32Table[7][uiCrc32 >> 24];
	}

	static inline void _Crc32NDecBig32( UINT32& uiCrc32, UINT32* &pBuffCur )
	{
		_Crc32NDecBig4( uiCrc32, pBuffCur );
		_Crc32NDecBig4( uiCrc32, pBuffCur );
		_Crc32NDecBig4( uiCrc32, pBuffCur );
		_Crc32NDecBig4( uiCrc32, pBuffCur );
		_Crc32NDecBig4( uiCrc32, pBuffCur );
		_Crc32NDecBig4( uiCrc32, pBuffCur );
		_Crc32NDecBig4( uiCrc32, pBuffCur );
		_Crc32NDecBig4( uiCrc32, pBuffCur );
	}

	// NOTE: This is not tested
	static UINT32 _Crc32NDecBig( UINT32 uiCrc32, size_t uiLen, BYTE *pBuff )
	{
		UINT32 crc;
		UINT32 *pBuf4;

		crc = Util::SwapByte(uiCrc32);
		crc = ~crc;

		pBuf4 = (UINT32*)pBuff;
		while (uiLen >= 32)
		{
			_Crc32NDecBig32( crc, pBuf4 );
			uiLen -= 32;
		}
		while (uiLen >= 4)
		{
			_Crc32NDecBig4( crc, pBuf4 );
			uiLen -= 4;
		}
		pBuff = (BYTE*)pBuf4;

		if (uiLen)
		{
			do 
			{
				crc = __g_CRC32Table[4][(crc >> 24) ^ *pBuff++] ^ (crc << 8);
			} while (--uiLen);
		}
		crc = ~crc;

		return Util::SwapByte(crc);
	}
	

	// Make crc32 checksum and decrypt
	UINT32 Crc32NDecrypt( size_t uiBuffSize, BYTE* pBuff )
	{
		if( pBuff == nullptr )
			return 0;

		UINT32 endian = 1;

		if (*((unsigned char *)(&endian)))
			return _Crc32NDecLt(0, uiBuffSize, pBuff);
		else
			return _Crc32NDecBig(0, uiBuffSize, pBuff);
	}


}
} // namespace BR

