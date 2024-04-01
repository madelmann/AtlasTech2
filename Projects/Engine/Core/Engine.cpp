
// // This is a compiler directive that includes libraries (For Visual Studio).
// #pragma comment(lib, "glaux.lib")
// #pragma comment(lib, "glu32.lib")
// #pragma comment(lib, "opengl32.lib")
// #pragma comment(lib, "winmm.lib")


// #pragma comment( linker, "/NODEFAULTLIB:libc.lib" ) 


#define ENGINE_VERSION_MAJOR	0
#define ENGINE_VERSION_MINOR	1
#define ENGINE_VERSION_REVISION	1


// Header
#include "Engine.h"

// Library includes
#include <SDL2/SDL.h>

// Project includes
#include "GeneralManager.h"
#include <Application/Application.h>
#include <Camera/Viewer.h>
#include <Clouds/Manager.h>
#include <Console/Console.h>
#include <Console/Plugins/SpawnObject.h>
#include <Core/Common/Viewport.h>
#include <Core/Configuration/Configurator.h>
#include <Core/Plugins/ConsolePlugin.h>
#include <Driver/DriverInformation.h>
#include <Driver/DriverUtils.h>
#include <Fog/Manager.h>
#include <Font/Font.h>
#include <Frustum/Frustum.h>
#include <Input/ConsolePlugin.h>
#include <Input/Keyboard.h>
#include <Input/Mouse.h>
#include <Interfaces/Renderer/IRendererPlugin.h>
#include <Light/Light.h>
#include <Light/Manager.h>
#include <Material/Manager.h>
#include <Mesh/Manager.h>
#include <Misc/Colors.h>
#include <Physic/Manager.h>
#include <Renderer/ScreenWriter.h>
#include <Scene/Manager.h>
#include <Shader/Manager.h>
#include <Sky/Manager.h>
#include <Sound/Manager.h>
#include <Terrain/Manager.h>
#include <Texture/Manager.h>
#include <Texture/Texture.h>
#include <UIKit/Manager.h>
#include <Water/Manager.h>

// Namespace declarations


namespace Core {


Engine::Engine(const Common::Logger *p)
: Common::Logger(p, "Engine"),
  mError(""),
  mCloudManager(0),
  mConsole(0),
  mDefaultTerrainManager(0),
  mDriver(0),
  mFogManager(0),
  mFont(0),
  mGeneralManager(0),
  mKeyboard(0),
  mLight(0),
  mLightManager(0),
  mMaterialManager(0),
  mMediaPathManager(0),
  mPhysicsManager(0),
  mRunTime(0.f),
  mSceneManager(0),
  mScreenManager(0),
  mScreenOutput(0),
  mShaderManager(0),
  mSkyManager(0),
  mSoundManager(0),
  mTerrainManager(0),
  mTextureManager(0),
  mViewer(0),
  mWaterManager(0)
{
	debug("Starting...");

	{	// set version numbers
		setMajorVersion(ENGINE_VERSION_MAJOR);
		setMinorVersion(ENGINE_VERSION_MINOR);
		setRevision(ENGINE_VERSION_REVISION);
	}
	{	// UIScreen setup
		mName = "screen.engine";
	}
	{	// load engine configuration
		loadConfig();
	}
}

Engine::~Engine()
{
	shutdown();

	debug("Stopped.");
}

void Engine::aeErrorCheck(char* file, int line)
{
	AEenum error;

	if ( (error = glGetError()) != GL_NO_ERROR ) {
		mError = "failed with error: " + Tools::toString(gluErrorString(error));

		LogError(mError, file, line);
		debug(aeStackInfo());
	}
}

void Engine::aePrint(float x, float y, const std::string& text, ...)
{
	char	strText[255];
	va_list	argumentPtr;														// This will hold the pointer to the argument list

	va_start(argumentPtr, text);												// Parse the arguments out of the string

	vsprintf(strText, text.c_str(), argumentPtr);								// Now add the arguments into the full string

	va_end(argumentPtr);														// This resets and frees the pointer to the argument list.

	mFont->PrintText(strText, x, y);
}

void Engine::changeToFullScreen()
{
	debug("changeToFullScreen()");

	DEVMODE dmSettings = {0};							// Device Mode variable

	// Get current settings -- This function fills in our settings.
	// This makes sure NT and Win98 machines change correctly.
	if ( !EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmSettings) ) {
		// Display error message if we couldn't get display settings
		MessageBox(NULL, "Could not enum display settings", "Error", MB_OK);
		return;
	}

	dmSettings.dmPelsWidth	= getScreen()->getWidth();			// Selected Screen Width
	dmSettings.dmPelsHeight	= getScreen()->getHeight();			// Selected Screen Height
	dmSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	// This function actually changes the screen to full screen.
	// CDS_FULLSCREEN gets rid of the start Bar.
	// We always want to get a result from this function to check if we failed.
	int result = ChangeDisplaySettings(&dmSettings, CDS_FULLSCREEN);	

	// Check if we didn't receive a good return message From the function
	if ( result != DISP_CHANGE_SUCCESSFUL ) {
		// Display the error message and quit the program
		MessageBox(NULL, "Display mode not compatible", "Error", MB_OK);
		PostQuitMessage(0);
	}
}

void Engine::checkDriverExtensions()
{
	debug("checkDriverExtensions()");

	DriverExtensions *driverExt = mDriver->provideDriverExtensions();

	setAnsitropicFilter(
		driverExt->mAnsitropicFilter
	);
	useFog(
		(useFog() && driverExt->mglFogCoordfEXT) ? true : false
	);
	useMultiTexturing(
		(useMultiTexturing() && driverExt->mglActiveTextureARB) ? true : false
	);
	useOcclusionQuery(
		(useOcclusionQuery() && driverExt->mOcclusionQuery) ? true : false
	);
	useShaders(
		(useShaders() && driverExt->mglShaders) ? true : false
	);
}

void Engine::clear()
{
	debug("Clearing...");

	mSceneManager->clear();

	mIsReady = false;
}

void Engine::ClientRectToScreen(HWND hWnd, LPRECT lprcClient, LPRECT lprcScreen)
{
    POINT pt1, pt2;

    // transform client to screen coordinates
    pt1.x = lprcClient->left;
    pt1.y = lprcClient->top;
    pt2.x = lprcClient->right;
    pt2.y = lprcClient->bottom;

    ClientToScreen(hWnd, &pt1);
    ClientToScreen(hWnd, &pt2);

    SetRect(lprcScreen, pt1.x, pt1.y, pt2.x, pt2.y);
}

void Engine::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	mConfigurationComplete = true;
}

void Engine::connectProcessHandler(Common::AProcessHandler *h)
{
	mProcessHandler = h;
}

HWND Engine::createClientWindow(Application *app)
{
	WNDCLASS wndclass;

	memset(&wndclass, 0, sizeof(WNDCLASS));					// Init the size of the class
	wndclass.style = CS_HREDRAW | CS_VREDRAW;				// Regular drawing capabilities
	wndclass.lpfnWndProc = (WNDPROC)WinProc;				// Pass our function pointer as the window procedure
	wndclass.hInstance = app->instance();					// Assign our hInstance
	wndclass.hIcon = LoadIcon(0, IDI_APPLICATION);			// General icon
	wndclass.hCursor = LoadCursor(0, IDC_ARROW);			// An arrow for the cursor
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW);		// A white window
	wndclass.lpszClassName = app->name().c_str();		// Assign the class name

	ATOM registerResult = RegisterClass(&wndclass);
	if ( !registerResult ) {								// Register the class
		MessageBox(0, "Failed to register window class!", "Error!", MB_OK|MB_ICONERROR);
		return 0;
	}

	DWORD dwStyle = 0;
	if ( isFullScreen() ) {								// Check if we wanted full screen mode
														// Set the window properties for full screen mode
		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		changeToFullScreen();							// Go to full screen
	}
	else {												// Assign styles to the window depending on the choice
		dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}

	RECT rWindow;
	rWindow.left	= getScreen()->getLeft();									// Set Left Value
	rWindow.right	= getScreen()->getLeft() + getScreen()->getWidth();			// Set Right Value To Requested Width
	rWindow.top	    = getScreen()->getTop();									// Set Top Value
	rWindow.bottom	= getScreen()->getTop() + getScreen()->getHeight();			// Set Bottom Value To Requested Height

	AdjustWindowRect( &rWindow, dwStyle, false );		// Adjust Window To True Requested Size

	// Create the window
	mHWND = CreateWindowA(
			wndclass.lpszClassName,
			"AtlasEngine",
			dwStyle,
			getScreen()->getLeft(),
			getScreen()->getTop(),
			getScreen()->getWidth(),
			getScreen()->getHeight(),
			NULL,
			NULL,
			app->instance(),
			NULL
	);

	if ( !mHWND ) {
		mError = "CreateWindow Error " + Tools::toString(GetLastError());
		fatal(mError);

		return 0;										// If we could get a handle, return NULL
	}

	ShowWindow(mHWND, SW_SHOWNORMAL);					// Show the window
	UpdateWindow(mHWND);								// Draw the window

	SetFocus(mHWND);									// Sets Keyboard Focus To The Window
	//ShowCursor(TRUE);									// Hide the cursor

	return mHWND;
}

void Engine::deletePlugins()
{
	debug("deletePlugins(" + toString(mRendererPlugins.size()) + ")");

	for ( RendererPlugins::iterator it = mRendererPlugins.begin(); it != mRendererPlugins.end(); ++it ) {
		aeDelete( (*it) );
	}

	mRendererPlugins.clear();
}

void Engine::doErrorCheck()
{
	mError = Tools::toString(GetLastError());
	if ( mError != "0" ) {
		error(mError);
	}
}

const std::string& Engine::getError() const
{
	return mError;
}

float Engine::getFrameRate() const
{
	return mFPSCounter.getFrameRate();
}

float Engine::getFrameTime() const
{
	return mFPSCounter.getFrameTime();
}

float Engine::getRunTime() const
{
	return mRunTime;
}

std::string Engine::getSceneName() const
{
	return mMediaPathManager->getMap();
}

HWND Engine::hwnd() const
{
	assert(mHWND);
	return mHWND;
}

void Engine::init()
{
	debug("Initializing...");

	mFont->Initialize(mHDC, 20, "Arial");

	mScreenManager->reshape((float)getScreen()->getLeft(), (float)getScreen()->getTop(), (float)getScreen()->getWidth(), (float)getScreen()->getHeight());
	mScreenManager->init();

	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	//aeDepthMask(GL_TRUE);

	//aeForceDisable(GL_LIGHTING);

	aeForceEnable(GL_COLOR_MATERIAL);
	//aeForceEnable(GL_DEPTH);
	aeForceEnable(GL_DEPTH_TEST);
	aeForceEnable(GL_TEXTURE_2D);


	// Fog - begin
	float fogColor[4] = {0.95f, 0.93f, 0.76f, 1.f};								// Pick a tan color for our fog with a full alpha
	glFogfv(GL_FOG_COLOR, fogColor);											// Fog color
	glFogf (GL_FOG_DENSITY, 0.0003f);											// Gog density
	glFogf (GL_FOG_END, 8192.f);												// Fog end depth
	glFogi (GL_FOG_MODE, GL_EXP2);												// Fog mode
	glFogf (GL_FOG_START, 2048.f);												// Fog start depth

	aeHint(GL_FOG_HINT, GL_NICEST);
	aeHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST);
	aeHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	aeHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	aeHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	aeHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	aeHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	aeHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);

	// Check if any errors occured
	aeErrorCheck(__FILE__, __LINE__);
}

void Engine::initialize()
{
	debug("Initializing...");

	aeErrorCheck(__FILE__, __LINE__);

	// Initialize OpenGL and driver first
	mDriver = new Driver(this);

	InitializeOpenGL();
	if ( !mDriver->initialize() ) {
		mError = "Driver failed: " + Tools::toString(mDriver->getError());
		error(mError);
	}

	checkDriverExtensions();

	DriverInformation::getInstance().readInfo();


	// Check if any errors occured
	aeErrorCheck(__FILE__, __LINE__);

	// Check connected classes
	assert(mProcessHandler);

	// Setup our objects
	mCloudManager = new CloudSpace::Manager(this);
	mConsole = new Console::Console(this, mHDC);
	mDefaultTerrainManager = new Terrain::Manager(this);
	mFogManager = new FogSpace::Manager(this);
	mFont = new FontSpace::Font();
	mFrustum = new Frustum();
	mGeneralManager = new GeneralManager(this);
	mKeyboard = new Input::Keyboard(this);
	mLight = new LightSpace::Light(this);
	mLightManager = new LightSpace::Manager(this);
	mMaterialManager = new MaterialSpace::Manager(this);
	mMediaPathManager = new MediaPathManager(this, "base");
	mMouse = new Input::Mouse();
	mObjectManager = new Mesh::ObjectManager(this);
	mPhysicsManager = new Physics::Manager(this);
	mSceneManager = new Scene::Manager(this);
	mScreenManager = new UIKit::Manager(this);
	mScreenWriter = new ScreenWriter();
	mShaderManager = new ShaderSpace::Manager(this);
	mSkyManager = new SkySpace::Manager(this);
	mSoundManager = new SoundSpace::Manager(this);
	mTextureManager = new TextureSpace::Manager(this);
	mWaterManager = new WaterSpace::Manager(this);

	// Setup replaceable objects
	// {
	if ( !mTerrainManager ) {
		mTerrainManager = mDefaultTerrainManager;
	}
	// }


	mCloudManager->connectConsole(mConsole);
	mCloudManager->connectEngineStates(this);
	mCloudManager->connectMaterialManager(mMaterialManager);
	mCloudManager->connectMediaPathManager(mMediaPathManager);
	mCloudManager->connectMeshManager(mObjectManager);
	mCloudManager->connectSceneManager(mSceneManager);
	mCloudManager->connectTextureManager(mTextureManager);

	mConsole->connectEngineSettings(this);
	mConsole->connectMediaPathManager(mMediaPathManager);
	mConsole->connectRenderer(this);
	mConsole->connectTextureManager(mTextureManager);

	mFogManager->connectConsole(mConsole);
	mFogManager->connectMediaPathManager(mMediaPathManager);

	mFont->connectTextureManager(mTextureManager);
	mFont->setWidthHeight(getScreen()->getWidth(), getScreen()->getHeight());

	//mKeyboard->

	mLight->connectMediaPathManager(mMediaPathManager);

	mLightManager->connectConsole(mConsole);
	mLightManager->connectMediaPathManager(mMediaPathManager);

	mMaterialManager->connectIdGenerator(&mIdGenerator);
	mMaterialManager->connectMediaPathManager(mMediaPathManager);
	mMaterialManager->connectTextureManager(mTextureManager);

	mObjectManager->connectIdGenerator(&mIdGenerator);
	mObjectManager->connectMaterialManager(mMaterialManager);
	mObjectManager->connectMediaPathManager(mMediaPathManager);
	mObjectManager->connectShaderManager(mShaderManager);
	mObjectManager->connectTextureManager(mTextureManager);

	mPhysicsManager->connectConsole(mConsole);
	mPhysicsManager->connectIdGenerator(&mIdGenerator);
	mPhysicsManager->connectMediaPathManager(mMediaPathManager);
	mPhysicsManager->connectObjectManager(mObjectManager);
	mPhysicsManager->connectSceneManager(mSceneManager);

	mProcessHandler->processStart(&mKeyGrabber);

	mSceneManager->connectConsole(mConsole);
	mSceneManager->connectCoreUtilities(this);
	mSceneManager->connectDriver(mDriver);
	mSceneManager->connectFrustum(mFrustum);
	mSceneManager->connectEngineSettings(this);
	mSceneManager->connectIdGenerator(&mIdGenerator);
	mSceneManager->connectMediaPathManager(mMediaPathManager);
	mSceneManager->connectObjectManager(mObjectManager);
	mSceneManager->connectTerrainManager(mTerrainManager);

	mScreenWriter->connectFont(mFont);

	mShaderManager->connectEngineSettings(this);
	mShaderManager->connectMediaPathManager(mMediaPathManager);

	mSkyManager->connectConsole(mConsole);
	mSkyManager->connectIdGenerator(&mIdGenerator);
	mSkyManager->connectMediaPathManager(mMediaPathManager);
	mSkyManager->connectSceneManager(mSceneManager);
	mSkyManager->connectTextureManager(mTextureManager);

	mSoundManager->connectEngineSettings(this);
	mSoundManager->connectMediaPathManager(mMediaPathManager);

	{	// Terrain manager setup
		mDefaultTerrainManager->connectConsole(mConsole);
		mDefaultTerrainManager->connectCoreUtilities(this);
		mDefaultTerrainManager->connectEngineSettings(this);
		mDefaultTerrainManager->connectMaterialManager(mMaterialManager);
		mDefaultTerrainManager->connectMediaPathManager(mMediaPathManager);
		mDefaultTerrainManager->connectPhysicManager(mPhysicsManager);
		mDefaultTerrainManager->connectSceneManager(mSceneManager);
		mDefaultTerrainManager->connectShaderManager(mShaderManager);
		mDefaultTerrainManager->connectTextureManager(mTextureManager);
	}

	mTextureManager->connectEngineSettings(this);
	mTextureManager->connectIdGenerator(&mIdGenerator);
	mTextureManager->connectMediaPathManager(mMediaPathManager);

	mWaterManager->connectConsole(mConsole);
	//mWaterManager->connectLight(mLight);
	mWaterManager->connectMaterialManager(mMaterialManager);
	mWaterManager->connectMediaPathManager(mMediaPathManager);
	mWaterManager->connectShaderManager(mShaderManager);
	mWaterManager->connectSceneManager(mSceneManager);
	mWaterManager->connectTextureManager(mTextureManager);
	//mWaterManager->connectViewer(mViewer);


	// Complete configuration
	mTextureManager->configurationComplete();		// has to be done at first because many components rely on this
	mFont->configurationComplete();
	mConsole->configurationComplete();
	mMaterialManager->configurationComplete();
	mSceneManager->configurationComplete();
	mScreenManager->configurationComplete();
	mScreenWriter->configurationComplete();
	mSkyManager->configurationComplete();

	mDefaultTerrainManager->configurationComplete();


	mGeneralManager->add(mCloudManager);
	mGeneralManager->add(mFogManager);
	mGeneralManager->add(mLightManager);
	mGeneralManager->add(mObjectManager);
	mGeneralManager->add(mPhysicsManager);
	mGeneralManager->add(mShaderManager);
	mGeneralManager->add(mSoundManager);
	mGeneralManager->add(mWaterManager);

	mGeneralManager->configurationComplete();


	// Console Plugins - begin
	{
		ConsolePlugin *consolePlugin = new ConsolePlugin();
		consolePlugin->connectEngine(this);
		consolePlugin->connectVersionInformation(this);
		mConsole->registerPlugin(consolePlugin);

		Input::ConsolePlugin *inputPlugin = new Input::ConsolePlugin();
		inputPlugin->connectKeyboard(mKeyboard);
		inputPlugin->connectKeyGrabber(&mKeyGrabber);
		inputPlugin->connectMouse(mMouse);
		mConsole->registerPlugin(inputPlugin);

		// SpawnObject Plugin
		SpawnObject *spawnCmd = new SpawnObject();
		spawnCmd->connectObjectManager(mObjectManager);
		spawnCmd->connectSceneManager(mSceneManager);
		//FIXME: does not support user provided terrain managers!
		spawnCmd->connectTerrainManager(mTerrainManager);
		mConsole->registerPlugin(spawnCmd);
	}
	// Console Plugins - end

	// Engine Plugins - begin
	this->setHeight((float)getScreen()->getHeight());
	this->setWidth((float)getScreen()->getWidth());
	mScreenManager->add(this);

	mConsole->setHeight((float)getScreen()->getHeight());
	mConsole->setWidth((float)getScreen()->getWidth());
	mScreenManager->add(mConsole);
	// Engine Plugins - end

	mScreenOutput = mTextureManager->createRenderTexture(
		mScreenOutput, getScreen()->getWidth(), getScreen()->getHeight(), 4, GL_RGBA
	);


	aeErrorCheck(__FILE__, __LINE__);

	reset();

	mIsReady = true;
}

//=============================== INITIALIZE GL =================================
//
//	This function handles all the initialization for OpenGL.
//
//=============================== INITIALIZE GL =================================

void Engine::InitializeOpenGL()
{
	debug("InitializeOpenGL()");

	aeErrorCheck(__FILE__, __LINE__);

	mHDC = GetDC(mHWND);														// This sets our global HDC
																				// We don't free this hdc until the end of our program
	if ( !SetupPixelFormat(mHDC) ) {											// This sets our pixel format/information
		PostQuitMessage(0);														// If there's an error, quit
	}

	mHGLRC = wglCreateContext(mHDC);											// This creates a rendering context from our hdc
	if ( !wglMakeCurrent(mHDC, mHGLRC) ) {										// This makes the rendering context we just created the one we want to use
		exit(1);
	}

	SizeOpenGLScreen();															// Setup the screen translations and viewport

	aeErrorCheck(__FILE__, __LINE__);
}

bool Engine::isConsoleVisible() const
{
	return mConsole->isVisible();
}

void Engine::loadConfig()
{
	Configuration::Configurator config(this, "engine.conf");
	if ( !config.success() ) {
		fatal("could not load engine configuration!");
	}

	// Engine settings
	pauseWhileInactive(config.getValue("pause_while_inactive").toBool());
	setEditorMode(config.getValue("use_editor").toBool());
	setFullScreen(config.getValue("is_fullscreen").toBool());
	setMaximumFramerate(config.getValue("max_frame_rate").toFloat());
	usePhysics(config.getValue("use_physics").toBool());

	// OpenGL settings
	clipFar(config.getValue("clip_far").toFloat());
	clipNear(config.getValue("clip_near").toFloat());
	setAnsitropicFilter(config.getValue("ansitropic_filter").toFloat());
	setColorMaterial(config.getValue("use_color_material").toBool());
	setCompressedTextures(config.getValue("use_compressed_textures").toBool());
	setFOV(config.getValue("fov").toDouble());
	useDetailTextures(config.getValue("use_detail_textures").toBool());
	useFlatShading(config.getValue("use_flat_shading").toBool());
	useFog(config.getValue("use_fog").toBool());
	useHighPerformanceCounter(config.getValue("use_highperformancecounter").toBool());
	useLight(config.getValue("use_light").toBool());
	setLightModelTwoSided(config.getValue("use_light_model_two_sided").toBool());
	setMaxFadeViewDepth(config.getValue("max_fade_depth").toFloat());
	setMaxLODLevel(config.getValue("max_lod_level").toInt());
	setMaxViewDepth(config.getValue("max_view_depth").toFloat());
	setMotionBlurValue(config.getValue("motion_blur").toFloat());
	useMotionBlur(config.getValue("use_motion_blur").toBool());
	useMultiTexturing(config.getValue("use_multi_texturing").toBool());
	useOcclusionQuery(config.getValue("use_occlusion_queries").toBool());
	setRenderDepth(config.getValue("render_depth").toFloat());
	useShaders(config.getValue("use_shaders").toBool());
	useTextures(config.getValue("use_textures").toBool());
	setTimeBetweenOcclusionQueries(config.getValue("occlusion_query_time").toFloat());

	DriverInformation& DI = DriverInformation::getInstance();
	DI.setCompactDriverInfo(config.getValue("compact_driver_info").toBool());

	// Console settings
	//mConsole->provideSettings().setEnabled(true);
	//mConsole->provideSettings().setFontName("Courier_New");
	//mConsole->provideSettings().setFontSize(20);

	// Screen settings
	Screen *screen = getScreen();
	screen->depth(config.getValue("screen_depth").toInt());
	screen->setHeight(config.getValue("screen_height").toInt());
	if ( !isFullScreen() ) {
		screen->setLeft(config.getValue("screen_left").toInt());
		screen->setTop(config.getValue("screen_top").toInt());
	}
	screen->setWidth(config.getValue("screen_width").toInt());
	// screen->getCenter();


	mFPSCounter.allowHighFrequencyTimerUsage(useHighPerformanceCounter());

	if ( useFlatShading() ) { glShadeModel(GL_FLAT); }
	else { glShadeModel(GL_SMOOTH); }
}

bool Engine::loadScene(const std::string& scene)
{
	unloadScene();

	debug("Loading scene '" + scene + "'...");
	if ( scene.empty() ) {
		error("invalid scene name provided!");
		return false;
	}

	mMediaPathManager->setMap(scene);

	if ( useFog() ) {
		mFogManager->init();
	}
	if ( useLight() ) {
		mLight->init();
	}

	mSceneManager->setViewer(mViewer);
	mSceneManager->init();

	//mWaterManager->connectViewer(mViewer);

	mSkyManager->init();
	if ( usePhysics() ) {
		mPhysicsManager->load();

		mPhysicsManager->provideSettings()->position(mSkyManager->provideSettings()->position());
		mPhysicsManager->provideSettings()->size(mSkyManager->provideSettings()->size());
	}
	mTerrainManager->init();

	if ( !mSceneManager->load(scene) ) {
		error("scene '" + scene + "' could not be loaded!");
		return false;
	}

	mGeneralManager->init();

	mIsReady = true;
	return true;
}

void Engine::manage()
{
	// Calculate our frame rate and set our frame interval for time-based movement
	mFPSCounter.markFrameEnd();
	mRunTime += mFPSCounter.getFrameTime();
	mFPSCounter.markFrameStart();

	updateOcclusionQuerys();

	resetDriverStates();

	glClear( GL_ACCUM_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	{	// update 2d mouse position
		static POINT mp;

		GetCursorPos(&mp);														// Gets The Current Cursor Coordinates (Mouse Coordinates)
		ScreenToClient(hwnd(), &mp);

		mMouse->setPosition2D(vector2l(mp.x, mp.y));
	}

	aePushAttrib(GL_ALL_ATTRIB_BITS);											// push all attribute bits
		mScreenManager->render();												// Render all screens
	aePopAttrib();

	SwapBuffers(mHDC);															// Swap the backbuffers to the foreground

	if ( mIsRunning ) {
		mGeneralManager->manage();
	}

	update(mFPSCounter.getFrameTime());
}

void Engine::Pause()
{
	mIsRunning = false;
}

void Engine::Play()
{
	if ( !mIsReady ) {
		return;
	}

	mIsRunning = true;
}

Console::IConsole* Engine::provideConsole() const
{
	return mConsole;
}

EngineSettings* Engine::provideEngineSettings()
{
	return this;
}

EngineStates* Engine::provideEngineStates()
{
	return this;
}

FogSpace::IManager* Engine::provideFogManager() const
{
	return mFogManager;
}

FontSpace::Font* Engine::provideFont() const
{
	return mFont;
}

IGeneralManager* Engine::provideGeneralManager() const
{
	return mGeneralManager;
}

IdGenerator* Engine::provideIdGenerator()
{
	return &mIdGenerator;
}

Input::IKeyboard* Engine::provideKeyboard() const
{
	return mKeyboard;
}

ICoreManagement* Engine::provideManagement()
{
	return this;
}

IMediaPathManager* Engine::provideMediaPathManager() const
{
	return mMediaPathManager;
}

Input::IMouse* Engine::provideMouse() const
{
	return mMouse;
}

Mesh::IObjectManager* Engine::provideObjectManager() const
{
	return mObjectManager;
}

Physics::IManager* Engine::providePhysicsManager() const
{
	return mPhysicsManager;
}

Scene::IManager* Engine::provideSceneManager() const
{
	return mSceneManager;
}

UIKit::IManager* Engine::provideScreenManager() const
{
	return mScreenManager;
}

IScreenWriter* Engine::provideScreenWriter() const
{
	return mScreenWriter;
}

SoundSpace::IManager* Engine::provideSoundManager() const
{
	return mSoundManager;
}

Terrain::IManager* Engine::provideTerrainManager() const
{
	return mTerrainManager;
}

TextureSpace::IManager* Engine::provideTextureManager() const
{
	return mTextureManager;
}

void Engine::registerPlugin(Core::IRendererPlugin *plugin)
{
	debug("registerPlugin('" + plugin->name() + "')");

	mRendererPlugins.push_back(plugin);
}

void Engine::render()
{
	aeColor(Color::white);
	aeViewport(mLeft, mTop, mWidth, mHeight);									// Reset our viewport in case any other screen did set it to some stupid values

	glMatrixMode(GL_MODELVIEW);													// Go back to our model view matrix like normal
	aePushMatrix();																// Push a new matrix
		aeLoadIdentity();

		aeForceEnable(GL_CULL_FACE);
		aeForceEnable(GL_DEPTH_TEST);
		aeDepthFunc(GL_LESS);

		mViewer->look();
		mFrustum->update();

		mFogManager->set();
		mLight->set();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		aePushMatrix();
			aePushAttrib(GL_ALL_ATTRIB_BITS);
				mSkyManager->render();
			aePopAttrib();
		aePopMatrix();

		aePushMatrix();
			aePushAttrib(GL_ALL_ATTRIB_BITS);
				mTerrainManager->render();
			aePopAttrib();
		aePopMatrix();

		aePushMatrix();
			aePushAttrib(GL_ALL_ATTRIB_BITS);
				mSceneManager->render();
			aePopAttrib();
		aePopMatrix();

		mLight->unset();
		mFogManager->unset();
	aePopMatrix();																// Pop current matrix


	if ( !mRendererPlugins.empty() ) {
		for ( RendererPlugins::iterator it = mRendererPlugins.begin(); it != mRendererPlugins.end(); it++ ) {
			glMatrixMode(GL_MODELVIEW);
			aePushMatrix();
				aeLoadIdentity();

				(*it)->render();
			aePopMatrix();
		}
	}


	{	// update 3d mouse position
		float l = glPosition3d(mMouse->getPosition2D()).length();
		vector3f n = mViewer->view();
		n.normalize();

		vector3f v = n * l;
		v += mViewer->position();
		mMouse->setPosition3D(v);
	}


	if ( useMotionBlur() ) {													// Do motion blurring (if enabled)
		glAccum(GL_MULT, getMotionBlurValue());
		glAccum(GL_ACCUM, 1.0f - getMotionBlurValue());
		glAccum(GL_RETURN, 1.0f);

		glFlush();
	}

	aePushAttrib(GL_ALL_ATTRIB_BITS);											// Push all attribute bits
		mScreenWriter->render();
	aePopAttrib();


	// reset texture states
	aeActiveTexture(GL_TEXTURE0);
	glClientActiveTexture(GL_TEXTURE0);
	aeForceEnable(GL_TEXTURE_2D);

	{	// write to to texture
		// Bind the current scene to our screen output texture
		mScreenOutput->bind();
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, getScreen()->getWidth(), getScreen()->getHeight());

		// set output to screen background
		this->setBackground(mScreenOutput);
	}

	UIScreen::render();
}

void Engine::RenderImage2D(std::string filename, float left, float top, float width, float height)
{
	RenderImage2D(mTextureManager->find(filename), left, top, width, height);
}

void Engine::RenderImage2D(Texture *texture, float left, float top, float width, float height)
{
	glMatrixMode(GL_PROJECTION);
	aePushMatrix();								// Push on a new matrix so that we can just pop it off to go back to perspective mode
		aeLoadIdentity();						// Reset the current matrix to our identify matrix

		// Pass in our 2D ortho screen coordinates like so (left, right, bottom, top).
		// The last 2 parameters are the near and far planes.
		glOrtho(0, getScreen()->getWidth(), 0, getScreen()->getHeight(), 0, 10);

		texture->bind();

		aeBegin(GL_QUADS);
			aeTexCoord2f(0.0f, 0.0f); aeVertex2f(left, top);
			aeTexCoord2f(1.0f, 0.0f); aeVertex2f(left + width, top);
			aeTexCoord2f(1.0f, 1.0f); aeVertex2f(left + width, top + height);
			aeTexCoord2f(0.0f, 1.0f); aeVertex2f(left, top + height);
		aeEnd();

	glMatrixMode(GL_MODELVIEW);					// Go back to our model view matrix like normal
	aePopMatrix();
}

void Engine::renderImageAtFullScreen(Texture *texture)
{
	RenderImage2D(texture, 0.0f, 0.0f, (float)getScreen()->getWidth(), (float)getScreen()->getHeight());
}

void Engine::replaceTerrainManager(Terrain::IManager *m)
{
	assert(!mTerrainManager);

	// replace default terrain manager with a user provided implementation
	mTerrainManager = m;
}

AEid Engine::requestID()
{
	return mIdGenerator.generate();
}

void Engine::reset()
{
	debug("Resetting...");

	clear();
	init();
	
	mGeneralManager->reset();
}

void Engine::resetDriverStates()
{
	TextureSpace::Texture::unbind();

	aeDisable(GL_CULL_FACE);
	aeDisable(GL_LIGHTING);
	aeDisable(GL_TEXTURE);

	aeColor3f(1.f, 1.f, 1.f);
}

void Engine::reshape(float left, float top, float width, float height)
{
	UIScreen::reshape(left, top, width, height);

	getScreen()->setLeft((int)left);
	getScreen()->setTop((int)top);
	getScreen()->setWidth((int)width);
	getScreen()->setHeight((int)height);

	if ( height == 0 ) {														// Prevent A Divide By Zero error
		height = 1;																// Make the Height Equal One
	}

	aeViewport(left, top, width, height);										// Make our viewport the whole window

	glMatrixMode(GL_PROJECTION);												// Select The Projection Matrix
	aeLoadIdentity();															// Reset The Projection Matrix

	gluPerspective(getFOV(), width / height, clipNear(), clipFar());			// Calculate The Aspect Ratio Of The Window

	glMatrixMode(GL_MODELVIEW);													// Select The Modelview Matrix
	aeLoadIdentity();															// Reset The Modelview Matrix

	if ( mTextureManager ) {
		mScreenOutput = mTextureManager->createRenderTexture(
			mScreenOutput, getScreen()->getWidth(), getScreen()->getHeight(), 4, GL_RGBA
		);
	}
}

void Engine::setViewer(CameraSpace::IViewer *viewer)
{
	assert(viewer);
	mViewer = viewer;
}

void Engine::setViewport(Viewport *viewport)
{
	assert(viewport);
	mViewport = viewport;
}

//=============================== SET UP PIXEL FORMAT =================================
//
//	This function sets the pixel format for OpenGL.
//
//=============================== SET UP PIXEL FORMAT =================================

bool Engine::SetupPixelFormat(HDC hdc)
{
	assert(hdc);

	PIXELFORMATDESCRIPTOR pfd = {0}; 
	int pixelformat; 

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			// Set the size of the structure
	pfd.nVersion = 1;									// Always set this to 1
														// Pass in the appropriate OpenGL flags
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;// | PFD_SUPPORT_COMPOSITION;
	pfd.dwLayerMask = PFD_MAIN_PLANE;					// We want the standard mask (this is ignored anyway)
	pfd.iPixelType = PFD_TYPE_RGBA;						// We want RGB and Alpha pixel type
	pfd.cColorBits = (BYTE)getScreen()->depth();		// Set color bits per pixels
	pfd.cDepthBits = (BYTE)getScreen()->depth();		// Depthbits is ignored for RGBA, but we do it anyway
	pfd.cAccumBits = 0;									// No special bitplanes needed
	pfd.cStencilBits = 0;								// We desire no stencil bits

	// This gets us a pixel format that best matches the one passed in from the device
	if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == false ) { 
		MessageBox(0, "ChoosePixelFormat failed", "Error", MB_OK); 
		return false; 
	}

	// This sets the pixel format that we extracted from above
	if ( SetPixelFormat(hdc, pixelformat, &pfd) == FALSE ) { 
		MessageBox(0, "SetPixelFormat failed", "Error", MB_OK); 
		return false; 
	}

	return true;
}

void Engine::shutdown()
{
	debug("Stopping...");

	deletePlugins();

	mGeneralManager->shutdown();

	mScreenManager->remove(mConsole);
	mScreenManager->remove(this);

	delete mCloudManager;
	//delete mConsole;
	aeDelete( mDefaultTerrainManager );
	delete mDriver;
	delete mFogManager;
	delete mFont;
	delete mFrustum;
	aeDelete( mGeneralManager );						// AtlasEngine's safe delete
	delete mKeyboard;
	delete mLight;
	delete mLightManager;
	delete mMaterialManager;
	delete mMediaPathManager;
	delete mMouse;
	delete mObjectManager;
	delete mPhysicsManager;
	delete mSceneManager;
	delete mScreenManager;
	delete mScreenWriter;
	delete mShaderManager;
	delete mSkyManager;
	delete mSoundManager;
	delete mTextureManager;
	delete mWaterManager;

	delete mConsole;

	if ( mHGLRC ) {
		wglMakeCurrent(0, 0);							// This frees our rendering memory and sets everything back to normal
		wglDeleteContext(mHGLRC);						// Delete our OpenGL Rendering Context	
	}

	if ( mHDC ) {
		ReleaseDC(mHWND, mHDC);							// Release our HDC from memory
	}

	if ( isFullScreen() ) {
		ChangeDisplaySettings(0, 0);					// If so switch back to the desktop
		ShowCursor(TRUE);								// Show the mouse pointer
	}
}

//=============================== SIZE OPENGL SCREEN =================================
//
//	This function resizes the viewport for OpenGL.
//
//=============================== SIZE OPENGL SCREEN =================================

void Engine::SizeOpenGLScreen()													// Initialize The GL Window
{
	float height = (float)getScreen()->getHeight();
	float left = (float)getScreen()->getLeft();
	float top = (float)getScreen()->getTop();
	float width = (float)getScreen()->getWidth();

	if ( height == 0 ) {														// Prevent A Divide By Zero error
		height = 1;																// Make the Height Equal One
	}

	if ( mScreenManager ) {
		mScreenManager->reshape(left, top, width, height);
	}

	aeViewport(left, top, width, height);										// Make our viewport the whole window

	glMatrixMode(GL_PROJECTION);												// Select The Projection Matrix
	aeLoadIdentity();															// Reset The Projection Matrix

	gluPerspective(getFOV(), width / height, clipNear(), clipFar());			// Calculate The Aspect Ratio Of The Window

	glMatrixMode(GL_MODELVIEW);													// Select The Modelview Matrix
	aeLoadIdentity();															// Reset The Modelview Matrix
}

bool Engine::unloadScene()
{
	debug("Unloading scene '" + mMediaPathManager->getMap() + "'...");

	mGeneralManager->clear();

	mFogManager->clear();
	mLight->clear();
	mMediaPathManager->setMap("");
	mSceneManager->clear();
	mSkyManager->clear();
	mTerrainManager->clear();

	return true;
}

void Engine::unregisterPlugin(Core::IRendererPlugin *plugin)
{
	debug("unregisterPlugin('" + plugin->name() + "')");

	for ( RendererPlugins::iterator it = mRendererPlugins.begin(); it != mRendererPlugins.end(); ++it ) {
		if ( (*it) == plugin ) {
			mRendererPlugins.remove((*it));
			return;
		}
	}
}

void Engine::update(float elapsedTime)
{
	if ( mViewer ) {
		mViewer->update(elapsedTime);											// Update the camera data
	}

	mScreenWriter->update(elapsedTime);

	if ( mIsRunning ) {
		mGeneralManager->update(elapsedTime);
		mSceneManager->update(elapsedTime);	
	}
}

void Engine::updateOcclusionQuerys()
{
	if ( !useOcclusionQuery() ) {
		return;
	}

	mSceneManager->updateOcclusionQuerys();
}


}
