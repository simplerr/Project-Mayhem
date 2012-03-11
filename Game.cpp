#include "Game.h"
#include "Input.h"
#include "PlayState.h"
#include "Graphics.h"
#include "Runnable.h"
#include "Math.h"
#include "MenuState.h"
#include "Stackwalker.h"
#include "Enemy.h"
#include "Camera.h"
#include "Vector.h"
#include "Item.h"
#include "ItemHandler.h"
#include "Scrap.h"
#include "AudioHandler.h"
#include <stdlib.h>
#include <crtdbg.h>

Graphics*			gGraphics		= NULL;
Input*				gInput			= NULL;
Runnable*			gGame			= NULL;
IDirect3DDevice9*	gd3dDevice		= NULL;
AudioHandler*		gAudio	= NULL;
Math*				gMath			= NULL;
Enemies*			gEnemies		= NULL;
ItemHandler*		gItemHandler	= NULL;
Scrap*				gScrap			= NULL;

bool operator==(const Item& a, const Item& b)
{
	/*if(a.getId() == b.getId())
		return true;
	else
		return false;*/
	return false;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	InitAllocCheck(ACOutput_XML); // ACOutput_XML

	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif
		
	Game app(hInstance, "Project Mayhem", SCREEN_WIDTH, SCREEN_HEIGHT, D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gGame = &app;

	gInput = new Input();

	gMath = new Math();

	/* this is where the game gets runned */
	return gGame->run();
}


Game::Game(HINSTANCE hInstance, std::string caption, int width, int height, D3DDEVTYPE devType, DWORD requestedVP)
	:Runnable(hInstance, caption, width, height, devType, requestedVP)
{
	gGraphics = new Graphics();
	
	gEnemies = new Enemies();
	gItemHandler = new ItemHandler();
	
	mGameState = NULL;
	gAudio = new AudioHandler();
	changeState(MenuState::Instance());
	mGameState->init(this);
	mGfxStats = new GfxStats();
	gScrap = new Scrap();
}

Game::~Game()
{
	// :WARNING: Maybe needs to call ~Runnable()
	mGameState->cleanup();

	// Print memory leaks to xml file
	DeInitAllocCheck();

	delete gInput;
	delete gGraphics;
	delete gItemHandler;
	delete mGfxStats;
}

void Game::init()
{
	SetCursor(LoadCursor(0, IDC_ARROW));
}

void Game::changeState(GameState* state)
{
	if(mGameState != NULL)
		mGameState->cleanup();

	mGameState = state;
	mGameState->init(this);

	// restores their positions
	//if(gInput != NULL)
	//	gInput->restoreCursor();
}

void Game::update(float t, float dt)
{
	mGameState->update(dt);
	mGfxStats->update(dt);
	gInput->update(dt);
}

void Game::draw()
{
	mGameState->draw();
	//gInput->draw();
	mGfxStats->display();
}

//! The starting msgProc function.
LRESULT Game::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = gInput->msgProc(msg, wParam, lParam);
	result = Runnable::msgProc(msg, wParam, lParam);

	return result;
}