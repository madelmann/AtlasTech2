
#ifndef _Game_Game_h_
#define _Game_Game_h_


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


class Game
{
public:
	Game();

public:
	void newGame();
	void nextLevel();

	void start();
	void stop();

	int getAct() const;
	int getLevel() const;
	int getLifes() const;
	bool isStarted() const;

protected:

private:
	int		mAct;
	int		mLevel;
	int		mLifes;
	bool	mStarted;

private:
	static const int InitialAct		= 1;
	static const int InitialLevel	= 1;
	static const int InitialLifes	= 3;
};


#endif
