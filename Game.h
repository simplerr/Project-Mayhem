#ifndef GAME_H
#define GAME_H

#include "Runnable.h"
#include "GfxStats.h"

class Camera;
class GameState;

class Game : public Runnable
{
public:
	Game(HINSTANCE hInstance, std::string caption, int width, int height, D3DDEVTYPE devType, DWORD requestedVP);
	~Game();

	// Changes the current state (PlayState, MenuState, OptionState etc..)
	void changeState(GameState* state);

	// Initialize everything, could replace ctor, but doesn't atm
	void init();

	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	// Update and draw gets called from Runnable each frame
	void update(float t, float dt);
	void draw();
private:
	GameState	*mGameState;
	GfxStats*	mGfxStats;
};	// Class

#endif