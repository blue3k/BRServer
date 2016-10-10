////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Base64 codec
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/Trace.h"
#include "Common/Utility.h"



namespace BR {
namespace Util {
	
	static const char* g_Crypto_Base64_chars =	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
												"abcdefghijklmnopqrstuvwxyz"
												"0123456789+/";

	static const char* g_Crypto_Base64URL_chars =	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
													"abcdefghijklmnopqrstuvwxyz"
													"0123456789-_";

	static inline Result Base64_DecodeChar(BYTE in, BYTE& out, const char* encoding_chars)
	{
		if ('A' <= in && in <= 'Z')
		{
			out = in - 'A';
		}
		else if ('a' <= in && in <= 'z')
		{
			out = in - 'a' + 26;
		}
		else if ('0' <= in && in <= '9')
		{
			out = in - '0' + 26 * 2;
		}
		else
		{
			if (encoding_chars[26 * 2 + 10 + 0] == in)
				out = 26 * 2 + 10 + 0;
			else if (encoding_chars[26 * 2 + 10 + 1] == in)
				out = 26 * 2 + 10 + 1;
			else
				return ResultCode::FAIL;
		}

		return ResultCode::SUCCESS;
	}


	// Base 64 encode/decode
	static Result Base64Encode_(size_t srcSize, const BYTE* bytes_to_encode, Array<BYTE> &destBuffer, BYTE dummyChar, const char* encodeChars)
	{
		Result hr = ResultCode::SUCCESS;
		int i = 0;
		int j = 0;
		unsigned char char_array_3[3];
		unsigned char char_array_4[4];

		for (; srcSize > 0; srcSize--)
		{
			char_array_3[i++] = *(bytes_to_encode++);
			if (i == 3)
			{
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (i = 0; (i < 4); i++)
				{
					defChk(destBuffer.push_back((BYTE)encodeChars[char_array_4[i]]));
				}
				i = 0;
			}

		}

		if (i)
		{
			for (j = i; j < 3; j++)
				char_array_3[j] = '\0';

			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (j = 0; (j < i + 1); j++)
			{
				defChk(destBuffer.push_back(encodeChars[char_array_4[j]]));
			}

			if (dummyChar != '\0')
			{
				while ((i++ < 3))
				{
					defChk(destBuffer.push_back((BYTE)'='));
				}
			}
		}

	Proc_End:

		return hr;
	}


	static Result Base64Decode_(size_t srcSize, const BYTE* bytes_to_decode, Array<BYTE> &destBuffer, BYTE dummyChar, const char* encodeChars)
	{
		Result hr = ResultCode::SUCCESS;
		int in_len = (int)srcSize;
		int i = 0;
		int j = 0;
		int in_ = 0;
		unsigned char char_array_4[4], char_array_3[3];

		while (in_len-- && (bytes_to_decode[in_] != dummyChar))
		{
			defChk(Base64_DecodeChar(bytes_to_decode[in_], char_array_4[i++], encodeChars));

			in_++;
			if (i == 4)
			{
				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (i = 0; (i < 3); i++)
				{
					defChk(destBuffer.push_back(char_array_3[i]));
				}
				i = 0;
			}
		}

		if (i)
		{
			for (j = i; j <4; j++)
				char_array_4[j] = 0;

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (j = 0; (j < i - 1); j++)
			{
				defChk(destBuffer.push_back(char_array_3[j]));
			}
		}

	Proc_End:

		return hr;
	}


	Result Base64Encode(size_t srcSize, const BYTE* bytes_to_encode, Array<BYTE> &destBuffer, BYTE dummyChar)
	{
		return Base64Encode_(srcSize, bytes_to_encode, destBuffer, dummyChar, g_Crypto_Base64_chars);
	}

	Result Base64Decode(size_t srcSize, const BYTE* bytes_to_decode, Array<BYTE> &destBuffer, BYTE dummyChar)
	{
		return Base64Decode_(srcSize, bytes_to_decode, destBuffer, dummyChar, g_Crypto_Base64_chars);
	}


	Result Base64URLEncode(size_t srcSize, const BYTE* bytes_to_encode, Array<BYTE> &destBuffer, BYTE dummyChar)
	{
		return Base64Encode_(srcSize, bytes_to_encode, destBuffer, dummyChar, g_Crypto_Base64URL_chars);
	}

	Result Base64URLDecode(size_t srcSize, const BYTE* bytes_to_decode, Array<BYTE> &destBuffer, BYTE dummyChar)
	{
		return Base64Decode_(srcSize, bytes_to_decode, destBuffer, dummyChar, g_Crypto_Base64URL_chars);
	}


}
} // namespace BR

