#include "MenuState.h"
#include "Graphics.h"
#include "Level.h"
#include "Input.h"
#include "Game.h"
#include "Menu.h"
#include "WonState.h"
#include "Vector.h"

#include "enums.h"

WonState WonState::mMenuState;

void WonState::init(Game *game)
{
	useCamera(false);
	// Load the background texture, temporary
	mBgkd = gGraphics->loadTexture("Data\\imgs\\level_done.png");

	mMenu = new Menu("Won", MOUSE, HOR, 1);
	mMenu->setMenuBackground("none", 1024/2, 768/2, 256, 512);
	mMenu->addMenuItem("menu", "Data\\imgs\\campaign.png", "Data\\imgs\\campaign_hoover.png");
	
	mMenu->buildMenu2(256, 64);
	mMenu->connect(&WonState::menuHandler, this);

	// Important when we later need to switch state
	setGame(game);
}

void WonState::cleanup()
{
	// Delete and release everything
	delete mMenu;
	ReleaseCOM(mBgkd);
}

void WonState::pause()
{

}
	
void WonState::resume()
{

}

void WonState::update(double dt)
{
	// Update all objects in the level
	mMenu->update(gInput->mousePosition());
}
	
void WonState::draw()
{
	// Draw the background
	gGraphics->drawTexture(mBgkd, 1024/2, 768/2, 1024, 768);
	mMenu->draw();
}

bool WonState::menuHandler(std::string name)
{
	// Returns false to tell the menu it no longer exists
	if(name == "menu")	{
		changeState(MenuState::Instance());
		//MenuState::Instance()->loadLevel("Data\\levels\\campaign.xml");
		return false;
	}
	return true;
}

void WonState::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// For windows messages
	// Pass to what needs it
}