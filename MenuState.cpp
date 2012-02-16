#include "MenuState.h"
#include "Graphics.h"
#include "Level.h"
#include "Input.h"
#include "Game.h"
#include "Menu.h"
#include "PlayState.h"
#include "EditorState.h"
#include "Vector.h"
#include "enums.h"

MenuState MenuState::mMenuState;

void MenuState::init(Game *game)
{
	useCamera(false);
	// Load the background texture, temporary
	mBgkd = gGraphics->loadTexture("Data\\imgs\\bkgd.bmp");

	mMenu = new Menu("MainMenu", MOUSE, HOR, 1);
	mMenu->setMenuBackground("none", 1024/2, 768/2, 256, 512);
	mMenu->addMenuItem("play", "Data\\imgs\\play_button.bmp", "Data\\imgs\\play_button.bmp");
	mMenu->addMenuItem("build", "Data\\imgs\\build_button.bmp", "Data\\imgs\\build_button.bmp");
	
	mMenu->buildMenu2(256, 128);
	mMenu->connect(&MenuState::menuHandler, this);

	// Important when we later need to switch state
	setGame(game);
}

void MenuState::cleanup()
{
	// Delete and release everything
	delete mMenu;
	ReleaseCOM(mBgkd);
}

void MenuState::pause()
{

}
	
void MenuState::resume()
{

}

void MenuState::update(double dt)
{
	// Update all objects in the level
	mMenu->update(gInput->mousePosition());
}
	
void MenuState::draw()
{
	// Draw the background
	gGraphics->drawTexture(mBgkd, 1024/2, 768/2, 1024, 768);
	mMenu->draw();
}

bool MenuState::menuHandler(std::string name)
{
	if(name == "play")	{
		changeState(PlayState::Instance());
		return false;	// To tell the menu it no longer exists
	}
	else if(name == "build")	{
		changeState(EditorState::Instance());
		return false;
	}

	return true;
}

void MenuState::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// For windows messages
	// Pass to what needs it
}