#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Game.h"

class Game;

// Reference: http://gamedevgeek.com/tutorials/managing-game-states-in-c/

// Abstract baseclass
class GameState
{
public:
	virtual void init(Game* game) = 0;
	virtual void cleanup(void) = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void update(double dt) = 0;
	virtual void draw() = 0;

	void setGame(Game* game) {mGame = game;};

	void changeState(GameState* state) {
		mGame->changeState(state);
	}
	void useCamera(bool b) {
		mUsesCamera = b;
	}
	bool usesCamera() {
		return mUsesCamera;
	}
protected:
	GameState(){};
private:
	bool mStateChanged;
	Game *mGame;
	bool mUsesCamera;
};	// Class

#endif