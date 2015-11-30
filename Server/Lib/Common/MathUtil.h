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
		float x;
		float y;

		inline Vector2();
		inline Vector2( const Vector2& src );
		inline Vector2( const Vector3& src );
		inline Vector2( float fX, float fY );
		inline Vector2( float init );

		inline Vector2 operator - () const;

		inline Vector2 operator * ( float fScale ) const;
		inline Vector2 operator / ( float fScale ) const;

		inline Vector2 operator + ( const Vector2& src ) const;
		inline Vector2 operator - ( const Vector2& src ) const;

		inline Vector2& operator += ( const Vector2& src );
		inline Vector2& operator -= ( const Vector2& src );

		inline Vector2& operator = ( const Vector2& src );
		inline Vector2& operator = ( const Vector3& src );

		inline bool operator == ( const Vector2& comp );
		inline bool operator != ( const Vector2& comp );

		inline float Normalize();
		inline Vector2 Normal() const;

		inline float Length() const;

		inline float Dot( const Vector2& opVec ) const;
	};



	//////////////////////////////////////////////////////////////////////////
	//
	// 3D floating-point vector
	//

	struct Vector3
	{
		float x;
		float y;
		float z;

		inline Vector3();
		inline Vector3( const Vector3& src );
		inline Vector3( const Vector2& src );
		inline Vector3( float fX, float fY, float fZ );
		inline Vector3( float init );

		inline Vector3 operator - () const;

		inline Vector3 operator * ( float fScale ) const;
		inline Vector3 operator / ( float fScale ) const;

		inline Vector3 operator + ( const Vector3& src ) const;
		inline Vector3 operator - ( const Vector3& src ) const;

		inline Vector3& operator += ( const Vector3& src );
		inline Vector3& operator -= ( const Vector3& src );

		inline Vector3& operator = ( const Vector3& src );
		inline Vector3& operator = ( const Vector2& src );

		inline bool operator == ( const Vector3& comp );
		inline bool operator != ( const Vector3& comp );

		inline float Normalize();
		inline Vector3 Normal() const;

		inline float Length() const;

		inline float Dot( const Vector3& opVec ) const;
		inline Vector3 Cross( const Vector3& opVec ) const;
	};



#include "MathUtil.inl"


}; // namespace BR


