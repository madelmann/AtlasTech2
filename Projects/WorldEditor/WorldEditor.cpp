
// Header
#include "WorldEditor.h"

// Library includes

// Project includes
#include <Camera/MovieCamera/MovieCamera.h>
#include <Driver/DriverUtils.h>
#include <Interfaces/Console/IConsole.h>
#include <Interfaces/Core/Types.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Mesh/IObjectManager.h>
#include <Interfaces/Physics/IManager.h>
#include <Interfaces/Scene/IManager.h>
#include <Interfaces/Script/IManager.h>
#include <Interfaces/Terrain/IManager.h>
#include <Interfaces/Texture/IManager.h>
#include <Interfaces/UIKit/IManager.h>
#include <JsonNG/Json.h>
#include <Math/Tools.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <ObjectiveScript/ObjectiveScript.h>
#include <Parsers/ConfigParser.h>
#include <Renderer/RenderObject.h>
#include <Scene/SceneGraphObject.h>
#include <Tools/Files.h>

// Application includes
#include <Camera/EditorCamera.h>
#include <Console/Plugins/EditorPlugin.h>
#include <Menu/MainMenu.h>

// Namespace declarations


WorldEditor::WorldEditor(HINSTANCE instance)
: Application("WorldEditor", instance),
  mCamera(0),
  mEditorMode(ModeInsertion),
  mFileBrowser(0),
  mMovieCamera(0),
  mObjectManager(0),
  mParser(0),
  mPhysicManager(0),
  mSceneManager(0),
  mSceneSelection(0),
  mScriptManager(0),
  mTerrainManager(0)
{
	debug("Starting...");
}

WorldEditor::~WorldEditor()
{
	debug("Stopping...");

	aeDelete( mCamera );
	aeDelete( mFileBrowser );
	aeDelete( mMovieCamera );
	aeDelete( mParser ) ;
}

void WorldEditor::dispatchMouseDown(const Common::Event& e)
{
	if ( mEngine->isConsoleVisible() ) {
		return;
	}

	vector3f m3d = mMouse->getPosition3D();
	m3d = mTerrainManager->heightAt(m3d);

	switch ( e.uMsg ) {
		case WM_LBUTTONDOWN:
			switch ( mEditorMode ) {
				case ModeSelection:
					pickSelection(m3d);
					break;
				case ModeInsertion:
					if ( !mInsertObject.empty() ) {
						Scene::SceneGraphObject *sgo = mSceneManager->create(mObjectManager->create(mInsertObject));
						if ( !sgo ) {
							return;
						}

						sgo->init(vector3f(m3d.x, m3d.y, m3d.z), vector3f());
						mSceneManager->attach(sgo, Core::invalidID, true);

						mSceneSelection = sgo;
					}
					break;
			}
			break;
		case WM_MBUTTONDOWN:
			mCamera->setFollowMouseView(true);
			break;
		case WM_RBUTTONDOWN:
			dropSelection();
			break;
	}
}

void WorldEditor::dispatchMouseMove(const Common::Event& /*e*/)
{
	if ( mEngine->isConsoleVisible() ) {
		return;
	}

	vector3f m3d = mMouse->getPosition3D();
	m3d = mTerrainManager->heightAt(m3d);

	switch ( mEditorMode ) {
		case ModeSelection:
			if ( mSceneSelection ) {
				mSceneSelection->setPosition(m3d);
			}
			break;
		case ModeInsertion:
			break;
	}
}

void WorldEditor::dispatchMouseUp(const Common::Event& e)
{
	if ( mEngine->isConsoleVisible() ) {
		return;
	}

	//vector3f m3d = mMouse->getPosition3D();
	//m3d = mTerrainManager->heightAt(m3d);

	switch ( e.uMsg ) {
		case WM_LBUTTONUP:
			break;
		case WM_MBUTTONUP:
			mCamera->setFollowMouseView(false);
			break;
		case WM_RBUTTONUP:
			break;
	}
}

void WorldEditor::dispatchMouseWheel(const Common::Event& /*e*/)
{
	if ( mEngine->isConsoleVisible() ) {
		return;
	}
}

void WorldEditor::dropSelection()
{
	mSceneSelection = 0;
}

vector3f WorldEditor::getRayTo(vector2l pos)
{
	//if (m_ortho)
	//{

	//	btScalar aspect;
	//	vector3f extents;
	//	aspect = m_glutScreenWidth / (btScalar)m_glutScreenHeight;
	//	extents.setValue(aspect * 1.0f, 1.0f,0);
	//	
	//	extents *= m_cameraDistance;
	//	vector3f lower = m_cameraTargetPosition - extents;
	//	vector3f upper = m_cameraTargetPosition + extents;

	//	btScalar u = x / btScalar(m_glutScreenWidth);
	//	btScalar v = (m_glutScreenHeight - y) / btScalar(m_glutScreenHeight);
	//	
	//	vector3f	p(0,0,0);
	//	p.setValue((1.0f - u) * lower.getX() + u * upper.getX(),(1.0f - v) * lower.getY() + v * upper.getY(),m_cameraTargetPosition.getZ());
	//	return p;
	//}

	float top = 0.f;
	float bottom = (float)-mEngine->getScreen()->getHeight();
	float nearPlane = 1.f;
	float tanFov = (top - bottom) * 0.5f / nearPlane;
	float fov = 2.f * atanf(tanFov);

	vector3f rayFrom = mCamera->position();
	vector3f rayForward = vector3f(mCamera->view()) - mCamera->position();
	rayForward.normalize();
	float farPlane = 10000.f;
	rayForward *= farPlane;

	vector3f rightOffset;
	vector3f vertical = mCamera->upVector();

	vector3f hor;
	hor = rayForward.cross(vertical);
	hor.normalize();
	vertical = hor.cross(rayForward);
	vertical.normalize();

	float tanfov = tanf(0.5f * fov);
	hor *= 2.f * farPlane * tanfov;
	vertical *= 2.f * farPlane * tanfov;

	float aspect = mEngine->getScreen()->getWidth() / (float)mEngine->getScreen()->getHeight();
	
	hor *= aspect;

	vector3f rayToCenter = rayFrom + rayForward;
	vector3f dHor = hor * 1.f / float(mEngine->getScreen()->getWidth());
	vector3f dVert = vertical * 1.f / float(mEngine->getScreen()->getHeight());


	vector3f rayTo = rayToCenter - 0.5f * hor + 0.5f * vertical;
	rayTo += (float)pos.x * dHor;
	rayTo -= (float)pos.y * dVert;
	return rayTo;
}

LRESULT WorldEditor::handleEvent(const Common::Event& e)
{
	switch ( e.uMsg ) {
		case WM_KEYDOWN:
			onKeyPress(e.wParam);
			break;
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
			break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			dispatchMouseDown(e);
			//queue(e);
			break;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			dispatchMouseUp(e);
			//queue(e);
			break;
		case WM_MOUSEMOVE:
			dispatchMouseMove(e);
			break;
		case WM_MOUSEWHEEL:
			dispatchMouseWheel(e);
			break;
	}

	return true;
}

void WorldEditor::initialize(const std::string& cmdline)
{
	debug("Initializing...");

	Application::initialize();
	Application::processAdd(this);


	mCamera = new EditorCamera();
	mFileBrowser = new Tools::Files::FileBrowser(mMediaPathManager->getGame() + "/Objects", "ao");
	mGeneralManager = mEngine->provideGeneralManager();
	mMovieCamera = new MovieCamera();
	mObjectManager = mEngine->provideObjectManager();
	mParser = new ConfigParser(this);
	mPhysicManager = mEngine->providePhysicsManager();
	mSceneManager = mEngine->provideSceneManager();
	mScriptManager = mEngine->provideScriptManager();
	mTerrainManager = mEngine->provideTerrainManager();


	mCamera->connectEngine(mEngine);
	mGeneralManager->add(mMovieCamera);


	{	// Console plugin setup
		Console::Plugins::EditorPlugin *editor = new Console::Plugins::EditorPlugin();
		editor->connectManagement(mEngine->provideManagement());
		mConsole->registerPlugin(editor);
	}
	{	// Main menu setup
		MainMenu *mainmenu = new MainMenu();
		mainmenu->setBackground(mEngine->provideTextureManager()->create("menu/menu_background.tga"));
		mainmenu->setFont(mEngine->provideFont());
		mainmenu->init();

		mScreenManager->add(mainmenu);
		mScreenManager->show("screen.mainmenu");
	}

	mEngine->setViewer(mCamera);
	mEngine->reset();

	// Keyboard setup
	{
		mKeyboard->bind("rotate-left", VK_LEFT);
		mKeyboard->bind("rotate-right", VK_RIGHT);
		mKeyboard->bind("strafe-left", 'a');
		mKeyboard->bind("strafe-right", 'd');
	}

	loadConfig();

	mEngine->aeErrorCheck(__FILE__, __LINE__);

	if ( !cmdline.empty() ) {
		mEngine->loadScene(cmdline);
	}
	else {
		// load demo scene
		mEngine->loadScene("shooter");
	}
}

void WorldEditor::loadConfig()
{
	if ( !mParser->load(mMediaPathManager->getGame() + "game.conf") ) {
		error("could not load engine configuration!");
		return;
	}

	// Console settings
	if ( mParser->getValue("console_enabled").toBool() ) {
		mConsole->enable();
	}
}

bool WorldEditor::onKeyPress(WPARAM keyCode)
{
	Input::Key *key = mKeyboard->getKey(keyCode);
	if ( !key ) {
		// an unbund key has been pressed
		switch ( keyCode ) {
			case VK_F1:
				switchEditorMode();
				break;
			case VK_F2:
				break;
			case VK_F3:
				mInsertObject = mFileBrowser->previousFile();
				break;
			case VK_F4:
				mInsertObject = mFileBrowser->nextFile();
				break;
			case VK_SPACE:
				shootBox();
				break;
			case VK_RETURN:
				mObjectManager->create(mInsertObject);
				break;
			case 'C': {
					static CameraSpace::IViewer *viewer = mCamera;

					if ( viewer == mCamera ) { viewer = mMovieCamera; }
					else { viewer = mCamera; }

					mEngine->setViewer(viewer);
				}
				break;
			case 'P':
				// add a position action to movie camera
				mMovieCamera->addAction(0, new CameraSpace::MoveAction(mCamera->position()));
				// add a rotation action to movie camera
				mMovieCamera->addAction(0, new CameraSpace::RotateAction(mCamera->view()));
				break;
		}

		return true;
	}

	if ( key->isFunction("escape") ) {
		if ( mScreenManager->get("screen.engine")->isVisible() ) {
			mCamera->setFollowMouseView(false);
			mEngine->Pause();
			mScreenManager->hide("screen.engine");
		}
		else {
			mScreenManager->show("screen.engine");
		}
	}
	else if ( key->isFunction("freecamera") ) {
		mCamera->setFollowMouseView(!mCamera->isFollowMouseView());
	}
	else if ( key->isFunction("play") ) {
		if ( mEngine->isRunning() ) { mEngine->Pause(); }
		else { mEngine->Play(); }
	}

	return true;
}

void WorldEditor::pickSelection(const vector3f& vector)
{
	mSceneSelection = mSceneManager->collidePoint(vector);
}

void WorldEditor::processDelayedEvents()
{
	while ( !mEventQueue.empty() ) {
		const Common::Event& e = mEventQueue.dequeue();

		switch ( e.uMsg ) {
			case WM_KEYDOWN:
				onKeyPress(e.wParam);
				break;
			case WM_LBUTTONDBLCLK:
			case WM_MBUTTONDBLCLK:
			case WM_RBUTTONDBLCLK:
				break;
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
				dispatchMouseDown(e);
				break;
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
				dispatchMouseUp(e);
				break;
			case WM_MOUSEWHEEL:
				dispatchMouseWheel(e);
				break;
		}
	}
}

void WorldEditor::queue(const Common::Event& e)
{
	mEventQueue.enqueue(e);
}

void WorldEditor::run()
{
	processDelayedEvents();

	{	// Camera position
		// Once we have the frame interval, we find the current speed
		float speed = 128.0f * mEngine->getFrameTime();

		// Check if we hit the "run" key
		if ( GetKeyState(VK_SHIFT) & 0x80 ) {
			speed *= 4;
		}

		// Check if we hit the "strafe left" key
		if ( GetKeyState('A') & 0x80 ) {
			mCamera->strafe(-speed);
		}
		// Check if we hit the "strafe right" key
		if ( GetKeyState('D') & 0x80 ) {
			mCamera->strafe(speed);
		}
		// Check if we hit the "backward" key
		if ( GetKeyState('S') & 0x80 ) {
			mCamera->move(-speed);
		}
		// Check if we hit the "forward" key
		if ( GetKeyState('W') & 0x80 ) {
			mCamera->move(speed);
		}

		if ( GetKeyState(VK_DOWN) & 0x80 ) {
			mCamera->move(-speed);
		}
		if ( GetKeyState(VK_LEFT) & 0x80 ) {
			mCamera->rotateY(speed);
		}
		if ( GetKeyState(VK_RIGHT) & 0x80 ) {
			mCamera->rotateY(-speed);
		}
		if ( GetKeyState(VK_UP) & 0x80 ) {
			mCamera->move(speed);
		}

		vector3f v = mCamera->position();

		mScreenWriter->write(-0.97f, 0.720f, "Camera.X = " + Tools::toString(v.x));
		mScreenWriter->write(-0.97f, 0.670f, "Camera.Y = " + Tools::toString(v.y));
		mScreenWriter->write(-0.97f, 0.620f, "Camera.Z = " + Tools::toString(v.z));
	}

	mScreenWriter->write(-0.97f, 0.56f, mInsertObject);
}

void WorldEditor::setEditorMode(eMode mode)
{
	mEditorMode = mode;
}

void WorldEditor::shootBox()
{
	mPhysicManager->shootBox(mCamera->position(), mCamera->view());
}

void WorldEditor::switchEditorMode()
{
	switch ( mEditorMode ) {
		case ModeSelection:
			setEditorMode(ModeInsertion);
			break;
		case ModeInsertion:
			setEditorMode(ModeSelection);
			break;
	}
}
