#pragma once

#include "GameState.h"
#include "enums.h"

class Menu;

class WonState : public GameState
{
public:
	void init(Game* game);
	void cleanup();

	void pause();
	void resume();

	void update(double dt);
	void draw();
	bool menuHandler(std::string name);
	void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam);


	static WonState* Instance() {
		return &mMenuState;
	}
protected:
	WonState() {};
private:
	static WonState	mMenuState;
	IDirect3DTexture9*	mBgkd;
	IDirect3DTexture9*	mLogo;
	Menu*				mMenu;
};	
