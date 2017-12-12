
#ifndef _Mesh_Format_Particle_Types_h_
#define _Mesh_Format_Particle_Types_h_


// Library includes
#include <vector>

// Project includes
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>

// Forward declarations

// Namespace declarations


namespace Mesh {


typedef std::vector<vector2<float> > Vector2fBuffer;
typedef std::vector<vector3<float> > Vector3fBuffer;
typedef std::vector<vector4<float> > Vector4fBuffer;


class ABuffer
{
public:
    unsigned int	mDriverId;

protected:
	ABuffer()
	: mDriverId(0)
	{ }
};

class ColorBuffer : public ABuffer
{
public:
	Vector4fBuffer	mData;
};

class TextureBuffer : public ABuffer
{
public:
	Vector2fBuffer	mData;
};

class VertexBuffer : public ABuffer
{
public:
	Vector3fBuffer	mData;
};


}


#endif
