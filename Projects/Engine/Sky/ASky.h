
#ifndef _Sky_ASky_h_
#define _Sky_ASky_h_


// Library includes

// Project includes
#include "Settings.h"
#include <Mesh/AMesh.h>

// Forward declarations

// Namespace declarations


namespace SkySpace {


class ASky : public Mesh::AMesh
{
public:
	ASky(Core::AEid id);
	virtual ~ASky();

public:
	// Core::RenderObject implementation
	//{
	void init();
	// }

	void connectSettings(Settings *s);

protected:
	virtual void preprocess() = 0;

protected:
	aeuint		mMeshId;
	Settings	*mSettings;

private:

};


}


#endif
