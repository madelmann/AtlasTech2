
// Header
#include "MainMenu.h"

// Library includes
#include <cassert>

// Project includes
#include <Interfaces/Texture/IManager.h>
#include <Menu/MenuItem.h>
#include <Misc/Colors.h>

// Namespace declarations


MainMenu::MainMenu()
: mTextureManager(0)
{
	mColor = vector3f(0.8f, 0.78f, 0.78f);
	mIsContainer = true;
	mName = "screen.mainmenu";
}

void MainMenu::connectTextureManager(TextureSpace::IManager *m)
{
	assert(m);
	assert(!mTextureManager);

	mTextureManager = m;
}

void MainMenu::init()
{
	UIScreen::init();

	MenuItem *Start = new MenuItem("Start");
	Start->colorMap(mTextureManager->create("menu/start.jpg"));
	Start->setLeft(400.f);
	Start->setTop(400.f);
	Start->height(64.f);
	Start->width(256.f);
	this->addMenuItem(Start);

	MenuItem *Credits = new MenuItem("Credits");
	Credits->colorMap(mTextureManager->create("menu/credits.jpg"));
	Credits->setLeft(400.f);
	Credits->setTop(250.f);
	Credits->height(64.f);
	Credits->width(256.f);
	this->addMenuItem(Credits);

	MenuItem *Quit = new MenuItem("Quit");
	Quit->colorMap(mTextureManager->create("menu/quit.jpg"));
	Quit->setLeft(400.f);
	Quit->setTop(100.f);
	Quit->height(64.f);
	Quit->width(256.f);
	this->addMenuItem(Quit);
}
