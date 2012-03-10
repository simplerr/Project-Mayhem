#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <string>
#include "GameState.h"
#include "enums.h"

using namespace std;

class Level;
class Object;
class Player;

class PlayState : public GameState
{
public:
	void init(Game* game);
	void cleanup();

	void pause();
	void resume();

	void update(double dt);
	void draw();
	void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam);
	void loadLevel(string source);

	static PlayState* Instance() {
		return &mPlayState;
	}
protected:
	PlayState() {};
private:
	static PlayState mPlayState;
	Level *mLevel;
	Player* mPlayer;

	IDirect3DTexture9* mBgkd;
};	// Class

#endif