////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : KyungKun Ko
//
// Description : BRCrypto
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Text;


namespace BR
{

    // Binary memory reader
    public unsafe static class Crypto
    {
#region Crc32NEncrypt
        	
	    const UInt32 CRYPTO_MASK = 0x45027602;

	    ///////////////////////////////////////////////////////////////////////////////////////
	    //
	    //	Crc32 and crypto of little endian
	    //

	    static void _Crc32NEncLt4( ref UInt32 uiCrc32, ref UInt32* pBuffCur )
	    {
		    UInt32 oldCrc = uiCrc32;

		    uiCrc32 ^= *pBuffCur;
		    uiCrc32 = Checksum.__g_CRC32Table[3][uiCrc32 & 0xff] ^ Checksum.__g_CRC32Table[2][(uiCrc32 >> 8) & 0xff] ^ 
					    Checksum.__g_CRC32Table[1][(uiCrc32 >> 16) & 0xff] ^ Checksum.__g_CRC32Table[0][uiCrc32 >> 24];

		    *pBuffCur++ ^= (oldCrc^CRYPTO_MASK);
	    }

	    static void _Crc32NEncLt32( ref UInt32 uiCrc32, ref UInt32* pBuffCur )
	    {
		    _Crc32NEncLt4( ref uiCrc32, ref pBuffCur );
		    _Crc32NEncLt4( ref uiCrc32, ref pBuffCur );
		    _Crc32NEncLt4( ref uiCrc32, ref pBuffCur );
		    _Crc32NEncLt4( ref uiCrc32, ref pBuffCur );
		    _Crc32NEncLt4( ref uiCrc32, ref pBuffCur );
		    _Crc32NEncLt4( ref uiCrc32, ref pBuffCur );
		    _Crc32NEncLt4( ref uiCrc32, ref pBuffCur );
		    _Crc32NEncLt4( ref uiCrc32, ref pBuffCur );
	    }


	    static UInt32 _Crc32NEncLt( UInt32 uiCrc32, UInt32 uiLen, byte *pBuff )
	    {
		    UInt32 crc;
		    UInt32 *pBuff4;

		    crc = uiCrc32;
		    crc = ~crc;

		    pBuff4 = (UInt32*)pBuff;
		    while (uiLen >= 32) {
			    _Crc32NEncLt32( ref crc, ref pBuff4 );
			    uiLen -= 32;
		    }
		    while (uiLen >= 4)
		    {
			    _Crc32NEncLt4( ref crc, ref pBuff4 );
			    uiLen -= 4;
		    }
		    pBuff = (byte*)pBuff4;

		    if (uiLen > 0)
		    {
			    do
			    {
				    crc = Checksum.__g_CRC32Table[0][(crc ^ *pBuff++) & 0xff] ^ (crc >> 8);
                    --uiLen;
			    } while (uiLen > 0);
		    }
		    crc = ~crc;

		    return crc;
	    }

	    // Make crc32 checksum and encrypt
	    public static UInt32 Crc32NEncrypt( UInt32 uiBuffSize, byte* pBuff )
	    {
		    if( pBuff == null )
			    return 0;

		    return _Crc32NEncLt(0, uiBuffSize, pBuff);
	    }

#endregion



        // Decrypt
	    static void _Crc32NDecLt4( ref UInt32 uiCrc32, ref UInt32* pBuffCur )
	    {
		    *pBuffCur ^= (uiCrc32^CRYPTO_MASK);
		    uiCrc32 ^= *pBuffCur++;
            uiCrc32 = Checksum.__g_CRC32Table[3][uiCrc32 & 0xff] ^ Checksum.__g_CRC32Table[2][(uiCrc32 >> 8) & 0xff] ^
                        Checksum.__g_CRC32Table[1][(uiCrc32 >> 16) & 0xff] ^ Checksum.__g_CRC32Table[0][uiCrc32 >> 24];

	    }

        static void _Crc32NDecLt32(ref UInt32 uiCrc32, ref UInt32* pBuffCur)
	    {
		    _Crc32NDecLt4( ref uiCrc32, ref pBuffCur );
		    _Crc32NDecLt4( ref uiCrc32, ref pBuffCur );
		    _Crc32NDecLt4( ref uiCrc32, ref pBuffCur );
		    _Crc32NDecLt4( ref uiCrc32, ref pBuffCur );
		    _Crc32NDecLt4( ref uiCrc32, ref pBuffCur );
		    _Crc32NDecLt4( ref uiCrc32, ref pBuffCur );
		    _Crc32NDecLt4( ref uiCrc32, ref pBuffCur );
		    _Crc32NDecLt4( ref uiCrc32, ref pBuffCur );
	    }


	    static UInt32 _Crc32NDecLt( UInt32 uiCrc32, UInt32 uiLen, byte *pBuff )
	    {
		    UInt32 crc;
		    UInt32 *pBuff4;

		    crc = uiCrc32;
		    crc = ~crc;

		    pBuff4 = (UInt32*)pBuff;
		    while (uiLen >= 32) {
			    _Crc32NDecLt32( ref crc, ref pBuff4 );
			    uiLen -= 32;
		    }
		    while (uiLen >= 4)
		    {
			    _Crc32NDecLt4( ref crc, ref pBuff4 );
			    uiLen -= 4;
		    }
		    pBuff = (byte*)pBuff4;

		    if (uiLen > 0)
		    {
			    do
			    {
				    crc = Checksum.__g_CRC32Table[0][(crc ^ *pBuff++) & 0xff] ^ (crc >> 8);
                    --uiLen;
			    } while (uiLen > 0);
		    }
		    crc = ~crc;

		    return crc;
	    }


	    // Make crc32 checksum and decrypt
	    public static UInt32 Crc32NDecrypt( UInt32 uiBuffSize, byte* pBuff )
	    {
		    if( pBuff == null )
			    return 0;

		    return _Crc32NDecLt(0, uiBuffSize, pBuff);
	    }

    };


}
