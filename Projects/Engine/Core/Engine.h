
#ifndef Core_Engine_h
#define Core_Engine_h


// Library includes
#include <list>

// Project includes
#include <Common/Logger.h>
#include <Driver/Driver.h>
#include <Input/KeyGrabber.h>
#include <Interfaces/Common/AManager.h>
#include <Interfaces/Core/IController.h>
#include <Interfaces/Core/ICoreManagement.h>
#include <Interfaces/Core/ICoreUtilities.h>
#include <Interfaces/Core/Types.h>
#include <Misc/FPSCounter.h>
#include <Renderer/IRenderer.h>
#include <Tools/SharedPtr.h>
#include <UIKit/UIScreen.h>
#include "EngineSettings.h"
#include "EngineStates.h"
#include "IdGenerator.h"
#include "Settings.h"

// Forward declartions
namespace CameraSpace {
	class IViewer;
}
namespace CloudSpace {
	class IManager;
	class Manager;
}
namespace Common {
	class AProcessHandler;
}
namespace Console {
	class Console;
	class IConsole;
}
namespace FogSpace {
	class IManager;
	class Manager;
}
namespace FontSpace {
	class Font;
}
class Frustum;
class IMediaPathManager;
namespace Input {
	class IKeyboard;
	class IMouse;
	class Keyboard;
	class Mouse;
}
namespace LightSpace {
	class IManager;
	class Light;
	class Manager;
}
namespace MaterialSpace {
	class IManager;
	class Manager;
}
class MediaPathManager;
namespace Mesh {
	class IObjectManager;
	class ObjectManager;
}
namespace Physics {
	class IManager;
	class Manager;
}
namespace Scene {
	class IManager;
	class Manager;
}
namespace ShaderSpace {
	class IManager;
	class Manager;
}
namespace SkySpace {
	class IManager;
	class Manager;
}
namespace SoundSpace {
	class IManager;
	class Manager;
}
namespace Terrain {
	class IManager;
	class Manager;
}
namespace TextureSpace {
	class IManager;
	class Manager;
	class Texture;
}
namespace UIKit {
	class IManager;
	class Manager;
}
namespace WaterSpace {
	class IManager;
	class Manager;
}

// Namespace declarations


namespace Core {


// Forward declarations
namespace Configuration {
	class Configurator;
}
class Application;
class GeneralManager;
class IGeneralManager;
class IRendererPlugin;
class IScreenWriter;
class ScreenWriter;
class Viewport;


class Engine : public EngineSettings,
			   public EngineStates,
			   public IController,
			   public ICoreManagement,
			   public ICoreUtilities,
			   public IRenderer,
			   public Common::AManager,
			   public UIKit::UIScreen,
			   private Common::Logger
{
public:
	Engine(const Common::Logger *p);
	~Engine();

public:
	// IController implementation
	// {
	void Pause();
	void Play();
	// }

	// ICoreManagement implementation
	// {
	std::string getSceneName() const;
	bool loadScene(const std::string& scene);
	bool unloadScene();
	// }

	// ICoreUtilities implementation
	// {
	void registerPlugin(IRendererPlugin *plugin);
	void unregisterPlugin(IRendererPlugin *plugin);
	// }

	// Common::AManager implementation
	// {
	void clear();
	void configurationComplete();
	void init();
	void manage();
	void reset();
	void shutdown();
	void update(float elapsedTime);
	// }

	// Connectors
	// {
	void connectProcessHandler(Common::AProcessHandler *h);
	// }

	// Providers
	// {
	Console::IConsole* provideConsole() const;
	EngineSettings* provideEngineSettings();
	EngineStates* provideEngineStates();
	FogSpace::IManager* provideFogManager() const;
	FontSpace::Font* provideFont() const;
	IGeneralManager* provideGeneralManager() const;
	IdGenerator* provideIdGenerator();
	Input::IKeyboard* provideKeyboard() const;
	ICoreManagement* provideManagement();
	IMediaPathManager* provideMediaPathManager() const;
	Input::IMouse* provideMouse() const;
	Mesh::IObjectManager* provideObjectManager() const;
	Physics::IManager* providePhysicsManager() const;
	Scene::IManager* provideSceneManager() const;
	UIKit::IManager* provideScreenManager() const;
	IScreenWriter* provideScreenWriter() const;
	SoundSpace::IManager* provideSoundManager() const;
	Terrain::IManager* provideTerrainManager() const;
	TextureSpace::IManager* provideTextureManager() const;
	// }

	// Replacements
	// {
	void replaceTerrainManager(Terrain::IManager *m);
	// }

	// Initialization
	// {
	void doErrorCheck();
	void initialize();
	void setViewer(CameraSpace::IViewer *viewer);
	// }

	// Commonly called
	// {
	void renderImageAtFullScreen(Texture *texture);
	AEid requestID();
	// }

	// Console
	// {
	bool isConsoleVisible() const;
	// }

	// Getter
	// {
	const std::string& getError() const;
	HWND hwnd() const;
	// }

	// Setter
	// {
	// }

	// Public functionality
	// {
	HWND createClientWindow(Application *app);
	// }

	// To be moved to private
	// {
	void aeErrorCheck(char* file, int line);
	void aePrint(float x, float y, const std::string& text, ...);
	// }


	// Timing states - begin
	float getFrameRate() const;
	float getFrameTime() const;
	float getRunTime() const;
	// Timing states - end

	// UIKit::UIScreen implementation
	// {
	void reshape(float left, float top, float width, float height);
	// }

protected:
	// IRenderer && UIKit::UIScreen implementation
	// {
	void render();
	// }

private:
	typedef std::list<IRendererPlugin*> RendererPlugins;

private:
	void ClientRectToScreen(HWND hWnd, LPRECT lprcClient, LPRECT lprcScreen);

	void RenderImage2D(std::string filename, float left, float top, float width, float height);
	void RenderImage2D(Texture *texture, float left, float top, float width, float height);

	void resetDriverStates();

	void updateOcclusionQuerys();

	void setViewport(Viewport *viewport);

	// Graphics driver setup - begin
	void changeToFullScreen();
	void checkDriverExtensions();
	void InitializeOpenGL();
	bool SetupPixelFormat(HDC hdc);
	void SizeOpenGLScreen();
	// Graphics driver setup - end

	void loadConfig();

private:		// shutdown process
	void deletePlugins();

private:		// Connected classes
	Common::AProcessHandler	*mProcessHandler;

private:		// Owned classes
	RendererPlugins	mRendererPlugins;
	Settings		mSettings;

	CloudSpace::Manager			*mCloudManager;
	Console::Console			*mConsole;
	Driver						*mDriver;
	FogSpace::Manager			*mFogManager;
	FontSpace::Font				*mFont;
	Frustum						*mFrustum;
	GeneralManager				*mGeneralManager;
	IdGenerator					mIdGenerator;
	Input::Keyboard				*mKeyboard;
	Input::KeyGrabber			mKeyGrabber;
	LightSpace::Light			*mLight;
	LightSpace::Manager			*mLightManager;
	MaterialSpace::Manager		*mMaterialManager;
	MediaPathManager			*mMediaPathManager;
	Input::Mouse				*mMouse;
	Mesh::ObjectManager			*mObjectManager;
	Physics::Manager			*mPhysicsManager;
	Scene::Manager				*mSceneManager;
	UIKit::Manager				*mScreenManager;
	ScreenWriter				*mScreenWriter;
	ShaderSpace::Manager		*mShaderManager;
	SoundSpace::Manager			*mSoundManager;
	SkySpace::Manager			*mSkyManager;
	TextureSpace::Manager		*mTextureManager;
	CameraSpace::IViewer		*mViewer;
	Viewport					*mViewport;
	WaterSpace::Manager			*mWaterManager;

private:	// replaceable managers
	Terrain::Manager			*mDefaultTerrainManager;
	Terrain::IManager			*mTerrainManager;

private:
	std::string			mError;
	Misc::FPSCounter	mFPSCounter;
	float				mRunTime;
	Texture				*mScreenOutput;

private:
	// Windows specific
	HDC mHDC;
	HGLRC mHGLRC;
	HWND mHWND;
};


}


#endif
