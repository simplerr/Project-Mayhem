#pragma once

#include <string>
#include "GameState.h"
#include "enums.h"
#include "WindowHandler.h"

using namespace std;

class Level;
class Object;
class Player;

class GameOver : public GameState
{
public:
	void init(Game* game);
	void cleanup();

	void pause();
	void resume();

	void update(double dt);
	void draw();
	void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam);
	bool messageHandler(wId id, wMessage msg);
	void setLevelSource(string source);

	static GameOver* Instance() {
		return &mGameOver;
	}
protected:
	GameOver() {};
private:
	static GameOver mGameOver;

	IDirect3DTexture9* mBgkd;
	WindowHandler* mWindowHandler;
	string mSource;
};	// Class