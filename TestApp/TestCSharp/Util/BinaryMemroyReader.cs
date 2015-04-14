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
    // Binary memory base class
    public abstract class BinaryMemoryIO
    {
        // memory buffer
        protected byte[] m_MemBuffer;
        // Stored Data length
        protected int m_Length;
        // Current access position
        protected int m_CurPosition;

        // Codec buffer
        protected UTF8Encoding m_StringEncoder = new UTF8Encoding(true, false);
        protected byte[] m_CodecBuffer;

        public UTF8Encoding StringEncoder { get { return m_StringEncoder; } }

        public int Length
        {
            set
            {
                m_Length = value;
                System.Diagnostics.Debug.Assert(m_MemBuffer.Length >= m_Length);
            }
            get
            {
                return m_Length;
            }
        }
        public int Capacity
        {
            get {
                System.Diagnostics.Debug.Assert(m_MemBuffer.Length >= m_Length);
                return m_MemBuffer.Length;
            }
        }
        public int Position { get { return m_CurPosition; } }
        public int RemainLength
        {
            get {
                System.Diagnostics.Debug.Assert(Length >= m_CurPosition);
                return Length - m_CurPosition; 
            }
        }

        public byte[] Buffer
        {
            get { return m_MemBuffer; }
            set {
                m_MemBuffer = value;
                m_CurPosition = 0;
                m_Length = m_MemBuffer != null ? m_MemBuffer.Length : 0;
            }
        }

        public void Skip(int iBytes)
        {
            m_CurPosition += iBytes;
            if (m_CurPosition > m_MemBuffer.Length) m_CurPosition = m_MemBuffer.Length;
        }

        public BinaryMemoryIO(byte[] memBuffer)
        {
            Buffer = memBuffer;
            m_CodecBuffer = new byte[1024];
        }
    };


    // Binary memory reader
    public unsafe class BinaryMemoryReader : BinaryMemoryIO
    {
        public BinaryMemoryReader(byte[] memBuffer)
            :base(memBuffer)
        {
        }

        // Validate and Decrypte data
        public UInt32 ValidateNDecrypt(uint length, bool isEncrypted)
        {
            UInt32 crc;

            if (length > RemainLength)
                return 0;

            fixed (byte* pData = &(m_MemBuffer[m_CurPosition]))
            {
                if( isEncrypted )
                    crc = Crypto.Crc32NDecrypt(length, pData);
                else
                    crc = Checksum.Crc32(0, length, pData);
            }
            return crc;
        }

        private void Read(byte* pDestBuff, int size)
        {
            if ((m_CurPosition + size) > m_MemBuffer.Length)
                throw new System.Exception("BinaryMemReader: No data is available");

            fixed (byte* pSrcDataOrg = &(m_MemBuffer[m_CurPosition]))
            {
                m_CurPosition += size;

                UInt32* pDest = (UInt32*)pDestBuff;
                UInt32* pSrc = (UInt32*)pSrcDataOrg;
                while (size > 4)
                {
                    *pDest++ = *pSrc++;
                    size -= 4;
                }

                pDestBuff = (Byte*)pDest;
                byte *pSrcData = (Byte*)pSrc;
                while (size > 0)
                {
                    *pDestBuff++ = *pSrcData++;
                    size--;
                }
            }
        }

        private void Read16(byte* pDestBuff)
        {
            if ((m_CurPosition + 2) > m_MemBuffer.Length)
                throw new System.Exception("BinaryMemoryWriter: Out of destination memory space");

            fixed (byte* pData = &(m_MemBuffer[m_CurPosition]))
            {
                m_CurPosition += 2;
                *(((UInt16*)pDestBuff)) = *(UInt16*)pData;
            }
        }

        private void Read32(byte* pDestBuff)
        {
            if ((m_CurPosition + 4) > m_MemBuffer.Length)
                throw new System.Exception("BinaryMemoryWriter: Out of destination memory space");

            fixed (byte* pData = &(m_MemBuffer[m_CurPosition]))
            {
                m_CurPosition += 4;
                *(((UInt32*)pDestBuff)) = *(UInt32*)pData;
            }
        }

        private void Read64(byte* pDestBuff)
        {
            if ((m_CurPosition + 8) > m_MemBuffer.Length)
                throw new System.Exception("BinaryMemoryWriter: Out of destination memory space");

            fixed (byte* pData = &(m_MemBuffer[m_CurPosition]))
            {
                m_CurPosition += 8;
                *(((UInt64*)pDestBuff)) = *(UInt64*)pData;
            }
        }

        public void ReadBytes( byte[] readBuffer )
        {
            if (readBuffer == null || readBuffer.Length == 0) return;

            if ((m_CurPosition + readBuffer.Length) > m_MemBuffer.Length)
                throw new System.Exception("BinaryMemReader: No data is available");

            fixed (byte* pDestBuff = &(readBuffer[0]))
            {
                Read(pDestBuff, readBuffer.Length);
            }
        }

        public Byte ReadByte()
        {
            if ((m_CurPosition + 1) > m_MemBuffer.Length)
                throw new System.Exception("BinaryMemReader: No data is available");

            byte value = m_MemBuffer[m_CurPosition];
            m_CurPosition++;
            return value;
        }

        public Byte ReadBoolean()
        {
            return ReadByte();
        }

        public Int16 ReadInt16()
        {
            Int16 value;
            Read16((byte*)&value);
            return value;
        }

        public UInt16 ReadUInt16()
        {
            UInt16 value;
            Read16((byte*)&value);
            return value;
        }

        public Int32 ReadInt32()
        {
            Int32 value;
            Read32((byte*)&value);
            return value;
        }

        public UInt32 ReadUInt32()
        {
            UInt32 value;
            Read32((byte*)&value);
            return value;
        }

        public Int64 ReadInt64()
        {
            Int64 value;
            Read64((byte*)&value);
            return value;
        }

        public UInt64 ReadUInt64()
        {
            UInt64 value;
            Read64((byte*)&value);
            return value;
        }

        public float ReadSingle()
        {
            float value;
            Read32((byte*)&value);
            return value;
        }

        public char[] ReadChars(int count)
        {
            char[] value = new char[count];
            fixed (char* pDest = &(value[0]))
            {
                Read((byte*)pDest, count);
            }
            return value;
        }

        public string ReadString()
        {
            int length = ReadUInt16();
            System.Diagnostics.Debug.Assert(length <= m_CodecBuffer.Length);
            fixed (byte* pBuffer = &m_CodecBuffer[0])
            {
                Read(pBuffer, length);
            }
            return m_StringEncoder.GetString(m_CodecBuffer, 0, length-1);
        }

        public string ReadString(int length)
        {
            System.Diagnostics.Debug.Assert(length <= m_CodecBuffer.Length);
            fixed (byte* pBuffer = &m_CodecBuffer[0])
            {
                Read(pBuffer, length);
            }
            // find actual string size
            int actualLength;
            for (actualLength = 0; actualLength < length && m_CodecBuffer[actualLength] != 0; actualLength++) ;

            return m_StringEncoder.GetString(m_CodecBuffer, 0, actualLength);
        }

        public byte[] ReadBytes(int count)
        {
            byte[] value = new byte[count];
            fixed (byte* pDest = &(value[0]))
            {
                Read(pDest, count);
            }
            return value;
        }

        // Seek
        public void Seek(int newPos)
        {
            System.Diagnostics.Debug.Assert(newPos >= 0 && newPos < m_MemBuffer.Length);
            m_CurPosition = newPos;
        }

        public NetAddress ReadNetAddress()
        {
            NetAddress value;
            value.Address = ReadString(16);
            value.Port = ReadUInt16();
            return value;
        }

    };


}
