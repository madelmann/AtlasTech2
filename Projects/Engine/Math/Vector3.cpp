
// Header
#include "Vector3.h"

// Library includes
#include <cmath>

// Project includes

// Namespace declarations


vector3f::vector3f()
//: vector3<float>(0.f, 0.f, 0.f)
: x(0.f),
  y(0.f),
  z(0.f)
{
}

vector3f::vector3f(float _x, float _y, float _z)
//: vector3<float>(_x, _y, _z)
: x(_x),
  y(_y),
  z(_z)
{
}

vector3f vector3f::cross(const vector3f& other) const
{
	return vector3f( y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

float vector3f::length( void )
{
    return static_cast<float>( sqrt( x * x + y * y + z * z ) );
}

void vector3f::normalize()
{
	float l = length();

	x = x / l;
	y = y / l;
	z = z / l;
}

//
// Operators...
//

vector3f vector3f::operator + ( const vector3f& other )
{
    return vector3f( x + other.x, y + other.y, z + other.z);
}

vector3f vector3f::operator + ( void ) const
{
    return vector3f( x, y, z );
}

vector3f vector3f::operator - ( const vector3f& other )
{
    return vector3f( x - other.x, y - other.y, z - other.z);
}

vector3f vector3f::operator - ( void ) const
{
    return vector3f( -x, -y, -z );
}

vector3f vector3f::operator * ( const vector3f& other )
{
    return vector3f( x * other.x, y * other.y, z * other.z );
}

vector3f vector3f::operator * ( const float scalar )
{
    return vector3f( x * scalar, y * scalar, z * scalar );
}

vector3f operator * ( const float scalar, const vector3f& other )
{
    return vector3f( other.x * scalar, other.y * scalar, other.z * scalar );
}

vector3f vector3f::operator / ( const vector3f& other )
{
    return vector3f( x / other.x, y / other.y, z / other.z );
}

vector3f vector3f::operator / ( const float scalar )
{
    return vector3f( x / scalar, y / scalar, z / scalar );
}

vector3f operator / ( const float scalar, const vector3f& other )
{
    return vector3f( other.x / scalar, other.y / scalar, other.z / scalar );
}

vector3f& vector3f::operator = ( const vector3f& other )
{
	x = other.x;
	y = other.y;
	z = other.z;

    return *this;
}

vector3f& vector3f::operator += ( const vector3f& other )
{
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
}

vector3f& vector3f::operator -= ( const vector3f& other )
{
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;
}

vector3f& vector3f::operator *= ( const float scalar )
{
    x *= scalar;
    y *= scalar;
    z *= scalar;

    return *this;
}

vector3f& vector3f::operator /= ( const float scalar )
{
    x /= scalar;
    y /= scalar;
    z /= scalar;

    return *this;
}

bool vector3f::operator == (const vector3f& other) const
{
	return (this->x == other.x) && (this->y == other.y) && (this->z == other.z);
}

bool vector3f::operator != (const vector3f& other) const
{
	return (this->x != other.x) || (this->y != other.y) || (this->z != other.z);
}

//
// Static utility methods...
//

float vector3f::distance( const vector3f& v1,  const vector3f& v2  )
{
	float dx = v1.x - v2.x;
	float dy = v1.y - v2.y;
	float dz = v1.z - v2.z;

	return (float)sqrt( dx * dx + dy * dy + dz * dz );
}

float vector3f::dotProduct( const vector3f& v1,  const vector3f& v2 )
{
	return( v1.x * v2.x + v1.y * v2.y + v1.z * v2.z  );
}

vector3f vector3f::crossProduct( const vector3f& v1,  const vector3f& v2 )
{
	return vector3f( v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x );
}
