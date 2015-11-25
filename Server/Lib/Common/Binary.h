////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Brave base types
//					- defines very basic game relative types
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"


namespace BR
{

#pragma pack(push)
#pragma pack(4)

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Binary header for binary data store/transfer
	//

	struct Binary
	{
		// Max Size for copy validation, only initialized by constructor
		UINT32 MaxSize;
		// Magic number for binary
		UINT32 Magic;
		// version number for binary
		UINT32 Version;
		// binary size, include this header
		UINT32 Size;


		// Constructor
		inline Binary( UINT32 uiMaxSize );
		inline Binary( UINT32 uiMaxSize, UINT32 uiMagic, UINT32 uiVersion );
		// Copy constructor , do not use
		Binary( const Binary& src );

		// Conversion to BLOB
		//inline operator BLOB();

		// Copy operator, do not use
		Binary& operator = ( const Binary& src );


		// Copy
		HRESULT CopyBinary( Binary& dst, const Binary& src );
	};

	

#pragma pack(pop)


#include "Binary.inl"


}; // namespace Svr



