
#ifndef Mesh_Format_MD2_MeshMD2_h
#define Mesh_Format_MD2_MeshMD2_h


// Library incudes

// Project includes
#include <Mesh/AMesh.h>

// Forward declarations
class t3DModel;

// Namespace declarations


namespace Mesh {


class MeshMD2 : public AMesh
{
public:
	MeshMD2();
	virtual ~MeshMD2();

public:
	// Core::RenderObject implementation
	// {
	void init();
	void render();
	// }

	// AMesh implementation
	// {
	void setAnimation(const std::string& animation);
	// }

	void setModel(t3DModel *model);

protected:

private:
	// Core::RenderObject implementation
	//{
	void setBoundingVolume();
	void unload();
	//}

	void renderWithoutDisplayList();
	float returnCurrentTime(int nextFrame);

private:
	float		mLastAnimTime;
	t3DModel	*mModel;
};


}


#endif
