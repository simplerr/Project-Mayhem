#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "GameState.h"
#include "enums.h"

class Menu;

class MenuState : public GameState
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


	static MenuState* Instance() {
		return &mMenuState;
	}
protected:
	MenuState() {};
private:
	static MenuState	mMenuState;
	IDirect3DTexture9*	mBgkd;
	IDirect3DTexture9*	mLogo;
	Menu*				mMenu;
};	// Class

#endif