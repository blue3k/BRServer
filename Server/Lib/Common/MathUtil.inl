

//////////////////////////////////////////////////////////////////////////
// 2D floating-point vector

Vector2::Vector2()
{
}

Vector2::Vector2( const Vector2& src )
:x(src.x), y(src.y)
{
}

Vector2::Vector2( const Vector3& src )
:x(src.x), y(src.y)
{
}

Vector2::Vector2( float fX, float fY )
:x(fX), y(fY)
{
}

Vector2::Vector2( float init )
:x(init), y(init)
{
}

Vector2 Vector2::operator - () const
{
	return Vector2( -x, -y );
}

Vector2 Vector2::operator * ( float fScale ) const
{
	return Vector2( x * fScale, y * fScale );
}

Vector2 Vector2::operator / ( float fScale ) const
{
	return Vector2( x / fScale, y / fScale );
}

Vector2 Vector2::operator + ( const Vector2& src ) const
{
	return Vector2( x + src.x, y + src.y );
}

Vector2 Vector2::operator - ( const Vector2& src ) const
{
	return Vector2( x - src.x, y - src.y );
}


Vector2& Vector2::operator += ( const Vector2& src )
{
	x += src.x;
	y += src.y;

	return *this;
}

Vector2& Vector2::operator -= ( const Vector2& src )
{
	x -= src.x;
	y -= src.y;

	return *this;
}


bool Vector2::operator == ( const Vector2& comp )
{
	return fabs((float)(x-comp.x)) <= ::std::numeric_limits<float>::epsilon() && fabs((float)(y-comp.y)) <= ::std::numeric_limits<float>::epsilon();
}

bool Vector2::operator != ( const Vector2& comp )
{
	return fabs((float)(x-comp.x)) > std::numeric_limits<float>::epsilon() || fabs((float)(y-comp.y)) > std::numeric_limits<float>::epsilon();
}


Vector2& Vector2::operator = ( const Vector2& src )
{
	x = src.x;
	y = src.y;

	return *this;
}

Vector2& Vector2::operator = ( const Vector3& src )
{
	x = src.x;
	y = src.y;

	return *this;
}


float Vector2::Normalize()
{
	float fLen = sqrt( x*x + y*y );
	if( fLen < std::numeric_limits<float>::epsilon() )
	{
		x = 0;
		y = 0;
	}
	else
	{
		float fInvLen = (float)(1.0 / fLen);
		x *= fInvLen;
		y *= fInvLen;
	}

	return fLen;
}

Vector2 Vector2::Normal() const
{
	Vector2 vec( *this );
	vec.Normalize();
	return vec;
}

float Vector2::Length() const
{
	float fLen = x*x + y*y;
	if( fLen < std::numeric_limits<float>::epsilon() )
	{
		return 0;
	}
	else
	{
		return (float)sqrt( fLen );
	}
}


float Vector2::Dot( const Vector2& opVec ) const
{
	return x * opVec.x + y * opVec.y;
}

//////////////////////////////////////////////////////////////////////////
// 2D floating-point vector

Vector3::Vector3()
{
}

Vector3::Vector3( const Vector3& src )
:x(src.x), y(src.y), z(src.z)
{
}

Vector3::Vector3( const Vector2& src )
:x(src.x), y(src.y), z(0)
{
}

Vector3::Vector3( float fX, float fY, float fZ )
:x(fX), y(fY), z(fZ)
{
}

Vector3::Vector3( float init )
:x(init), y(init), z(init)
{
}

Vector3 Vector3::operator - () const
{
	return Vector3( -x, -y, -z );
}

Vector3 Vector3::operator * ( float fScale ) const
{
	return Vector3( x * fScale, y * fScale, z * fScale );
}

Vector3 Vector3::operator / ( float fScale ) const
{
	return Vector3( x / fScale, y / fScale, z / fScale );
}

Vector3 Vector3::operator + ( const Vector3& src ) const
{
	return Vector3( x + src.x, y + src.y, z + src.z );
}

Vector3 Vector3::operator - ( const Vector3& src ) const
{
	return Vector3( x - src.x, y - src.y, z - src.z );
}


Vector3& Vector3::operator += ( const Vector3& src )
{
	x += src.x;
	y += src.y;
	z += src.z;

	return *this;
}

Vector3& Vector3::operator -= ( const Vector3& src )
{
	x -= src.x;
	y -= src.y;
	z -= src.z;

	return *this;
}


bool Vector3::operator == ( const Vector3& comp )
{
	return fabs((float)(x-comp.x)) <= std::numeric_limits<float>::epsilon() 
		&& fabs((float)(y-comp.y)) <= std::numeric_limits<float>::epsilon()
		&& fabs((float)(z-comp.z)) <= std::numeric_limits<float>::epsilon();
}

bool Vector3::operator != ( const Vector3& comp )
{
	return fabs((float)(x-comp.x)) > std::numeric_limits<float>::epsilon() 
		|| fabs((float)(y-comp.y)) > std::numeric_limits<float>::epsilon()
		|| fabs((float)(z-comp.z)) > std::numeric_limits<float>::epsilon();
}


Vector3& Vector3::operator = ( const Vector3& src )
{
	x = src.x;
	y = src.y;
	z = src.z;

	return *this;
}


Vector3& Vector3::operator = ( const Vector2& src )
{
	x = src.x;
	y = src.y;
	z = 0;

	return *this;
}


float Vector3::Normalize()
{
	float fLen = sqrt( x*x + y*y + z*z );
	if( fLen < std::numeric_limits<float>::epsilon() )
	{
		x = 0;
		y = 0;
		z = 0;
	}
	else
	{
		float fInvLen = (float)(1.0 / fLen);
		x *= fInvLen;
		y *= fInvLen;
		z *= fInvLen;
	}

	return fLen;
}

Vector3 Vector3::Normal() const
{
	Vector3 vec( *this );
	vec.Normalize();
	return vec;
}

float Vector3::Length() const
{
	float fLen = x*x + y*y + z*z;
	if( fLen < std::numeric_limits<float>::epsilon() )
	{
		return 0;
	}
	else
	{
		return (float)sqrt( fLen );
	}
}


float Vector3::Dot( const Vector3& opVec ) const
{
	return x * opVec.x + y * opVec.y + z * opVec.z;
}

Vector3 Vector3::Cross( const Vector3& opVec ) const
{
	return Vector3( y*opVec.z-z*opVec.y, z*opVec.x-x*opVec.z, x*opVec.y-y*opVec.x );
}



