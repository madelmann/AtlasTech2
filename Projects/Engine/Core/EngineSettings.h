
#ifndef Core_EngineSettings_h
#define Core_EngineSettings_h


// Library includes

// Project includes
#include <Driver/GLWrapper.h>
#include <Interfaces/Common/ISettingsService.h>
#include <Interfaces/Common/Types.h>
#include <Renderer/FrameSettings.h>

// Forward declarations

// Namespace declarations


namespace Core {


class EngineSettings : public Renderer::FrameSettings,
					   public Common::ISettingsService
{
public:
	class Screen
	{
	public:
		Screen()
		: mDepth(0),
		  mHeight(0),
		  mLeft(0),
		  mTop(0),
		  mWidth(0)
		{ }

		int depth() const {
			return mDepth;
		}
		void depth(int depth) {
			mDepth = depth;
		}

		int getHeight() const {
			return mHeight;
		}
		void setHeight(int height) {
			mHeight = height;
		}

		int getLeft() const {
			return mLeft;
		}
		void setLeft(int left) {
			mLeft = left;
		}

		int getTop() const {
			return mTop;
		}
		void setTop(int top) {
			mTop = top;
		}

		int getWidth() const {
			return mWidth;
		}
		void setWidth(int width) {
			mWidth = width;
		}

		POINT getCenter() const {
			POINT c;
			c.x = getCenterX();
			c.y = getCenterY();

			return c;
		}
		int getCenterX() const {
			return mWidth / 2 + mLeft;
		}
		int getCenterY() const {
			return mHeight / 2 + mTop;
		}

	private:
		int mDepth;
		int	mHeight;
		int	mLeft;
		int	mTop;
		int	mWidth;
	};

public:
	EngineSettings();
	virtual ~EngineSettings() { }

public:
	Screen* getScreen() { return &mScreen; }

	void reset();

	float clipFar() const { return mClipFar; }
	void clipFar(float value) { mClipFar = value; }

	float clipNear() const { return mClipNear; }
	void clipNear(float value) { mClipNear = value; }

	bool pauseWhileInactive() const { return mPauseWhileInactive; }
	void pauseWhileInactive(bool state) { mPauseWhileInactive = state; }

	float getAnsitropicFilter() const { return mAnsitropicFilter; }
	void setAnsitropicFilter(float filter) { mAnsitropicFilter = filter; }

	bool isColorMaterial() const { return mColorMaterial; }
	void setColorMaterial(bool state) { mColorMaterial = state; }

	bool isCompressedTextures() const { return mCompressedTextures; }
	void setCompressedTextures(bool state) { mCompressedTextures = state; }

	bool isEditorMode() const { return mEditorMode; }
	void setEditorMode(bool state) { mEditorMode = state; }

	bool isLightModelTwoSided() const { return mLightModelTwoSided; }
	void setLightModelTwoSided(bool state) { mLightModelTwoSided = state; }

	GLdouble getFOV() const { return mFOV; }
	void setFOV(GLdouble fov) { mFOV = fov; }

	float getMaxFadeViewDepth() const { return mMaxFadeDepth; }
	void setMaxFadeViewDepth(float max) { mMaxFadeDepth = max; }

	float getMaximumFramerate() const { return mMaximumFramerate; }
	void setMaximumFramerate(float max) { mMaximumFramerate = max; }

	int getMaxLights() const { return mMaxLights; }
	void setMaxLights(aeint max) { mMaxLights = max; }

	int getMaxLODLevel() const { return mMaxLODLevel; }
	void setMaxLODLevel(int max) { mMaxLODLevel = max; }

	GLuint getMaxOcclusionCalculatedBeforeFPS() const { return mMaxOcclusionCalculatedBeforeFPS; }
	void setMaxOcclusionCalculatedBeforeFPS(GLuint fps) { mMaxOcclusionCalculatedBeforeFPS = fps; }

	AEfloat getMaxViewDepth() const { return mMaxViewDepth; }
	void setMaxViewDepth(AEfloat max) { mMaxViewDepth = max; }

	float getMotionBlurValue() const { return mMotionBlur; }
	void setMotionBlurValue(float blur) { mMotionBlur = blur; }

	float getTimeBetweenOcclusionQueries() const { return mTimeBetweenOcclusionQueries; }
	void setTimeBetweenOcclusionQueries(float time) { mTimeBetweenOcclusionQueries = time; }

	float getRenderDepth() const { return mRenderDepth; }
	void setRenderDepth(float depth) { mRenderDepth = depth; }

	bool isFullScreen() const { return mFullScreen; }
	void setFullScreen(bool state){ mFullScreen = state; }


	bool useDetailTextures() const { return mUseDetailTextures; }
	void useDetailTextures(bool state) { mUseDetailTextures = state; }

	bool useFlatShading() const { return mUseFlatShading; }
	void useFlatShading(bool state) { mUseFlatShading = state; }

	bool useFog() const { return mUseFog; }
	void useFog(bool state) { mUseFog = state; }

	bool useHighPerformanceCounter() const { return mUseHighPerformanceCounter; }
	void useHighPerformanceCounter(bool state) { mUseHighPerformanceCounter = state; }

	bool useLight() const { return mUseLight; }
	void useLight(bool state) { mUseLight = state; }

	bool useMotionBlur() const { return mUseMotionBlur; }
	void useMotionBlur(bool state) { mUseMotionBlur = state; }

	bool useMultiTexturing() const { return mUseMultiTexturing; }
	void useMultiTexturing(bool state) { mUseMultiTexturing = state; }

	bool useOcclusionQuery() const { return mUseOcclusionQuery; }
	void useOcclusionQuery(bool state) { mUseOcclusionQuery = state; }

	bool usePhysics() const { return mUsePhysics; }
	void usePhysics(bool state) { mUsePhysics = state; }

	bool useShaders() const { return mUseShaders; }
	void useShaders(bool state) { mUseShaders = state; }

	bool useTextures() const { return mUseTextures; }
	void useTextures(bool state) { mUseTextures = state; }

	GL::Hint::E getFogHint() const;
	void setFogHint(GL::Hint::E hint);

	GL::Hint::E getFragmentShaderDerivativeHint() const;
	void setFragmentShaderDerivativeHint(GL::Hint::E hint);

	GL::Hint::E getGenerateMipmapHint() const;
	void setGenerateMipmapHint(GL::Hint::E hint);

	GL::Hint::E getLineSmoothHint() const;
	void setLineSmoothHint(GL::Hint::E hint);

	GL::Hint::E getPerspectiveCorrectionHint() const;
	void setPerspectiveCorrectionHint(GL::Hint::E hint);

	GL::Hint::E getPointSmoothHint() const;
	void setPointSmoothHint(GL::Hint::E hint);

	GL::Hint::E getPolygonSmoothHint() const;
	void setPolygonSoothHint(GL::Hint::E hint);

	GL::Hint::E getTextureCompressionHint() const;
	void setTextureCompressionHint(GL::Hint::E hint);

protected:

private:
	bool	mEditorMode;
	bool	mIsConsoleVisible;
	float	mMaximumFramerate;
	bool	mPauseWhileInactive;
	Screen	mScreen;

	// OpenGL settings - begin
	float			mAnsitropicFilter;
	float			mClipFar;
	float			mClipNear;
	bool			mColorMaterial;
	bool			mCompressedTextures;
	GL::Hint::E		mFogHint;
	AEdouble		mFOV;
	GL::Hint::E		mFragmentShaderDerivativeHint;
	bool			mFullScreen;
	GL::Hint::E		mGenerateMipmapHint;
	bool			mLightModelTwoSided;
	GL::Hint::E		mLineSmoothHint;
	AEfloat			mMaxFadeDepth;
	aeint			mMaxLights;
	int				mMaxLODLevel;
	AEuint			mMaxOcclusionCalculatedBeforeFPS;
	AEfloat			mMaxViewDepth;
	float			mMotionBlur;
	GL::Hint::E		mPerspectiveCorrectionHint;
	GL::Hint::E		mPointSmoothHint;
	GL::Hint::E		mPolygonSmoothHint;
	float			mRenderDepth;
	GL::Hint::E		mTextureCompressionHint;
	float			mTimeBetweenOcclusionQueries;
	bool			mUseDetailTextures;
	bool			mUseFlatShading;
	bool			mUseFog;
	bool			mUseHighPerformanceCounter;
	bool			mUseLight;
	bool			mUseMotionBlur;
	bool			mUseMultiTexturing;
	bool			mUsePhysics;
	bool			mUseOcclusionQuery;
	bool			mUseShaders;
	bool			mUseTextures;
	// OpenGL settings - end
};


}


#endif
