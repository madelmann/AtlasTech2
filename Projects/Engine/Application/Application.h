
#ifndef _Application_Application_h_
#define _Application_Application_h_


// Library includes
#include <list>

#ifdef _WIN32
#	include <windows.h>
#endif

// Project includes
#include <Common/Logger.h>
#include <Core/Engine.h>
#include <Core/EngineSettings.h>
#include <Core/EngineStates.h>
#include <Interfaces/Common/AProcess.h>
#include <Interfaces/Common/Event.h>
#include <Interfaces/Common/IEventHandler.h>
#include <Interfaces/Common/AProcessHandler.h>
#include <Interfaces/Core/IGeneralManager.h>
#include <Interfaces/Console/IConsole.h>
#include <Interfaces/Input/IKeyboard.h>
#include <Interfaces/Input/IMouse.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Renderer/IScreenWriter.h>
#include <Interfaces/Scene/IManager.h>
#include <Interfaces/Sound/IManager.h>
#include <Interfaces/UIKit/IManager.h>
#include <UIKit/UIScreen.h>

// Forward declarations

// Namespace declarations
using Console::IConsole;
using Core::IScreenWriter;


#ifdef _WIN32
extern LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif


namespace Core {


class Application : public Common::AProcess,
					public Common::AProcessHandler,
					public UIKit::UIScreen,
					protected Common::Logger
{
public:
	Application(const std::string& name, HINSTANCE instance);
	virtual ~Application();

public:
	virtual void initialize(const std::string& cmdline) = 0;

public:
#ifdef _WIN32
	LRESULT CALLBACK handle(const Common::Event &e, HWND hwnd);
#endif

	void exec();
	void initialize();
	void update(float elapsedTime);

	HWND hwnd() const;
	HINSTANCE instance() const;
	const std::string& name() const;

protected:
	IConsole				*mConsole;
	Engine					*mEngine;
	EngineSettings			*mEngineSettings;
	Core::IGeneralManager	*mGeneralManager;
	Core::IdGenerator		*mIdGenerator;
	Input::IKeyboard		*mKeyboard;
	IMediaPathManager		*mMediaPathManager;
	Input::IMouse			*mMouse;
	Scene::IManager			*mSceneManager;
	UIKit::IManager			*mScreenManager;
	IScreenWriter			*mScreenWriter;
	SoundSpace::IManager	*mSoundManager;

private:
	void autoexec();

private:
	HWND		mHWND;
	HINSTANCE	mInstance;
	std::string	mName;

private:
	bool	mShowDebugInterface;
};


}


#endif
