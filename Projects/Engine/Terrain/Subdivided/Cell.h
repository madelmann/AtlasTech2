
#ifndef Terrain_Subdivided_Cell_h
#define Terrain_Subdivided_Cell_h


// Library includes
#include <vector>

// Project includes
#include <Math/Normals.h>
#include <Renderer/RenderObject.h>
#include <Terrain/Types.h>

// Forward declarations
namespace Core {
	class EngineSettings;
}

// Namespace declarations


namespace Terrain {
namespace Subdivided {


class Cell : public Core::RenderObject
{
public:
	class Mesh
	{
	public:
		ColorBuffer		mColorBuffer;
		IndexBuffer		mIndexBuffer;
		NormalBuffer	mNormalBuffer;
		PositionBuffer	mPositionBuffer;
		TexCoordBuffer	mTex0Buffer;
	};

public:
	Cell();
	~Cell();

public:
	// Core::RenderObject implementation
	// {
	void render();
	// }

	// Connectors
	// {
	void connectEngineSettings(Core::EngineSettings *s);
	// }

	void init();

	void setAdditionalTexture(Texture *additional);
	void setAlphaTexture(Texture *alpha);
	void setBaseTexture(Texture *base);
	void setCellSize(vector2i size);
	void setMesh(const Mesh& mesh);

protected:

private:
    void generateIndexBuffer();
    void generateNormals();
    void generateVertexBuffers();
	void renderMultitextured();
	void renderNormals();
	void renderSingletextured();
	void unload();

private:
	Core::EngineSettings	*mEngineSettings;

	Texture		*mAdditionalTexture;
	Texture		*mAlphaTexture;
	Texture		*mBaseTexture;
	vector2i	mCellSize;
	Mesh		mMesh;

    AEuint mGLColorBuffer;
    AEuint mGLIndexBuffer;
    AEuint mGLNormalBuffer;
    AEuint mGLTex0Buffer;
    AEuint mGLTex1Buffer;
    AEuint mGLTex2Buffer;
    AEuint mGLVertexBuffer;
};


}
}


#endif
