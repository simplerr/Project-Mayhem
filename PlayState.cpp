#include "PlayState.h"
#include "Graphics.h"
#include "Level.h"
#include "Input.h"
#include "MenuState.h"
#include "Player.h"
#include "enums.h"

PlayState PlayState::mPlayState;

void PlayState::init(Game *game)
{
	// Create the level
	mLevel = new Level();

	mLevel->init();
	mLevel->setInEditor(false);
	useCamera(true);

	// Create and add player to the level
	// :NOTE: Could instead lie in Level
	mPlayer = new Player(400, 300);
	mPlayer->setLevel(mLevel);
	mLevel->addPlayer(mPlayer);

	// Load the background texture, temporary
	mBgkd = gGraphics->loadTexture("Data\\imgs\\dust_bkgd.bmp");

	// Important when we later need to switch state
	setGame(game);
}

void PlayState::cleanup()
{
	// Delete and release everything
	delete mLevel;
	ReleaseCOM(mBgkd);
}

void PlayState::pause()
{

}
	
void PlayState::resume()
{

}

void PlayState::update(double dt)
{
	// Update all objects in the level
	mLevel->update(dt);

	if(gInput->keyPressed(VK_ESCAPE))
		changeState(MenuState::Instance());
}
	
void PlayState::draw()
{
	// Draw the background
	gGraphics->drawTexture(mBgkd, 400, 300, 800, 600);

	// Draw all objects in the level
	mLevel->draw();
}

void PlayState::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// For windows messages
	// Pass to what needs it
}