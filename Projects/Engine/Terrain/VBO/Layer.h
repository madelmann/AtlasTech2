
#ifndef Terrain_VBO_Layer_h
#define Terrain_VBO_Layer_h


// Library includes

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Core/Types.h>
#include <Terrain/ALayer.h>
#include <Terrain/Types.h>

// Forward declarations
class btHeightfieldTerrainShape;
namespace Physics {
	class IManager;
	class PhysicalObject;
}

// Namespace declarations


namespace Terrain {
namespace VBO {


class Layer : public ALayer,
			  private Common::Logger
{
public:
	Layer(const Common::Logger *p);
	~Layer();

public:
	// Core::RenderObject implementation
	// {
	void render();
	// }

	// ALayer implementation
	// {
	bool build();
	void clear();
	vector3f heightAt(const vector3f& pos);
	vector3f normalAt(const vector3f& pos);
	// }

	// Connectors
	// {
	void connectPhysicManager(Physics::IManager *m);
	// }

	Physics::PhysicalObject* physicalObject() { return mPhysicalObject; }

protected:
	bool load();

	void generateCollisionMap();
    void generateIndexBuffer();
    void generateNormals();
    void generateVertexBuffers();

	void renderMultitextured();
	void renderNormals();
	void renderSingletextured();

private:
	Physics::IManager		*mPhysicManager;
	Physics::PhysicalObject	*mPhysicalObject;

	btHeightfieldTerrainShape	*mHeightfieldShape;
	float	*mRawHeightfieldData;

    ColorBuffer     mColorBuffer;
    IndexBuffer     mIndexBuffer;
    NormalBuffer    mNormalBuffer;
    PositionBuffer  mPositionBuffer;
    TexCoordBuffer  mTex0Buffer;

    AEuint  mGLColorBuffer;
    AEuint  mGLIndexBuffer;
    AEuint  mGLNormalBuffer;
    AEuint  mGLTex0Buffer;
    AEuint  mGLTex1Buffer;
    AEuint  mGLTex2Buffer;
    AEuint  mGLVertexBuffer;
};


}
}


#endif
