////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Math definitions
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"


namespace BR
{	
	struct Vector3;

	//////////////////////////////////////////////////////////////////////////
	//
	// 2D floating-point vector
	//

	struct Vector2
	{
		FLOAT x;
		FLOAT y;

		inline Vector2();
		inline Vector2( const Vector2& src );
		inline Vector2( const Vector3& src );
		inline Vector2( FLOAT fX, FLOAT fY );
		inline Vector2( FLOAT init );

		inline Vector2 operator - () const;

		inline Vector2 operator * ( FLOAT fScale ) const;
		inline Vector2 operator / ( FLOAT fScale ) const;

		inline Vector2 operator + ( const Vector2& src ) const;
		inline Vector2 operator - ( const Vector2& src ) const;

		inline Vector2& operator += ( const Vector2& src );
		inline Vector2& operator -= ( const Vector2& src );

		inline Vector2& operator = ( const Vector2& src );
		inline Vector2& operator = ( const Vector3& src );

		inline bool operator == ( const Vector2& comp );
		inline bool operator != ( const Vector2& comp );

		inline FLOAT Normalize();
		inline Vector2 Normal() const;

		inline FLOAT Length() const;

		inline FLOAT Dot( const Vector2& opVec ) const;
	};



	//////////////////////////////////////////////////////////////////////////
	//
	// 3D floating-point vector
	//

	struct Vector3
	{
		FLOAT x;
		FLOAT y;
		FLOAT z;

		inline Vector3();
		inline Vector3( const Vector3& src );
		inline Vector3( const Vector2& src );
		inline Vector3( FLOAT fX, FLOAT fY, FLOAT fZ );
		inline Vector3( FLOAT init );

		inline Vector3 operator - () const;

		inline Vector3 operator * ( FLOAT fScale ) const;
		inline Vector3 operator / ( FLOAT fScale ) const;

		inline Vector3 operator + ( const Vector3& src ) const;
		inline Vector3 operator - ( const Vector3& src ) const;

		inline Vector3& operator += ( const Vector3& src );
		inline Vector3& operator -= ( const Vector3& src );

		inline Vector3& operator = ( const Vector3& src );
		inline Vector3& operator = ( const Vector2& src );

		inline bool operator == ( const Vector3& comp );
		inline bool operator != ( const Vector3& comp );

		inline FLOAT Normalize();
		inline Vector3 Normal() const;

		inline FLOAT Length() const;

		inline FLOAT Dot( const Vector3& opVec ) const;
		inline Vector3 Cross( const Vector3& opVec ) const;
	};



#include "MathUtil.inl"


}; // namespace BR


