
// Header
#include "Game.h"

// Library includes

// Project includes

// Namespace declarations


Game::Game()
: mAct(0),
  mLevel(0),
  mLifes(0),
  mStarted(false)
{
}

int Game::getAct() const
{
	return mAct;
}

int Game::getLevel() const
{
	return mLevel;
}

int Game::getLifes() const
{
	return mLifes;
}

bool Game::isStarted() const 
{
	return mStarted;
}

void Game::newGame()
{
	mAct	= InitialAct;
	mLevel	= InitialLevel;
	mLifes	= InitialLifes;
}

void Game::nextLevel()
{
	mLevel++;
}

void Game::start()
{
	mStarted = true;
}

void Game::stop()
{
	mStarted = false;
}
