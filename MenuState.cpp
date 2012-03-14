#include "MenuState.h"
#include "Graphics.h"
#include "Level.h"
#include "Input.h"
#include "Game.h"
#include "Menu.h"
#include "PlayState.h"
#include "EditorState.h"
#include "Vector.h"
#include "Sound.h"
#include "enums.h"
#include "GameOver.h"

MenuState MenuState::mMenuState;

void MenuState::init(Game *game)
{
	useCamera(false);
	// Load the background texture, temporary
	mBgkd = gGraphics->loadTexture("Data\\imgs\\dark_bkgd.png");
	mLogo = gGraphics->loadTexture("Data\\imgs\\logo.png");

	mMenu = new Menu("MainMenu", MOUSE, HOR, 1);
	mMenu->setMenuBackground("none", 1024/2, 768/2, 256, 512);
	mMenu->addMenuItem("campaign", "Data\\imgs\\campaign.png", "Data\\imgs\\campaign_hoover.png");
	mMenu->addMenuItem("custom", "Data\\imgs\\custom.png", "Data\\imgs\\custom_hoover.png");
	mMenu->addMenuItem("build", "Data\\imgs\\editor.png", "Data\\imgs\\editor_hoover.png");
	
	mMenu->buildMenu2(256, 64);
	mMenu->connect(&MenuState::menuHandler, this);

	gAudio->playMusic("Data\\sounds\\Tranquil Feelings.ogg", true, false);

	// Important when we later need to switch state
	setGame(game);
}

void MenuState::cleanup()
{
	// Delete and release everything
	delete mMenu;
	ReleaseCOM(mBgkd);
	ReleaseCOM(mLogo);
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
	gGraphics->drawTexture(mLogo, 1024/2, 125, 665, 165);
	mMenu->draw();
}

bool MenuState::menuHandler(std::string name)
{
	// Returns false to tell the menu it no longer exists
	if(name == "campaign")	{
		changeState(PlayState::Instance());
		PlayState::Instance()->loadLevel("Data\\levels\\campaign.xml");
		return false;	
	}
	else if(name == "custom") {
		changeState(PlayState::Instance());
		PlayState::Instance()->loadLevel("Data\\levels\\custom.xml");
		return false;
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