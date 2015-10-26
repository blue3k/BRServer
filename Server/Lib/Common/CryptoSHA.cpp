////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : KyungKun Ko
//
// Description : SHA256 hash
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/Trace.h"
#include "Common/Utility.h"
#include "openssl/sha.h"



namespace BR {
namespace Util {
	

	// SHA 256 hashing
	HRESULT SHA256Hash(size_t srcSize, const BYTE* bytes_to_encode, Array<BYTE> &destBuffer)
	{
		HRESULT hr = S_OK;

		destBuffer.SetSize(SHA256_DIGEST_LENGTH);

		SHA256_CTX sha256;
		if (SHA256_Init(&sha256) == FALSE)
			return E_FAIL;

		if (SHA256_Update(&sha256, bytes_to_encode, srcSize) == FALSE)
			return E_FAIL;

		if (SHA256_Final(destBuffer.data(), &sha256) == FALSE)
			return E_FAIL;

	Proc_End:

		return hr;
	}


}
} // namespace BR

