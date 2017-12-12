
// Header
#include "MainMenu.h"

// Library includes

// Project includes
#include <UIKit\Button.h>
#include <UIKit\Label.h>

// Namespace declarations
using UIKit::Button;
using UIKit::Label;


MainMenu::MainMenu()
{
	mColor = vector3f(0.8f, 0.78f, 0.78f);
	mIsContainer = true;
	mName = "screen.mainmenu";
}

void MainMenu::init()
{
	UIFrame::init();

	Button *button = new Button();
	button->setColor(vector3f(0.f, 1.f, 0.f));
	button->setHeight(50.f);
	button->setLeft(100.f);
	button->setTop(100.f);
	button->setWidth(50.f);
	button->show();
	this->add(button);

	Label *label = new Label();
	label->setColor(vector3f(1.f, 0.f, 0.f));
	label->setLeft(50.f);
	label->setText("Label1");
	label->setTop(50.f);
	label->show();
	this->add(label);
}
