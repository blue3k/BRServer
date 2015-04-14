////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Server Type definitions. 
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/Memory.h"
#include "Common/BrAssert.h"
#include "Common/BrBaseTypes.h"


namespace BR {



	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity State
	//
	enum class EntityState
	{
		FREE,
		WORKING,
		CLOSING,
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	BLOB wrapper class, control memory from CRT heap
	//

	struct BLOBBuffer : public BLOB
	{
	public:
	private:
	public:
		inline BLOBBuffer();
		inline BLOBBuffer( UINT uiSize, const BYTE *pData );
		inline BLOBBuffer( const BLOBBuffer& src );
		inline BLOBBuffer( const BLOB& src );
		inline ~BLOBBuffer();

		// Allocate given size of buffer 
		HRESULT Allocate( UINT uiSize );

		// Reallocate given size of buffer , data will be copied to new memory
		HRESULT Reallocate( UINT uiSize );


		// check 
		HRESULT IsSameSize( UINT uiSize, const BYTE *pData );

		inline operator LPBLOB() const;

		inline BLOBBuffer& operator = ( const BLOBBuffer& src );
		inline BLOBBuffer& operator = ( const BLOB& src );
	};





#include "SvrTypes.inl"



}; // namespace BR


