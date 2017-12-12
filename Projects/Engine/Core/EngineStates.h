
#ifndef Core_EngineStates_h
#define Core_EngineStates_h


// Library includes

// Project includes
#include <Driver/GLWrapper.h>
#include <Math/Vector3.h>
//#include <Misc/FPSCounter.h>
#include <Tools/Tools.h>

// Forward declarations

// Namespace declarations


namespace Core {


class EngineStates
{
public:
	EngineStates();
	virtual ~EngineStates() { }

public:
	const vector3f& getAmbientColor() const;

	// Cell shading states - begin
	void beginCellShading();
	void endCellShading();
	bool isCellShading() const;
	// Cell shading states - end

	// Script sequence states - begin
	void beginScriptSequence();
	void endScriptSequence();
	bool isScriptSequence() const;
	// Script sequence states - end

	// Sky face culling states - begin
	GLuint getSkyFaceCulling() const;
	void setSkyFaceCulling(GLuint culling);
	// Sky face culling states - end

	// Water rendering states - begin
	void beginRenderWater();
	void endRenderWater();
	float getWaterHeight() const;
	bool isRenderWater() const;
	// Water rendering states - end

protected:

private:
	vector3f	mAmbientColor;
	bool		mCellShading;
	bool		mRenderWater;
	bool		mScriptSequence;
	GLuint		mSkyFaceCulling;
	float		mWaterHeight;
};


}

#endif
