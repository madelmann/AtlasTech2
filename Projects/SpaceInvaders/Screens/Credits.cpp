
// Header
#include "Credits.h"

// Library includes

// Project includes

// Namespace declarations


ScreenCredits::ScreenCredits()
{
}

void ScreenCredits::init()
{
	MenuItem *Back = new MenuItem("Back");
	//Back->colorMap(mTextureManager->create("menu/quit.jpg"));
	Back->setLeft(128.f);
	Back->setTop(100.f);
	Back->height(64.f);
	Back->width(256.f);
	//Back->setMouseClickEvent(&BackButton_Click);
	this->addMenuItem(Back);
}
