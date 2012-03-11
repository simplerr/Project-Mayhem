#include "PlayState.h"
#include "Graphics.h"
#include "Level.h"
#include "Input.h"
#include "MenuState.h"
#include "Player.h"
#include "enums.h"
#include "AudioHandler.h"

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
	mPlayer = new Player(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	mPlayer->setLevel(mLevel);
	mLevel->addPlayer(mPlayer);

	// Load the background texture, temporary
	mBgkd = gGraphics->loadTexture("Data\\imgs\\dust_bkgd.bmp");
	
	//gAudio->ambientSound("Data\\sounds\\Retribution.ogg", true);

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
	if(gInput->keyPressed(VK_ESCAPE) && !mPlayer->inInventory())
		changeState(MenuState::Instance());
	else
		// Update all objects in the level
		mLevel->update(dt);
}
	
void PlayState::draw()
{
	// Draw the background
	gGraphics->drawTexture(mBgkd, 1024/2, 768/2, 1024, 768);

	// Draw all objects in the level
	mLevel->draw();
}

void PlayState::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// For windows messages
	// Pass to what needs it
}

void PlayState::loadLevel(string source)
{
	mLevel->loadFromFile(source);
}