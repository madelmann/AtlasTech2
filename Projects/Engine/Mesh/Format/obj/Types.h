
#ifndef Mesh_Format_obj_Types_h
#define Mesh_Format_obj_Types_h


// Library includes
#include <vector>

// Project includes
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>

// Forward declarations

// Namespace declarations


namespace Mesh {


typedef std::vector<vector4<float> > ColorBuffer;
typedef std::vector<unsigned int> IndexBuffer;
typedef std::vector<vector3f > NormalBuffer;
typedef std::vector<vector4<unsigned short> > PolygonBuffer;
typedef std::vector<vector2<float> > TexCoordBuffer;
typedef std::vector<vector3f > VertexBuffer;


class obj_t
{
public:
	char name[20];

    unsigned int mGLColorBuffer;
    unsigned int mGLIndexBuffer;
    unsigned int mGLNormalBuffer;
    unsigned int mGLTex0Buffer;
    unsigned int mGLVertexBuffer;

	NormalBuffer	normal;
	IndexBuffer		normal_index;
	TexCoordBuffer	texture;
	IndexBuffer		texture_index;
	VertexBuffer	vertex;
	IndexBuffer		vertex_index;
};


}


#endif
