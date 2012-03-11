#include "GameOver.h"
#include "Graphics.h"
#include "Level.h"
#include "Input.h"
#include "MenuState.h"
#include "Player.h"
#include "enums.h"
#include "AudioHandler.h"
#include "wButton.h"
#include "PlayState.h"

GameOver GameOver::mGameOver;

void GameOver::init(Game *game)
{
	useCamera(true);

	// Load the background texture, temporary
	mBgkd = gGraphics->loadTexture("Data\\imgs\\game_over.png");
	
	mWindowHandler = new WindowHandler();

	wButton* menu = new wButton(SCREEN_WIDTH/2 + 200, SCREEN_HEIGHT/2 + 131, 100, 40, "Menu", WID_MENU, "Data\\imgs\\menu.png");
	menu->connect(&GameOver::messageHandler, this);
	mWindowHandler->addWindow(menu);

	wButton* retry = new wButton(SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 + 131, 107, 46, "Retry", WID_RETRY, "Data\\imgs\\retry.png");
	retry->connect(&GameOver::messageHandler, this);
	mWindowHandler->addWindow(retry);

	// Important when we later need to switch state
	setGame(game);
}

void GameOver::cleanup()
{
	// Delete and release everything
	ReleaseCOM(mBgkd);
	delete mWindowHandler;
}

void GameOver::pause()
{

}
	
void GameOver::resume()
{

}

void GameOver::update(double dt)
{
	if(gInput->keyPressed(VK_ESCAPE))
		changeState(MenuState::Instance());

	mWindowHandler->update(dt);
}
	
void GameOver::draw()
{
	// Draw the background
	gGraphics->drawRect(1024/2, 768/2, 1024, 768, 0xff000000);
	gGraphics->drawTexture(mBgkd, 1024/2, 768/2, 600, 353);
	mWindowHandler->draw();
}

bool GameOver::messageHandler(wId id, wMessage msg)
{
	if(id == WID_RETRY) {
		changeState(PlayState::Instance());
		PlayState::Instance()->loadLevel(mSource);
	}
	else if(id == WID_MENU)
		changeState(MenuState::Instance());
	return true;
}

void GameOver::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// For windows messages
	// Pass to what needs it
	
}

void GameOver::setLevelSource(string source)
{
	mSource = source;
}