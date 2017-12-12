
#ifndef Terrain_Subdivided_Layer_h
#define Terrain_Subdivided_Layer_h


// Library includes
#include <list>

// Project includes
#include <Common/Logger.h>
#include <Terrain/ALayer.h>
#include <Terrain/Types.h>

// Forward declarations

// Namespace declarations


namespace Terrain {
namespace Subdivided {

// Forward declarations
class Cell;

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

protected:

private:
	typedef std::list<Cell*> CellList;

private:
	bool load();
	void subdivide();

private:
	CellList		mCells;
    ColorBuffer     mColorBuffer;
	//IndexBuffer		mIndexBuffer;
    NormalBuffer    mNormalBuffer;
    PositionBuffer  mPositionBuffer;
    TexCoordBuffer  mTex0Buffer;
};


}
}


#endif
