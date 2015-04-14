////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : network packet class
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Text;


namespace BR
{
    // Unpacked packet header
    public unsafe class BinaryMemoryWriter : BinaryMemoryIO
    {

        public BinaryMemoryWriter(byte[] memBuffer)
            : base(memBuffer)
        {
        }

        // Validate and Decrypte data
        public UInt32 Crc32NEncrypt(bool isEncrypted)
        {
            UInt32 crc;

            fixed (byte* pData = &(m_MemBuffer[m_CurPosition]))
            {
                if (isEncrypted)
                    crc = Crypto.Crc32NEncrypt((uint)RemainLength, pData);
                else
                    crc = Checksum.Crc32(0, (uint)RemainLength, pData);
            }
            return crc;
        }

        private void Write(byte* pSrcBuff, int size)
        {
            if ((m_CurPosition + size) > m_MemBuffer.Length)
                throw new System.Exception("BinaryMemoryWriter: Out of destination memory space");

            fixed (byte* pDestData = &(m_MemBuffer[m_CurPosition]))
            {
                m_CurPosition += size;
                if (m_Length < m_CurPosition) m_Length = m_CurPosition;

                UInt32* pDest = (UInt32*)pDestData;
                UInt32* pSrc = (UInt32*)pSrcBuff;
                while (size > 4)
                {
                    *pDest++ = *pSrc++;
                    size -= 4;
                }

                pSrcBuff = (Byte*)pSrc;
                byte* pDstData = (Byte*)pDest;
                while (size > 0)
                {
                    *pDstData++ = *pSrcBuff++;
                    size--;
                }
            }
        }

        private void Write16(byte* pSrcBuff)
        {
            if ((m_CurPosition + 2) > m_MemBuffer.Length)
                throw new System.Exception("BinaryMemoryWriter: Out of destination memory space");

            fixed (byte* pData = &(m_MemBuffer[m_CurPosition]))
            {
                m_CurPosition += 2; if (m_Length < m_CurPosition) m_Length = m_CurPosition;
                *(((UInt16*)pData)) = *(UInt16*)pSrcBuff;
            }
        }

        private void Write32(byte* pSrcBuff)
        {
            if ((m_CurPosition + 4) > m_MemBuffer.Length)
                throw new System.Exception("BinaryMemoryWriter: Out of destination memory space");

            fixed (byte* pData = &(m_MemBuffer[m_CurPosition]))
            {
                m_CurPosition += 4; if (m_Length < m_CurPosition) m_Length = m_CurPosition;
                *(((UInt32*)pData)) = *(UInt32*)pSrcBuff;
            }
        }

        private void Write64(byte* pSrcBuff)
        {
            if ((m_CurPosition + 8) > m_MemBuffer.Length)
                throw new System.Exception("BinaryMemoryWriter: Out of destination memory space");

            fixed (byte* pData = &(m_MemBuffer[m_CurPosition]))
            {
                m_CurPosition += 8; if (m_Length < m_CurPosition) m_Length = m_CurPosition;
                *(((UInt64*)pData)) = *(UInt64*)pSrcBuff;
            }
        }

        public void Write( Byte value )
        {
            if ((m_CurPosition + 1) > m_MemBuffer.Length)
                throw new System.Exception("BinaryMemReader: No data is available");

            m_MemBuffer[m_CurPosition] = value;
            m_CurPosition++; if (m_Length < m_CurPosition) m_Length = m_CurPosition;
        }

        public void Write(Boolean value)
        {
            Write((byte)(value ? 1 : 0));
        }

        public void Write( Int16 value )
        {
            Write16((byte*)&value);
        }

        public void Write( UInt16 value )
        {
            Write16((byte*)&value);
        }

        public void Write( Int32 value)
        {
            Write32((byte*)&value);
        }

        public void Write( UInt32 value )
        {
            Write32((byte*)&value);
        }

        public void Write( Int64 value )
        {
            Write64((byte*)&value);
        }

        public void Write(UInt64 value)
        {
            Write64((byte*)&value);
        }

        public void Write( float value )
        {
            Write32((byte*)&value);
        }

        public void Write(char[] value)
        {
            fixed (char* pDest = &(value[0]))
            {
                Write((byte*)pDest, value.Length);
            }
        }

        public void Write(string value)
        {
            int encodedLength = m_StringEncoder.GetBytes(value, 0, value.Length, m_CodecBuffer, 0);
            System.Diagnostics.Debug.Assert(encodedLength < m_CodecBuffer.Length);
            if( encodedLength >= m_CodecBuffer.Length ) encodedLength = m_CodecBuffer.Length -1;
            Write((UInt16)(encodedLength + 1));
            fixed (byte* pBuffer = &m_CodecBuffer[0])
            {
                pBuffer[encodedLength] = 0;
                Write(pBuffer, encodedLength+1);
            }
        }

        public void Write(string value, int forceLength)
        {
            int encodedLength = m_StringEncoder.GetBytes(value, 0, value.Length, m_CodecBuffer, 0);
            System.Diagnostics.Debug.Assert(encodedLength < m_CodecBuffer.Length);
            if (encodedLength >= m_CodecBuffer.Length) encodedLength = m_CodecBuffer.Length - 1;

            fixed (byte* pBuffer = &m_CodecBuffer[0])
            {
                pBuffer[encodedLength] = 0;
                Write(pBuffer, forceLength);
            }
        }

        public void Write(byte[] value)
        {
            Write((UInt16)(value.Length));
            fixed (byte* pDest = &(value[0]))
            {
                Write(pDest, value.Length);
            }
        }

        public void Write(UInt16[] value)
        {
            Write((UInt16)(value.Length));
            fixed (UInt16* pDest = &(value[0]))
            {
                Write((byte*)pDest, sizeof(UInt16)*value.Length);
            }
        }

        public void Write(UInt32[] value)
        {
            Write((UInt16)(value.Length));
            fixed (UInt32* pDest = &(value[0]))
            {
                Write((byte*)pDest, sizeof(UInt32) * value.Length);
            }
        }

        public void Write(UInt64[] value)
        {
            Write((UInt16)(value.Length));
            fixed (UInt64* pDest = &(value[0]))
            {
                Write((byte*)pDest, sizeof(UInt64) * value.Length);
            }
        }

        // Seek
        public void Seek(int newPos)
        {
            System.Diagnostics.Debug.Assert(newPos >= 0 && newPos < m_MemBuffer.Length);
            m_CurPosition = newPos;
        }


        public void Write(ref NetAddress value)
        {
            Write(value.Address, 16);
            Write(value.Port);
        }

    };

}
