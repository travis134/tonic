#ifndef _BLANK_SCREEN_GAME_
#define _BLANK_SCREEN_GAME_
#include "Game.h"


class BlankScreenGame : public Game
{
public:
	BlankScreenGame(void);
	virtual ~BlankScreenGame(void);
	bool LoadContent(void);
	void UnloadContent(void);
	void Update(float gameTime);
	void Draw(float gameTime);
};

#endif